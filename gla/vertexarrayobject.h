#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

#include <gla/global.h>
#include <vector>
#include <tuple>
#include <array>
#include <gla/arraybuffer.h>
#include <memory>
#include <typeindex>


namespace gla
{


    /**
     * @brief The GPUArrayObject class.
     * A GPU array object is an VertexArrayObject that exists in the GPU.
     * This class is small and easily copyable/assignable
     *
     * Assigning this object to another will not make a copy of the
     * object in the GPU. It will be shared. Freeing the GPUArrayObject
     *
     * GPUArrayObject will not clear GPU memory when the GPUArrayObject goes out of
     * scope. You MUST call clear() before it goes out of scope.
     */

    struct GPUArrayObjectInfo
    {
        unsigned int UseCount = 0;
        //std::shared_ptr<std::vector< GPUArrayBuffer > > Buffers;

        std::vector<GPUVertexBuffer> VertexBuffer;
        GPUElementBuffer             ElementBuffer;

        int       _size;                    // Either the number of indices if the GPUArrayObject is indexed
                                            // Or the number of vertices if the GPUArrayObject is not indexed.
        bool      _isIndexed;

    };

    struct GPUArrayObjectHandler
    {
        inline static void Create  (GLuint & h) { glGenVertexArrays(1, &h); }
        inline static void Release (GLuint & h) { glDeleteVertexArrays(1, &h); }
        inline static void Bind    (GLuint & h) { glBindVertexArray(    h); }
        inline static void Unbind  (GLuint & h) { glBindVertexArray(    0);  }
    };

    class GPUArrayObject
    {

        public:
            using HandleType = gla::Handle<GLuint, GPUArrayObjectHandler ,GPUArrayObjectInfo>;

            HandleType   m_Handle;
            unsigned int _size; // If this is indexed, _size = Number of Indices, else _size = number of vertices
            Primitave    _PrimitaveType;
            bool         _indexed;

        public:

            void Bind()     { m_Handle.Bind();    }
            void Unbind()   { m_Handle.Unbind();  }
            void Release()  { m_Handle.Release(); }
             /**
             * @brief Create
             * @param V
             * @param p
             *
             * Create a VAO on the GPU from a GPUVertexBuffer and a primitave
             */
            template<typename... BufferVertexTypes>
            void Create(GPUVertexBuffer V, Primitave p)
            {
                m_Handle.Create();
                m_Handle.Bind();

                auto & In = m_Handle.__GetInfo( );

                EnableVertexAttribute<BufferVertexTypes...>( V );

                m_Handle.Unbind();

                In.VertexBuffer.push_back( V );
                _size = V.m_Handle.GetInfo().NumVertices;//V.GetInfo().ByteSize / V.GetInfo().NumVertices;//sizeof( std::tuple<BufferVertexTypes...> );
                _indexed = false;
                _PrimitaveType = p;

            }

            /**
             * @brief Create
             * @param V - vertex buffer
             * @param E - The element index buffer
             * @param p - primative type when rendering
             *
             * Creates a VAO on the gpu using a vertexbuffer and a element buffer
             */
            template<typename... BufferVertexTypes>
            void Create(GPUVertexBuffer V, GPUElementBuffer E, Primitave p)
            {
                m_Handle.Create();
                m_Handle.Bind();

                auto & In = m_Handle.__GetInfo( );

                EnableVertexAttribute<BufferVertexTypes...>( V );

                E.Bind();

                In.VertexBuffer.push_back( V );
                In.ElementBuffer = E;
                _indexed = true;
                _PrimitaveType = p;
                _size = E.m_Handle.GetInfo().NumVertices;//V.GetInfo().ByteSize / V.GetInfo().NumVertices;//sizeof( std::tuple<BufferVertexTypes...> );

                m_Handle.Unbind();
            }

            /**
             * @brief ClearBuffers
             */
            void ClearBuffers()
            {
                m_Handle.Bind();

                auto & In = m_Handle.__GetInfo();
                In.VertexBuffer.clear();
                In.ElementBuffer.Release();
            }

