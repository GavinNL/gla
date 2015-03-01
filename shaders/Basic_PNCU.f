#version 330 core

// Interpolated values from the vertex shaders
in vec4 oColour;
in vec2 oTexCoord0;

in vec3 oPosition_ws;
in vec3 oNormal_cs;
in vec3 oEyeDirection_cs;
in vec3 oLightDirection_cs;

// Ouput data
out vec4 colour;

// Values that stay constant for the whole mesh.
uniform sampler2D uSampler;
uniform vec3 uLightPosition;

void main()
{

        // Light emission properties
        // You probably want to put them as uniforms
        vec3 LightColor = vec3(1,1,1);
        float LightPower = 10000.0f;

        // Material properties
        vec3 MaterialDiffuseColor = texture2D( uSampler, oTexCoord0 ).rgb;
        vec3 MaterialAmbientColor = vec3(0.0,0.1,0.1) * MaterialDiffuseColor.rgb;
        vec3 MaterialSpecularColor = vec3(1.0,0.3,0.3);

        // Distance to the light
        float distance = length( uLightPosition - oPosition_ws );

        // Normal of the computed fragment, in camera space
        vec3 n = normalize( oNormal_cs );

        // Direction of the light (from the fragment to the light)
        vec3 l = normalize( oLightDirection_cs );


        // Cosine of the angle between the normal and the light direction,
        // clamped above 0
        //  - light is at the vertical of the triangle -> 1
        //  - light is perpendicular to the triangle -> 0
        //  - light is behind the triangle -> 0
        float cosTheta = clamp( dot( n,l ), 0,1 );

        // Eye vector (towards the camera)
        vec3 E = normalize(oEyeDirection_cs);

        // Direction in which the triangle reflects the light
        vec3 R = reflect(-l,n);

        // Cosine of the angle between the Eye vector and the Reflect vector,
        // clamped to 0
        //  - Looking into the reflection -> 1
        //  - Looking elsewhere -> < 1
        float cosAlpha = clamp( dot( E,R ), 0,1 );

        //colour = vec4(1.0,0.0,1.0,1.0);
        colour =
                vec4(MaterialAmbientColor +
                MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
                MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance),1.0);

}
