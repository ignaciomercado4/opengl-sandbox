#version 330 core
layout (location = 0) in vec3 l_aPosition;
layout (location = 1) in vec3 l_aNormal;
layout (location = 2) in vec2 l_aTextureCoordinate;

out vec3 Normal;
out vec2 TextureCoordinate;
out vec3 FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    TextureCoordinate = l_aTextureCoordinate;
    Normal = mat3(transpose(inverse(u_Model))) * l_aNormal;  
    FragPos = vec3(u_Model * vec4(l_aPosition, 1.0));

    gl_Position = u_Projection * u_View * u_Model * vec4(l_aPosition, 1.0);
}