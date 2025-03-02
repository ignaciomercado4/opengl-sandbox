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
#include <algorithm>
#include <unordered_set>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <stb_perlin.h>
#include "gtx/string_cast.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shader.hpp"
#include <stb_image.h>

const int SCREEN_WIDTH = 1980;
const int SCREEN_HEIGHT = 1080;
const int CHUNK_WIDTH = 32;
const int CHUNK_LENGTH = 32;
const int CHUNK_HEIGHT = 10;

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

glm::vec3 lightPos = glm::vec3(0.0f, 22.0f, 0.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 voxelColor = glm::vec3(0.34f, 0.22f, 0.15f);

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
        
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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

    void printVoxelAmount(std::vector<glm::vec3> positions)
    {
        std::cout << "Voxels: " << positions.size() << std::endl;
    }

    void printVertAmount(std::vector<glm::vec3> positions)
    {
        std::cout << "Vertices: " << positions.size() * 36 << std::endl;
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

        // MOVEMENT
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

namespace Render
{
    struct Vec3Hasher
    {
        size_t operator()(const glm::vec3 &v) const
        {
            return std::hash<float>()(v.x) ^
                   (std::hash<float>()(v.y) << 1) ^
                   (std::hash<float>()(v.z) << 2);
        }
    };

    bool IsCubeVisible(const glm::vec3 &cubePosition,
                       const std::unordered_set<glm::vec3, Vec3Hasher> &cubePositions)
    {
        const std::vector<glm::vec3> neighbors = {
            glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0),
            glm::vec3(0, 1, 0), glm::vec3(0, -1, 0),
            glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)};

        for (const auto &offset : neighbors)
        {
            glm::vec3 neighborPos = cubePosition + offset;
            if (cubePositions.find(neighborPos) == cubePositions.end())
            {
                return true;
            }
        }

        return false;
    }

    std::vector<glm::vec3> calculateSmoothedNormals(const std::vector<glm::vec3>& positions, const std::unordered_set<glm::vec3, Vec3Hasher>& positionsSet) 
    {
        std::vector<glm::vec3> normals(positions.size(), glm::vec3(0.0f));

        for (size_t i = 0; i < positions.size(); ++i) {
            const glm::vec3& pos = positions[i];
            glm::vec3 normal(0.0f);

            const std::vector<glm::vec3> neighbors = {
                glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0),
                glm::vec3(0, 1, 0), glm::vec3(0, -1, 0),
                glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)
            };

            for (const auto& offset : neighbors) {
                glm::vec3 neighborPos = pos + offset;
                if (positionsSet.find(neighborPos) == positionsSet.end()) {
                    normal += offset;
                }
            }

            normals[i] = glm::normalize(normal);
        }

        return normals;
    }
}

int main()
{
    GL::Init(SCREEN_WIDTH, SCREEN_HEIGHT, "Sandbox");
    GLFWwindow *window = GL::GetWindowPointer();
    if (!window)
    {
        return 0;
    }

        float vertices[] = {
        // Pos                  // Tex coord    // Normal 
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f,     0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,     0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,     0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,     1.0f, 0.0f,     0.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 0.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    1.0f, 1.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    1.0f, 1.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     -1.0f, 0.0f, 0.0f,

        0.5f,  0.5f,  0.5f,     0.0f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     0.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     0.0f, 0.0f,     1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     1.0f, 1.0f,     0.0f, -1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     1.0f, 1.0f,     0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,     0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f, -1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f,     1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 0.0f,     1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f,     1.0f, 1.0f, 0.0f
    };

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

    std::unordered_set<glm::vec3, Render::Vec3Hasher> cubePositionsSet(cubePositions.begin(), cubePositions.end());
    std::vector<glm::vec3> smoothedNormals = Render::calculateSmoothedNormals(cubePositions, cubePositionsSet);

    for (int i = 0; i < cubePositions.size(); i++)
    {
        Render::IsCubeVisible(cubePositions[i], cubePositionsSet);
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Tex coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("src/resources/dirt.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    Shader ourShader("src/shaders/vertexShader.vert", "src/shaders/fragmentShader.frag");
    Shader lightShader("src/shaders/lightVertexShader.vert", "src/shaders/lightFragmentShader.frag");
    

    glEnable(GL_DEPTH_TEST);

    while (GL::IsWindowOpen())
    {
        Input::processInput(window);

        if (drawWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        ourShader.use();
        glBindTexture(GL_TEXTURE_2D, texture);
        ourShader.setVec3("lightPos", lightPos);
        ourShader.setVec3("viewPos", cameraPos);
        ourShader.setVec3("lightColor", lightColor);

        // Create transformation matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);

        // Set uniforms
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // Render cubes
        glBindVertexArray(VAO);
        for (const auto &position : cubePositions)
        {
            if (Render::IsCubeVisible(position, cubePositionsSet))
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, position);
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        // Render light
        lightShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setMat4("model", model);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        GL::SwapBuffersPollEvents();
    }

    GL::Cleanup();
    return 0;
}
