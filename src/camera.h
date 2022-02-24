#ifndef CAMERA_H
#define CAMERA_H
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

// defines several posable camera movements
enum Camera_Movement{
    FORWARD,
    BACKWORD,
    LEFT,
    RIGHT
};

// defult camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// an abstract camera class that processes inputs and calculates the Euler Angles, Vectors and Matrices for OpenGL

class Camera {
public:
    // camera atrributes
    glm::vec3 Position, Front, Up, Right, WorldUp;
    //Euler Angles
    float Yaw, Pitch;
    //camera options
    float movementSpeed, mouseSensitivity,Zoom;

    // constructor for vectors
    Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f), float yaw = YAW, float pitch = PITCH) : 
    Front(glm::vec3(0.0f,0.0f,-1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameravectors();
    }
    // constructor for scalar values
    Camera(float posX, float posY, float posZ, float upX,float upY, float upZ,float yaw,float pitch) : 
    Front(glm::vec3(0.0f,0.0f,-1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX,posY,posZ);
        WorldUp = glm::vec3(upX,upY,upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameravectors();
    }
    // returns the view matrix calculated by the Euler angles and lookat matrix
    glm::mat4 GetViewMatrix(){
        return glm::lookAt(Position,Position+Front,Up);
    }
    // proceses inputs from keyboard like system. Accepts input parameter in the form of camera defined ENUM
    void processKeyboard(Camera_Movement direction, float deltaTime){
        float velocity = movementSpeed * deltaTime;
        if(direction == FORWARD)
            Position += Front * velocity;
        if(direction == BACKWORD)
            Position -= Front * velocity;
        if(direction == RIGHT)
            Position += Right * velocity;
        if(direction == LEFT)
            Position -= Right * velocity;
    }
    // process mouse inputs 
    void processMouseInput(float xoffset, float yoffset, GLboolean constrainPitch = true){
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // stops pitch from going out of bounds. screen would flip otherwise
        if(constrainPitch){
            if(Pitch < -89.0f)
                Pitch = -89.0f;
            if(Pitch > 89.0f)
                Pitch = 89.0f;
        }
        updateCameravectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void mouseScrollOffset(float yoffset){
        Zoom -= yoffset;
        if(Zoom < 1.0f)
            Zoom = 1.0f;
        if(Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
// calculate the front vector from cameras updated Euler angles
    void updateCameravectors(){
        // calculate new vector
        glm::vec3 front = glm::vec3(
            cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),//direction.x
            sin(glm::radians(Pitch)),//direction.y
            sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))//direction.z
        );
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right,Front));
    }
};
#endif