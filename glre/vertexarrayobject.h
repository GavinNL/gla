#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

#include <glre/global.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>
#include <array>
#include <glre/arraybuffer.h>
#include <memory>

namespace glre
{

    typedef enum
    {
        F1, F2, F3 ,F4,  // floating point 1d, 2d, 3d, 4d vectors (float, vec2, vec3, vec4)
        I1, I2, I3, I4,  // integer 1d, 2d, 3d, 4d vectors (int, ivec2, ivec3, ivec4)
        U1, U2, U3, U4   // unsigned integer 1d, 2d, 3d, 4d vectors (uint, uvec2, uvec3, uvec4)
    } TypeSizesIndex;


    /*
     * A GPU array object is an VertexArrayObject that exists in the GPU.
     * This class is small and easily copyable/assignable
     *
     * Assigning this object to another will not make a copy of the
     * object in the GPU. It will be shared. Freeing the GPUArrayObject
     *
     * GPUArrayObject will not clear GPU memory when the GPUArrayObject goes out of
     * scope. You MUST call clear() before it goes out of scope.
     */
    class GPUArrayObject
    {
        public:

            inline virtual void Render(Primitave PrimitaveType)
            {
                    //std::cout << "Render: " << _size << " : " << _VAO << std::endl;
                    glBindVertexArray( _VAO );
                        _isIndexed ? glDrawElementsBaseVertex(PrimitaveType, _size, GL_UNSIGNED_INT, 0,0) : glDrawArrays(PrimitaveType, 0, _size);
                    glBindVertexArray(0);
            }

            inline virtual void Render()
            {
                    Render( _PrimitaveType );
            }

            inline virtual void RenderNoBind()
            {
                    _isIndexed ? glDrawElementsBaseVertex(_PrimitaveType, _size, GL_UNSIGNED_INT, 0,0) : glDrawArrays(_PrimitaveType, 0, _size);
            }

            inline void bind()
            {
                glBindVertexArray( _VAO );
            }

            inline void clear()
            {
                if( _VAO ) glDeleteVertexArrays(1, &_VAO);
                _VAO = 0;
            }

            GLint getID() { return _VAO; }

            GLuint    _VAO;
            Primitave _PrimitaveType;
            int       _size;                    // Either the number of indices if the GPUArrayObject is indexed
                                                // Or the number of vertices if the GPUArrayObject is not indexed.
            bool      _isIndexed;
    };


    /*
     * A templated class for non-indexed meshs, (ie: mesh that do not need a separate index buffer)
     */
    template <class VertexType,                         // The type of vertex to use
              Primitave DefaultRenderPrimitave,         // The default render primitative (LINES/TRIANGLES/QUADS)
              TypeSizesIndex N,                         // The first variable type in the Vertex (see declaration of TypeSizeIndex)
              TypeSizesIndex... Rest>                   // The rest of the variable types in the Vertex (see declaration of TypeSizeIndex)
    class VertexArrayObject
    {
        public:
            VertexArrayObject()
            {

            };

//            void Render(Primitave PrimitaveType = DefaultRenderPrimitave)
//            {
//                    glBindVertexArray(m_VAO);

//                    glDrawArrays( PrimitaveType, 0, mVertexBuffer.gpuBufferSize() );

//                    glBindVertexArray(0);
//            }

            inline void insertVertex(const VertexType & T)
            {
                mVertexBuffer.insert(T);
            }


            /* will create a NEW GPU object*/
            GPUArrayObject CreateGPUObject()
            {
                GPUArrayObject GPU;

                std::cout << "Sending Model to GPU:\n";
                std::cout << " #vertices: " <<  mVertexBuffer.cpuBufferSize() << std::endl;

                //===============================
                glGenVertexArrays(1, &GPU._VAO );
                glBindVertexArray(    GPU._VAO );

                mVertexBuffer.sendToGPU();
                _EnableVertexAttribArray<N, Rest...>( 0, 0 );

                glBindVertexArray( 0 );
                //===============================

                GPU._size          = mVertexBuffer.gpuBufferSize();
                GPU._isIndexed     = false;
                GPU._PrimitaveType = DefaultRenderPrimitave;

                return GPU;

            }

            void clear()
            {
                mVertexBuffer.clearCPU();
            };

            //===========================================================
            // An attempt to loop through all Buffers in the tuple
            //===========================================================
    private:

