#include "Input.h"
#include "GL.h"

namespace Input
{
    void ProcessKeyboardInputs(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            GL::SetWindowShouldClose(true);
        }
    }
}