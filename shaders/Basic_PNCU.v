#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec4 inColour;
layout (location = 3) in vec2 inUV;

uniform mat4 inCameraMatrix;
uniform mat4 inModelMatrix;

out vec4 vColour;
out vec2 vTexCoord0;


void main()
{
    vec4 v      = vec4(inPosition + inNormal-inNormal, 1);
    vColour     = inColour;
    vTexCoord0  = inUV;
    gl_Position = inCameraMatrix * inModelMatrix * v;
}
