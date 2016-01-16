#version 410 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;

uniform mat4 modelViewMatrix;


out vec3 vPosition;
out vec4 vColor;
//out vec4 color_vs;

void main(void)
{
       // color_vs.xyz =  gl_Color.xyz;//mix( vec4(1,1,0,0),vec4(0,1,1,0),  vertex.w);

        vColor    = in_Color;
        vPosition = (modelViewMatrix*vec4(in_Position.xyz,1.0)).xyz;
}
