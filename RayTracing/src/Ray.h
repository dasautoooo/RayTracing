//
// Created by Leonard Chan on 6/8/24.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include <glm/glm.hpp>

struct Ray {
    glm::vec3 Origin;
    glm::vec3 Direction;
};

#endif //RAYTRACING_RAY_H
