#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum CameraMovement{FRONT, BACK, RIGHT,LEFT,UP,DOWN};

class Camera{
public:
    glm::vec3 Position, Up, Front, Right, WorldUp;

    float baseYaw, basePitch, yaw, pitch, movSpeed, sensitivity, zoom;
    bool firstMouse;

    Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f)){
        Position = position;
        Up = glm::vec3(0.0f,1.0f,0.0f);
        WorldUp = Up;
        Front = glm::vec3(0.0f,0.0f,-1.0f);
        movSpeed = 2.5f;
        sensitivity = 0.1f;
        zoom =45.0f;
        baseYaw = -90.0f;
        yaw = baseYaw;
        basePitch = 0.0f;
        pitch = basePitch;
        firstMouse = true;
        updateCamera();
    }
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    void processkey(CameraMovement direction) {
        switch (direction) {
        case LEFT:
            baseYaw -= 90.0f;
            yaw = baseYaw;
            break;
        case RIGHT:
            baseYaw += 90.0f;
            yaw = baseYaw;
            break;
        case UP:
            basePitch += 90.0f;
            pitch = basePitch;
            break;
        case DOWN:
            basePitch -= 90.0f;
            pitch = basePitch;
            break;
        default:
            break;
        }
        updateCamera();
    }
    void processMouseMov(float xoffset, float yoffset){
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw += xoffset;
        pitch += yoffset;
        
        // if(yaw < 0.0f) yaw = 0.0f;
        // if(yaw > 180.0f) yaw = 180.0f;
        // if(pitch < basePitch - 60.0f) pitch = basePitch - 60.0f;
        // if(pitch > basePitch + 60.0f) pitch = basePitch + 60.0f;

        updateCamera();
    }
    void processMouseScroll(float yoffset){
        if(zoom > 1.0f && zoom < 45.0f)
            zoom -= yoffset;

    }
private:
    void updateCamera(){
        glm::vec3 front = glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        );
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front,WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));

    }
};
#endif