/*
 * MIT License
 *
 * Copyright (c) [year] [fullname]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef EXP_TYPES
#define EXP_TYPES



#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>


#ifndef GLA_DOUT
#define GLA_DOUT std::cout
#endif

namespace gla { namespace experimental {

class NormalizeFlags
{
    public:
    enum Flags : unsigned int
    {
        none = 0,
        _0   = (1<<0),
        _1   = (1<<1),
        _2   = (1<<2),
        _3   = (1<<3),
        _4   = (1<<4),
        _5   = (1<<5),
        _6   = (1<<6),
        _7   = (1<<7),
        _8   = (1<<8),
        _9   = (1<<9),
        _10  = (1<<10),
        _11  = (1<<11),
        _12  = (1<<12),
        _13  = (1<<13),
        _14  = (1<<14),
        _15  = (1<<15),
        _16  = (1<<16),
        _17  = (1<<17),
        _18  = (1<<18),
        _19  = (1<<19)
    };

    NormalizeFlags(unsigned int f)
    {
        _flags = f;
    }

    NormalizeFlags( Flags flags)
    {
        _flags = static_cast<unsigned int>(flags);
    }

    unsigned int _flags;
};

inline NormalizeFlags::Flags operator | (const NormalizeFlags::Flags & left, const NormalizeFlags::Flags & right )
{
    return static_cast<NormalizeFlags::Flags>( static_cast<unsigned int>(left) | static_cast<unsigned int>(right) );
}

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


using bvec2 = glm::bvec2 ;
using bvec3 = glm::bvec3 ;
using bvec4 = glm::bvec4 ;
using vec2  = glm::vec2  ;
using vec3  = glm::vec3  ;
using vec4  = glm::vec4  ;
using uvec4 = glm::uvec4 ;
using uvec3 = glm::uvec3 ;
using uvec2 = glm::uvec2 ;
using ivec2 = glm::ivec2 ;
using ivec3 = glm::ivec3 ;
using ivec4 = glm::ivec4 ;
using mat2  = glm::mat2  ;
using mat3  = glm::mat3  ;
using mat4  = glm::mat4  ;
using quat  = glm::quat  ;
using ucol4 = glm::u8vec4;
using ucol3 = glm::u8vec3;
using ucol2 = glm::u8vec2;
using ucol1 = glm::u8vec1;
using col4  = glm::vec4  ;
using col3  = glm::vec3  ;
using col2  = glm::vec2  ;

}}

#endif
