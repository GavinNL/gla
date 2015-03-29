#version 330 core

in  vec2 FragmentTexCoord;

uniform sampler2D uSampler;

out vec4 color;

void main()
{
    // Output color = red
    color = texture2D( uSampler, FragmentTexCoord ).rgba;
}