            template <int First>
            void _EnableVertexAttribArray( int index, long offset )
            {

              const uint ElementsPerAttribute[] = {1,2,3,4, 1,2,3,4, 1,2,3,4};
              const uint ElementType[]          = {GL_FLOAT,GL_FLOAT,GL_FLOAT,GL_FLOAT, GL_INT,GL_INT,GL_INT,GL_INT, GL_UNSIGNED_INT, GL_UNSIGNED_INT,GL_UNSIGNED_INT,GL_UNSIGNED_INT};
              const uint ElementStride[]        = {4,8,12,16,4,8,12,16,4,8,12,16};

              glEnableVertexAttribArray( index );
              glVertexAttribPointer(index, ElementsPerAttribute[First], ElementType[First], GL_FALSE, sizeof(VertexType), (void*)offset);
            }

            template <int First, int Second, int... AllTheRest>
            void _EnableVertexAttribArray( int index, long offset )
            {

              const uint ElementsPerAttribute[] = {1,2,3,4,1,2,3,4,1,2,3,4};
              const uint ElementType[]          = {GL_FLOAT,GL_FLOAT,GL_FLOAT,GL_FLOAT, GL_INT,GL_INT,GL_INT,GL_INT, GL_UNSIGNED_INT, GL_UNSIGNED_INT,GL_UNSIGNED_INT,GL_UNSIGNED_INT};
              const uint ElementStride[]        = {4,8,12,16,4,8,12,16,4,8,12,16};

              glEnableVertexAttribArray( index );
              glVertexAttribPointer(index, ElementsPerAttribute[First], ElementType[First], GL_FALSE, sizeof(VertexType), (void*)offset);

              _EnableVertexAttribArray<Second, AllTheRest...>( index+1, offset+ElementStride[First] );
            }

            //===========================================================


        public:

            uint   IndicesPerElement;
            ArrayBuffer<VertexType ,GL_ARRAY_BUFFER>         mVertexBuffer;


    };



    template <class VertexType,                         // The type of vertex to use
              class ElementType,                        // The vector type to use for the index buffer (uvec2/uvec3/uvec4 for lines/triangles/quads)
              Primitave DefaultRenderPrimitave,         // The default render primitative (LINES/TRIANGLES/QUADS)
              TypeSizesIndex N,                         // The first variable type in the Vertex (see declaration of TypeSizeIndex)
              TypeSizesIndex... Rest>                   // The rest of the variable types in the Vertex (see declaration of TypeSizeIndex)
    class IndexedVertexArrayObject
    {

        public:
            typedef ArrayBuffer<VertexType , GL_ARRAY_BUFFER>         VertexBufferType;
            typedef ArrayBuffer<ElementType, GL_ELEMENT_ARRAY_BUFFER> IndexBufferType;

            IndexedVertexArrayObject() {};


            inline void insertVertex(const VertexType & T)
            {
                mVertexBuffer.insert(T);
            }

            inline void insertElement(const ElementType & T)
            {
                IndicesPerElement = T.length();
                mIndexBuffer.insert(T);
            }

            GPUArrayObject CreateGPUObject()
            {
                GPUArrayObject GPU;

                std::cout << "Sending Model to GPU:\n";
                std::cout << " #vertices: " << mVertexBuffer.cpuBufferSize() << std::endl;
                std::cout << " #indices : " << mIndexBuffer.cpuBufferSize()  << std::endl;

                //===============================
                glGenVertexArrays(1, &GPU._VAO);
                glBindVertexArray(    GPU._VAO);

                mVertexBuffer.sendToGPU();
                _EnableVertexAttribArray<N, Rest...>( 0, 0 );
                mIndexBuffer.sendToGPU();
                glBindVertexArray( 0 );
                //===============================

                GPU._size          = mIndexBuffer.cpuBufferSize() * IndicesPerElement;
                GPU._isIndexed     = true;
                GPU._PrimitaveType = DefaultRenderPrimitave;

                return GPU;

            }


            void clear()
            {
                 mIndexBuffer.clearCPU();
                mVertexBuffer.clearCPU();
            };


            //===========================================================
            // An attempt to loop through all Buffers in the tuple
            //===========================================================
    private:

            template <int First>
            void _EnableVertexAttribArray( int index, long offset )
            {

              const uint ElementsPerAttribute[] = {1,2,3,4, 1,2,3,4, 1,2,3,4};
              const uint ElementType[]          = {GL_FLOAT,GL_FLOAT,GL_FLOAT,GL_FLOAT, GL_INT,GL_INT,GL_INT,GL_INT, GL_UNSIGNED_INT, GL_UNSIGNED_INT,GL_UNSIGNED_INT,GL_UNSIGNED_INT};
              const uint ElementStride[]        = {4,8,12,16,4,8,12,16,4,8,12,16};

              glEnableVertexAttribArray( index );
              glVertexAttribPointer(index, ElementsPerAttribute[First], ElementType[First], GL_FALSE, sizeof(VertexType), (void*)offset);
            }

