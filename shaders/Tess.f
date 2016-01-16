#version 410 core

layout(location = 0, index = 0) out vec4 fragColor;

//in vec4 color_gs;


in vec3 tePosition;
in vec4 teColor;

void main(void)
{
        fragColor = teColor;
}
