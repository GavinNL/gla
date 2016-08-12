#version 330 core

in vec2 fs_TexCoord0;
in vec3 fs_Normal;
in vec4 fs_ShadowCoord;

out vec4 OutColour;

uniform sampler2D uTexture;
uniform sampler2D uShadowMap;

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
