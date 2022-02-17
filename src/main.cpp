#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"

#include "shader.h"

#include <iostream>
#include <math.h>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, float &movementX, float &movementY, float &size);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    Shader shader("src/3.3.shader.v","src/3.3.shader.f");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------ 
    float size = 0.3f;
    float vertices[] = {
        // positions       //color      //texture coords
        size, size,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f, //top right
        size,-size,0.0f, 0.0f,1.0f,0.0f, 1.0f,0.0f, //bottom right
        -size,-size,0.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f, //bottom left
        -size,size,0.0f, 1.0f,1.0f,0.0f, 0.0f,1.0f //top left
        
    };
    unsigned int indicies[]={
        0,1,3,//first triangle
        1,2,3 //second triangle
    };

    unsigned int VBO[3], VAO[3], EBO[3];
    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(3,EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
    // uncomment this call to draw in wireframe polygons.
    // *glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // load and create textures
    // ------------------------
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    // texture wraping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    // texture filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // mipmap
    
    // load image textures and mipmap
    int width, height, rnChannels;
    unsigned char* data = stbi_load("img/container.jpg", &width, &height, &rnChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,0, GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else{
        std::cout<<"FAILED TO LOAD TEXTURE"<<std::endl;
    }
    stbi_image_free(data);

    // render loop
    // -----------
    float movementX = 0.0f;
    float movementY = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window,movementX,movementY,size);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D,texture);
        // draws
        shader.use();
        int vertexLocation = glGetUniformLocation(shader.ID,"offset");
        glUniform3f(vertexLocation,movementX,movementY,0.0f);
        glBindVertexArray(VAO[0]); 
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        // glBindVertexArray(0); // no need to unbind it every time 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(3, VAO);
    glDeleteBuffers(3, VBO);
    glDeleteBuffers(3,EBO);
    

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, float &movementX, float &movementY, float &size)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
        
    if(glfwGetKey(window,GLFW_KEY_RIGHT)==GLFW_PRESS){
        movementX += 0.001f;
        if(movementX > 1.0f + size){movementX = -1.0f - size;}
    } 
    if(glfwGetKey(window,GLFW_KEY_LEFT)==GLFW_PRESS){
        movementX -= 0.001f;
        if(movementX < -1.0f - size){movementX = 1.0f + size;}
    }
    if(glfwGetKey(window,GLFW_KEY_UP)==GLFW_PRESS ){
        movementY += 0.001f;
        if(movementY > 1.0f + size){movementY = -1.0f - size;}
    } 
    if(glfwGetKey(window,GLFW_KEY_DOWN)==GLFW_PRESS){
        movementY -= 0.001f;
        if(movementY < -1.0f - size){movementY = 1.0f + size;}
    }
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}