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

#ifndef GLA_MESH_BUFFER
#define GLA_MESH_BUFFER

#include<gla/core/buffers.h>

#include "memory_pool.h"

namespace gla {


/**
 * @brief The Mesh_T class
 *
 * A reference counted object that exists on the GPU.
 *
 * A Mesh_T is a wrapper around a vertex buffer, index buffer and encapsulated
 * in a single VertexArrayObject.
 *
 * This object is created by a MeshBuffer object which manages the vertex and index buffers
 * as well as deals with freeing the data when no more references are available.
 */
class Mesh_T
{

public:
    //std::size_t          first=0;
    std::size_t            count = 0;
    std::size_t            base_index_location = 0;
    std::size_t            base_vertex = 0;
    gla::DataType          index_type=gla::DataType::UNKNOWN;
    gla::VertexArray       vao;
    std::shared_ptr< std::pair<std::size_t,std::size_t> > mem; //

    Mesh_T(){}
    Mesh_T(const Mesh_T & other) :
       //first               (other.first),
       count               (other.count),
       base_index_location (other.base_index_location),
       base_vertex         (other.base_vertex),
       index_type          (other.index_type),
       vao                 (other.vao),
       mem                 (other.mem)
    {
    }

    Mesh_T( Mesh_T && other) :
       //first               ( std::move(other.first)),
       count               ( std::move(other.count)),
       base_index_location ( std::move(other.base_index_location)),
       base_vertex         ( std::move(other.base_vertex)),
       index_type          ( std::move(other.index_type)),
       vao                 ( std::move(other.vao)),
       mem                 ( std::move(other.mem))
    {
    }

    Mesh_T& operator=( const Mesh_T & other)
    {
        if( this != &other)
        {
       //first               = (other.first);
       count               = (other.count);
       base_index_location = (other.base_index_location);
       base_vertex         = (other.base_vertex);
       index_type          = (other.index_type);
       vao                 = (other.vao);
       mem                 = (other.mem);
        }
        return *this;
    }

    Mesh_T& operator=( Mesh_T && other)
    {
        if( this != &other)
        {
       //first               = std::move(other.first);
       count               = std::move(other.count);
       base_index_location = std::move(other.base_index_location);
       base_vertex         = std::move(other.base_vertex);
       index_type          = std::move(other.index_type);
       vao                 = std::move(other.vao);
       mem                 = std::move(other.mem);
        }
        return *this;
    }

public:


    template<bool bind_first=true>
    void Draw( gla::Primitave prim = gla::Primitave::TRIANGLES) const
    {
        if(bind_first) vao.Bind();

        index_type==gla::DataType::UNKNOWN ?
                    gla::DrawArrays( prim, count, base_vertex) :
                    gla::DrawElementsBaseVertex( prim, count, index_type, base_index_location, base_vertex);
    }

    template<bool bind_first=true>
    void DrawInstanced( std::size_t draw_count=1, gla::Primitave prim = gla::Primitave::TRIANGLES) const
    {
        if(bind_first) vao.Bind();

        index_type==gla::DataType::UNKNOWN ?
                    gla::DrawArraysInstanced( prim, count, base_vertex, draw_count) :
                    gla::DrawElementsInstancedBaseVertex(prim, count, index_type, base_index_location, base_vertex, draw_count);

    }

    template<typename index_type, typename ...GLM_Types>
        friend class MeshBuffer;
};

//template<typename VertexStruct, typename index_type=unsigned int>
/**
 * @brief The MeshBuffer class
 *
 * The MeshBuffer class is a manager that manages vertex/index buffers of multiple mesh objects
 * but stores all the vertex and index data in one large buffer.
 *
 * Use the MeshBuffer if you have lots of different meshes that use the same vertex attributes
 * eg:  positions,UV, normals     or positions,uv,normals,colours
 *
 * Careful when calling the MeshBuffer's destructor. All mesh's created by the buffer
 * are still active and will not be released until all references to the underlying
 * arraybuffers, index buffers and vertexarrayobjects are released.
 */
template<typename index_type, typename ...GLM_Types>
class MeshBuffer
{
private:
    // meta function for determining the max and min size type in
    // a parameter pack.
    template<typename T, typename ...Tail>
    struct type_size{};

