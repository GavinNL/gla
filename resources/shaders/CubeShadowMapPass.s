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


out vec3 f_Position;
out vec2 f_TexCoord;
out vec3 f_Normal;


void main()
{
    vec4 mPosition = u_ModelMatrix * vec4(in_Position,1.0);

    gl_Position = u_ProjMatrix * u_ViewMatrix * mPosition;

    f_Position = mPosition.xyz;
    f_Normal   = transpose( inverse(mat3(u_ModelMatrix))) * in_Normal;
    //f_Normal   = mat3(u_ModelMatrix)*in_Normal;
    f_TexCoord = in_TexCoord;

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



uniform sampler2D   u_Diffuse;

uniform samplerCube u_ShadowMap;

uniform vec3        u_LightPos;
uniform vec3        u_ViewPos;
uniform float       u_FarPlane;


float ShadowCalculation(vec3 fragPosition, samplerCube shadow_map)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPosition.xyz - u_LightPos;


    // Use the light to fragment vector to sample from the depth map
    float closestDepth = texture(shadow_map, normalize(fragToLight) ).r;

    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= u_FarPlane;

    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    // Now test for shadows

    float bias   = 0.01;
    float shadow = (currentDepth -  bias) > closestDepth ? 1.0 : 0.0;

    return shadow;// + closestDepth/u_FarPlane;

}


float CalculateDiffuse( vec3 lightDir, vec3 normal)
{
    float diff    = max(dot(lightDir, normal), 0.0);

    return diff;
}

float CalculateSpecular(vec3 lightDir, vec3 viewDir, vec3 normal)
{
    float spec1      = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);

    spec1            = pow(max(dot(normal, halfwayDir), 0.0), 64.0);

    return spec1;
}




void main()
{
    vec3 color      = texture( u_Diffuse, f_TexCoord).rgb;
    vec3 normal     = normalize(f_Normal);
    vec3 lightColor = vec3(1.0,1,1);


    vec3 lightDir = normalize(u_LightPos - f_Position);
    vec3 viewDir  = normalize(u_ViewPos  - f_Position);


    vec3 ambient  = 0.15 * color;

    vec3 diffuse  = CalculateDiffuse(  lightDir , f_Normal) * lightColor;
    vec3 specular = CalculateSpecular( lightDir , viewDir, f_Normal) * lightColor;

    // Calculate shadow
    float shadow = ShadowCalculation( f_Position, u_ShadowMap);


    vec3 lighting = ambient + (1.0 - shadow) * (diffuse*0.0001 + specular*0.0001 + vec3(1.0)) * color;



    out_AlbedoSpec = vec4(lighting,1.0);// * 0.0001 + vec4(shadow,shadow ,shadow, 1.0);

    //out_AlbedoSpec = vec4(shadow) + vec4(lighting,1)*0.0001;// * 0.0001 + vec4(shadow,shadow ,shadow, 1.0);
    //out_AlbedoSpec.a = 1.0;

    out_Normal     = f_Normal;
    out_Position   = f_Position / 25.0;
}



</fragment>
