#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>


class Texture {
public:
    GLuint ID;
    GLenum type;
    Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType){
        type = texType;
        int width, height, colChannel;
        stbi_set_flip_vertically_on_load(true);
        // reads image and stores it into bytes
        unsigned char* bytes = stbi_load(image,&width,&height,&colChannel,0);
        if(!bytes) printf("Texture failed to load");
        // generates texture and assign it to a unit
        glGenTextures(1, &ID);
        glActiveTexture(slot);
        glBindTexture(texType, ID);
        // configs agorithems for making image bigger/smaller and repeating
        glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(texType,0,GL_RGB,width,height,0, format,pixelType,bytes);
        glGenerateMipmap(texType);
        stbi_image_free(bytes);
        glBindTexture(texType, 0);
    }
    void Bind(){
        glBindTexture(type, ID);
    }
    void unBind(){
        glBindTexture(type, 0);
    }
    void Delete(){
        glDeleteTextures(1, &ID);
    }
};
#endif