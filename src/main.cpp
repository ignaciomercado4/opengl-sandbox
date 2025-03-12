#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GL.h"
#include "Shader.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

float vertices[] = {
    // aPos             // aColor
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f
};

int main() {
    GL::Init(WINDOW_WIDTH, WINDOW_HEIGHT, "Rewrite");
    GLFWwindow* window = GL::GetWindowPointer();
    
    Shader OurShader("src/shaders/vertexShader.vert", "src/shaders/fragmentShader.frag");

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (GL::GetWindowShouldClose()) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 
        OurShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        GL::SwapBuffersPollEvents();
    }

    GL::Cleanup();
    return 0;
}