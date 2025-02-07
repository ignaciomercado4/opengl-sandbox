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
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shader.hpp"

// DEC&DEF
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void processInput(GLFWwindow *window);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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

    // LOCAL SPACE VERT COORDS
    float vertices[] = {
        // coord                // color
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    // TRIANGLE WORLD SPACE POSITIONS
    std::vector<glm::vec3> trianglePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.5f, 0.0f, 0.0f),
        glm::vec3(-1.5f, 0.0f, 0.0f)};

    /*VAO AND VBO THING STEPS*/
    // 1. gen VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 2. bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 3. configure and enable 'em
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    /*-----------------------------------------------------------------------------------*/

    Shader ourShader("src/shaders/vertexShader.vert", "src/shaders/fragmentShader.frag");
    ourShader.use();

    // MAIN LOOP
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        // GOING 3D
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(80.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < trianglePositions.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, trianglePositions[i]);
            model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    // MISC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    { 
        glfwSetWindowShouldClose(window, true);
    }

    // MOVEMENT
    const float cameraSpeed = 0.05f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}