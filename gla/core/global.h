#ifndef GLA_GLOBAL_H
#define GLA_GLOBAL_H

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

#include <gla/core/types.h>


namespace gla
{

    template <typename T>
    struct TBox
    {
        T x;
        T y;
        T z;
        T w;
        T h;
        T d;

    };

    template<typename T>
    struct TRect
    {
        T x;
        T y;
        T w;
        T h;
    };

    struct Vertex_PNCUBI
    {
        vec3 p; // position
        vec3 n; // normal
        col4 c; // colour
        vec2 u; // uv coords
        vec4 b; // bone weights
        uint i; // bone index
    };

    struct Vertex_PNCU
    {
        vec3 p; // position
        vec3 n; // normal
        col4 c; // colour
        vec2 u; // uv coords
    };

    struct Vertex_PNU
    {
        vec3 p;  // position
        vec3 n;  // normal
        vec3 u;  // UV
    };

    struct Vertex_PC
    {
        vec3 p; // position
        col4 c; // colour
    };

}

#include <gla/core/transformation.h>
#include <gla/core/vertexarrayobject.h>
#include <gla/core/texture.h>

namespace gla
{
    // Array Buffers
    typedef gla::ArrayBuffer_T<vec2>  v2ArrayBuffer;
    typedef gla::ArrayBuffer_T<vec3>  v3ArrayBuffer;
    typedef gla::ArrayBuffer_T<vec4>  v4ArrayBuffer;

    typedef gla::ArrayBuffer_T<uvec2> u2ArrayBuffer;
    typedef gla::ArrayBuffer_T<uvec3> u3ArrayBuffer;
    typedef gla::ArrayBuffer_T<uvec4> u4ArrayBuffer;


    typedef gla::VertexArrayObject_N VertexArrayObject;
    // Indexed Meshs
    //                                       VertexType         IndexType   ElementType      VertexAttributeTypes
    typedef gla::IndexedVertexArrayObject< gla::Vertex_PNCU,   gla::uvec3, TRIANGLES,       F3, F3, F4, F2        >   iTriMesh_PNCU;
    typedef gla::IndexedVertexArrayObject< gla::Vertex_PNCUBI, gla::uvec3, TRIANGLES,       F3, F3, F4, F2,F4,U1    > iTriMesh_PNCUBI;

    // non-indexed meshes.
    //                                       VertexType,    ElementType    VertexAttributeTypes
    typedef gla::VertexArrayObject_old< gla::Vertex_PC,         LINES,         F3, F4 >                  Line_PC;
    typedef gla::VertexArrayObject_old< gla::Vertex_PNCU,   TRIANGLE_STRIP,    F3, F3, F4, F2     >      TriStripMesh_PNCU;
    //typedef glre::IndexedVertexArrayObject< glre::Vertex_PC,   glre::uvec2, LINES,     2, F3, F4 >       Line_PC;

    typedef TBox<float>  fBox;
    typedef TBox<int>    iBox;
    typedef TBox<uint>   uBox;

    typedef TRect<float> fRect;
    typedef TRect<int>   iRect;
    typedef TRect<uint>  uRect;


    typedef Texture_T<glm::u8vec1> TextureR;
    typedef Texture_T<glm::u8vec2> TextureRG;
    typedef Texture_T<glm::u8vec3> TextureRGB;
    typedef Texture_T<glm::u8vec4> TextureRGBA;


    /* Shared Pointer stuff*/

    typedef std::shared_ptr<ArrayBuffer_b> spArrayBuffer;

    inline void GetGLError()
    {
        auto err = glGetError();
        switch(err)
        {
            case GL_INVALID_OPERATION:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_OPERATION\n");
                break;
            case GL_INVALID_ENUM:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_ENUM\n");
                break;
            case GL_INVALID_VALUE:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_VALUE\n");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n");
                break;
            case GL_OUT_OF_MEMORY:
                throw gla::GLA_EXCEPTION("ERROR: GL_OUT_OF_MEMORY\n");
                break;
            case GL_STACK_UNDERFLOW:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n");
                break;
            case GL_STACK_OVERFLOW:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n");
                break;

            default:
                break;
        }
    }
}


#endif // GLOBAL_H
