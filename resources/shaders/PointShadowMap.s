<vertex>

#version 330 core

layout (location = 0) in vec3 in_Position;

uniform mat4 m_ModelMatrix;

void main()
{
    gl_Position = m_ModelMatrix * vec4(in_Position, 1.0);
}

</vertex>


<geometry>

#version 330 core

layout (triangles)                       in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_ShadowMatrix[6];

out vec4 f_FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            f_FragPos = gl_in[i].gl_Position;
            gl_Position = u_ShadowMatrix[face] * f_FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
</geometry>


<fragment>
#version 330 core
in vec4 f_FragPos;

uniform vec3  u_LightPos;
uniform float u_FarPlane;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(f_FragPos.xyz - u_LightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / u_FarPlane;

    // Write this as modified depth
    gl_FragDepth = lightDistance;
}
</fragment>