            template <int First, int Second, int... AllTheRest>
            void _EnableVertexAttribArray( int index, long offset )
            {

              const uint ElementsPerAttribute[] = {1,2,3,4,1,2,3,4,1,2,3,4};
              const uint ElementType[]          = {GL_FLOAT,GL_FLOAT,GL_FLOAT,GL_FLOAT, GL_INT,GL_INT,GL_INT,GL_INT, GL_UNSIGNED_INT, GL_UNSIGNED_INT,GL_UNSIGNED_INT,GL_UNSIGNED_INT};
              const uint ElementStride[]        = {4,8,12,16,4,8,12,16,4,8,12,16};

              glEnableVertexAttribArray( index );
              glVertexAttribPointer(index, ElementsPerAttribute[First], ElementType[First], GL_FALSE, sizeof(VertexType), (void*)offset);

              _EnableVertexAttribArray<Second, AllTheRest...>( index+1, offset+ElementStride[First] );
            }

            //===========================================================


        public:

            uint   IndicesPerElement;

            IndexBufferType  mIndexBuffer;
            VertexBufferType mVertexBuffer;

    };



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
            GPUArrayObject createGPUObject()
            {
                GPUArrayObject GPU;

                //===============================
                glGenVertexArrays(1, &GPU._VAO);
                glBindVertexArray(    GPU._VAO);

                if( !GPU._VAO  )
                {
                     throw glre::GLRE_EXCEPTION( "ARRAY OBJECT NOT CREATED" );
                }

                std::vector< GPUArrayBuffer > pGPUBuffers;

                for(int i = 0; i < mBuffers.size();  i++)
                {
                    pGPUBuffers.push_back( mBuffers[i]->createGPUObject(ARRAY_BUFFER) );  // creates the GPU Array buffer and binds it

                    _ActivateAttribute(i, mBuffers[i]->getValuesPerVertex(), mBuffers[i]->getIntegralType(), GL_FALSE);
                }

                //===============================

                GPU._isIndexed     = false;
                if(mIndexBuffer.get())
                {
                    mIndexBuffer->createGPUObject( ELEMENT_ARRAY_BUFFER );
                    GPU._isIndexed     = true;
                    GPU._size          = mIndexBuffer->getVertexCount() * mIndexBuffer->getValuesPerVertex();
                } else {
                    GPU._isIndexed     = false;
                    GPU._size          = 0;
                }

                glBindVertexArray(0);
                std::cout << "============VOA Created============" << std::endl;
                std::cout << "  size     : " << GPU._size      << std::endl;
                std::cout << "  isindexed: " << GPU._isIndexed << std::endl;
                if(mIndexBuffer->getValuesPerVertex() == 4)
                {
                    GPU._PrimitaveType = QUADS;
                    std::cout << "elementtype: QUADS" << std::endl;
                }
                else if(mIndexBuffer->getValuesPerVertex() == 3 )
                {
                    GPU._PrimitaveType = TRIANGLES;
                    std::cout << "elementtype: TRIANGLES" << std::endl;
                }
                else if(mIndexBuffer->getValuesPerVertex() == 2)
                {
                    GPU._PrimitaveType = LINES;
                    std::cout << "elementtype: LINES" << std::endl;
                }
                std::cout << "===================================" << std::endl;


                return GPU;

            }

            void _ActivateAttribute(int index, uint Num_Values_Per_Vertex, GLenum IntegralType, bool ShouldNormalize)
            {
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, Num_Values_Per_Vertex, IntegralType, ShouldNormalize, 0, 0);

