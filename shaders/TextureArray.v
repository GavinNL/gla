#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;

out vec2 outTexCoord0;

void main()
{
    gl_Position.xyz = inPosition;
    gl_Position.w   = 1.0;
    outTexCoord0    = inUV;
}