            inline virtual void Render( )
            {
                    m_Handle.Bind();
                    _indexed ?
                         glDrawElementsBaseVertex((GLenum)_PrimitaveType, _size, GL_UNSIGNED_INT, 0,0) :
                         glDrawArrays( (GLenum)_PrimitaveType, 0, _size);
                    m_Handle.Unbind();
            }

            inline virtual void Render(Primitave PrimitaveType )
            {
                    m_Handle.Bind();
                    _indexed ?
                                glDrawElementsBaseVertex((GLenum)PrimitaveType, _size, GL_UNSIGNED_INT, 0,0) :
                                glDrawArrays( (GLenum)PrimitaveType, 0, _size);


                    m_Handle.Unbind();
            }


#ifdef TEST
            /**
             * Renders the ArrayObject as a partuclar type (triangle, quads ,etc)
             *
             * @param PrimitaveType The type of primitave to render as.
             */
            inline virtual void Render(PRIMITAVE PrimitaveType)
            {
                  //  std::cout << "Render: " << _size << " : " << _isIndexed << std::endl;
                    glBindVertexArray( _VAO );
                        _isIndexed ? glDrawElementsBaseVertex(PrimitaveType, _size, GL_UNSIGNED_INT, 0,0) : glDrawArrays(PrimitaveType, 0, _size);
                    glBindVertexArray(0);
            }

            /**
             * @brief Render
             * @param PrimitaveType
             * @param IndexRange an vec2 of unsigned int that indicate the range of indices to draw, start and end indices
             */
            inline virtual void Render(PRIMITAVE PrimitaveType, const gla::uvec2 & IndexRange)
            {
                    //std::cout << "Render: " << _size << " : " << _VAO << std::endl;
                    glBindVertexArray( _VAO );
                        _isIndexed ? glDrawElementsBaseVertex(PrimitaveType, _size, GL_UNSIGNED_INT, 0,0) : glDrawArrays(PrimitaveType, IndexRange.x, IndexRange.y-IndexRange.x);
                    glBindVertexArray(0);
            }

            /**
             * Renders the ArrayObject as the default primitave type as defined when
             * it was created on the CPU
             *
             */
            inline virtual void Render()
            {
               // std::cout << "REndering: " << _PrimitaveType << std::endl;
                    Render( _PrimitaveType );
            }

            /**
             * Renders the ArrayObject but does not bind or unbind it. Make sure you know what you're doing
             * if you plan on using this.
             *
             */
            inline virtual void RenderNoBind()
            {
                    _isIndexed ? glDrawElementsBaseVertex(_PrimitaveType, _size, GL_UNSIGNED_INT, 0,0) : glDrawArrays(_PrimitaveType, 0, _size);
            }

            /**
             * Clears the VertexArrayObject from the GPU. If there are any buffers associated with it they will be
             * cleared if they are not used by another VAO.
             *
             */

            GLuint    _VAO           = 0;
            PRIMITAVE _PrimitaveType = UNKNOWN_PRIMITAVE;
            int       _size          = 0;                    // Either the number of indices if the GPUArrayObject is indexed
                                                // Or the number of vertices if the GPUArrayObject is not indexed.
            bool      _isIndexed     = false;

            std::vector<BUFFER_ELEMENT_TYPE>    _attachedBuffers;
            std::shared_ptr<GPUArrayObjectInfo> mInfo;
#endif
    };


    template <typename... attrb>
    class InterleavedVAO
    {

        public:
            using VertexType        =  std::tuple<attrb...>;
            using VertexBufferType  =  ArrayBuffer< VertexType >;
            using ElementBufferType =  ArrayBuffer< unsigned int >;

            //typedef ArrayBuffer_T< std::tuple<attrb...> >         VertexBufferType;

            InterleavedVAO(Primitave defaultPrim = Primitave::TRIANGLES) { mDefaultPrim = defaultPrim; }


            template<class... U>
            void InsertVertex(const U&... u)
            {
                mVertexBuffer.Insert( std::make_tuple( std::forward<const U>(u)... ) );
            }