                auto err = glGetError();
                if(err != 0)
                {
                    char str[100];
                    sprintf(str, "Error activating index: %d.\nValesPerVertex: %d\n, IntegralType: %d\n, ShouldNormalize: %d\n", index, Num_Values_Per_Vertex, IntegralType, ShouldNormalize);
                    throw glre::GLRE_EXCEPTION( std::string(str) );
                }

            }

            void insertBuffer(std::shared_ptr<ArrayBuffer_b> B)
            {
                mBuffers.push_back(B);
            }

            void setIndexBuffer( std::shared_ptr<ArrayBuffer_b> indexbuffer)
            {
                mIndexBuffer = indexbuffer;
            }

            std::shared_ptr<ArrayBuffer_b > & getBuffer(int i ) { return mBuffers[i];  }
            std::shared_ptr<ArrayBuffer_b > & getIndexBuffer()  { return mIndexBuffer; }

            unsigned int numBuffers() { return mBuffers.size(); }

            void mergeWith( VertexArrayObject_N & other)
            {
                if( mBuffers.size() == 0)
                {
                    mBuffers.resize( other.mBuffers.size() );
                }
                if( mBuffers.size() != other.mBuffers.size() )
                {
                    throw glre::GLRE_EXCEPTION("VertexArrayObjects must have the same number of buffers to merge.");
                }

                for(int i=0; i < other.mBuffers.size(); i++)
                {
                    if( !other.mBuffers[i]->sameTypeAs( *mBuffers[i] ) )
                    {
                        char str[100];
                        sprintf(str, "Cannot Merge VertexArrayObjects, Vertex buffer %d in slave is not the same type as in master\n", i);
                        throw glre::GLRE_EXCEPTION( std::string(str) );
                    }
                }

                for(int i=0; i < other.mBuffers.size(); i++)
                {
                    if( mBuffers[i]->getIntegralType() == GL_FLOAT)
                    {
                        #define VALUETYPE GL_FLOAT
                            if( mBuffers[i]->getValuesPerVertex()==4 )
                            {
                                _joinBuffers<glre::vec4, VALUETYPE>( mBuffers[i], other.mBuffers[i], vec4(0.0,0.0,0.0,0.0));
                            }
                            else if(  mBuffers[i]->getValuesPerVertex()==3 )
                            {
                                _joinBuffers<glre::vec3, VALUETYPE>( mBuffers[i], other.mBuffers[i], vec3(0.0,0.0,0.0));
                            }
                            else if( mBuffers[i]->getValuesPerVertex()==2 )
                            {
                                _joinBuffers<glre::vec2, VALUETYPE>( mBuffers[i], other.mBuffers[i], vec2(0.0,0.0));
                            }
                            else if( mBuffers[i]->getValuesPerVertex()==1 )
                            {
                                _joinBuffers<float, VALUETYPE>( mBuffers[i], other.mBuffers[i], 0.f);
                            }
                        #undef VALUETYPE
                    }
                    else if( mBuffers[i]->getIntegralType() == GL_UNSIGNED_INT)
                    {
                        #define VALUETYPE GL_UNSIGNED_INT
                            if( mBuffers[i]->getValuesPerVertex()==4 )
                            {
                                _joinBuffers<glre::uvec4, VALUETYPE>( mBuffers[i], other.mBuffers[i], uvec4(0,0,0,0));
                            }
                            else if(  mBuffers[i]->getValuesPerVertex()==3 )
                            {
                                _joinBuffers<glre::uvec3, VALUETYPE>( mBuffers[i], other.mBuffers[i], uvec3(0,0,0));
                            }
                            else if( mBuffers[i]->getValuesPerVertex()==2 )
                            {
                                _joinBuffers<glre::uvec2, VALUETYPE>( mBuffers[i], other.mBuffers[i], uvec2(0,0));
                            }
                            else if( mBuffers[i]->getValuesPerVertex()==1 )
                            {
                                _joinBuffers<unsigned int, VALUETYPE>( mBuffers[i], other.mBuffers[i], 0);
                            }
                        #undef VALUETYPE
                    }
                }
            }

            template<class VertexType, GLenum INTEGRAL_TYPE>
            void _joinBuffers( std::shared_ptr<ArrayBuffer_b > master, std::shared_ptr<ArrayBuffer_b > slave, const VertexType & OffsetValue)
            {
                auto me    = std::dynamic_pointer_cast< glre::ArrayBuffer_T<VertexType, INTEGRAL_TYPE> >( master );
                auto s     = std::dynamic_pointer_cast< glre::ArrayBuffer_T<VertexType, INTEGRAL_TYPE> >( slave );

                for(int i=0;i<slave->getVertexCount();i++)
                {
                    VertexType T = s->getVertex(i) + OffsetValue;
                    me->insert( T );
                }
            }

        private:
            std::vector< std::shared_ptr<ArrayBuffer_b > >   mBuffers;
            std::shared_ptr<ArrayBuffer_b>                   mIndexBuffer;
    };

}

#endif // VERTEXARRAYOBJECT_H
