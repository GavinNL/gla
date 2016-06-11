#ifndef GLA_GLOBAL_H
#define GLA_GLOBAL_H


#include <gla/types.h>


namespace gla
{

    template <typename T>
    struct TBox
    {
        TBox() : pos(0), dim(0) {}
        T pos;
        T dim;
    };


}

// #include <gla/core/transformation.h>
// #include <gla/core/vertexarrayobject.h>
// #include <gla/core/texture.h>

namespace gla
{
    // Array Buffers
    // typedef gla::ArrayBuffer_T<vec2>  v2ArrayBuffer;
    // typedef gla::ArrayBuffer_T<vec3>  v3ArrayBuffer;
    // typedef gla::ArrayBuffer_T<vec4>  v4ArrayBuffer;
    //
    // typedef gla::ArrayBuffer_T<uvec2> u2ArrayBuffer;
    // typedef gla::ArrayBuffer_T<uvec3> u3ArrayBuffer;
    // typedef gla::ArrayBuffer_T<uvec4> u4ArrayBuffer;
    //
    //
    //typedef gla::VertexArrayObject_N VertexArrayObject;
    // Indexed Meshs
    //                                       VertexType         IndexType   ElementType      VertexAttributeTypes
    //typedef gla::IndexedVertexArrayObject< gla::Vertex_PNCU,   gla::uvec3, TRIANGLES,       F3, F3, F4, F2        >   iTriMesh_PNCU;
    //typedef gla::IndexedVertexArrayObject< gla::Vertex_PNCUBI, gla::uvec3, TRIANGLES,       F3, F3, F4, F2,F4,U1    > iTriMesh_PNCUBI;

    // non-indexed meshes.
    //                                       VertexType,    ElementType    VertexAttributeTypes
    //typedef gla::VertexArrayObject_old< gla::Vertex_PC,         LINES,         F3, F4 >                  Line_PC;
    //typedef gla::VertexArrayObject_old< gla::Vertex_PNCU,   TRIANGLE_STRIP,    F3, F3, F4, F2     >      TriStripMesh_PNCU;
    //typedef glre::IndexedVertexArrayObject< glre::Vertex_PC,   glre::uvec2, LINES,     2, F3, F4 >       Line_PC;

    typedef TBox<gla::vec3>  fBox;
    typedef TBox<gla::ivec3> iBox;
    typedef TBox<gla::uvec3> uBox;

    typedef TBox<gla::vec2>  fRect;
    typedef TBox<gla::ivec2> iRect;
    typedef TBox<gla::uvec2> uRect;


    //typedef Texture_T<glm::u8vec1> TextureR;
    //typedef Texture_T<glm::u8vec2> TextureRG;
    //typedef Texture_T<glm::u8vec3> TextureRGB;
    //typedef Texture_T<glm::u8vec4> TextureRGBA;


    /* Shared Pointer stuff*/

    //typedef std::shared_ptr<ArrayBuffer_b> spArrayBuffer;




}


#endif // GLOBAL_H
