#version 330 core

in vec2 fs_TexCoord0;
in vec3 fs_Normal;

out vec4 OutColour;

uniform sampler2D u_Texture;

void main()
{
    //OutColour  = texture2D( u_Texture, fs_TexCoord0);
    //OutColour.a = 1;
    OutColour = vec4( texture2D( u_Texture, fs_TexCoord0 ).rgb, 1.0);
}
