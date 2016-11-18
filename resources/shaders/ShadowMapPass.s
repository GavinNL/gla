<vertex>
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;

// Values that stay constant for the whole mesh.
uniform mat4 u_ProjMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

uniform mat4 u_LightSpaceMatrix;

out vec3 f_Position;
out vec2 f_TexCoord;
out vec3 f_Normal;
out vec4 f_PosLightSpace;

void main()
{
    vec4 mPosition = u_ModelMatrix * vec4(in_Position,1.0);

    gl_Position = u_ProjMatrix * u_ViewMatrix * mPosition;

    f_Position = mPosition.xyz;
    f_Normal   = transpose( inverse(mat3(u_ModelMatrix))) * in_Normal;
    f_TexCoord = in_TexCoord;

    f_PosLightSpace = u_LightSpaceMatrix * vec4(f_Position,1.0);

}
</vertex>





<fragment>
#version 330 core

// Ouput data
layout (location = 0) out vec3 out_Position;
layout (location = 1) out vec3 out_Normal;
layout (location = 2) out vec4 out_AlbedoSpec;

in vec3 f_Position;
in vec2 f_TexCoord;
in vec3 f_Normal;
in vec4 f_PosLightSpace;


uniform sampler2D u_Diffuse;
uniform sampler2D u_ShadowMap;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;


float ShadowCalculation(vec4 fragPosLightSpace, sampler2D shadow_map)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadow_map, projCoords.xy).r;


    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    if( length( projCoords.xy-vec2(0.5,0.5) ) > 0.5)
        return 1.0;

    // Check whether current frag pos is in shadow
    float shadow = currentDepth-0.005 > closestDepth  ? 1.0 : 0.0;

    return shadow;
}



//vec3 lightDir = normalize(light_position - position)
//vec3 viewDir  = normalize(camera_position - position);

float CalculateDiffuse( vec3 lightDir, vec3 normal)
{
    //vec3 lightDir = normalize(light_position - position)

    float diff    = max(dot(lightDir, normal), 0.0);
    //vec3 diffuse  = diff;

    return diff;
}

float CalculateSpecular(vec3 lightDir, vec3 viewDir, vec3 normal)
{
    // Specular
    //vec3 viewDir    = normalize(camera_position - position);

    float spec      = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);

    spec            = pow(max(dot(normal, halfwayDir), 0.0), 64.0);

    return spec;
}



void main()
{
    vec3 color      = texture( u_Diffuse, f_TexCoord).rgb;
    vec3 normal     = normalize(f_Normal);
    vec3 lightColor = vec3(1.0,1,1);


    vec3 lightDir = normalize(u_LightPos - f_Position);
    vec3 viewDir  = normalize(u_ViewPos - f_Position);


    vec3 ambient  = 0.15 * color;

    vec3 diffuse  = CalculateDiffuse(  lightDir , f_Normal) * lightColor;
    vec3 specular = CalculateSpecular( lightDir , viewDir, f_Normal) * lightColor;


    // Calculate shadow
    float shadow = ShadowCalculation( f_PosLightSpace, u_ShadowMap);


    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;


    out_AlbedoSpec = vec4(lighting,1.0);
    out_Normal     = f_Normal;
    out_Position   = f_Position;
}



</fragment>
