//
// Created by DasAuto on 5/30/24.
//

#ifndef RAYTRACING_RENDERER_H
#define RAYTRACING_RENDERER_H

#include "Pecan/Image.h"
#include "Camera.h"
#include "Ray.h"

#include "glm/glm.hpp"

class Renderer {
public:
    Renderer() = default;

    void Render(const Camera& camera);

    void OnResize(uint32_t width, uint32_t height);

    std::shared_ptr<Pecan::Image> GetFinalImage() const { return m_FinalImage; };

private:
    glm::vec4 TraceRay(const Ray& ray);

private:
    std::shared_ptr<Pecan::Image> m_FinalImage;
    uint32_t* m_ImageData = nullptr;
};


#endif //RAYTRACING_RENDERER_H
