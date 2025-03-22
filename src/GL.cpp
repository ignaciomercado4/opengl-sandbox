#include "GL.h"
#include <iostream>

namespace GL
{
    GLFWwindow *_window = nullptr;

    void init(int width, int height, std::string title)
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to init GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!_window)
        {
            std::cerr << "Failed to create window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(_window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to init GLAD" << std::endl;
            glfwTerminate();
            return;
        }

        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    GLFWwindow *getWindowPointer()
    {
        return _window;
    }

    bool getWindowShouldClose()
    {
        return !glfwWindowShouldClose(_window);
    }

    void setWindowShouldClose(bool val)
    {
        glfwSetWindowShouldClose(_window, val);
    }

    void swapBuffersPollEvents()
    {
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    void cleanup()
    {
        glfwTerminate();
    }
}