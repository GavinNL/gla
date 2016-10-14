#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec2 vertexPosition_modelspace2;
//layout(location = 2) in vec3 vertexPosition_modelspace3;

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;
uniform mat4 uModel;

void main()
{
	// Same, but with the light's view matrix
    gl_Position =  depthMVP * uModel vec4(vertexPosition_modelspace,1);
}
