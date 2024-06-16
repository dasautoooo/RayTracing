//
// Created by DasAuto on 5/30/24.
//

#include "Renderer.h"

#include "Pecan/Random.h"

namespace Utils {
    static uint32_t ConvertToRGBA(const glm::vec4& color) {
        uint8_t r = (uint8_t)(color.r * 255.0f);
        uint8_t g = (uint8_t)(color.g * 255.0f);
        uint8_t b = (uint8_t)(color.b * 255.0f);
        uint8_t a = (uint8_t)(color.a * 255.0f);

        uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;

        return result;
    }
}

void Renderer::Render(const Scene& scene, const Camera& camera) {
    m_ActiveScene = &scene;
    m_ActiveCamera = &camera;

    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
            glm::vec4 color = PerPixel(x, y);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
        }
    }

    m_FinalImage->SetData(m_ImageData);
}

void Renderer::OnResize(uint32_t width, uint32_t height) {
    if (m_FinalImage) {
        if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
            return;

        m_FinalImage->Resize(width, height);
    } else {
        m_FinalImage = std::make_shared<Pecan::Image>(width, height, Pecan::ImageFormat::RGBA);
    }
    m_FinalImage->Resize(width, height);

    delete[] m_ImageData;
    m_ImageData = new uint32_t[width * height];
}

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y) {
    Ray ray;
    ray.Origin = m_ActiveCamera->GetPosition();
    ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

    Renderer::HitPayload payload = TraceRay(ray);
    if (payload.HitDistance < 0.0f)
        return glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};

    glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
    float lightIntensity = glm::max(glm::dot(payload.WorldNormal, -lightDir), 0.0f); // == cos(angle)

    const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
    glm::vec3 sphereColor = sphere.Albedo;
    sphereColor *= lightIntensity;

    return glm::vec4(sphereColor, 1.0f);
}

Renderer::HitPayload Renderer::TraceRay(const Ray& ray) {
    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    // where
    // a = ray origin
    // b = ray direction
    // r = radius
    // t = hit distance

    int closestSphere = -1;
    float hitDistance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < m_ActiveScene->Spheres.size(); ++i) {
        const Sphere& sphere = m_ActiveScene->Spheres[i];
        glm::vec3 origin = ray.Origin - sphere.Position;

        float a = glm::dot(ray.Direction, ray.Direction);
        float b = 2.0f * glm::dot(origin, ray.Direction);
        float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

        // Quadratic formula discriminant:
        // b^2 - 4ac

        float discriminant = b * b - 4.0f * a * c;

        if (discriminant < 0.0f)
            continue;

        // Quadratic formula:
        // (-b +- sqrt(discriminant)) / 2a

        // float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a); // Second hit distance (currently unused)
        float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
        if (closestT > 0.0f && closestT < hitDistance) {
            hitDistance = closestT;
            closestSphere = (int)i;
        }

    }

    if (closestSphere < 0)
        return Miss(ray);

    return ClosestHit(ray, hitDistance, closestSphere);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray &ray, float hitDistance, int objectIndex) {
    Renderer::HitPayload payload;
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;

    const Sphere& closestSphere = m_ActiveScene->Spheres[objectIndex];

    glm::vec3 origin = ray.Origin - closestSphere.Position;
    payload.WorldPosition = origin + ray.Direction * hitDistance;
    payload.WorldNormal = glm::normalize(payload.WorldPosition);

    payload.WorldPosition += closestSphere.Position;

    return payload;
}


Renderer::HitPayload Renderer::Miss(const Ray &ray) {
    Renderer::HitPayload payload;
    payload.HitDistance = -1.0f;
    return payload;
}



