#version 330 core

in vec2 outTexCoord0;

out vec4 OutColour;

uniform sampler2DArray uTextureArray;

void main()
{

    float t = texture( uTextureArray, vec3(outTexCoord0,2)).r;

    OutColour = (1.0-t)*texture( uTextureArray, vec3(outTexCoord0, 0 ) ) +
                      t*texture( uTextureArray, vec3(outTexCoord0, 1 ) );
}
