//
// Created by Leonard Chan on 6/16/24.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <glm/glm.hpp>

#include <vector>

struct Sphere {
    glm::vec3 Position{0.0f};
    float Radius = 0.5f;

    glm::vec3 Albedo{1.0f};
};

struct Scene {
    std::vector<Sphere> Spheres;
};

#endif //RAYTRACING_SCENE_H
