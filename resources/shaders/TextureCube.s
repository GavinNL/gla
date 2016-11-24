<vertex>
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;

out vec2 outTexCoord0;

void main()
{
    gl_Position.xyz = inPosition;
    gl_Position.w   = 1.0;
    outTexCoord0    = inUV;
}

</vertex>


<fragment>
#version 330 core

in vec2 outTexCoord0;

out vec4 OutColour;

uniform samplerCube uTextureArray;

void main()
{
    float x = sin( outTexCoord0.x * 3.14159 )*sin( outTexCoord0.x * 3.14159 * 2);
    float y = sin( outTexCoord0.x * 3.14159 )*cos( outTexCoord0.y * 3.14159 * 2);
    float z = cos( outTexCoord0.x * 3.14159 );

    OutColour.rgb = texture( uTextureArray, vec3(x,y,z) ).rgb;
    OutColour.a = 1.0;
}
</fragment>
