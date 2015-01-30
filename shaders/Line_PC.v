#version 330 core

layout (location=0) in vec3 inPosition;
layout (location=1) in vec4 inColour;

out vec4 vColour;

uniform mat4 inCameraMatrix;
uniform mat4 inModelMatrix;

void main()
{
    vec4 v      = vec4(inPosition ,1);
    vColour     = inColour;
    gl_Position = inCameraMatrix * inModelMatrix * v;
}
