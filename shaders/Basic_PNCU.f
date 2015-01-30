#version 330 core

in vec4 vColour;
in vec2 vTexCoord0;

out vec4 oColour;

void main()
{
    oColour = vColour + 0*(vTexCoord0.xyxx - vTexCoord0.xyxx);
}
