#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <functional>

#include "shaderutil.h"
#include "entities.h"

//todo:  cooler background/senary, add more pipes on screen at once, keep track of and show score (pipes cleared and time) 


void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600; 

bool canFlap = true;



int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Flappy Bird", NULL,NULL);
    if(!window){
        printf("Failed to load window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, 
    [](GLFWwindow* window, int width, int height)-> void{glViewport(0,0,width,height);});

    gladLoadGL();

    unsigned int program = compileShaderAndLink("resourceFiles/shaders/shader.shader");
    
    
    float vertices[] = {
        //position      texture coords
        -0.5f,0.5f,0.0f,    0.0,1.0, 
         0.5f,0.5f,0.0f,    1.0,1.0, 
         0.5f,-0.5f,0.0f,   1.0,0.0, 
         0.5f,-0.5f,0.0f,   1.0,0.0, 
        -0.5f,-0.5f,0.0f,   0.0,0.0, 
        -0.5f,0.5f,0.0f,    0.0,1.0,
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int bird = loadImage("resourceFiles/img/bird.png", true);
    unsigned int bottomPipe = loadImage("resourceFiles/img/pipe.png", true);
    unsigned int topPipe = loadImage("resourceFiles/img/pipe.png", false);
    unsigned int bg = loadImage("resourceFiles/img/bg.png", true);
    glUseProgram(program);
    setUniInt(program,"Tex",0);


    while(!glfwWindowShouldClose(window)){
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO); 
        drawbg(bg, program, sizeof(vertices)/sizeof(float));
        updateAndDrawBird(bird,program,sizeof(vertices)/sizeof(float));
        updateAndDrawPipes(bottomPipe,topPipe, program,sizeof(vertices)/sizeof(float));
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(program);
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && canFlap){
        flap();
        canFlap = false;
    }
        
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        canFlap = true;
}