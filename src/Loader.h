#ifndef LOADER_H
#define LOADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"

struct Material {
    unsigned int diffuse;
    unsigned int specular;
    unsigned int emission;
    float shininess;
}; 

struct DirLight {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight {
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct SpotLight {
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

namespace Loader {
    int loadShaderRenderData(Shader& shader, Material& material, DirLight& dirLight, PointLight& pointLight, SpotLight& spotLight, Camera& camera) {
        shader.use();

        // Set camera position
        shader.setVec3("u_ViewPos", camera.Position);

        // Set material
        shader.setFloat("u_Material.shininess", material.shininess);

        // Set directional light
        shader.setVec3("u_DirLight.direction", dirLight.direction);
        shader.setVec3("u_DirLight.ambient", dirLight.ambient);
        shader.setVec3("u_DirLight.diffuse", dirLight.diffuse);
        shader.setVec3("u_DirLight.specular", dirLight.specular);

        // Set point light
        shader.setVec3("u_PointLight.position", pointLight.position);
        shader.setVec3("u_PointLight.ambient", pointLight.ambient);
        shader.setVec3("u_PointLight.diffuse", pointLight.diffuse);
        shader.setVec3("u_PointLight.specular", pointLight.specular);
        shader.setFloat("u_PointLight.constant", pointLight.constant);
        shader.setFloat("u_PointLight.linear", pointLight.linear);
        shader.setFloat("u_PointLight.quadratic", pointLight.quadratic);

        // Set spotlight
        shader.setVec3("u_SpotLight.position", spotLight.position);
        shader.setVec3("u_SpotLight.direction", spotLight.direction);
        shader.setVec3("u_SpotLight.ambient", spotLight.ambient);
        shader.setVec3("u_SpotLight.diffuse", spotLight.diffuse);
        shader.setVec3("u_SpotLight.specular", spotLight.specular);
        shader.setFloat("u_SpotLight.constant", spotLight.constant);
        shader.setFloat("u_SpotLight.linear", spotLight.linear);
        shader.setFloat("u_SpotLight.quadratic", spotLight.quadratic);
        shader.setFloat("u_SpotLight.cutOff", spotLight.cutOff);
        shader.setFloat("u_SpotLight.outerCutOff", spotLight.outerCutOff);

        return 0;
    }
}

#endif
