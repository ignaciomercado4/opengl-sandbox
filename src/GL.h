#ifndef GL_H
#define GL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace GL {
    void Init(int width, int height, std::string title);
    GLFWwindow* GetWindowPointer();
    bool GetWindowShouldClose();
    void SetWindowShouldClose(bool val);
    void SwapBuffersPollEvents();
    void Cleanup();
}

#endif // GL_H
