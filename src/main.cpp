// /*
// ▄▄▄█████▓ ██░ ██ ▓█████     ▄▄▄▄   ▓█████   ▄████   ▄████  ██▓ ███▄    █  ██▓ ███▄    █   ▄████
// ▓  ██▒ ▓▒▓██░ ██▒▓█   ▀    ▓█████▄ ▓█   ▀  ██▒ ▀█▒ ██▒ ▀█▒▓██▒ ██ ▀█   █ ▓██▒ ██ ▀█   █  ██▒ ▀█▒
// ▒ ▓██░ ▒░▒██▀▀██░▒███      ▒██▒ ▄██▒███   ▒██░▄▄▄░▒██░▄▄▄░▒██▒▓██  ▀█ ██▒▒██▒▓██  ▀█ ██▒▒██░▄▄▄░
// ░ ▓██▓ ░ ░▓█ ░██ ▒▓█  ▄    ▒██░█▀  ▒▓█  ▄ ░▓█  ██▓░▓█  ██▓░██░▓██▒  ▐▌██▒░██░▓██▒  ▐▌██▒░▓█  ██▓
//   ▒██▒ ░ ░▓█▒░██▓░▒████▒   ░▓█  ▀█▓░▒████▒░▒▓███▀▒░▒▓███▀▒░██░▒██░   ▓██░░██░▒██░   ▓██░░▒▓███▀▒
//   ▒ ░░    ▒ ░░▒░▒░░ ▒░ ░   ░▒▓███▀▒░░ ▒░ ░ ░▒   ▒  ░▒   ▒ ░▓  ░ ▒░   ▒ ▒ ░▓  ░ ▒░   ▒ ▒  ░▒   ▒
//     ░     ▒ ░▒░ ░ ░ ░  ░   ▒░▒   ░  ░ ░  ░  ░   ░   ░   ░  ▒ ░░ ░░   ░ ▒░ ▒ ░░ ░░   ░ ▒░  ░   ░
//   ░       ░  ░░ ░   ░       ░    ░    ░   ░ ░   ░ ░ ░   ░  ▒ ░   ░   ░ ░  ▒ ░   ░   ░ ░ ░ ░   ░
//           ░  ░  ░   ░  ░    ░         ░  ░      ░       ░  ░           ░  ░           ░       ░
//                                  ░
// */

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <stb_perlin.h>
#include "gtx/string_cast.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shader.hpp"

const int SCREEN_WIDTH = 1980;
const int SCREEN_HEIGHT = 1080;
const int CHUNK_WIDTH = 10;
const int CHUNK_LENGTH = 10;
const int CHUNK_HEIGHT = 2;

glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float fov = 45.0f;
float pitch = 0.0f;
float lastX = (float)SCREEN_WIDTH / 2.0;
float lastY = (float)SCREEN_HEIGHT / 2.0;

bool drawWireframe = false;

namespace GL
{
    GLFWwindow *_window;

    void mouseCallback(GLFWwindow *window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        pitch = std::max(-89.0f, std::min(89.0f, pitch));

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }

    void Init(int width, int height, const std::string &title)
    {
        if (!glfwInit())
        {
            std::cerr << "Error initializing GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!_window)
        {
            std::cerr << "Error creating GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(_window);
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(_window, mouseCallback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Error initializing GLAD" << std::endl;
            return;
        }
    }

    GLFWwindow *GetWindowPointer() { return _window; }
    bool IsWindowOpen() { return !glfwWindowShouldClose(_window); }
    void SetWindowShouldClose(bool value) { glfwSetWindowShouldClose(_window, value); }
    void SwapBuffersPollEvents()
    {
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
    void Cleanup() { glfwTerminate(); }
}

namespace Utils
{
    void printCurrentCoordinates()
    {
        std::cout << "Camera Position: ("
                  << cameraPos.x << ", "
                  << cameraPos.y << ", "
                  << cameraPos.z << ") | Looking At: ("
                  << cameraFront.x << ", "
                  << cameraFront.y << ", "
                  << cameraFront.z << ")" << std::endl;
    }

    std::vector<std::vector<int>> generatePerlinNoiseArray(int width, int height, float scale)
    {
        std::vector<std::vector<int>> noiseArray(height, std::vector<int>(width));

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                float noiseValue = stb_perlin_noise3(x * scale, y * scale, 0.0f, 0, 0, 0);

                int mappedValue = CHUNK_HEIGHT + static_cast<int>(round((noiseValue + 1) * 4.5f));

                mappedValue = std::max(CHUNK_HEIGHT, mappedValue);

                noiseArray[y][x] = mappedValue;
            }
        }

        return noiseArray;
    }
}

namespace Input
{
    void processInput(GLFWwindow *window)
    {
        const float cameraSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 0.2f : 0.05f;

        // CLOSE
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);


        // TP TO 0,0,0
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
            cameraPos = glm::vec3(0.0f, 5.0f, 10.0f);

        // WIREFRAME MODE
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            drawWireframe = !drawWireframe;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

}

int main()
{
    GL::Init(SCREEN_WIDTH, SCREEN_HEIGHT, "Sandbox");
    GLFWwindow *window = GL::GetWindowPointer();
    if (!window)
    {
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f};

    std::vector<std::vector<int>> perlinNoise = Utils::generatePerlinNoiseArray(CHUNK_WIDTH, CHUNK_LENGTH, 0.1f);
    std::vector<glm::vec3> cubePositions;

    for (int i = 0; i < CHUNK_WIDTH; i++)
    {
        for (int k = 0; k < CHUNK_LENGTH; k++)
        {
            if (i < perlinNoise.size() && k < perlinNoise[i].size())
            {
                int heightValue = perlinNoise[i][k];
                for (int y = 0; y <= heightValue; y++)
                {
                    cubePositions.push_back(glm::vec3(
                        static_cast<float>(k),
                        static_cast<float>(y),
                        static_cast<float>(i)));
                }
            }
        }
    }

    // VAO and VBO thing setup
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position vert attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Color vert attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader ourShader("src/shaders/vertexShader.vert", "src/shaders/fragmentShader.frag");
    ourShader.use();

    glEnable(GL_DEPTH_TEST);

    while (GL::IsWindowOpen())
    {
        Utils::printCurrentCoordinates();
        Input::processInput(window);

        if (drawWireframe) 
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        } else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        // Create transformation matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);

        // Set uniforms
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // Render terrain cubes
        glBindVertexArray(VAO);
        for (const auto &position : cubePositions)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        Utils::printCurrentCoordinates();

        GL::SwapBuffersPollEvents();
    }

    GL::Cleanup();
    return 0;
}
