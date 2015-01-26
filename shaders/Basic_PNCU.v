#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec4 inColour;
in vec2 inUV;

out vec4 vColour;

uniform mat4 inCameraMatrix;
uniform mat4 inModelMatrix;

void main()
{
    vec4 v      = vec4(inPosition + inNormal-inNormal,1);
    vColour     = inColour;
    gl_Position = inCameraMatrix * inModelMatrix * v;
}
