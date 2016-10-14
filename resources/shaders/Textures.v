#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;

out vec2 FragmentTexCoord;

uniform mat4      uTransform;

void main(){

    gl_Position = uTransform * vec4(vertexPosition,1.0);

    FragmentTexCoord = vertexTexCoord;
}
