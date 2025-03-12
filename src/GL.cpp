#include "GL.h"
#include <iostream>

namespace GL {
    GLFWwindow* _window = nullptr;

    void Init(int width, int height, std::string title) {
        if (!glfwInit()) {
            std::cerr << "Failed to init GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!_window) {
            std::cerr << "Failed to create window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(_window);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cerr << "Failed to init GLAD" << std::endl;
            glfwTerminate();
            return;
        }

        glViewport(0, 0, width, height);
    }

    GLFWwindow* GetWindowPointer() {
        return _window;
    }

    bool GetWindowShouldClose() {
        return !glfwWindowShouldClose(_window);
    }

    void SetWindowShouldClose(bool val) {
        glfwSetWindowShouldClose(_window, val);
    }

    void SwapBuffersPollEvents() {
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    void Cleanup() {
        glfwTerminate();
    }
}