#ifndef GLA_MESH_BUFFER
#define GLA_MESH_BUFFER

#include<gla/core/buffers.h>


namespace gla {

namespace eng {

struct Mesh_T
{

    std::size_t            first;
    std::size_t            count;
    std::size_t            base_vertex;
    gla::core::DataType    index_type;
    gla::core::VertexArray vao;
};

template<typename VertexStruct, typename index_type=unsigned int>
/**
 * @brief The MeshBuffer class
 *
 * The MeshBuffer class is a manager that manages vertex/index buffers of multiple mesh objects
 * but stores all the vertex and index data in one large buffer.
 *
 * Use the MeshBuffer if you have lots of different meshes that use the same vertex attributes
 * eg:  positions,UV, normals     or positions,uv,normals,colours
 */
class MeshBuffer
{
    MeshBuffer()
    {

    }

    ~MeshBuffer()
    {

    }

    void ReserveVertices(std::size_t num_vertices)
    {
        vertex_buffer.Reserve( sizeof(VertexStruct) * num_vertices);
    }

    void ReserveIndices(std::size_t num_indices)
    {
        static_assert(
        std::is_same< index_type, std::uint8_t >::value  ||
        std::is_same< index_type, std::uint16_t >::value ||
        std::is_same< index_type, std::uint32_t >::value ||
        std::is_same< index_type, std::int8_t >::value   ||
        std::is_same< index_type, std::int16_t >::value  ||
        std::is_same< index_type, std::int32_t >::value , "Not correct index type" );


        ElementArrayBuffer.Reserve( sizeof(VertexStruct) * num_indices);
    }

    template<typename ...GLM_Types>
    Mesh_T Append( const std::vector<VertexStruct> & V, const std::vector<index_type> & I)
    {
        auto v_byte = vertex_buffer.Append( V ); // insert the data and return the byte index of where it was placed.
        auto i_byte = index_buffer.Append( I );  // insert the data and return the byte index of where it was placed.

        Mesh_T M;

        M.base_vertex = v_byte / sizeof(VertexStruct );  // determine which index the base vertex is actually at
        M.base_index  = i_byte / sizeof(index_type);     // determine which index the base index is actually at
        M.count       = I.size();
        M.vao         = gla::core::VertexArray::MakeVAO<GLM_Types...>( vertex_buffer, index_buffer);

        if( std::is_same< index_type, std::uint8_t >::value  ) M.index_type = gla::core::DataType::UNSIGNED_BYTE;
        if( std::is_same< index_type, std::uint16_t >::value ) M.index_type = gla::core::DataType::UNSIGNED_SHORT;
        if( std::is_same< index_type, std::uint32_t >::value ) M.index_type = gla::core::DataType::UNSIGNED_INT;
        if( std::is_same< index_type, std::int8_t >::value   ) M.index_type = gla::core::DataType::BYTE;
        if( std::is_same< index_type, std::int16_t >::value  ) M.index_type = gla::core::DataType::SHORT;
        if( std::is_same< index_type, std::int32_t >::value  ) M.index_type = gla::core::DataType::INT;

        // when drawing the mesh, we will have to draw it as follows:
        //glDrawElementsBaseVertex(GL_TRIANGLES,
        //    M.count,
        //    M.index_type,
        //    static_cast<char*>(0)+M.base_index,
        //    M.base_vertex);


        return M;
    }

    gla::core::ArrayBuffer         vertex_buffer;
    gla::core::ElementArrayBuffer  index_buffer;

};

} // namespace eng
} // namespace gla
#endif
