#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>
#include <functional>
#include <stb/stb_image.h>

unsigned int compileShaderAndLink(const char* path);

unsigned int loadImage(const char* path, bool flipped);

void setUniInt(unsigned int &program, const char* name, int value);
void setUniFloat(unsigned int &program, const char* name, float value);
void setUniVec3(unsigned int &program, const char* name, glm::vec3 value);

void setUniMat4(unsigned int &program, const char* name, glm::mat4 value);