            template<class... U>
            void InsertVertex(const U&&... u)
            {
                mVertexBuffer.Insert( std::make_tuple( std::forward<const U>(u)... ) );
            }


            void InsertElement(const uvec2 & lines)
            {

                    mElementBuffer.Insert(lines.x);
                    mElementBuffer.Insert(lines.y);
                    mDefaultPrim = Primitave::LINES;

            }

            void InsertElement(const uvec3 & tri)
            {

                    mElementBuffer.Insert(tri.x);
                    mElementBuffer.Insert(tri.y);
                    mElementBuffer.Insert(tri.z);
                    mDefaultPrim = Primitave::TRIANGLES;

            }

            void InsertElement(const uvec4 & quad)
            {

                mElementBuffer.Insert(quad.x);
                mElementBuffer.Insert(quad.y);
                mElementBuffer.Insert(quad.z);
                mElementBuffer.Insert(quad.w);
                mDefaultPrim = Primitave::QUADS;
            }

            //template<class V>
            //bool InsertElement(const V & item)
            //{
            //    if( !mIndexBuffer )
            //    {
            //        mIndexBuffer = std::make_shared< ArrayBuffer<V> >();
            //    }
            //
            //    auto b = std::dynamic_pointer_cast< ArrayBuffer<V> >( mIndexBuffer );
            //
            //    if(b)
            //    {
            //       b->Insert( item );
            //       return true;
            //    }
            //
            //    return false;
            //}


            VertexType & operator[](int i)
            {
                return mVertexBuffer[i];
            }

            GPUArrayObject ToGPU()
            {
                GPUArrayObject GPU;

                //if( mVertexBuffer..size() == 0)
                //{
                //    throw std::runtime_error( "ERROR: Buffer does not contain any vertices.");
                //}


                //===============================
                //GPU.Create( mVertexBuffer.ToGPU());
                auto V = mVertexBuffer.ToGPU();


                if( mElementBuffer.Data.size() )
                {
                    GPU.Create<attrb...>( V, mElementBuffer.ToGPU<ArrayType::ELEMENT_ARRAY_BUFFER>(), mDefaultPrim);
                } else {
                    GPU.Create<attrb...>( V, mDefaultPrim );
                }

                //glGenVertexArrays(1, &GPU._VAO);
                //glBindVertexArray(    GPU._VAO);

                if( !GPU.m_Handle.GetID() )
                    throw std::runtime_error( "Interleaved VAO NOT CREATED" );


                return GPU;
//                //auto id = GPU._VAO;
//                //GPU.mInfo = std::shared_ptr<GPUArrayObjectInfo>( new GPUArrayObjectInfo, [=](GPUArrayObjectInfo* a){ auto vao = id; delete a; glDeleteVertexArrays(1, &vao); std::cout << "Deleting VertexArrayObject: " << id << std::endl; } );
//
//                auto & In =
//                GPU.mInfo->Buffers = std::make_shared< std::vector<GPUArrayBuffer> > ();
//
//                auto g = mVertexBuffer.toGPU(ARRAY_BUFFER);
//
//                //EnableVertexAttribArray<attrb...>(g);
//                EnableVertexAttribArrayFromTuple<0, VertexType>();
//                GPU.mInfo->Buffers->push_back(g);
//
//
//                //===============================
//
//                GPU._isIndexed     = false;
//                if( mIndexBuffer.get() )
//                {
//                    auto ind = mIndexBuffer->toGPU( ELEMENT_ARRAY_BUFFER );
//                    GPU.mInfo->Buffers->push_back(ind);
//                    GPU._isIndexed     = true;
//                    GPU._size          = mIndexBuffer->getVertexCount() * mIndexBuffer->getValuesPerVertex();
//
//                    const PRIMITAVE P[5] = {UNKNOWN_PRIMITAVE, UNKNOWN_PRIMITAVE, LINES, TRIANGLES, QUADS};
//                    GPU._PrimitaveType   = P[mIndexBuffer->getValuesPerVertex()];
//
//                } else {
//                    GPU._isIndexed     = false;
//                    GPU._size          = mVertexBuffer.getVertexCount();
//                    GPU._PrimitaveType = mDefaultPrim;
//                }
//
//                //GPU._isIndexed     = false;
//                //GPU._size          = mVertexBuffer.getVertexCount();
//                //GPU._PrimitaveType = TRIANGLES;
//
//                glBindVertexArray(0);
//
//
//                //std::cout << "============VOA Created============" << std::endl;
//                //std::cout << "  ID       : " << GPU._VAO       << std::endl;
//                //std::cout << "  size     : " << GPU._size      << std::endl;
//                //std::cout << "  isindexed: " << GPU._isIndexed << std::endl;
//                //std::cout << "  SizePerV: "  << GPU._size      << std::endl;
//                //switch(GPU._PrimitaveType )
//                //{
//                //    case UNKNOWN_PRIMITAVE: std::cout << "elementtype: UNKNOWN"   << std::endl; break;
//                //    case LINES: std::cout             << "elementtype: LINES"   << std::endl; break;
//                //    case TRIANGLES: std::cout         << "elementtype: TRIANGLES" << std::endl; break;
//                //    case QUADS: std::cout             << "elementtype: QUADS"     << std::endl; break;
//                //}
//
//                //std::cout << "===================================" << std::endl;
//
//
//                /* NOTE
//                 *
//                 * We must check that all buffers have the same length, otherwise throw an exception.
//                 *
//                 * */
//
                return GPU;

            }

