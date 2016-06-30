#version 410 core

layout(triangles,  invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;


in vec3 tePosition[3];
in vec4 teColor[3];

out vec3 gPosition;
out vec4 gColor;

void main(void)
{
        gl_Position = gl_in[0].gl_Position;

        gColor     = teColor[0];
        gPosition  = tePosition[0];
        EmitVertex();

        gl_Position  = gl_in[1].gl_Position;
        gColor    = teColor[2];
        gPosition  = tePosition[1];
        EmitVertex();

        gl_Position  = gl_in[2].gl_Position;
        gColor    = teColor[2];
        gPosition  = tePosition[2];
        EmitVertex();


        EndPrimitive();
}
