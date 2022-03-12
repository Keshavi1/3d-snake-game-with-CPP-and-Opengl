#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum CameraMovement{RIGHT,LEFT,UP,DOWN};

class Camera{
public:
    glm::vec3 Position, Up, Front, Right, WorldUp;

    float baseYaw, basePitch, yaw, pitch, sensitivity, zoom;
    bool firstMouse;

    Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f)){
        Position = position;
        Up = glm::vec3(0.0f,1.0f,0.0f);
        WorldUp = Up;
        Front = glm::vec3(0.0f,0.0f,-1.0f);
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
    // chages the direction of the camera in the appropriate direction
    void processkey(CameraMovement direction) {
        switch (direction) {
        case LEFT:
            baseYaw -= 90.0f;
            yaw = baseYaw;
            basePitch = 0.0f;
            break;
        case RIGHT:
            baseYaw += 90.0f;
            yaw = baseYaw;
            basePitch = 0.0f;
            break;
        case UP:
            basePitch = 89.0f;
            pitch = basePitch;
            break;
        case DOWN:
            basePitch = -89.0f;
            pitch = basePitch;
            break;
        default:
            break;
        }
    }
    // moves the camera with the mouse
    void processMouseMov(float xoffset, float yoffset){
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw += xoffset;
        pitch += yoffset;
        

        if(pitch < -89.0f) pitch = -89.0f;
        if(pitch > 89.0f) pitch = 89.0f;

        updateCamera();
    }
    void processMouseScroll(float yoffset){
        if(zoom > 1.0f && zoom < 60.0f)
            zoom -= yoffset;

    }
private:
// sets the direction the camera is facing
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
