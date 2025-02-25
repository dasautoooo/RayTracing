#include "Pecan/Application.h"
#include "Pecan/EntryPoint.h"

#include "Pecan/Image.h"
#include "Pecan/Random.h"
#include "Pecan/Timer.h"

#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Pecan;

class ExampleLayer : public Pecan::Layer
{
public:
    ExampleLayer()
        : m_Camera(45.0f, 0.1f, 100.0f) {

        Material& pinkSphere = m_Scene.Materials.emplace_back();
        pinkSphere.Albedo = { 1.0f, 0.0f, 1.0f };
        pinkSphere.Roughness = 0.0f;

        Material& blueSphere = m_Scene.Materials.emplace_back();
        blueSphere.Albedo = { 0.2f, 0.3f, 1.0f };
        blueSphere.Roughness = 0.1f;

        {
            Sphere sphere;
            sphere.Position = { 0.0f, 0.0f, -3.0f };
            sphere.Radius = 1.0f;
            sphere.MaterialIndex = 0;
            m_Scene.Spheres.push_back(sphere);
        }

        {
            Sphere sphere;
            sphere.Position = { 1.0f, -101.0f, -5.0f };
            sphere.Radius = 100.0f;
            sphere.MaterialIndex = 1;
            m_Scene.Spheres.push_back(sphere);
        }
    }

    virtual void OnUpdate(float ts) override
    {
        if (m_Camera.OnUpdate(ts))
            m_Renderer.ResetFrameIndex();
    }

    virtual void OnUIRender() override
    {
        ImGui::Begin("Settings");
        ImGui::Text("Last render: %.3fms", m_LastRenderTime);
        if (ImGui::Button("Render")) {
            Render();
        }
        ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().Accumulate);

        if (ImGui::Button("Reset")) {
            m_Renderer.ResetFrameIndex();
        }
        ImGui::End();

        ImGui::Begin("Camera");

        ImGui::Text("Position x: %.5f, y: %5f, z: %.5f", m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);
        ImGui::Text("Direction x: %.5f, y: %5f, z: %.5f", m_Camera.GetDirection().x, m_Camera.GetDirection().y, m_Camera.GetDirection().z);

        ImGui::End();

        ImGui::Begin("Scene");
        for (size_t i = 0; i < m_Scene.Spheres.size(); i++) {
            ImGui::PushID(i);

            Sphere& sphere = m_Scene.Spheres[i];
            ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
            ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
            ImGui::DragInt("Material", &sphere.MaterialIndex, 1, 0, (int)m_Scene.Materials.size() - 1);

            ImGui::Separator();

            ImGui::PopID();
        }

        for (size_t i = 0; i < m_Scene.Materials.size(); i++) {
            ImGui::PushID(i);

            Material& material = m_Scene.Materials[i];
            ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
            ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
            ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);

            ImGui::Separator();

            ImGui::PopID();
        }
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");

        m_ViewportWidth = ImGui::GetContentRegionAvail().x;
        m_ViewportHeight = ImGui::GetContentRegionAvail().y;

        auto image = m_Renderer.GetFinalImage();
        if (image)
            ImGui::Image(image->GetDescriptorSet(),
                         { (float)image->GetWidth(), (float)image->GetHeight() },
                         ImVec2(0, 1),
                         ImVec2(1, 0));

        ImGui::End();
        ImGui::PopStyleVar();

        Render();
    }

    void Render()
    {
        Timer timer;

        m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
        m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
        m_Renderer.Render(m_Scene, m_Camera);

        m_LastRenderTime = timer.ElapsedMillis();
    }
private:
    Renderer m_Renderer;
    Camera m_Camera;
    Scene m_Scene;
    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
    float m_LastRenderTime = 0.0f;
};

Pecan::Application* Pecan::CreateApplication(int argc, char** argv)
{
    Pecan::ApplicationSpecification spec;
    spec.Name = "Ray Tracing";

    Pecan::Application* app = new Pecan::Application(spec);
    app->PushLayer<ExampleLayer>();
    app->SetMenubarCallback([app]()
                            {
                                if (ImGui::BeginMenu("File"))
                                {
                                    if (ImGui::MenuItem("Exit"))
                                    {
                                        app->Close();
                                    }
                                    ImGui::EndMenu();
                                }
                            });
    return app;
}