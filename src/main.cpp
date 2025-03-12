#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GL.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

int main() {
    GL::Init(WINDOW_WIDTH, WINDOW_HEIGHT, "Rewrite");
    GLFWwindow* window = GL::GetWindowPointer();

    while (GL::GetWindowShouldClose()) {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 
        GL::SwapBuffersPollEvents();
    }

    GL::Cleanup();
    return 0;
}