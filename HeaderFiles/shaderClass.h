#ifndef SHASER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>



class Shader{
public:
    GLuint ID;
    Shader(const char* vertexPath, const char* fragmentPath){
        std::string vertexCode = get_file_contents(vertexPath);
        std::string fragmentCode = get_file_contents(fragmentPath);

        const char* vSource = vertexCode.c_str();
        const char* fSource = fragmentCode.c_str();

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex,1, &vSource,NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment,1, &fSource,NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
    }

    void Activate(){
        glUseProgram(ID);
    }
    void Delete(){
        glDeleteProgram(ID);
    }
    // utility fuctions for setting the uniforms
    void setUni(const std::string &name, bool value) const{
        glUniform1i(glGetUniformLocation(ID,name.c_str()), (int)value);
    }
    void setUni(const std::string &name, int value) const{
        glUniform1i(glGetUniformLocation(ID,name.c_str()), value);
    }
    void setUni(const std::string &name, float value) const{
        glUniform1f(glGetUniformLocation(ID,name.c_str()), value);
    }
    void setUni(const std::string &name, const glm::vec2 &vec) const{
        glUniform2fv(glGetUniformLocation(ID,name.c_str()), 1, &vec[0]);
    }
    void setUni(const std::string &name, const glm::vec3 &vec) const{
        glUniform3fv(glGetUniformLocation(ID,name.c_str()), 1, &vec[0]);
    }
    void setUni(const std::string &name, const glm::vec4 &vec) const{
        glUniform4fv(glGetUniformLocation(ID,name.c_str()), 1, &vec[0]);
    }
    void setUni(const std::string &name, const glm::mat2 &mat) const{
        glUniformMatrix2fv(glGetUniformLocation(ID,name.c_str()), 1,GL_FALSE, &mat[0][0]);
    }
    void setUni(const std::string &name, const glm::mat3 &mat) const{
        glUniformMatrix3fv(glGetUniformLocation(ID,name.c_str()), 1,GL_FALSE, &mat[0][0]);
    }
    void setUni(const std::string &name, const glm::mat4 &mat) const{
        glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()), 1,GL_FALSE, &mat[0][0]);
    }
    
    

private:
// check for errors in the comilling of the shaders
    void checkCompileErrors(GLuint shader, std::string type){
        GLint success;
        GLchar logInfo[512];

        if(type != "PROGRAM"){
            glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
            if(!success){
                glGetShaderInfoLog(shader,512,NULL,logInfo);
                std::cout<<"SHADER COMPILE ERROR \nType: "<< type<<'\n'<<logInfo<<std::endl;
            }
        } else{
            glGetProgramiv(shader,GL_LINK_STATUS,&success);
            if(!success){
                glGetProgramInfoLog(shader,512,NULL,logInfo);
                std::cout<<"PROGRAM LINK ERROR \nType: "<< type<<'\n'<<logInfo<<std::endl;
            }
        }
    }
    // converts text form a file to a string
    std::string get_file_contents(const char* fileName) {
        std::ifstream shaderFile;
        shaderFile.exceptions(std::fstream::failbit | std::fstream::badbit);
        try{
            shaderFile.open(fileName);
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            return shaderStream.str();
        } catch(std::fstream::failure& e){
            std::cout<<"FAILLED TO READ SHADER FILE \"" << fileName << "\" " << e.what() << std::endl;
            std::abort();
        }

    }
};

#endif