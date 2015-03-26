#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

uniform mat4 inCameraMatrix;
uniform mat4 inModelMatrix;

out vec2 outTexCoord0;

void main()
{
    vec4 v       = vec4(inPosition ,1);
    outTexCoord0 = inUV;
    gl_Position  = inCameraMatrix * inModelMatrix * v;
}
