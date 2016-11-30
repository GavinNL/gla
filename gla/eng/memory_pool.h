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

#ifndef GLA_MULTI_OBJECT_BUFFER
#define GLA_MULTI_OBJECT_BUFFER


#include <set>

namespace gla {


//template<typename VertexStruct, typename index_type=unsigned int>
/**
 * @brief The MeshBuffer class
 *
 * The MeshBuffer class is a manager that manages vertex/index buffers of multiple mesh objects
 * but stores all the vertex and index data in one large buffer.
 *
 * Use the MeshBuffer if you have lots of different meshes that use the same vertex attributes
 * eg:  positions,UV, normals     or positions,uv,normals,colours
 */
class MemoryPool
{

public:
    MemoryPool()
    {

    }


    using buffer_interval = std::pair< std::size_t, std::size_t>;

    ~MemoryPool()
    {

    }

    void Reserve( std::size_t bytes)
    {
        m_FreeData.clear();
        m_FreeData.insert( buffer_interval(0,bytes) );
        //buffer.Reserve( bytes );
        print_free_space();
    }


    void print_used_space()
    {
        GLA_LOGV << "Used Space;" << std::endl;
        for(auto & i : m_UsedData)
        {
            GLA_LOGV << i.first << "  " << i.second << std::endl;
        }
    }


    void print_free_space()
    {
        GLA_LOGV << "Free Space;" << std::endl;
        for(auto & i : m_FreeData)
        {
            GLA_LOGV << i.first << "  " << i.second << std::endl;
        }
    }

    void Defragment()
    {
        std::set< buffer_interval>::iterator i = m_FreeData.begin();

        while( i != m_FreeData.end() )
        {
            auto next_mem_location = i->first+i->second;

            auto next = std::next(i);

            if( next != m_FreeData.end() )
            {
                if( next_mem_location == next->first )
                {
                    buffer_interval newint(i->first, i->second+next->second );

                    next = m_FreeData.erase(next);
                    i    = m_FreeData.erase(i);

                    i = m_FreeData.insert( i,  newint );

                }
                else
                {
                    ++i;
                }
            } else {
                ++i;
            }

        }
    }

    std::size_t Free(std::size_t address)
    {

        auto i = m_UsedData.begin();

        while( i != m_UsedData.end() )
        {
            if( i->first == address )
            {
                m_FreeData.insert( *i );
                auto s = i->second;
                m_UsedData.erase(i);
                Defragment();
                return s;
            }
            else
            {
                ++i;
            }
        }

        return 0;
    }

    std::size_t Malloc(std::size_t bytes)
    {
        auto in = FindOpenSpace( bytes );

        m_FreeData.erase( in );
        m_UsedData.insert( buffer_interval( in.first, bytes) );
        m_FreeData.insert( buffer_interval( in.first+bytes, in.second-bytes) );

        GLA_LOGV << "Malloc: " << in.first << std::endl;
        return in.first;
    }


protected:
    buffer_interval FindOpenSpace( std::size_t bytes )
    {
        for(auto & i : m_FreeData)
        {
            if( bytes < i.second )
            {
                return buffer_interval( i.first, i.second );
            }
        }

        return buffer_interval( std::numeric_limits<std::size_t>::max() ,0);
    }

    std::set< buffer_interval>  m_FreeData;     //! A vector of freely available data
    std::set< buffer_interval>  m_UsedData;     //! A vector of freely available data

};


} // namespace gla
#endif
