#version 330 core

in vec3 pNormal;
in vec3 pPos;

out vec4 OutColour;

uniform sampler2D  uTex0;
uniform sampler2D  uTex1;

void main()
{

    vec3 blend_weights = abs(pNormal.xyz);

    blend_weights = (blend_weights - 0.2)* 7;
    blend_weights = max(blend_weights, 0);
    blend_weights /= (blend_weights.x + blend_weights.y + blend_weights.z);


    vec2 c1 = pPos.yz;
    vec2 c2 = pPos.zx;
    vec2 c3 = pPos.xy;

    vec4 co1 = texture(uTex0, c1);
    vec4 co2 = texture(uTex1, c2);
    vec4 co3 = texture(uTex0, c3);

    OutColour = blend_weights.xxxx * co1.xyzw +
                blend_weights.yyyy * co2.xyzw +
                blend_weights.zzzz * co3.xyzw;
  
    //OutColour = vec4(1.0,1.0,1.0,1.0);
    //vec2 r = outTexCoord0;
   
    //float t = abs(cos( length(r) * 3.14159 * 2.0 ));
   
    //OutColour = (1.0-t)*texture( uTex0, outTexCoord0 ) +
                      //t*texture( uTex1, outTexCoord0 );

}