            //void clear()
            //{
            //    mVertexBuffer.clearCPU();
            //};




        public:
            //uint                           IndicesPerElement;
            Primitave                        mDefaultPrim;
            VertexBufferType                 mVertexBuffer;
            ElementBufferType                mElementBuffer;
            //std::shared_ptr<ArrayBuffer>     mIndexBuffer;


    };


#if 0
    //====================================================================
    //  New VAO
    //====================================================================

    /*
     * VertexArrayObject is a CPU based representation of a VertexArrayObject.
     * It holds pointers to CPU Array buffers.
     */

    class VertexArrayObject_N
    {
        public:

            VertexArrayObject_N(PRIMITAVE DefaultType = TRIANGLES)
            {
                mDefaultPrimitave = DefaultType;
            }

            GPUArrayObject toGPU()
            {
                GPUArrayObject GPU;

                if( mBuffers.size() == 0)
                {
                    throw gla::GLA_EXCEPTION( "ERROR: Does not contain any buffers.");
                }

                for(int i=0; i < mBuffers.size() ; i++)
                {
                    if( mBuffers[0]->getVertexCount() != mBuffers[i]->getVertexCount() )
                        throw gla::GLA_EXCEPTION( "ERROR: All buffers in the VAO must be the same size before creating a GPUArrayObject");
                }

                //===============================

                glGenVertexArrays(1, &GPU._VAO);
                glBindVertexArray(    GPU._VAO);

                if( !GPU._VAO  )  throw gla::GLA_EXCEPTION( "ARRAY OBJECT NOT CREATED" );

                auto id = GPU._VAO;
                GPU.mInfo = std::shared_ptr<GPUArrayObjectInfo>( new GPUArrayObjectInfo, [=](GPUArrayObjectInfo* a){ auto vao = id; delete a; glDeleteVertexArrays(1, &vao); std::cout << "Deleting VertexArrayObject: " << id << std::endl; } );

                GPU.mInfo->Buffers = std::make_shared< std::vector<GPUArrayBuffer> > ();

               // std::vector< GPUArrayBuffer > & pGPUBuffers = GPU.mInfo->Buffers.get();

                for(int i = 0; i < mBuffers.size();  i++)
                {
                    auto g = mBuffers[i]->toGPU(ARRAY_BUFFER);
                    g.EnableAttribute(i);
                    GPU.mInfo->Buffers->push_back(g);

                    GPU._attachedBuffers.push_back( g.GetElementType() );

                }

                //===============================

                GPU._isIndexed     = false;
                if( mIndexBuffer.get() )
                {
                    auto g = mIndexBuffer->toGPU( ELEMENT_ARRAY_BUFFER );
                    GPU._isIndexed     = true;
                    GPU._size          = mIndexBuffer->getVertexCount() * mIndexBuffer->getValuesPerVertex();

                    const PRIMITAVE P[5] = {UNKNOWN_PRIMITAVE, UNKNOWN_PRIMITAVE, LINES, TRIANGLES, QUADS};
                    GPU._PrimitaveType   = P[mIndexBuffer->getValuesPerVertex()];

                    GPU.mInfo->Buffers->push_back(g);
                } else {
                    GPU._isIndexed     = false;
                    GPU._size          = mBuffers[0]->getVertexCount();
                    GPU._PrimitaveType = mDefaultPrimitave;
                }

                glBindVertexArray(0);


                std::cout << "============VOA Created============" << std::endl;
                std::cout << "  ID       : " << GPU._VAO       << std::endl;
                std::cout << "  size     : " << GPU._size      << std::endl;
                std::cout << "  isindexed: " << GPU._isIndexed << std::endl;
                switch(GPU._PrimitaveType )
                {
                    case UNKNOWN_PRIMITAVE: std::cout << "elementtype: UNKNOWN"   << std::endl; break;
                    case LINES: std::cout             << "elementtype: UNKNOWN"   << std::endl; break;
                    case TRIANGLES: std::cout         << "elementtype: TRIANGLES" << std::endl; break;
                    case QUADS: std::cout             << "elementtype: QUADS"     << std::endl; break;
                }

                //std::cout << "===================================" << std::endl;


                /* NOTE
                 *
                 * We must check that all buffers have the same length, otherwise throw an exception.
                 *
                 * */

                return GPU;

            }

