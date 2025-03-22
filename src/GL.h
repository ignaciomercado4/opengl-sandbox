#ifndef GL_H
#define GL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace GL {
    void init(int width, int height, std::string title);
    GLFWwindow* getWindowPointer();
    bool getWindowShouldClose();
    void setWindowShouldClose(bool val);
    void swapBuffersPollEvents();
    void cleanup();
}

#endif // GL_H
