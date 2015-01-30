#ifndef GLOBAL_H
#define GLOBAL_H

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <string>

namespace glre
{
    typedef glm::vec2   vec2;
    typedef glm::vec3   vec3;
    typedef glm::vec4   vec4;
    typedef glm::uvec3  uvec3;
    typedef glm::uvec2  uvec2;
    typedef glm::ivec3  ivec3;
    typedef glm::mat4   mat4;
    typedef glm::quat   quat;
    typedef glm::u8vec4 ucol4;
    typedef glm::u8vec3 ucol3;
    typedef glm::vec4    col4;
    typedef glm::vec3    col3;


    struct Vertex_PNCU
    {
        vec3 p; // position
        vec3 n; // normal
        col4 c; // colour
        vec2 u; // uv coords
    };

    struct Vertex_PC
    {
        vec3 p; // position
        col4 c; // colour
    };

//    typedef ArrayBuffer<Vertex_PNCU, GL_ARRAY_BUFFER>  ArrayBuffer_PNCU;
//    typedef ArrayBuffer<vec2, GL_ARRAY_BUFFER>         ArrayBuffer_vec2;
//    typedef ArrayBuffer<vec3, GL_ARRAY_BUFFER>         ArrayBuffer_vec3;
//    typedef ArrayBuffer<vec4, GL_ARRAY_BUFFER>         ArrayBuffer_vec4;
//    typedef ArrayBuffer<uvec3,GL_ELEMENT_ARRAY_BUFFER> ArrayBuffer_uvec3;
//    typedef ArrayBuffer<uvec2,GL_ELEMENT_ARRAY_BUFFER> ArrayBuffer_uvec2;


    typedef enum
    {
        LINES     = GL_LINES,
        QUADS     = GL_QUADS,
        TRIANGLES = GL_TRIANGLES
    } Primitave;

}


#endif // GLOBAL_H
