#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;

out vec4 fragmentColor;

void main(){

    gl_Position.xyz = vertexPosition;
    gl_Position.w = 1.0;
    fragmentColor = vertexColor;
}
