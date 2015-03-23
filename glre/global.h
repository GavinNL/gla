#ifndef GLRE_GLOBAL_H
#define GLRE_GLOBAL_H

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

#include <glre/types.h>


namespace glre
{


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


}

#include <glre/transformation.h>
#include <glre/vertexarrayobject.h>

namespace glre
{
    // Array Buffers
    typedef glre::ArrayBuffer_T<vec2, FLOAT>  v2ArrayBuffer;
    typedef glre::ArrayBuffer_T<vec3, FLOAT>  v3ArrayBuffer;
    typedef glre::ArrayBuffer_T<vec4, FLOAT>  v4ArrayBuffer;

    typedef glre::ArrayBuffer_T<uvec2, UNSIGNED_INT>  u2ArrayBuffer;
    typedef glre::ArrayBuffer_T<uvec3, UNSIGNED_INT>  u3ArrayBuffer;
    typedef glre::ArrayBuffer_T<uvec4, UNSIGNED_INT>  u4ArrayBuffer;

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
