#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TexCoord;
layout (location = 2) in vec3 in_Normal;

// Passed onto fragment shader
out vec2 fs_TexCoord0;
out vec3 fs_Normal;


// Uniform Blocks
layout (std140) uniform u140_Global
{
    mat4 ub_CameraMatrix;
    mat4 ub_ProjectionMatrix;

    vec2   ub_Viewport;          // viewport resolution (in pixels)
    vec2   ub_FrameGlobalTime;   // .x = frame time, .y = total time

    vec2   ub_MousePosition;
};

// varying uniforms
uniform mat4 u_TransformMatrix;   // full transformation matrix:  projection*camera*model

void main()
{
    mat4 MTransform = ub_CameraMatrix * u_TransformMatrix;
    gl_Position  = ub_ProjectionMatrix * MTransform * vec4(in_Position ,1);
    fs_TexCoord0 = in_TexCoord;
    fs_Normal    = (MTransform * vec4(in_Normal,0)).xyz;


    //gl_Position.xyz = inPosition;
    //gl_Position.w   = 1.0;

}
