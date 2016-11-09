<vertex>
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;


out vec2 TexCoords;

void main(){

    gl_Position.xyz = vertexPosition;
    gl_Position.w = 1.0;
    TexCoords = vertexTexCoord;
}

</vertex>

<fragment>

#version 330 core
out vec4 FragColor;
in  vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;

struct Light
{
    vec3 Position;
    float Linear;

    vec3 Color;
    float Quadratic;

    float Radius;
};

const int NR_LIGHTS = 2;

layout (std140) uniform LightBlock140
{
    Light LightInfo
    [NR_LIGHTS];

    vec3 CameraPosition;
};



void main()
{
    // Retrieve data from gbuffer
    vec3 FragPos   = texture2D(gPosition, TexCoords).rgb;
    vec3 Normal    = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse   = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    float Depth    = texture(gDepth,      TexCoords).r;

/*
    // Then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.2; // hard-coded ambient component
    vec3 viewDir   = normalize(CameraPosition - FragPos);

    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // Calculate distance between light source and current fragment
        int j = i*9;
        vec3 Position     = LightInfo[i].Position; //vec3( lights[j  ], lights[j+1], lights[j+2] );
        vec3 Color        = LightInfo[i].Color; //vec3( lights[j+3], lights[j+4], lights[j+5] );
        float Linear      = LightInfo[i].Linear; //lights[j+6];
        float Quadratic   = LightInfo[i].Quadratic; //lights[j+7];
        float Radius      = LightInfo[i].Radius; //lights[j+8];

        float distance = length( Position - FragPos);
        if(distance < Radius)
        {
            // Diffuse
            vec3 lightDir = normalize(Position - FragPos);
            vec3 diffuse  = max(dot(Normal, lightDir), 0.0) * Diffuse * Color;

            // Specular
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = Color * spec * Specular;

            // Attenuation
            float attenuation = 1.0 / (1.0 + Linear * distance + Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }



    }*/

    FragColor = vec4( Diffuse,1.0);

}

</fragment>





#version 330 core

in vec2 fs_TexCoord0;
in vec3 fs_Normal;
in vec4 fs_ShadowCoord;

out vec4 OutColour;

uniform sampler2D uTexture;
uniform sampler2D uShadowMap;


void CalculateShadow(vec4 shadowcoord)
{
    float visibility = 1.0;
    float bias = 0.005;

    //	float bias = 0.005*tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
    bias = clamp(bias, 0,0.01);

    if ( texture( uShadowMap, shadowcoord.xy ).z  <  shadowcoord.z - bias)
    {
            visibility = 0.5;
    }

    return visibility;
}

void main()
{

	float visibility = 1.0;
	float bias = 0.005;

//	float bias = 0.005*tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
	bias = clamp(bias, 0,0.01);

	if ( texture( uShadowMap, fs_ShadowCoord.xy ).z  <  fs_ShadowCoord.z - bias)
	{
		visibility = 0.5;
	}

  	OutColour = vec4( texture2D( uTexture, fs_TexCoord0 ).rgb, 1.0) * visibility;
}
