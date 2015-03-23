#ifndef GLRE_ENUMS_H
#define GLRE_ENUMS_H


#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

namespace glre {

enum PRIMITAVE
{
    UNKNOWN_PRIMITAVE       = -1,
    LINES                   = GL_LINES,
    LINE_LOOP               = GL_LINE_LOOP,
    POINT_BIT               = GL_POINT_BIT,
    CLIENT_VERTEX_ARRAY_BIT = GL_CLIENT_VERTEX_ARRAY_BIT,
    LINE_STRIP              = GL_LINE_STRIP,
    LINE_BIT                = GL_LINE_BIT,
    TRIANGLES               = GL_TRIANGLES,
    TRIANGLE_STRIP          = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN            = GL_TRIANGLE_FAN,
    QUADS                   = GL_QUADS,
    QUAD_STRIP              = GL_QUAD_STRIP

};


enum ARRAY_TYPE
{
    ARRAY_BUFFER            = GL_ARRAY_BUFFER,
    ELEMENT_ARRAY_BUFFER    = GL_ELEMENT_ARRAY_BUFFER
};

enum FUNDAMENTAL_TYPE {

    BYTE           = GL_BYTE ,
    UNSIGNED_BYTE  = GL_UNSIGNED_BYTE ,
    SHORT          = GL_SHORT ,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT ,
    INT            = GL_INT ,
    UNSIGNED_INT   = GL_UNSIGNED_INT ,
    FLOAT          = GL_FLOAT ,
    DOUBLE         = GL_DOUBLE

};



typedef glm::vec2   vec2;
typedef glm::vec3   vec3;
typedef glm::vec4   vec4;
typedef glm::uvec4  uvec4;
typedef glm::uvec3  uvec3;
typedef glm::uvec2  uvec2;
typedef glm::ivec3  ivec3;
typedef glm::mat4   mat4;
typedef glm::quat   quat;
typedef glm::u8vec4 ucol4;
typedef glm::u8vec3 ucol3;
typedef glm::vec4    col4;
typedef glm::vec3    col3;


}
#endif // ENUMS_H

