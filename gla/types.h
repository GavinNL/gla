#ifndef GLA_ENUMS_H
#define GLA_ENUMS_H


#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include <iostream>
#include <tuple>


#define IS_SAME(First, Second) std::is_same<First, Second>::value


static inline std::ostream& operator <<(std::ostream & out, const glm::vec3 & v)
{
    out << "{" << v[0] << "," << v[1] << "," << v[2] << "}";
    return out;
}

static inline std::ostream& operator <<(std::ostream & out, const glm::vec4 & v)
{
    out << "{" << v[0] << "," << v[1] << "," << v[2] << "," << v[3] << "}";
    return out;
}

static inline std::ostream& operator <<(std::ostream & out, const glm::mat4 & v)
{
    out << v[0] << std::endl;
    out << v[1] << std::endl;
    out << v[2] << std::endl;
    out << v[3] << std::endl;

    return out;
}

static inline std::ostream& operator <<(std::ostream & out, const glm::mat3 & v)
{
    out << v[0] << std::endl;
    out << v[1] << std::endl;
    out << v[2] << std::endl;

    return out;
}

namespace gla {

/*

template<std::size_t> struct int_{};

template <class Tuple, size_t Pos>
std::ostream& print_tuple(std::ostream& out, const Tuple& t, int_<Pos> )
{
  out << std::get< std::tuple_size<Tuple>::value-Pos >(t) << ',';
  return print_tuple(out, t, int_<Pos-1>());
}

template <class Tuple>
std::ostream& print_tuple(std::ostream& out, const Tuple& t, int_<1> )
{
  return out << std::get<std::tuple_size<Tuple>::value-1>(t);
}

template <class... Args>
ostream& operator<<(ostream& out, const std::tuple<Args...>& t)
{
  out << '(';
  print_tuple(out, t, int_<sizeof...(Args)>());
  return out << ')';
}
*/

class GPUTexture;
class FrameBufferObject;



typedef enum {
    NEAREST                = GL_NEAREST,
    LINEAR                 = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
} TEXTUREFILTERTYPE;


typedef enum {
    CLAMP                  = GL_CLAMP,
    REPEAT                 = GL_REPEAT
} TEXTUREWRAPTYPE;


typedef enum
{
    RED            = GL_RED,
    RG             = GL_RG,
    RGB            = GL_RGB,
    BGR            = GL_BGR,
    RGBA           = GL_RGBA,
    BGRA           = GL_BGRA,
    DEPTH_COMPONENT= GL_DEPTH_COMPONENT,
    DEPTH_STENCIL  = GL_DEPTH_STENCIL
} TEXTURECOLOURFORMAT;

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
    uS1, uS2, uS3, uS4,

    UNKNOWN_ELEMENT_TYPE
};




typedef glm::bvec2  bvec2;
typedef glm::bvec3  bvec3;
typedef glm::bvec4  bvec4;

typedef glm::vec2   vec2;
typedef glm::vec3   vec3;
typedef glm::vec4   vec4;

typedef glm::uvec4  uvec4;
typedef glm::uvec3  uvec3;
typedef glm::uvec2  uvec2;


typedef glm::ivec2  ivec2;
typedef glm::ivec3  ivec3;
typedef glm::ivec4  ivec4;

typedef glm::mat2   mat2;
typedef glm::mat3   mat3;
typedef glm::mat4   mat4;
typedef glm::quat   quat;

typedef glm::u8vec4 ucol4;
typedef glm::u8vec3 ucol3;
typedef glm::u8vec2 ucol2;
typedef glm::u8vec1 ucol1;

typedef glm::vec4     col4;
typedef glm::vec3     col3;
typedef glm::vec2     col2;


}



#endif // ENUMS_H

