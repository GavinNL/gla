#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

uniform mat4 inCameraMatrix;
uniform mat4 inModelMatrix;

out vec3 pNormal;
out vec3 pPos;

void main()
{
    vec4 v       = vec4(inPosition ,1);

    pNormal      = inNormal;

    gl_Position  = inCameraMatrix * inModelMatrix * v;

    pPos = (inModelMatrix * v).xyz;
}
