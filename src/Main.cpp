#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#include <vector>
#include "GL.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"
#include "Utils.h"
#include "Loader.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
bool wireframe = false;

Camera camera(glm::vec3(0.0f, 1.0f, 0.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

std::vector<glm::vec3> floorTilePositions;

int main()
{

    GL::init(SCREEN_WIDTH, SCREEN_HEIGHT, "Rewrite");

    GLFWwindow *window = GL::getWindowPointer();
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader lightingShader("src/Shaders/lightingVertexShader.vert", "src/Shaders/lightingFragmentShader.frag");

    Model batwingModel("src/Resources/Models/B-AO_X360_VEHICLE_Batwing_Arkham_Origins/B-AO_X360_VEHICLE_Batwing_Arkham_Origins.obj");
    Model floorTileModel("src/Resources/Models/Floor_Tile/Floor_Tile.obj");
    Model cottageModel("src/Resources/Models/Cottage/Snow covered CottageOBJ.obj");

    for (int i = 0; i <= 16; i++)
    {
        for (int j = 0; j <= 16; j++)
        {
            floorTilePositions.push_back(glm::vec3(1.0f * (float)i, 0.0f, 1.0f * (float)j));
        }
    }

    while (GL::getWindowShouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        Input::ProcessKeyboardInputs(window, camera, deltaTime);

        Material material = {0, 0, 0, 32.0f};
        DirLight dirLight = {glm::vec3(-0.2f, -1.0f, -0.3f),
                             glm::vec3(0.2f), glm::vec3(0.8f), glm::vec3(1.0f)};
        PointLight pointLight = {glm::vec3(5.0f),
                                 1.0f, 0.09f, 0.032f,
                                 glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.5f)};
        SpotLight spotLight = {camera.Position, camera.Front,
                               glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)),
                               1.0f, 0.09f, 0.032f,
                               glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)};

        Loader::loadShaderRenderData(lightingShader, material, dirLight, pointLight, spotLight, camera);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("u_Projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("u_View", view);

        for (auto &pos : floorTilePositions)
        {
            glm::mat4 floorTileModelMatrix = glm::mat4(1.0f);
            floorTileModelMatrix = glm::translate(floorTileModelMatrix, pos);
            floorTileModelMatrix = glm::scale(floorTileModelMatrix, glm::vec3(0.5f)); // change floor tile model size to be 1x1
            lightingShader.setMat4("u_Model", floorTileModelMatrix);
            floorTileModel.Draw(lightingShader);
        }

        glm::mat4 batwingModelMatrix = glm::mat4(1.0f);
        batwingModelMatrix = glm::translate(batwingModelMatrix, glm::vec3(2.0f));
        batwingModelMatrix = glm::scale(batwingModelMatrix, glm::vec3(0.3f));
        lightingShader.setMat4("u_Model", batwingModelMatrix);
        batwingModel.Draw(lightingShader);

        glm::mat4 cottageModelMatrix = glm::mat4(1.0f);
        cottageModelMatrix = glm::translate(cottageModelMatrix, glm::vec3(0.0f, 0.0f, -5.0f));
        cottageModelMatrix = glm::scale(cottageModelMatrix, glm::vec3(0.1f));
        lightingShader.setMat4("u_Model", cottageModelMatrix);
        cottageModel.Draw(lightingShader);

        GL::swapBuffersPollEvents();
    }

    GL::cleanup();
    return 0;
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

    camera.ProcessMouseMovement(xoffset, yoffset);
}
