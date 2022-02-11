#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 400 core\n"
    "layout (location = 0) in vect3 aPos\n"
    "void main() {\n"
    "   gl_Position = vect4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 400 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
    "}\0";

int main(){
    // glfw initialization and config
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"LearnOpengl",NULL,NULL);
    if(window == NULL){
        std::cout<<"Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        return 1;
    } 
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glad: load all pointer fuctions 
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to initialize glad"<<std::endl;
        glfwTerminate();
        return 1;
    }
    // builds and compiles shader program
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // checks for errors
    int success;
    char loginfo[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512,NULL,loginfo);
        std::cout<<"ERROR:SHADER:VERTEX:COMPILATIONS_FAILED"<<loginfo<<std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    // checking compile errors
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,loginfo);
        std::cout<<"ERROR:SHADER:FRAGMENT:COMPILATIONS_FAILED"<<loginfo<<std::endl;
    }
    float vertices[] = {
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        0.0f,0.5f,0.0f
    };
    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // render loop
    while(!glfwWindowShouldClose(window)){
        // inputs
        processInput(window);

        // render
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // checks and calls evens and swaps the buffer
        glfwSwapBuffers(window);
        glfwPollEvents();

        
    }
    glfwTerminate();
    return 0;
}

// proceses all input:  keys pressed/released by the player
void processInput(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
}

// glfw: whenever window size changes, this fuction executes 
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}