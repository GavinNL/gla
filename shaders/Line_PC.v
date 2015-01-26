#version 330 core

in vec3 inPosition;
in vec4 inColour;

out vec4 vColour;

uniform mat4 inCameraMatrix;
uniform mat4 inModelMatrix;

void main()
{
    vec4 v      = vec4(inPosition ,1);
    vColour     = inColour;
    gl_Position = inCameraMatrix * inModelMatrix * v;
}
