//
// Created by Leonard Chan on 6/2/24.
//

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Pecan/Input/Input.h"



Camera::Camera(float verticalFOV, float nearClip, float farClip)
    : m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip) {

    m_ForwardDirection = glm::vec3(0, 0, -1);
    m_Position = glm::vec3(0, 0, 3);
}

bool Camera::OnUpdate(float ts) {
    glm::vec2 mousePos = Pecan::Input::GetMousePosition();
    glm::vec2 delta = (mousePos - m_LastMousePosition) * 0.002f;
    m_LastMousePosition = mousePos;

    if (!Pecan::Input::IsMouseButtonDown(Pecan::MouseButton::Right)) {
        Pecan::Input::SetCursorMode(Pecan::CursorMode::Normal);
        return false;
    }

    Pecan::Input::SetCursorMode(Pecan::CursorMode::Locked);

    bool moved = false;

    constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
    // Using the right-hand rule to determine the point direction of the result vector:
    // The right-hand rule states that if you point your index finger along vector A and
    // your middle finger along vector B, then your thumb will point along vector C.
    glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

    float speed = 5.0f;

    if (Pecan::Input::IsKeyDown(Pecan::KeyCode::W)) {
        m_Position += m_ForwardDirection * speed * ts;
        moved = true;
    } else if (Pecan::Input::IsKeyDown(Pecan::KeyCode::S)) {
        m_Position -= m_ForwardDirection * speed * ts;
        moved = true;
    } else if (Pecan::Input::IsKeyDown(Pecan::KeyCode::A)) {
        m_Position -= rightDirection * speed * ts;
        moved = true;
    } else if (Pecan::Input::IsKeyDown(Pecan::KeyCode::D)) {
        m_Position += rightDirection * speed * ts;
        moved = true;
    } else if (Pecan::Input::IsKeyDown(Pecan::KeyCode::Q)) {
        m_Position -= upDirection * speed * ts;
        moved = true;
    } else if (Pecan::Input::IsKeyDown(Pecan::KeyCode::E)) {
        m_Position += upDirection * speed * ts;
        moved = true;
    }

    if (delta.x != 0.0f || delta.y != 0.0f) {
        float pitchDelta = delta.y * GetRotationSpeed();
        float yawDelta = delta.x * GetRotationSpeed();

        // Calculate the delta of entire rotation, result in a new forward direction
        glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection), glm::angleAxis(-yawDelta, glm::vec3(0.0f, 1.0f, 0.0f))));
        m_ForwardDirection = glm::rotate(q, m_ForwardDirection);

        moved = true;
    }

    if (moved) {
        RecalculateView();
        RecalculateRayDirections();
    }

    return moved;
}

void Camera::OnResize(uint32_t width, uint32_t height) {
    if (width == m_ViewportWidth && height == m_ViewportHeight)
        return;

    m_ViewportWidth = width;
    m_ViewportHeight = height;

    RecalculateProjection();
    RecalculateRayDirections();
}

float Camera::GetRotationSpeed() {
    return 0.3f;
}

void Camera::RecalculateView() {
    m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0.0f, 1.0f, 0.0f));
    m_InverseView = glm::inverse(m_View);
}

void Camera::RecalculateRayDirections() {
    m_RayDirections.resize(m_ViewportWidth * m_ViewportHeight);

    for (uint32_t y = 0; y < m_ViewportHeight; y++) {
        for (uint32_t x = 0; x < m_ViewportWidth; x++) {
            glm::vec2 coord = glm::vec2((float)x / (float)m_ViewportWidth, (float)y / (float)m_ViewportHeight);
            coord = coord * 2.0f - 1.0f; // -1 -> 1

            glm::vec4 target = m_InverseProjection * glm::vec4(coord.x, coord.y, 1, 1);
            // Convert to world space
            glm::vec3 rayDirection = glm::vec3(m_InverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World spa
            m_RayDirections[x + y * m_ViewportWidth] = rayDirection;
        }
    }
}

void Camera::RecalculateProjection() {
    // Create a new perspective view matrix
    m_Projection = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_NearClip, m_FarClip);
    m_InverseProjection = glm::inverse(m_Projection);
}
