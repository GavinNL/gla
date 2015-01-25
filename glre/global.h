#ifndef GLOBAL_H
#define GLOBAL_H

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glre/arraybuffer.h>

#include <string>

namespace glre
{
    typedef glm::vec2  V2;
    typedef glm::vec3  V3;
    typedef glm::vec4  V4;
    typedef glm::uvec3 uV3;
    typedef glm::uvec2 uV2;
    typedef glm::ivec3 iV3;
    typedef glm::mat4  M4;
    typedef glm::quat  Quat;

    typedef ArrayBuffer<V2, GL_ARRAY_BUFFER>         ArrayBuffer_V2;
    typedef ArrayBuffer<V3, GL_ARRAY_BUFFER>         ArrayBuffer_V3;
    typedef ArrayBuffer<V4, GL_ARRAY_BUFFER>         ArrayBuffer_V4;
    typedef ArrayBuffer<uV3,GL_ELEMENT_ARRAY_BUFFER> ArrayBuffer_uV3;
    typedef ArrayBuffer<uV2,GL_ELEMENT_ARRAY_BUFFER> ArrayBuffer_uV2;

    typedef enum
    {
        LINES = GL_LINES,
        QUADS = GL_QUADS,
        TRIANGLES = GL_TRIANGLES
    } Primitave;

}


#endif // GLOBAL_H
