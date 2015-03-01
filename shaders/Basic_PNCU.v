#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec4 inColour;
layout (location = 3) in vec2 inUV;

uniform mat4 inCameraMatrix;
uniform mat4 inModelMatrix;
uniform mat4 inProjectionMatrix;

//==================================
uniform vec3 uLightPosition;
//==================================

out vec4 oColour;
out vec2 oTexCoord0;

out vec3 oPosition_ws;
out vec3 oNormal_cs;
out vec3 oEyeDirection_cs;
out vec3 oLightDirection_cs;

void main()
{
    // Position of the vertex, in worldspace : M * position
    oPosition_ws = (inModelMatrix * vec4(inPosition,1)).xyz;

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  inProjectionMatrix * inCameraMatrix * vec4(oPosition_ws,1);

    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0,0,0).
    vec3 position_cs = ( inCameraMatrix * inModelMatrix * vec4(inPosition,1)).xyz;

    oEyeDirection_cs = vec3(0,0,0) - position_cs;

    // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
    vec3 LightPosition_cameraspace = ( inCameraMatrix * vec4(uLightPosition, 1) ).xyz;

    oLightDirection_cs = LightPosition_cameraspace + oEyeDirection_cs;

    // Normal of the the vertex, in camera space
    oNormal_cs = ( inCameraMatrix * inModelMatrix * vec4(inNormal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

    // UV of the vertex. No special space for this one.
    oTexCoord0 = inUV;
    oColour    = inColour;
}
