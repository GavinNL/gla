#ifndef EXP_TYPES
#define EXP_TYPES


//#define USE_GLAD

#ifdef USE_GLAD
#include<gla/glad.h>
#endif

#include <glm/glm.hpp>

namespace gla { namespace experimental {


enum class DataType
{
    UNKNOWN        = -1,
    BYTE           = GL_BYTE ,
    UNSIGNED_BYTE  = GL_UNSIGNED_BYTE ,
    SHORT          = GL_SHORT ,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT ,
    INT            = GL_INT ,
    UNSIGNED_INT   = GL_UNSIGNED_INT ,
    FLOAT          = GL_FLOAT ,
    DOUBLE         = GL_DOUBLE
};

enum class Primitave
{
    UNKNOWN_PRIMITAVE       = -1,
    LINES                   = GL_LINES,
    LINE_LOOP               = GL_LINE_LOOP,
//    POINT_BIT               = GL_POINT_BIT,
//    CLIENT_VERTEX_ARRAY_BIT = GL_CLIENT_VERTEX_ARRAY_BIT,
    LINE_STRIP              = GL_LINE_STRIP,
//    LINE_BIT                = GL_LINE_BIT,
    TRIANGLES               = GL_TRIANGLES,
    TRIANGLE_STRIP          = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN            = GL_TRIANGLE_FAN,
//    QUADS                   = GL_QUADS,
//    QUAD_STRIP              = GL_QUAD_STRIP,
    PATCHES                 = GL_PATCHES

};

enum class AttributeTypes
{
    v1 ,
    v1n, // normalized
    v2 ,
    v2n, // normalized
    v3 ,
    v3n, // normalized
    v4 ,
    v4n, // normalized

    // Integer
    iv1 ,
    iv1n, // normalized
    iv2 ,
    iv2n, // normalized
    iv3 ,
    iv3n, // normalized
    iv4 ,
    iv4n, // normalized

    // unsigned int
    uv1 ,
    uv1n, // normalized
    uv2 ,
    uv2n, // normalized
    uv3 ,
    uv3n, // normalized
    uv4 ,
    uv4n, // normalized

    // short
    i16v1 ,
    i16v1n, // normalized
    i16v2 ,
    i16v2n, // normalized
    i16v3 ,
    i16v3n, // normalized
    i16v4 ,
    i16v4n, // normalized

    // unsigned short
    u16v1 ,
    u16v1n, // normalized
    u16v2 ,
    u16v2n, // normalized
    u16v3 ,
    u16v3n, // normalized
    u16v4 ,
    u16v4n, // normalized

    // byte
    i8v1 ,
    i8v1n, // normalized
    i8v2 ,
    i8v2n, // normalized
    i8v3 ,
    i8v3n, // normalized
    i8v4 ,
    i8v4n, // normalized

    // unsigned byte
    u8v1 ,
    u8v1n, // normalized
    u8v2 ,
    u8v2n, // normalized
    u8v3 ,
    u8v3n, // normalized
    u8v4 ,
    u8v4n // normalized
};

}}

#endif
