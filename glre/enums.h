#ifndef GLRE_ENUMS_H
#define GLRE_ENUMS_H


#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>

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

}
#endif // ENUMS_H

