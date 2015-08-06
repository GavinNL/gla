#version 330 core

in vec2 outTexCoord0;

out vec4 OutColour;

uniform sampler2DArray uTextureArray;
uniform vec2           uSpeed;

void main()
{


    float t = texture( uTextureArray, vec3(outTexCoord0+uSpeed,2) ).r;

    vec4 c1 = texture( uTextureArray, vec3(outTexCoord0, 0 ) );
    vec4 c2 = texture( uTextureArray, vec3(outTexCoord0, 1 ) );

    OutColour = (1.0-t)*c1 + t*c2;
}