            void clear()
            {
                mIndexBuffer.reset();
                mBuffers.clear();
            }

            void _ActivateAttribute(int index, uint Num_Values_Per_Vertex, GLenum IntegralType, bool ShouldNormalize)
            {
                glEnableVertexAttribArray(index);

                printf("Activating Attirubte (%d) as %d elements of type %s\n", index, Num_Values_Per_Vertex,
                       IntegralType==FLOAT ? "Float" : "non-float");
                glVertexAttribPointer(index, Num_Values_Per_Vertex, IntegralType, ShouldNormalize, 0, 0);

                auto err = glGetError();
                if(err != 0)
                {
                    char str[100];
                    sprintf(str, "Error activating index: %d.\nValesPerVertex: %d\n, IntegralType: %u\n, ShouldNormalize: %u\n", index, Num_Values_Per_Vertex, IntegralType, ShouldNormalize);
                    throw gla::GLA_EXCEPTION( std::string(str) );
                }

            }


            /**
             * @brief insert
             * @param index - The attribute index
             * @param item - the attribute to insert
             * @return
             *
             * Inserts a vertex attribute into the VAO. Will throw an exception if the index does not exist.
             */
            template<class V>
            bool insert(int index, const V & item)
            {
                if( index > mBuffers.size()-1 )
                {
                    throw gla::GLA_EXCEPTION("Buffer does not exist. Create the buffer using the VertexArrayObject.createBuffer<type>() method.");
                }

                auto b = std::dynamic_pointer_cast< ArrayBuffer_T<V> >( mBuffers[ index ] );

                if(b)
                {
                   b->insert( item );
                   return true;
                }

                return false;
            }

            template<class V>
            bool insertElement(const V & item)
            {
                if( !mIndexBuffer )
                {
                    mIndexBuffer = std::make_shared< ArrayBuffer_T<V> >();
                    //throw gla::GLA_EXCEPTION("Buffer does not exist. Create the buffer using the VertexArrayObject.createBuffer<type>() method.");
                }

                auto b = std::dynamic_pointer_cast< ArrayBuffer_T<V> >( mIndexBuffer );

                if(b)
                {
                   b->insert( item );
                   return true;
                }

                return false;
            }

