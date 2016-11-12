const std::string gaussian_src = R"foo(

<vertex>
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;

out vec2 f_TexCoord;

void main(){

    gl_Position.xyzw = vec4(in_Position, 1.0);
    f_TexCoord = in_TexCoord;
}

</vertex>

<fragment>

#version 330 core
in  vec2 f_TexCoord;


out vec4 out_Colour;

uniform vec2      u_TexelSize;
uniform vec2      u_Direction;
uniform sampler2D u_Texture;

void main()
{
    // Retrieve data from gbuffer
    out_Colour = vec4(0.0f);

    out_Colour += texture(u_Texture, f_TexCoord + u_TexelSize*u_Direction*vec2(-3.0) ) * ( 1.0 / 64.0);
    out_Colour += texture(u_Texture, f_TexCoord + u_TexelSize*u_Direction*vec2(-2.0) ) * ( 6.0 / 64.0);
    out_Colour += texture(u_Texture, f_TexCoord + u_TexelSize*u_Direction*vec2(-1.0) ) * (15.0 / 64.0);
    out_Colour += texture(u_Texture, f_TexCoord + u_TexelSize*u_Direction            ) * (20.0 / 64.0);
    out_Colour += texture(u_Texture, f_TexCoord + u_TexelSize*u_Direction*vec2( 1.0) ) * (15.0 / 64.0);
    out_Colour += texture(u_Texture, f_TexCoord + u_TexelSize*u_Direction*vec2( 2.0) ) * ( 6.0 / 64.0);
    out_Colour += texture(u_Texture, f_TexCoord + u_TexelSize*u_Direction*vec2( 3.0) ) * ( 1.0 / 64.0);


}

</fragment>


)foo";
