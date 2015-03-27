#version 330 core

in vec2 outTexCoord0;

out vec4 OutColour;

uniform sampler2DArray uTextureArray;

void main()
{
    vec2 r = outTexCoord0-vec2(0.5,0.5);
    float t = abs(cos( length(r) * 3.14159 * 2.0 ));
    t = abs( length(r) );

    OutColour = (1.0-t)*texture( uTextureArray, vec3(outTexCoord0, 0 ) ) +
                t*texture( uTextureArray, vec3(outTexCoord0, 1 ) );
}
