#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <random>
#include <functional>
#include <time.h>

void drawbg(unsigned int texture, unsigned int &program, size_t size);

void updateAndDrawBird(unsigned int texture, unsigned int &program, size_t size);
void flap();



void updateAndDrawPipes(unsigned int bottom,unsigned int top, unsigned int &program, size_t size);

