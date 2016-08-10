#version 330 core

in  vec2 FragmentTexCoord;

uniform sampler2D uSampler;


out vec4 color;

void main()
{
    // Output color = red
    color = vec4( texture2D( uSampler, FragmentTexCoord ).rgb, 1.0);

}
