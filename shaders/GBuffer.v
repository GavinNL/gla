#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 uTransform;
uniform mat4 uCamera;

void main()
{
    gl_Position = uCamera * uTransform * vec4(inPosition,1.0);
    vec4 N4     = uTransform * vec4(inNormal,0.0);

    FragPos    = gl_Position.xyz;
    TexCoords  = inUV;
    Normal     = N4.xyz;
}
