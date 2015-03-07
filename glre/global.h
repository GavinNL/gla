#ifndef GLOBAL_H
#define GLOBAL_H

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>


namespace glre
{
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

    class GPUTexture;
    class Texture;

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

    typedef enum
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

    } Primitave;
}

#include <glre/transformation.h>
#include <glre/vertexarrayobject.h>


namespace glre
{
    // Array Buffers
    typedef glre::ArrayBuffer_T<vec2, GL_FLOAT>  v2ArrayBuffer;
    typedef glre::ArrayBuffer_T<vec3, GL_FLOAT>  v3ArrayBuffer;
    typedef glre::ArrayBuffer_T<vec4, GL_FLOAT>  v4ArrayBuffer;

    typedef glre::ArrayBuffer_T<uvec2, GL_UNSIGNED_INT>  u2ArrayBuffer;
    typedef glre::ArrayBuffer_T<uvec3, GL_UNSIGNED_INT>  u3ArrayBuffer;
    typedef glre::ArrayBuffer_T<uvec4, GL_UNSIGNED_INT>  u4ArrayBuffer;

    // Indexed Meshs
    //                                       VertexType         IndexType   ElementType      VertexAttributeTypes
    typedef glre::IndexedVertexArrayObject< glre::Vertex_PNCU,   glre::uvec3, TRIANGLES,       F3, F3, F4, F2        >   iTriMesh_PNCU;
    typedef glre::IndexedVertexArrayObject< glre::Vertex_PNCUBI, glre::uvec3, TRIANGLES,       F3, F3, F4, F2,F4,U1    > iTriMesh_PNCUBI;

    // non-indexed meshes.
    //                                       VertexType,    ElementType    VertexAttributeTypes
    typedef glre::VertexArrayObject< glre::Vertex_PC,         LINES,         F3, F4 >                  Line_PC;
    typedef glre::VertexArrayObject< glre::Vertex_PNCU,   TRIANGLE_STRIP,    F3, F3, F4, F2     >      TriStripMesh_PNCU;
    //typedef glre::IndexedVertexArrayObject< glre::Vertex_PC,   glre::uvec2, LINES,     2, F3, F4 >       Line_PC;

    typedef TBox<float>  fBox;
    typedef TBox<int>    iBox;
    typedef TBox<uint>   uBox;

    typedef TRect<float> fRect;
    typedef TRect<int>   iRect;
    typedef TRect<uint>  uRect;

    /* Implementation in texture.cpp */
    Texture LoadTexture(const std::string & path);



    /* Shared Pointer stuff*/

    typedef std::shared_ptr<ArrayBuffer_b> spArrayBuffer;
}


#endif // GLOBAL_H
