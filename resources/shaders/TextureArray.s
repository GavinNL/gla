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

uniform sampler2DArray uTextureArray;
uniform vec2           uSpeed;



void main()
{
    float t = texture( uTextureArray, vec3(outTexCoord0+uSpeed,2) ).r;


    vec4 c1 = texture( uTextureArray, vec3(outTexCoord0, 0 ) );
    vec4 c2 = texture( uTextureArray, vec3(outTexCoord0, 1 ) );

    t = smoothstep(0,1,t);
    t = smoothstep(0,1,t);
    t = smoothstep(0,1,t);
   // t = round(t);
    OutColour = (1.0-t)*c1 + t*c2;
}
</fragment>
