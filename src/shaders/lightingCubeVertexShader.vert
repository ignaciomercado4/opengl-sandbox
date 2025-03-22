#version 330 core
layout (location = 0) in vec3 l_aPosition;

uniform vec3 u_Model;
uniform vec3 u_View;
uniform vec3 u_Projection;


void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(l_aPosition, 1.0);
}