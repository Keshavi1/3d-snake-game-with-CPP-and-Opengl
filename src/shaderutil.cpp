#include "shaderutil.h"




unsigned int compileShaderAndLink(const char* path){
    //lamdas for checking errors
    auto checkErrors = [](unsigned int shader, std::string type)-> void{
        int success;
        char buffer[512];
        if(type == "PROGRAM"){
            glGetProgramiv(shader,GL_LINK_STATUS, &success);
            if(!success){
                glGetProgramInfoLog(shader,512,NULL,buffer);
                printf("!PROGRAM LINK ERROR!\n Type: %s \n %s", type.c_str(), buffer);
            }
        } else{
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(shader,512,NULL,buffer);
                printf("!SHADER COMPILE ERROR!\n Type: %s\n %s", type.c_str(), buffer);
            }
        }
    };
    struct Source{
        std::string vertexSource;
        std::string fragmentSource;
    };

    // seperates the contents of the file into two strings
    Source src = [](const char* fileName)-> Source{
        enum Type{NONE = -1, VERTEX = 0, FRAGMENT = 1};
        std::fstream file(fileName);

        std::string line;
        std::stringstream stream[2];
        Type type = NONE;
        while(getline(file, line)){
            if(line == "#VERTEX SHADER")
                type = VERTEX;
            else if(line == "#FRAGMENT SHADER")
                type = FRAGMENT;
            else
                stream[type] << line<<'\n';
        }
        return {stream[0].str(), stream[1].str()};

    }(path);
    const char* vertexCode = src.vertexSource.c_str();
    const char* fragmentCode = src.fragmentSource.c_str();

    unsigned int vertex, fragment;
    // compiles the strings and links them to the program
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1, &vertexCode, NULL);
    glCompileShader(vertex);
    checkErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,1, &fragmentCode, NULL);
    glCompileShader(fragment);
    checkErrors(fragment, "FRAGMENT");

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    checkErrors(program, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

unsigned int loadImage(const char* path, bool flipped){
    unsigned int ID;
    glGenTextures(1, &ID);
    stbi_set_flip_vertically_on_load(flipped);

    int width, height, rnComp;
    unsigned char* data = stbi_load(path, &width, &height,&rnComp, 0);

    if(data){
        GLenum format;
        if(rnComp == 1)
            format = GL_RED;
        else if(rnComp == 3)
            format = GL_RGB;
        else if(rnComp == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D,0, format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else{
        printf("Failed to load image; path: %s", path);
    }
    stbi_image_free(data);
    return ID;
}

void setUniInt(unsigned int &program, const char* name, int value){
    glUniform1i(glGetUniformLocation(program, name),value);
}
void setUniFloat(unsigned int &program, const char* name, float value){
    glUniform1f(glGetUniformLocation(program, name),value);
}
void setUniVec3(unsigned int &program, const char* name, glm::vec3 value){
    glUniform3fv(glGetUniformLocation(program, name),1, &value[0]);
}
void setUniMat4(unsigned int &program, const char* name, glm::mat4 value){
    glUniformMatrix4fv(glGetUniformLocation(program, name),1, GL_FALSE, &value[0][0]);
}