
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderClass.h"
#include "cameraClass.h"

#include <iostream>
#include <vector>
#include <random>

// todo make a shake be able to move in 2d them 3d

void frame_buffer_callback(GLFWwindow* window, int width, int height);
void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void snakeMovement(CameraMovement headDirection);

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

// time between currentframe and last frame
float deltaTime;
float lastFrame;
float lastMov = 0;

Camera camera(glm::vec3(0.0f,2.0f,3.0f));
float lastx = SCR_WIDTH /2.0f;
float lasty = SCR_HEIGHT /2.0f;
// makes sure you can't hold down on the keys
bool keyPressed[4] = {false,false,false,false};

GLfloat vertices[] = {
    //   position   //     color      // texture coords
    -0.5f,0.5f,0.5f,   //0.0f,1.0f,0.0f,  0.0,1.0, //0: top left corner front
    0.5f,0.5f,0.5f,    //0.0f,1.0f,0.0f,  1.0,1.0, //1: top right corner front
    -0.5f,-0.5f,0.5f, // 0.0f,1.0f,0.0f,  0.0,0.0, //2: bottom left corner front
    0.5f,-0.5f,0.5f,   //0.0f,1.0f,0.0f,  1.0,0.0, //3: bottom right corner front
    -0.5f,0.5f,-0.5f, // 0.0f,1.0f,0.0f,  1.0,0.0, //4: top left corner back
    0.5f,0.5f,-0.5f,  // 0.0f,1.0f,0.0f,  0.0,0.0, //5: top right corner back
    -0.5f,-0.5f,-0.5f,// 0.0f,1.0f,0.0f,  1.0,1.0, //6: bottom left corner back
    0.5f,-0.5f,-0.5f, // 0.0f,1.0f,0.0f,  0.0,1.0, //7: bottom right corner back
};

GLuint indices[] = {
    0,2,3,0,1,3, //front side
    4,6,7,4,5,7, //back side
    1,3,7,1,5,7, //right side
    0,2,6,0,4,6, //left side
    0,1,5,0,4,5, //top side
    2,3,7,2,6,7 //bottom side
};
// stores the vec positions of the bodyparts
std::vector<glm::vec3> bodyPartPositions;
std::vector<glm::vec3> objects;
// direction moving and stores previous directions and positions
glm::vec3 direction = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 lastdir = direction;
glm::vec3 lastpos = glm::vec3(0.0f,0.0f,0.0f);

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL",NULL,NULL);
    if(!window){
        printf("failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frame_buffer_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    gladLoadGL();
    glEnable(GL_DEPTH_TEST);


    Shader shader("resourceFiles/shaders/default.vert", "resourceFiles/shaders/default.frag");

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    for(float i = 0.0f; i < 5.0f;i++)
        bodyPartPositions.push_back(glm::vec3(0.0f,0.0f,i));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-30.0f,30.0f), col(0.0f,1.0f);
    for(int i = 0; i < 100; i++)
        objects.push_back(glm::vec3(dis(gen), dis(gen), dis(gen)));
    

    while(!glfwWindowShouldClose(window)){
        // timing logic
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        processInput(window);

        glClearColor(0.07f,0.2f,0.33f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // geting alls the camera uniforms set and frawing the element
        shader.Activate();
        glm::mat4 perspective = glm::perspective(glm::radians(camera.zoom),(float)SCR_WIDTH/SCR_HEIGHT,0.1f,100.0f);
        shader.setUni("perspective", perspective);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setUni("view", view);
        glBindVertexArray(VAO);
        if((currentTime - lastMov)/deltaTime >= 2000.0f && glfwGetInputMode(window,GLFW_CURSOR) == GLFW_CURSOR_DISABLED){
            lastMov = currentTime;
            for(unsigned int i = 0; i < bodyPartPositions.size(); i++){
                if(i == 0){//head
                    //stores the curent position and moves the head and camera in the direction
                    lastpos = bodyPartPositions[0];
                    bodyPartPositions[0] += direction;
                } else{
                    // sets the position of the body to the previous position of the body in front of it
                    glm::vec3 temp = bodyPartPositions[i];
                    bodyPartPositions[i] = lastpos;
                    lastpos = temp;
                    // sets the camera position
                    if(i == 1){
                        if(direction.y == 0.0f){
                            camera.Position = bodyPartPositions[i] + glm::vec3(0.0f,2.0f,0.0f);
                        } else if(direction.y >= 0.0f){
                            direction.x == 0.0f ? camera.Position = bodyPartPositions[i] + glm::vec3(0.0f,0.0f,2.0f)
                            :camera.Position = bodyPartPositions[i] + glm::vec3(-2.0f,0.0f,0.0f);
                        } else if(direction.y <= 0.0f){
                            direction.x == 0.0f ? camera.Position = bodyPartPositions[i] + glm::vec3(0.0f,0.0f,-2.0f)
                            :camera.Position = bodyPartPositions[i] + glm::vec3(2.0f,0.0f,0.0f);
                        }
                    }
                }
            }
        }
        for(auto pos: bodyPartPositions){
            // updates the model and draws the part
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,pos);
            shader.setUni("model",model);
            shader.setUni("color", glm::vec3(0.0f,1.0f,0.0f));
            glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int),GL_UNSIGNED_INT,0);
        }
        
        // other objects
        for(auto o: objects){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, o);
            shader.setUni("model",model);
            shader.setUni("color",glm::vec3(col(gen), col(gen), col(gen)));
            glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int),GL_UNSIGNED_INT,0);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1,&EBO);
    shader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void frame_buffer_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}
