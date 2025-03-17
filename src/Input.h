#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

namespace Input 
{
    void ProcessKeyboardInputs(GLFWwindow* window, Camera& camera, float deltaTime);
}

#endif