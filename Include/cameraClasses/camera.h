#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 4.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float FIELDOFVIEW = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float FieldOfView;

    // constructor with vectors
    Camera(const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), FieldOfView(FIELDOFVIEW)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), FieldOfView(FIELDOFVIEW)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        else if (direction == LEFT)
            Position -= Right * velocity;
        else if (direction == RIGHT)
            Position += Right * velocity;
        else if (direction == BACKWARD)
            Position -= Front * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        FieldOfView -= (float)yoffset;
        if (FieldOfView < 1.0f)
            FieldOfView = 1.0f;
        if (FieldOfView > 45.0f)
            FieldOfView = 45.0f;
    }

private:
    float lastYaw, lastPitch;
    float sinPitch, cosPitch, sinYaw, cosYaw;
    bool needUpdate;

    void updateTrigValuesIfNeeded() {
        if (Yaw != lastYaw || Pitch != lastPitch) {
            sinYaw = sin(glm::radians(Yaw));
            cosYaw = cos(glm::radians(Yaw));
            sinPitch = sin(glm::radians(Pitch));
            cosPitch = cos(glm::radians(Pitch));

            lastYaw = Yaw;
            lastPitch = Pitch;
            needUpdate = true;
        }
    }

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        updateTrigValuesIfNeeded();

        if (!needUpdate) {
            return;
        }

        // Utilisez sinYaw, cosYaw, sinPitch, et cosPitch au lieu de recalculer
        glm::vec3 front{};
        front.x = cosPitch * cosYaw;
        front.y = sinPitch;
        front.z = cosPitch * sinYaw;
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));

        needUpdate = false;
    }
};
#endif