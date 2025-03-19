#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>

namespace Texture {
    unsigned int loadTexture(char const * path);
    void setGrayscaleTextureSwizzle(GLuint textureID);
}
#endif // TEXTURE_H