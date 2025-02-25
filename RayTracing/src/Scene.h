//
// Created by Leonard Chan on 6/16/24.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <glm/glm.hpp>

#include <vector>

struct Material {
    glm::vec3 Albedo{ 1.0f };
    float Roughness = 1.0f;
    float Metallic = 0.0f;
};

struct Sphere {
    glm::vec3 Position{0.0f};
    float Radius = 0.5f;

    int MaterialIndex = 0;
};

struct Scene {
    std::vector<Sphere> Spheres;
    std::vector<Material> Materials;
};

#endif //RAYTRACING_SCENE_H
