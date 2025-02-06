/*
▄▄▄█████▓ ██░ ██ ▓█████     ▄▄▄▄   ▓█████   ▄████   ▄████  ██▓ ███▄    █  ██▓ ███▄    █   ▄████
▓  ██▒ ▓▒▓██░ ██▒▓█   ▀    ▓█████▄ ▓█   ▀  ██▒ ▀█▒ ██▒ ▀█▒▓██▒ ██ ▀█   █ ▓██▒ ██ ▀█   █  ██▒ ▀█▒
▒ ▓██░ ▒░▒██▀▀██░▒███      ▒██▒ ▄██▒███   ▒██░▄▄▄░▒██░▄▄▄░▒██▒▓██  ▀█ ██▒▒██▒▓██  ▀█ ██▒▒██░▄▄▄░
░ ▓██▓ ░ ░▓█ ░██ ▒▓█  ▄    ▒██░█▀  ▒▓█  ▄ ░▓█  ██▓░▓█  ██▓░██░▓██▒  ▐▌██▒░██░▓██▒  ▐▌██▒░▓█  ██▓
  ▒██▒ ░ ░▓█▒░██▓░▒████▒   ░▓█  ▀█▓░▒████▒░▒▓███▀▒░▒▓███▀▒░██░▒██░   ▓██░░██░▒██░   ▓██░░▒▓███▀▒
  ▒ ░░    ▒ ░░▒░▒░░ ▒░ ░   ░▒▓███▀▒░░ ▒░ ░ ░▒   ▒  ░▒   ▒ ░▓  ░ ▒░   ▒ ▒ ░▓  ░ ▒░   ▒ ▒  ░▒   ▒
    ░     ▒ ░▒░ ░ ░ ░  ░   ▒░▒   ░  ░ ░  ░  ░   ░   ░   ░  ▒ ░░ ░░   ░ ▒░ ▒ ░░ ░░   ░ ▒░  ░   ░
  ░       ░  ░░ ░   ░       ░    ░    ░   ░ ░   ░ ░ ░   ░  ▒ ░   ░   ░ ░  ▒ ░   ░   ░ ░ ░ ░   ░
          ░  ░  ░   ░  ░    ░         ░  ░      ░       ░  ░           ░  ░           ░       ░
                                 ░
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main()
{
    // GLFW
    if (!glfwInit())
    {
        std::cout << "Error initializing GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sandbox", NULL, NULL);
    if (!window)
    {
        std::cout << "Error creating GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Error initializing GLAD" << std::endl;
        return -1;
    } 

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // MAIN LOOP
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}