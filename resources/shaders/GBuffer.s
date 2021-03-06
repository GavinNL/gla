<vertex>
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 uTransform;
uniform mat4 uCameraView;
uniform mat4 uCameraProj;

void main()
{

    vec4 p      = uTransform * vec4(inPosition,1.0);
    gl_Position = uCameraProj * uCameraView * p;
    vec4 N4     = uTransform * vec4(inNormal,0.0);

    FragPos    = p.xyz;
    TexCoords  = inUV;
    Normal     = N4.xyz;
}

</vertex>

<fragment>

#version 330 core
//out vec4 outColour;
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D uSampler1;
uniform sampler2D uSampler2;

void main()
{

//   outColour = vec4( texture2D(texture_diffuse1,TexCoords).rgb, 1);

    // Store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;

    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);

    // And the diffuse per-fragment color
    gAlbedoSpec.rgb = texture2D(uSampler1, TexCoords).rgb;

    // Store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture2D(uSampler2, TexCoords).r;

}

</fragment>
