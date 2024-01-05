#version 330 core
layout (location = 0) in vec3 vtxPos;

uniform mat4 M;
uniform mat4 lightVP;

void main()
{
    gl_Position = vec4(vtxPos, 1.0) * M * lightVP;
}  