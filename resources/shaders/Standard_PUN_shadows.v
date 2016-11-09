#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TexCoord;
layout (location = 2) in vec3 in_Normal;

// Passed onto fragment shader
out vec2 fs_TexCoord0;
out vec3 fs_Normal;
out vec4 fs_ShadowCoord;

uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uDepthBias;

void main()
{
    mat4 MTransform = uViewMatrix * uModelMatrix;

    gl_Position    = uProjMatrix * MTransform * vec4( in_Position, 1 );

    fs_ShadowCoord = uDepthBias * vec4(in_Position,1);

    fs_TexCoord0 = in_TexCoord;

    fs_Normal    = (MTransform * vec4(in_Normal,0)).xyz;
}