    template<typename T, typename Head>
    struct type_size<T,Head>
    {
        static const std::size_t max   = sizeof(T)>sizeof(Head) ? sizeof(T) : sizeof(Head);
        static const std::size_t min   = sizeof(T)<sizeof(Head) ? sizeof(T) : sizeof(Head);
        static const std::size_t sum   = sizeof(T) + sizeof(Head);
    };

    template<typename T, typename Head, typename ...Tail>
    struct type_size<T,Head,Tail...>
    {
        static const std::size_t max = sizeof(T) > type_size<Head,Tail...>::max ? sizeof(T) : type_size<Head,Tail...>::max;
        static const std::size_t min = sizeof(T) < type_size<Head,Tail...>::min ? sizeof(T) : type_size<Head,Tail...>::min;
        static const std::size_t sum = sizeof(T) + type_size<Head,Tail...>::sum;

    };

public:
    using buffer_interval = std::pair< std::size_t, std::size_t>;
    static const std::size_t vertex_size = type_size<GLM_Types...>::sum;

    MeshBuffer() : m_VertexPool( new MemoryPool() ),  m_IndexPool( new MemoryPool() )
    {

    }

    ~MeshBuffer()
    {
//        vertex_buffer.ForceDelete();
//        index_buffer.ForceDelete();
//        vao.ForceDelete();
    }

    void ReserveVertices(std::size_t num_vertices)
    {
        vertex_buffer.Reserve( vertex_size * num_vertices);
        m_VertexPool->Reserve(vertex_size * num_vertices);
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

        index_buffer.Reserve( sizeof(index_type) * num_indices);

        m_IndexPool->Reserve( sizeof(index_type) * num_indices);

        //index_buffer.Resize();
    }

    std::size_t VertexBufferSize() const
    {
        return vertex_buffer.Size();
    }

    std::size_t IndexBufferSize() const
    {
        return index_buffer.Size();
    }


    template<typename VertexStruct>
    Mesh_T Insert( const std::vector<VertexStruct> & V )
    {

        static_assert( sizeof(VertexStruct) == vertex_size , "The struct used to hold the vertex is not the same size as the template parameter arguments of the MeshBuffer");

        auto vertex_size= V.size() * sizeof(VertexStruct);


        auto v_byte = m_VertexPool->Malloc(vertex_size);


        assert(v_byte != std::numeric_limits<std::size_t>::max() );

        vertex_buffer.CopyData(V, v_byte);

        if( !vao )
        {
            vao = gla::VertexArray::MakeVAO<GLM_Types...>( vertex_buffer );
        }

        Mesh_T M;

        M.base_vertex          = v_byte / sizeof(VertexStruct );  // determine which index the base vertex is actually at
        M.count                = V.size();

        auto vpool = m_VertexPool;
        M.mem = std::shared_ptr< std::pair< std::size_t, std::size_t> >( new std::pair<std::size_t,std::size_t>(v_byte, 0),
                                                                         [vpool](std::pair<std::size_t,std::size_t> * p)
                                                                         {
                                                                            GLA_LOG << "Freeing Data!:" << std::endl;
                                                                            GLA_LOG << "  vertex  Data!:" << vpool->Free(p->first) << std::endl;
                                                                            delete p;
                                                                         }
                                                                         );

        M.index_type = gla::DataType::UNKNOWN;

        M.vao = vao;

        GLA_LOG << "Non-Indexed Mesh Generated:" << std::endl;
        GLA_LOG << "   Base Vertex Byte location: " << v_byte<< std::endl;
        GLA_LOG << "   Base Vertex: " << M.base_vertex << std::endl;
        GLA_LOG << "   Num Vert:" << M.count << std::endl;
        GLA_LOG << "   VAO: " << M.vao.Get() << std::endl;
        GLA_LOG << "   Vertex Bytes Allocated: " << vertex_size << std::endl;

        return M;
    }

