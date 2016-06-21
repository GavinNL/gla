#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;

out vec2 TexCoords;

void main(){

    gl_Position.xyz = vertexPosition;
    gl_Position.w = 1.0;
    TexCoords = vertexTexCoord;
}
