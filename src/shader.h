#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    public:
    // the program ID
    unsigned int ID;
    // constructs reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath){
        // receive the vertex and fragment source code from file path
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensures if stream objects can throw exceptions
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try{
            // opens files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // closes file handlers
            vShaderFile.close();
            fShaderFile.close();
            // converts stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }catch(std::ifstream::failure e){
            std::cout<<"ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ"<<std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // compile shaders
        unsigned int vertex, fragment;
        int success;
        char loginfo[512];
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex,1,&vShaderCode,NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex,"VERTEX");
        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment,1,&fShaderCode,NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment,"FRAGMENT");
        // shader program
        ID = glCreateProgram();
        glAttachShader(ID,vertex);
        glAttachShader(ID,fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID,"PROGRAM");
        // delete shaders since that have ben linked
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }

    void use(){
        glUseProgram(ID);
    }

    // utility uniform fuctions
    void setBool(const std::string &name, bool value) const{
        glUniform1f(glGetUniformLocation(ID,name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const{
        glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
    }
    void setFloat(const std::string &name, float value) const{
        glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
    }
    
    private:
    // utility fuction for checking link/compilation errors
    void checkCompileErrors(unsigned int shader, std::string type){
        int success;
        char loginfo[512];
        if(type != "PROGRAM"){
            glGetShaderiv(shader,GL_COMPILE_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(shader,512,NULL,loginfo);
                std::cout<<"ERROR:SHADER_COMPILATION_FAILED of type:"<<type<<'\n'<<loginfo<<std::endl;
            }
        }else{
            glGetProgramiv(ID,GL_LINK_STATUS,&success);
            if(!success){
                glGetProgramInfoLog(ID,512,NULL,loginfo);
                std::cout<<"ERROR:PROGRAM_LINK_FAILED of type:"<<type<<'\n'<<loginfo<<std::endl;
            }
        }
    }

};
#endif