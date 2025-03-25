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

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
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
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            floorTilePositions.push_back(glm::vec3(1.0f*(float)i, 0.0f, 1.0f*(float)j));
        }   
    }


    GL::init(SCREEN_WIDTH, SCREEN_HEIGHT, "Rewrite");

    GLFWwindow *window = GL::getWindowPointer();
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader lightingShader("src/shaders/lightingVertexShader.vert", "src/shaders/lightingFragmentShader.frag");

    Model batwingModel("src/resources/models/B-AO_X360_VEHICLE_Batwing_Arkham_Origins/B-AO_X360_VEHICLE_Batwing_Arkham_Origins.obj");
    Model floorTileModel("src/resources/models/Floor_Tile/Floor_Tile.obj");

    while (GL::getWindowShouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        Input::ProcessKeyboardInputs(window, camera, deltaTime);

        lightingShader.use();
        lightingShader.setVec3("u_ViewPos", camera.Position);
        lightingShader.setFloat("u_Material.shininess", 32.0f);
        // directional light
        lightingShader.setVec3("u_DirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("u_DirLight.ambient", 0.2f, 0.2f, 0.2f);  
        lightingShader.setVec3("u_DirLight.diffuse", 0.8f, 0.8f, 0.8f);  
        lightingShader.setVec3("u_DirLight.specular", 1.0f, 1.0f, 1.0f); 

        // point light 1
        lightingShader.setVec3("u_PointLight.position", glm::vec3(5.0f));
        lightingShader.setVec3("u_PointLight.ambient", 0.2f, 0.2f, 0.2f); 
        lightingShader.setVec3("u_PointLight.diffuse", 1.0f, 1.0f, 1.0f); 
        lightingShader.setVec3("u_PointLight.specular", 1.5f, 1.5f, 1.5f);
        lightingShader.setFloat("u_PointLight.constant", 1.0f);
        lightingShader.setFloat("u_PointLight.linear", 0.09f);
        lightingShader.setFloat("u_PointLight.quadratic", 0.032f);
        // spotLight
        lightingShader.setVec3("u_SpotLight.position", camera.Position);
        lightingShader.setVec3("u_SpotLight.direction", camera.Front);
        lightingShader.setVec3("u_SpotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("u_SpotLight.diffuse", 1.0f, 1.0f, 1.0f); 
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f); 
        lightingShader.setFloat("u_SpotLight.constant", 1.0f);
        lightingShader.setFloat("u_SpotLight.linear", 0.09f);
        lightingShader.setFloat("u_SpotLight.quadratic", 0.032f);
        lightingShader.setFloat("u_SpotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("u_SpotLight.outerCutOff", glm::cos(glm::radians(15.0f)));  

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("u_Projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("u_View", view);
        
        for (auto& pos : floorTilePositions) {
            glm::mat4 floorTileModelMatrix = glm::mat4(1.0f);
            floorTileModelMatrix = glm::translate(floorTileModelMatrix, pos);
            floorTileModelMatrix = glm::scale(floorTileModelMatrix, glm::vec3(0.5f)); // TODO: make floorTile model 1x1
            lightingShader.setMat4("u_Model", floorTileModelMatrix);
            floorTileModel.Draw(lightingShader);
        }
        

        glm::mat4 batwingModelMatrix = glm::mat4(1.0f);
        batwingModelMatrix = glm::translate(batwingModelMatrix, glm::vec3(2.0f));
        batwingModelMatrix = glm::scale(batwingModelMatrix, glm::vec3(0.3f));
        lightingShader.setMat4("u_Model", batwingModelMatrix);
        batwingModel.Draw(lightingShader);
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
