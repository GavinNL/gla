#ifndef GLA_ENUMS_H
#define GLA_ENUMS_H


#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

namespace gla {

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

//#pragma message "Note to self: if adding more types, make sure to make the appropriate chanes in teh GPUArrayBuffer"
enum BUFFER_ELEMENT_TYPE
{
    F1, F2, F3 ,F4,  // floating point 1d, 2d, 3d, 4d vectors (float, vec2, vec3, vec4)
    I1, I2, I3, I4,  // integer 1d, 2d, 3d, 4d vectors (int, ivec2, ivec3, ivec4)
    U1, U2, U3, U4,  // unsigned integer 1d, 2d, 3d, 4d vectors (uint, uvec2, uvec3, uvec4)
    B1, B2, B3, B4,  // unsigned byte
    S1, S2, S3, S4,  // unsigned short
    uB1, uB2, uB3, uB4,
    uS1, uS2, uS3, uS4
};

typedef glm::vec2   vec2;
typedef glm::vec3   vec3;
typedef glm::vec4   vec4;
typedef glm::uvec4  uvec4;
typedef glm::uvec3  uvec3;
typedef glm::uvec2  uvec2;

typedef glm::ivec2  ivec2;
typedef glm::ivec3  ivec3;
typedef glm::ivec4  ivec4;

typedef glm::mat4   mat4;
typedef glm::quat   quat;

typedef glm::u8vec4 ucol4;
typedef glm::u8vec3 ucol3;
typedef glm::u8vec2 ucol2;

typedef glm::vec4    col4;
typedef glm::vec3    col3;


}
#endif // ENUMS_H

