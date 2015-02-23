#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

#include <glre/global.h>
#include <vector>
#include <tuple>
#include <array>
#include <glre/arraybuffer.h>
#include <memory>

namespace glre
{

    typedef enum
    {
        F1,F2,F3,F4,  I1, I2, I3, I4,  U1, U2, U3, U4
    } TypeSizesIndex;


    template <class VertexType, class ElementType, Primitave DefaultRenderPrimitave, int NumberOfVertices, TypeSizesIndex N, TypeSizesIndex... Rest>
    class VertexArrayObject
    {
        public:
            VertexArrayObject() : m_VAO(0) {};

            inline GLint getID() const
            {
                return m_VAO;
            }

            void Render(Primitave PrimitaveType = DefaultRenderPrimitave)
            {
                    glBindVertexArray(m_VAO);

                    glDrawElementsBaseVertex(DefaultRenderPrimitave,
                                             mIndexBuffer.gpuBufferSize() * NumberOfVertices,
                                             GL_UNSIGNED_INT,
                                             0,
                                             0);

                // Make sure the VAO is not changed from the outside
                glBindVertexArray(0);
            }

            inline void insertVertex(const VertexType & T)
            {
                mVertexBuffer.insert(T);
            }

            inline void insertElement(const ElementType & T)
            {
                mIndexBuffer.insert(T);
            }

            bool sendToGPU()
            {
                if( m_VAO )
                {
                    std::cout << "Sending Vertex Array to GPU. Current Handle: " << m_VAO << std::endl;
                    //clearGPU();
                }

                glGenVertexArrays(1, &m_VAO);
                glBindVertexArray(    m_VAO);

                mVertexBuffer.sendToGPU();
                _EnableVertexAttribArray<N, Rest...>( 0, 0 );
                mIndexBuffer.sendToGPU();

                std::cout << "Vertex Array Object sent to gpu.  Current Handle: " << m_VAO << std::endl;
                std::cout << "   -Vertices: "  << mVertexBuffer.cpuBufferSize() << std::endl;
                std::cout << "   -Triangles: " << mIndexBuffer.cpuBufferSize() << std::endl;
                glBindVertexArray( 0 );
            }

            void clearGPU()
            {
                mIndexBuffer.clearGPU();
                mVertexBuffer.clearGPU();
                glDeleteVertexArrays(1, &m_VAO);
                m_VAO = 0;
            };

            void clearCPU()
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

            GLuint m_VAO;

            ArrayBuffer<ElementType,GL_ELEMENT_ARRAY_BUFFER> mIndexBuffer;
            ArrayBuffer<VertexType ,GL_ARRAY_BUFFER>         mVertexBuffer;


    };
}

#endif // VERTEXARRAYOBJECT_H
