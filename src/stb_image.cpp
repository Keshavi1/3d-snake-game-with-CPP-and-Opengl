#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


int width, height, rnChannels;
unsigned char* data = stbi_load("img/container.jpg", &width, &height, &rnChannels, 0)