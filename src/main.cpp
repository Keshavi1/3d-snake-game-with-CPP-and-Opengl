
/** 
 * todo: Fix the load texture method in Cube
 * todo: improve colision
 * todo: make enemies
 * todo: make player able to shoot bullets
 * 
 * !Bugs: the texture method, player teleports to other objects when they collide with an object some times.
 */ 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders/shader.h"
#include "player.h"
#include "cube.h"


#include <iostream>
#include <math.h>
#include <vector>
#include <random>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callBack(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float size = 0.2f;
float mixValue = 0.0f;
// camera
Player player(glm::vec3(0.0f,0.0f,3.0f));
float lastX = SCR_WIDTH/2.0, lastY=SCR_HEIGHT/2.0;
bool firstMouse = true;
// timing
float deltaTime = 0.0f; // time between curent frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window,scroll_callBack);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    Shader shader("src/shaders/shader.v","src/shaders/shader.f");

    // creates a bunch of randomly sized, positioned, and collored cubes
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDis(0.0, 100.0), sizeDis(0.5,2.0), colDis(0.0,1.0);
    std::vector<Cube> cubes;
    Cube cube(155.0f,1.0f,155.0f, 0.2f, 0.8f, 0.1f);
    cube.position = glm::vec3(-100.0f,-2.0f,0.0f);
    cubes.push_back(cube);
    for(float i = 0.0f; i < 10.0f; i++){
        Cube cube(3.0f,2.0f,2.0f, colDis(gen), colDis(gen), colDis(gen));
        cube.position = glm::vec3(0.0f + (i*10),-1.0f,0.0f); 
        cubes.push_back(cube);
    }
    
    
    player.setCollisionGroup(cubes, false);
    
    // unsigned int texture1;
    // cube.loadTexture("img/container.jpg", false, texture1);
    

    // load and create textures
    // ------------------------
    unsigned int texture1, texture2;

    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    // texture wraping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    // texture filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
    // load image textures and mipmap
    int width, height, rnChannels;
    unsigned char* data = stbi_load("img/container.jpg", &width, &height, &rnChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,0, GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else{
        std::cout<<"FAILED TO LOAD CONTAINER"<<std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    // texture wraping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    // texture filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("img/awesomeface.png", &width, &height, &rnChannels,0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout<<"FAILED TO LOAD AWESOMEFACE"<<std::endl;
    }
    stbi_image_free(data);

    // tells OpenGl for each sampler which texture unit it belongs to
    shader.use();
    shader.setUniform("texture1",0);
    shader.setUniform("texture2",1);

    
    // render loop
    // -----------
    
    while (!glfwWindowShouldClose(window))
    {   
        // pre frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        
        

        // transformation fuctions for the position, size and rotation of the image
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(player.Zoom), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
        shader.setUniform("projection", projection);
        // camera/ view transforplayer
        glm::mat4 view = player.GetViewMatrix();
        shader.setUniform("view", view);
        
        shader.setUniform("mixValue",mixValue); 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        for(auto cube: cubes){
            cube.draw(shader);
        }
        player.checkCollisions();
        player.applyGravity();
        
        
        
        // glBindVertexArray(0); // no need to unbind it every time 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{ 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    // transitions the player in a direction
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)
        player.processKeyboard(FORWARD, deltaTime, glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS); 

    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS)
        player.processKeyboard(BACKWORD, deltaTime, glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS);

    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS)
        player.processKeyboard(LEFT, deltaTime, glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS);

    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)
        player.processKeyboard(RIGHT, deltaTime, glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS);

    if(glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS)
        player.processKeyboard(UP, deltaTime);

    // changing the mix of over laping textures
    if(glfwGetKey(window,GLFW_KEY_UP)==GLFW_PRESS){
        if(mixValue < 1.0f){mixValue += 0.001f;}
    }
    if(glfwGetKey(window,GLFW_KEY_DOWN)==GLFW_PRESS){
        if(mixValue > 0.0f){mixValue -= 0.001f;}
    }
    
}
// when ever mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn){

    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);
    if(firstMouse){
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    player.processMouseInput(xOffset,yOffset);
    
}
// whenever the mouse wheel scrolls, this callback is called
void scroll_callBack(GLFWwindow* window, double xOffset, double yOffset){
    player.mouseScrollOffset(static_cast<float>(yOffset));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}