#version 330 core
//out vec4 outColour;
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{

//   outColour = vec4( texture2D(texture_diffuse1,TexCoords).rgb, 1);

    // Store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;

    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);

    // And the diffuse per-fragment color
    gAlbedoSpec.rgb = texture2D(texture_diffuse1, TexCoords).rgb;


    // Store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture2D(texture_specular1, TexCoords).r;

}