// callsed when the mouse moves and calculates how much the camera should move
void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos){
    xpos = static_cast<float>(xpos);
    ypos = static_cast<float>(ypos);
    if(camera.firstMouse){
        lastx = xpos;
        lasty = ypos;
        camera.firstMouse = false;
    }
    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos;

    lastx = xpos;
    lasty = ypos;
    if(glfwGetInputMode(window,GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        camera.processMouseMov(xoffset,yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.processMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window,GLFW_KEY_W)== GLFW_PRESS && !keyPressed[0]){
        snakeMovement(UP);
        camera.processkey(UP);
        keyPressed[0] = true;
    }
    if(glfwGetKey(window,GLFW_KEY_W)== GLFW_RELEASE)
        keyPressed[0] = false;
        
    if(glfwGetKey(window,GLFW_KEY_S)== GLFW_PRESS && !keyPressed[1]){
        snakeMovement(DOWN);
        camera.processkey(DOWN);
        keyPressed[1] = true;
    }
    if(glfwGetKey(window,GLFW_KEY_S)== GLFW_RELEASE)
        keyPressed[1] = false;
        
    if(glfwGetKey(window,GLFW_KEY_A)== GLFW_PRESS && !keyPressed[2]){
        snakeMovement(LEFT);
        camera.processkey(LEFT);
        keyPressed[2] = true;
    }
    if(glfwGetKey(window,GLFW_KEY_A)== GLFW_RELEASE)
        keyPressed[2] = false;
        
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS && !keyPressed[3]){
        snakeMovement(RIGHT);
        camera.processkey(RIGHT);
        keyPressed[3] = true;
    }
    if(glfwGetKey(window,GLFW_KEY_D)== GLFW_RELEASE)
        keyPressed[3] = false;
    
    if(glfwGetKey(window,GLFW_KEY_P) == GLFW_PRESS)
        glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if(glfwGetKey(window,GLFW_KEY_R) == GLFW_PRESS)
        glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void snakeMovement(CameraMovement headDirection){
    glm::vec3 temp = direction;
    switch (headDirection){
    case LEFT:
        //turns the snake counter clock wise from its current
        if(direction.y == 0.0f){
            direction.x == 0.0f ? direction =glm::vec3(direction.z,0.0f,0.0f) :
            direction =glm::vec3(0.0f,0.0f,-(direction.x));
        // tuns the snake counter clockwise from it's direction when it was moving in the x/z direction
        }else{
            lastdir.x == 0.0f ? direction =glm::vec3(lastdir.z,0.0f,0.0f) :
            direction =glm::vec3(0.0f,0.0f,-(lastdir.x));
        }
        break;
    // same as LEFT but counter clockwise instead of clock wise
    case RIGHT:
        if(direction.y == 0.0f){
            direction.x == 0.0f ? direction =glm::vec3(-(direction.z),0.0f,0.0f) :
            direction =glm::vec3(0.0f,0.0f,direction.x);
        } else{
            lastdir.x == 0.0f ? direction =glm::vec3(-(lastdir.z),0.0f,0.0f) :
            direction =glm::vec3(0.0f,0.0f,lastdir.x);
        }
        break;

    case UP:
        // if not moving in y axis, then it goes up
        if(direction.y == 0.0f){
            direction = glm::vec3(0.0f,1.f,0.0f);
        // goes in the opposite direction from the last time it was moving in the x/z direction
        } else if(direction.y > 0.0f){
            direction = lastdir * -1.0f;
        // goes in the same direction from the last time it was moving in the x/z direction
        } else if(direction.y < 0.0f){
            direction = lastdir;
        }
        break;
    // same as up but reversed
    case DOWN:
        if(direction.y == 0.0f){
            direction = glm::vec3(0.0f,-1.f,0.0f);
        }else if(direction.y > 0.0f){
            direction = lastdir;
        } else if(direction.y < 0.0f){
            direction = lastdir * -1.0f;
        }
        break;
    default:
        break;
    }
    lastdir = temp;
}