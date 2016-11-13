R"foo(

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


uniform sampler2D u_Texture;

void main()
{
    out_Colour = texture(u_Texture, f_TexCoord );
}

</fragment>


)foo";
