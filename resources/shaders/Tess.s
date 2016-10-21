<vertex>
#version 410 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;

uniform mat4 modelViewMatrix;


out vec3 vPosition;
out vec4 vColor;


void main(void)
{
       // color_vs.xyz =  gl_Color.xyz;//mix( vec4(1,1,0,0),vec4(0,1,1,0),  vertex.w);

        vColor    = in_Color;
        vPosition = (modelViewMatrix*vec4(in_Position.xyz,1.0)).xyz;
}

</vertex>



<tessellation_control>
#version 410 core

layout(vertices = 3) out;

in vec3 vPosition[];
in vec4 vColor[];

out vec3 tcPosition[];
out vec4 tcColor[];

void main(void)
{
        if( gl_InvocationID == 0 )
        {
            gl_TessLevelOuter[0] = 6.0;
            gl_TessLevelOuter[1] = 1.0;
            gl_TessLevelOuter[2] = 1.0;

            gl_TessLevelInner[0] = 3.0;
        }

        tcPosition[ gl_InvocationID ]  = vPosition[ gl_InvocationID];
        tcColor[ gl_InvocationID ]     = vColor[ gl_InvocationID];

        //gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

}
</tessellation_control>



<tessellation_evaluation>
#version 410 core

layout(triangles, equal_spacing, ccw) in;


in vec3 tcPosition[];
in vec4 tcColor[];

out vec3 tePosition;
out vec4 teColor;


void main()
{
        tePosition       = gl_TessCoord.x * tcPosition[0] +
                           gl_TessCoord.y * tcPosition[1] +
                           gl_TessCoord.z * tcPosition[2];

        teColor          = gl_TessCoord.x * tcColor[0] +
                           gl_TessCoord.y * tcColor[1] +
                           gl_TessCoord.z * tcColor[2];

        gl_Position = vec4(tePosition, 1.0);
        //gl_Position.xyzw =  gl_in[0].gl_Position.xyzw * gl_TessCoord.x +
        //                    gl_in[1].gl_Position.xyzw * gl_TessCoord.y +
        //                    gl_in[2].gl_Position.xyzw * gl_TessCoord.z;
}

</tessellation_evaluation>



<geometry>
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

</geometry>




<fragment>
#version 410 core

layout(location = 0, index = 0) out vec4 fragColor;

//in vec4 color_gs;


in vec3 tePosition;
in vec4 teColor;

void main(void)
{
        fragColor = teColor;
}
</fragment>
