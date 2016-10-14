#ifndef GLA_ENUMS_H
#define GLA_ENUMS_H


//#include <GL/glew.h>
//#include <GL/glext.h>
//#include <GL/gl.h>


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include <iostream>
#include <tuple>

#define GLA_DOUT if(1) std::cout
#define IS_SAME(First, Second) std::is_same<First, Second>::value

static inline std::ostream& operator <<(std::ostream & out, const glm::vec2 & v)
{
    out << "{" << v[0] << "," << v[1] << "}";
    return out;
}

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


enum class TexFilter {
    NEAREST                = GL_NEAREST,
    LINEAR                 = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
};


typedef enum {
    CLAMP_TO_EDGE          = GL_CLAMP_TO_EDGE,
    REPEAT                 = GL_REPEAT
} TEXTUREWRAPTYPE;

enum class TexWrap {
    CLAMP_TO_EDGE          = GL_CLAMP_TO_EDGE,
    REPEAT                 = GL_REPEAT
};


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

//enum class TexColourFormat
//{
//    RED            = GL_RED,
//    RG             = GL_RG,
//    RGB            = GL_RGB,
//    BGR            = GL_BGR,
//    RGBA           = GL_RGBA,
//    BGRA           = GL_BGRA,
//    DEPTH_COMPONENT= GL_DEPTH_COMPONENT,
//    DEPTH_STENCIL  = GL_DEPTH_STENCIL,

//};

enum class TexFormat
{
    RED               = GL_RED,
    RG                = GL_RG,
    RGB               = GL_RGB,
    BGR               = GL_BGR,
    RGBA              = GL_RGBA,
    BGRA              = GL_BGRA,
    RED_INTEGER       = GL_RED_INTEGER,
    RG_INTEGER        = GL_RG_INTEGER,
    RGB_INTEGER       = GL_RGB_INTEGER,
    BGR_INTEGER       = GL_BGR_INTEGER,
    RGBA_INTEGER      = GL_RGBA_INTEGER,
    BGRA_INTEGER      = GL_BGRA_INTEGER,
    STENCIL_INDEX     = GL_STENCIL_INDEX,
    DEPTH_COMPONENT   = GL_DEPTH_COMPONENT,
    DEPTH_STENCIL     = GL_DEPTH_STENCIL
};

enum class TexInternalFormat
{
    RED            = GL_RED,
    RG             = GL_RG,
    RGB            = GL_RGB,
    BGR            = GL_BGR,
    RGBA           = GL_RGBA,
    BGRA           = GL_BGRA,
    DEPTH_COMPONENT= GL_DEPTH_COMPONENT,
    DEPTH_STENCIL  = GL_DEPTH_STENCIL,