    template<typename VertexStruct>
    Mesh_T Insert( const std::vector<VertexStruct> & V, const std::vector<index_type> & I)
    {

        static_assert( sizeof(VertexStruct) == vertex_size , "The struct used to hold the vertex is not the same size as the template parameter arguments of the MeshBuffer");

        auto vertex_size= V.size() * sizeof(VertexStruct);
        auto index_size = I.size() * sizeof(index_type);

        auto v_byte = m_VertexPool->Malloc(vertex_size);
        auto i_byte =  m_IndexPool->Malloc(index_size);

        assert(v_byte != std::numeric_limits<std::size_t>::max() );
        assert(i_byte != std::numeric_limits<std::size_t>::max() );

        vertex_buffer.CopyData(V, v_byte);
        index_buffer.CopyData(I,  i_byte);


        //auto v_byte = vertex_buffer.Append( V ); // insert the data and return the byte index of where it was placed.
        //auto i_byte = index_buffer.Append( I );  // insert the data and return the byte index of where it was placed.

        if( !vao )
        {
            vao = gla::VertexArray::MakeVAO<GLM_Types...>( vertex_buffer, index_buffer);
        }

        Mesh_T M;

        M.base_vertex          = v_byte / sizeof(VertexStruct );  // determine which index the base vertex is actually at
        M.base_index_location  = i_byte ;     // determine which index the base index is actually at
        M.count                = I.size();

        auto ipool = m_IndexPool;
        auto vpool = m_VertexPool;
        M.mem = std::shared_ptr< std::pair< std::size_t, std::size_t> >( new std::pair<std::size_t,std::size_t>(v_byte, i_byte),
                                                                         [ipool, vpool](std::pair<std::size_t,std::size_t> * p)
                                                                         {
                                                                            GLA_LOG << "Freeing Data!:" << std::endl;
                                                                            GLA_LOG << "  vertex  Data!:" << vpool->Free(p->first) << std::endl;
                                                                            GLA_LOG << "  index   Data!:" << ipool->Free(p->second) << std::endl;
                                                                            delete p;
                                                                         }
                                                                         );

        if( std::is_same< index_type, std::uint8_t >::value  ) M.index_type = gla::DataType::UNSIGNED_BYTE;
        if( std::is_same< index_type, std::uint16_t >::value ) M.index_type = gla::DataType::UNSIGNED_SHORT;
        if( std::is_same< index_type, std::uint32_t >::value ) M.index_type = gla::DataType::UNSIGNED_INT;
        if( std::is_same< index_type, std::int8_t >::value   ) M.index_type = gla::DataType::BYTE;
        if( std::is_same< index_type, std::int16_t >::value  ) M.index_type = gla::DataType::SHORT;
        if( std::is_same< index_type, std::int32_t >::value  ) M.index_type = gla::DataType::INT;

        M.vao = vao;

        GLA_LOG << "Mesh Generated:" << std::endl;
        GLA_LOG << "   Base Vertex Byte location: " << v_byte<< std::endl;
        GLA_LOG << "   Base Index Byte location: " << i_byte<< std::endl;
        GLA_LOG << "   Base Vertex: " << M.base_vertex << std::endl;
        GLA_LOG << "   Indices: " << M.count << std::endl;
        GLA_LOG << "   VAO: " << M.vao.Get() << std::endl;
        GLA_LOG << "   Vertex Bytes Allocated: " << vertex_size << std::endl;
        GLA_LOG << "   Index Bytes Allocated: " << index_size  << std::endl;

        return M;
    }



protected:


    std::shared_ptr<
    std::vector< buffer_interval>
    >                                m_FreeData;     //! A vector of freely available data

    gla::VertexArray                 vao;
    gla::ArrayBuffer                 vertex_buffer;
    gla::ElementArrayBuffer          index_buffer;

    std::shared_ptr<MemoryPool>                       m_VertexPool;
    std::shared_ptr<MemoryPool>                       m_IndexPool;

};




} // namespace gla
#endif
