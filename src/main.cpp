#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GL.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

int main()
{
    GL::init(SCREEN_WIDTH, SCREEN_HEIGHT, "Rewrite");
    GLFWwindow *window = GL::getWindowPointer();
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader lightingShader("src/shaders/lightingVertexShader.vert", "src/shaders/lightingFragmentShader.frag");

    Model greggModel("src/resources/models/Gregg The Grim Reaper/Gregg.obj");
    Model cotaggeModel("src/resources/models/Cottage/Snow covered CottageOBJ.obj");

    while (GL::getWindowShouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Input::ProcessKeyboardInputs(window, camera, deltaTime);

        lightingShader.use();
        lightingShader.setFloat("time", glfwGetTime());
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);
        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        lightingShader.setVec3("pointLight.position", glm::vec3(2.0f));
        lightingShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLight.constant", 1.0f);
        lightingShader.setFloat("pointLight.linear", 0.09f);
        lightingShader.setFloat("pointLight.quadratic", 0.032f);
        // spotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f); 
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f); 
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));  

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("u_Projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("u_View", view);
        glm::mat4 greggModelMatrix = glm::mat4(1.0f);
        greggModelMatrix = glm::translate(greggModelMatrix, glm::vec3(0.0f));
        greggModelMatrix = glm::scale(greggModelMatrix, glm::vec3(0.1f));
        lightingShader.setMat4("u_Model", greggModelMatrix);
        
        // greggModel.Draw(lightingShader);
        cotaggeModel.Draw(lightingShader);

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