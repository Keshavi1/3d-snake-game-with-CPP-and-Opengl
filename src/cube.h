#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>

#include "shaders/shader.h"

#include <iostream>

class Cube {
public:
    unsigned int VAO, VBO, EBO;
    unsigned int texture1, texture2;
    bool Rotate;
    glm::vec3 position;
    // constructer for creating a cube 
    Cube(float sizeX, float sizeY, float sizeZ, float r, float g, float b, bool rotate = false){
        float vertices[] = {
            //position        | color   | texture cords
            0.0f, 0.0f, 0.0f,   r, g, b, 0.0f, 1.0f, //0, left,top,front
            sizeX, 0.0f, 0.0f,  r, g, b, 1.0f, 1.0f, //1, right, top, front
            0.0f, sizeY, 0.0f,  r, g, b, 0.0f, 0.0f, //2, left, bottom, front
            sizeX, sizeY,0.0f,  r, g, b, 1.0f, 0.0f, //3, right, bottom, front
            0.0f, 0.0f, sizeZ,  r, g, b, 0.0f, 1.0f, //4, left,top,back
            sizeX, 0.0f,sizeZ,  r, g, b, 1.0f, 1.0f, //5, right, top, back
            0.0f,sizeY,sizeZ,   r, g, b, 0.0f, 0.0f, //6, left, bottom, back
            sizeX,sizeY,sizeZ,  r, g, b, 1.0f, 0.0f //7, right, bottom, back
        };
        unsigned int indices[] ={
            1,3,0,3,2,0,//front face
            1,5,7,1,3,7,// right face
            0,4,6,0,2,6,// left face
            0,1,5,0,4,5,// top face
            2,3,7,2,6,7,// bottom face
            4,5,7,4,6,7// back face
        };
    
        // generates the buffers and binds 
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE,8*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3,GL_FLOAT, GL_FALSE,8*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2,GL_FLOAT, GL_FALSE,8*sizeof(float), (void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        Rotate = rotate;

        position = glm::vec3(0.0f,0.0f,0.0f);

    }
    // !very buggy
    void loadTexture(const char *Filename, bool transparent, unsigned int &texture){
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, rnChannels;
        unsigned char* data = stbi_load(Filename,&width,&height,&rnChannels,0);
        if(data){
            glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_INT, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else{std::cout<<"TEXTURE FAILED TO LOAD"<<std::endl;}
        stbi_image_free(data);
    }

    void draw(Shader &shader){
        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        if(Rotate)
            model = glm::rotate(model, glm::radians(20.0f * (float)glfwGetTime()),glm::vec3(1.0f,0.5f,0.5f));
        shader.setUniform("model", model);
        glDrawElements(GL_TRIANGLES,36, GL_UNSIGNED_INT, 0);
    }


};

#endif