<vertex>

#version 330 core
layout (location = 0) in vec3 in_Position;

uniform mat4 u_LightMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
    gl_Position = u_LightMatrix * u_ModelMatrix * vec4(in_Position, 1.0f);
}

</vertex>



<fragment>

#version 330 core

void main()
{
     gl_FragDepth = gl_FragCoord.z;
}

</fragment>