            void eraseBuffer(int BufferNumber)
            {
                mBuffers.erase( mBuffers.begin()+BufferNumber );
            }
            void eraseIndexBuffer()
            {
                mIndexBuffer.reset();
            }

            void insertBuffer(std::shared_ptr<ArrayBuffer_b> B)
            {
                mBuffers.push_back(B);
            }

            void setIndexBuffer( std::shared_ptr<ArrayBuffer_b> indexbuffer)
            {
                mIndexBuffer = indexbuffer;
            }


            template<class V>
            ArrayBuffer_T<V> & getBuffer(int i) { return *std::dynamic_pointer_cast<ArrayBuffer_T<V> >( mBuffers[i] ); };

            std::shared_ptr<ArrayBuffer_b > & getIndexBuffer()  { return mIndexBuffer; }

            unsigned int numBuffers() { return mBuffers.size(); }


            /**
             * @brief createBuffer
             * @return
             *
             * Creates a buffer. Use the template argument to indicate what kind of buffer you want to create.
             * eg: vec3, vec2, vec4, etc
             */
            template<class V>
            int createBuffer()
            {
                mBuffers.push_back(  std::make_shared<ArrayBuffer_T<V>>()  );
                return( mBuffers.size() - 1 );
            }

            /**
             * @brief createIndexBuffer
             *
             * Create an index buffer. Use the template argument to indicate what kind of element to use.
             * eg: use uvec2 for lines, uvec3 for triangles, uvec4 for quads.
             */
            template<class V>
            void createIndexBuffer()
            {
                mIndexBuffer = std::make_shared<ArrayBuffer_T<V> >();
            }

            void mergeWith( VertexArrayObject_N & other)
            {
                if( mBuffers.size() == 0)
                {
                    mBuffers.resize( other.mBuffers.size() );
                }
                if( mBuffers.size() != other.mBuffers.size() )
                {

                    char str[100];
                    sprintf(str, "VertexArrayObjects must have the same number of buffers to merge (%u,%u).\n", (uint)mBuffers.size(), (uint)other.mBuffers.size());
                    throw gla::GLA_EXCEPTION( std::string(str) );
                }

                for(int i=0; i < other.mBuffers.size(); i++)
                {
                    if( !other.mBuffers[i]->sameTypeAs( *mBuffers[i] ) )
                    {
                        char str[100];
                        sprintf(str, "Cannot Merge VertexArrayObjects, Vertex buffer %d in slave is not the same type as in master\n", i);
                        throw gla::GLA_EXCEPTION( std::string(str) );
                    }
                }

                if( mIndexBuffer && other.mIndexBuffer)
                {
                    unsigned int off = mIndexBuffer->getVertexCount();
                    _joinBuffers<gla::uvec3, UNSIGNED_INT>( mIndexBuffer, other.mIndexBuffer, uvec3(off,off,off) );
                }
            }

            template<class VertexType, FUNDAMENTAL_TYPE INTEGRAL_TYPE>
            void _joinBuffers( std::shared_ptr<ArrayBuffer_b > master, std::shared_ptr<ArrayBuffer_b > slave, const VertexType & OffsetValue)
            {
                auto me    = std::dynamic_pointer_cast< gla::ArrayBuffer_T< VertexType > >( master );
                auto s     = std::dynamic_pointer_cast< gla::ArrayBuffer_T< VertexType > >( slave  );

                if(me && s)
                {
                    for(int i=0; i < slave->getVertexCount() ; i++)
                    {
                        VertexType T = s->getVertex(i) + OffsetValue;
                        me->insert( T );
                    }

                    std::cout << "Buffers joined: new size: " << me->getVertexCount() << std::endl;
                }
            }

        private:
            std::vector< std::shared_ptr<ArrayBuffer_b > >   mBuffers;
            std::shared_ptr<ArrayBuffer_b>                   mIndexBuffer;
            PRIMITAVE                                        mDefaultPrimitave;
    };

#endif
//    typedef gla::VertexArrayObject_N VertexArrayObject;
}

#endif // VERTEXARRAYOBJECT_H