    R8               = GL_R8                   ,
    R8_SNORM	     = GL_R8_SNORM	            ,
    R16	             = GL_R16	                ,
    R16_SNORM	     = GL_R16_SNORM	        ,
    RG8		         = GL_RG8		            ,
    RG8_SNORM	     = GL_RG8_SNORM	 	    ,
    RG16	         = GL_RG16	           ,
    RG16_SNORM	     = GL_RG16_SNORM	        ,
    R3_G3_B2	     = GL_R3_G3_B2	            ,
    RGB4	         = GL_RGB4	                ,
    RGB5	         = GL_RGB5	                ,
    RGB8	         = GL_RGB8	                ,
    RGB8_SNORM       = GL_RGB8_SNORM           ,
    RGB10	         = GL_RGB10	            ,
    RGB12	         = GL_RGB12	            ,
    RGB16_SNORM	     = GL_RGB16_SNORM	        ,
    RGBA2	         = GL_RGBA2	            ,
    RGBA4	         = GL_RGBA4	            ,
    RGB5_A1	         = GL_RGB5_A1	            ,
    RGBA8	         = GL_RGBA8	            ,
    RGBA8_SNORM      = GL_RGBA8_SNORM          ,
    RGB10_A2	     = GL_RGB10_A2	            ,
    RGB10_A2UI	     = GL_RGB10_A2UI	        ,
    RGBA12           = GL_RGBA12               ,
    RGBA16           = GL_RGBA16               ,
    SRGB8            = GL_SRGB8                ,
    SRGB8_ALPHA8     = GL_SRGB8_ALPHA8         ,
    R16F             = GL_R16F                 ,
    RG16F            = GL_RG16F                ,
    RGB16F           = GL_RGB16F               ,
    RGBA16F          = GL_RGBA16F              ,
    R32F	         = GL_R32F	                ,
    RG32F	         = GL_RG32F	            ,
    RGB32F	         = GL_RGB32F	            ,
    RGBA32F	         = GL_RGBA32F	            ,
    R11F_G11F_B10F   = GL_R11F_G11F_B10F	    ,
    RGB9_E5	         = GL_RGB9_E5	            ,
    R8I	             = GL_R8I	                ,
    R8UI	         = GL_R8UI	                ,
    R16I	         = GL_R16I	                ,
    R16UI	         = GL_R16UI	            ,
    R32I	         = GL_R32I	                ,
    R32UI            = GL_R32UI                ,
    RG8I	         = GL_RG8I	           ,
    RG8UI	         = GL_RG8UI	            ,
    RG16I	         = GL_RG16I	            ,
    RG16UI	         = GL_RG16UI	            ,
    RG32I	         = GL_RG32I	            ,
    RG32UI	         = GL_RG32UI	            ,
    RGB8I	         = GL_RGB8I	            ,
    RGB8UI	         = GL_RGB8UI	            ,
    RGB16I	         = GL_RGB16I	            ,
    RGB16UI	         = GL_RGB16UI	            ,
    RGB32I	         = GL_RGB32I	            ,
    RGB32UI	         = GL_RGB32UI	            ,
    RGBA8I	         = GL_RGBA8I	            ,
    RGBA8UI	         = GL_RGBA8UI	            ,
    RGBA16I	         = GL_RGBA16I	            ,
    RGBA16UI	     = GL_RGBA16UI	            ,
    RGBA32I	         = GL_RGBA32I	            ,
    RGBA32UI	     = GL_RGBA32UI,
    COMPRESSED_RED                     = GL_COMPRESSED_RED,
    COMPRESSED_RG	                   = GL_COMPRESSED_RG	,
    COMPRESSED_RGB	                   = GL_COMPRESSED_RGB	,
    COMPRESSED_RGBA                    = GL_COMPRESSED_RGBA,
    COMPRESSED_SRGB                    = GL_COMPRESSED_SRGB,
    COMPRESSED_SRGB_ALPHA              = GL_COMPRESSED_SRGB_ALPHA,
    COMPRESSED_RED_RGTC1               = GL_COMPRESSED_RED_RGTC1,
    COMPRESSED_SIGNED_RED_RGTC1        = GL_COMPRESSED_SIGNED_RED_RGTC1,
    COMPRESSED_RG_RGTC2	               = GL_COMPRESSED_RG_RGTC2	,
    COMPRESSED_SIGNED_RG_RGTC2	       = GL_COMPRESSED_SIGNED_RG_RGTC2	,
    COMPRESSED_RGBA_BPTC_UNORM	       = GL_COMPRESSED_RGBA_BPTC_UNORM	,
    COMPRESSED_SRGB_ALPHA_BPTC_UNORM   = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM	,
    COMPRESSED_RGB_BPTC_SIGNED_FLOAT   = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT	,
    COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
};

enum PRIMITAVE
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
//    QUAD_STRIP              = GL_QUAD_STRIP

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


enum class ArrayBufferUsage
{
    STATIC_DRAW  = GL_STATIC_DRAW,
    DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    STREAM_DRAW  = GL_STREAM_DRAW
};

enum class ArrayType
{
    ARRAY_BUFFER            = GL_ARRAY_BUFFER,
    ELEMENT_ARRAY_BUFFER    = GL_ELEMENT_ARRAY_BUFFER
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

enum class DataType {
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

enum class VertexType
{
    F1, F2, F3 ,F4,  // floating point 1d, 2d, 3d, 4d vectors (float, vec2, vec3, vec4)
    I1, I2, I3, I4,  // integer 1d, 2d, 3d, 4d vectors (int, ivec2, ivec3, ivec4)
    U1, U2, U3, U4,  // unsigned integer 1d, 2d, 3d, 4d vectors (uint, uvec2, uvec3, uvec4)
    B1, B2, B3, B4,  // unsigned byte
    S1, S2, S3, S4,  // unsigned short
    uB1, uB2, uB3, uB4,
    uS1, uS2, uS3, uS4
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




