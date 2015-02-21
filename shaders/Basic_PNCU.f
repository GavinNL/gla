#version 330 core

in vec4 vColour;
in vec2 vTexCoord0;

out vec4 oColour;

uniform sampler2D uSampler;

void main()
{
    oColour   = texture2D(uSampler, vTexCoord0.st);
    //oColour   = vColour + 0*(vTexCoord0.xyxx - vTexCoord0.xyxx);
}
