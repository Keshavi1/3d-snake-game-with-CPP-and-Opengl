#ifndef PLAYER_H
#define PLATER_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "cube.h"
#include "shaders/shader.h"

#include <vector>

// defines several posable player movements
enum Player_Movement{
    FORWARD,
    BACKWORD,
    LEFT,
    RIGHT,
    UP
};

// defult camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 7.5f;
const float SENSITIVITY = 0.15f;
const float ZOOM = 45.0f;

// an abstract camera class that processes inputs and calculates the Euler Angles, Vectors and Matrices for OpenGL

class Player {
public:
    // camera atrributes
    glm::vec3 Position, Front, movementFront, Up, Right, WorldUp;
    //Euler Angles
    float Yaw, Pitch;
    //camera options
    float movementSpeed, mouseSensitivity,Zoom;

    Player_Movement lastDirection;
    float velocityY;
    std::vector<Cube> CollisionGroup;
    std::vector<Cube> EnemyCollisionGroup;
    bool collidingX, collidingY, collidingZ;

    // constructor for vectors
    Player(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f), float yaw = YAW, float pitch = PITCH) : 
    Front(glm::vec3(0.0f,0.0f,-1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        movementFront = Front;
        Yaw = yaw;
        Pitch = pitch;
        collidingX = collidingY = collidingZ = false;
        updateCameravectors();
    }
    // constructor for scalar values
    Player(float posX, float posY, float posZ, float upX,float upY, float upZ,float yaw,float pitch) : 
    Front(glm::vec3(0.0f,0.0f,-1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX,posY,posZ);
        WorldUp = glm::vec3(upX,upY,upZ);
        movementFront = Front;
        Yaw = yaw;
        Pitch = pitch;
        collidingX = collidingY = collidingZ = false;
        updateCameravectors();
    }
    void setCollisionGroup(std::vector<Cube> group, bool deadly){
        if(!deadly){
            CollisionGroup = group;
        } else{
            EnemyCollisionGroup = group;
        }
    }
    void checkCollisions(){
        checkColX();
        checkColY();
        checkColZ();
    }
    void applyGravity(){
        if(!collidingY && velocityY < 10.0f)
            velocityY += 0.01f;
        Position.y -= velocityY * 0.001f;
        if(Position.y <-10.0f)
            Position = glm::vec3(0.0f,0.0f,0.0f);
    }
    // returns the view matrix calculated by the Euler angles and lookat matrix
    glm::mat4 GetViewMatrix(){
        return glm::lookAt(Position,Position+Front,Up);
    }
    
    // proceses inputs from keyboard like system. Accepts input parameter in the form of camera defined ENUM
    void processInput(Player_Movement direction, float deltaTime, GLboolean sprinting = false){
        // normalize speed fps
        float velocity = movementSpeed * deltaTime;
        if(sprinting)
            velocity *=2;
        if(direction == FORWARD){
            if(!collidingZ){
                Position += movementFront * velocity;
            } else if(lastDirection != FORWARD){
                Position += movementFront * velocity;
            } 
            lastDirection = FORWARD;
        }         
        if(direction == BACKWORD){
            if(!collidingZ){
                Position -= movementFront * velocity;
            } else if(lastDirection != BACKWORD){
                Position -= movementFront * velocity;
            } 
            lastDirection = BACKWORD;
        }   
        if(direction == RIGHT){
            if(!collidingX){
                Position += Right * velocity;
            } else if(lastDirection != RIGHT){
                Position += Right * velocity;
            } 
            lastDirection = RIGHT;
        }        
        if(direction == LEFT){
            if(!collidingX){
                Position -= Right * velocity;
            } else if(lastDirection != LEFT){
                Position -= Right * velocity;
            } 
            lastDirection = LEFT;
        }
            
        if(direction == UP && collidingY){
            velocityY = -10.0f;
            Position.y += 0.01f;
        }
            
    }
    // process input from mouse buttons
    Cube processInput(){
        Cube bullet(0.1f,0.1f,0.5f,0.8f,0.3f,0.2f);
        bullet.setBulletVals(Position,Front, Up);
        return bullet;
    }
    // process mouse movement for camera 
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true){
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // stops pitch from going out of bounds. screen would flip otherwise
        if(constrainPitch){
            if(Pitch < -45.0f)
                Pitch = -45.0f;
            if(Pitch > 45.0f)
                Pitch = 45.0f;
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
        // calculate new vector for the camera
        glm::vec3 front = glm::vec3(
            cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),//direction.x
            sin(glm::radians(Pitch)),//direction.y
            sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))//direction.z
        );
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right,Front));
        // calculates the direction of the x and z vectors
        front = glm::vec3(
            cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),//direction.x
            sin(glm::radians(0.0f)),//direction.y
            sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))//direction.z
        );
        movementFront = glm::normalize(front);
    }
    // checks collision in the x direction
    void checkColX(){
        for(auto obj: CollisionGroup){
            collidingX = false;
            float z = Position.z, oz = obj.Position.z, y = Position.y, oy = obj.Position.y;
            //checks if the other coordinates are in a position were colision is possible;
            if(y > oy && y < oy + obj.SizeY && z > oz && z < oz + obj.SizeZ){
                // checks the colision on both right and left sides
                if(Position.x + 1.0f >= obj.Position.x && !(Position.x > obj.Position.x + obj.SizeX)){
                    Position.x = obj.Position.x - 1.0f;
                    collidingX = true;
                    break;
                } else if(Position.x - 1.0f<= obj.Position.x + obj.SizeX && !(Position.x + 1.0f < obj.Position.x)){
                    Position.x = obj.Position.x + obj.SizeX + 1.0f;
                    collidingX = true;
                    break;
                }
            }
                
        }
    }
    // gets colision in the y direction
    void checkColY(){
        for(auto obj: CollisionGroup){
            collidingY = false;
            float x = Position.x, ox = obj.Position.x, z = Position.z, oz = obj.Position.z;
            //checks if the other coordinates are in a position were colision is possible;
            if(x > ox && x < ox + obj.SizeX && z > oz && z < oz + obj.SizeZ){
                // checks on both top and bottom sides 
                if(Position.y - 1.0f<= obj.Position.y + obj.SizeY && !(Position.y < obj.Position.y)){
                    Position.y = obj.Position.y + obj.SizeY + 1.0f;
                    collidingY = true;
                    velocityY = 0.0f;
                    break;
                } else if(Position.y + 1.0f > obj.Position.y && !(Position.y > obj.Position.y + obj.SizeY)){
                    Position.y = obj.Position.y - 1.0f;
                    velocityY *= -1;
                    break;
                }
            }
                
                
        }
    }
    
    void checkColZ(){
        for(auto obj: CollisionGroup){
            collidingZ = false;
            float x = Position.x, ox = obj.Position.x, y = Position.y, oy = obj.Position.y;
            //checks if the other coordinates are in a position were colision is possible;
            if(x > ox && x < ox + obj.SizeX && y > oy && y < oy + obj.SizeY){
                // checks on both front and back sides
                if(Position.z + 1.0f >= obj.Position.z && !(Position.z > obj.Position.z + obj.SizeZ)){
                    Position.z = obj.Position.z - 1.0f;
                    collidingZ = true;
                    break;
                } else if(Position.z - 1.0f<= obj.Position.z + obj.SizeZ && !(Position.z + 1.0f < obj.Position.z)){
                    Position.z = obj.Position.z + obj.SizeZ + 1.0f;
                    collidingZ = true;
                    break;
                }
            }
                
                
        }
    }
    

    
    
};


#endif