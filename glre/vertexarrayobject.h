#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

#include <glre/global.h>
#include <vector>
#include <tuple>
#include <array>
#include <glre/arraybuffer.h>

namespace glre
{

    typedef enum
    {
        F1,F2,F3,F4,  I1, I2, I3, I4,  U1, U2, U3, U4
    } TypeSizesIndex;


    template <class VertexType, class ElementType, int N, int... Rest>
    class VertexArrayObject
    {
        public:
            VertexArrayObject() : m_VAO(0) {};

            void Render(Primitave PrimitaveType = TRIANGLES)
            {
                    glBindVertexArray(m_VAO);
                    //std::cout << "Vertex Array Render: " << m_VAO << std::endl;
                    int NumberOfVertices=3;  // number of vertices int he primitave
                    switch(PrimitaveType)
                    {
                        case TRIANGLES:
                            NumberOfVertices = 3;
                            break;
                        case LINES:
                            NumberOfVertices = 2;
                            break;
                        case QUADS:
                            NumberOfVertices = 4;
                            break;
                    }

                    //std::cout << "glDrawElementsBaseVertex(" << PrimitaveType << ","
                    //                         << mIndexBuffer.gpuBufferSize() * NumberOfVertices << ","
                    //                         << GL_UNSIGNED_INT << ","
                    //                         << 0 << ","
                    //                         << 0 << ");" << std::endl;
                    glDrawElementsBaseVertex(PrimitaveType,
                                             mIndexBuffer.gpuBufferSize() * NumberOfVertices,
                                             GL_UNSIGNED_INT,
                                             0,
                                             0);

                   // std::cout << "array drawn\n";
                //}

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
                EnableVertexAttribArray<N, Rest...>( 0, 0 );
                mIndexBuffer.sendToGPU();

                glBindVertexArray( 0 );
            }

            void clearGPU()
            {
               // clearBuffersFromGPU( ArrayObjects );
            };


            //===========================================================
            // An attempt to loop through all Buffers in the tuple
            //===========================================================
             template<std::size_t> struct int_{};

            template <int First>
            void EnableVertexAttribArray( int index, long offset )
            {

              const uint ElementsPerAttribute[] = {1,2,3,4, 1,2,3,4, 1,2,3,4};
              const uint ElementType[]          = {GL_FLOAT,GL_FLOAT,GL_FLOAT,GL_FLOAT, GL_INT,GL_INT,GL_INT,GL_INT, GL_UNSIGNED_INT, GL_UNSIGNED_INT,GL_UNSIGNED_INT,GL_UNSIGNED_INT};
              const uint ElementStride[]        = {4,8,12,16,4,8,12,16,4,8,12,16};

              glEnableVertexAttribArray( index );
              std::cout << "glEnableVertexAttribArray( " << index  << ");\n";
              glVertexAttribPointer(index, ElementsPerAttribute[First], ElementType[First], GL_FALSE, sizeof(VertexType), (void*)offset);
              std::cout << "glVertexAttribPointer(" << index << "," << ElementsPerAttribute[First] << "," << ElementType[First] << "," << GL_FALSE << "," <<  0 << ","  << offset << ");\n";



            }

            template <int First, int Second, int... AllTheRest>
            void EnableVertexAttribArray( int index, long offset )
            {

              const uint ElementsPerAttribute[] = {1,2,3,4,1,2,3,4,1,2,3,4};
              const uint ElementType[]          = {GL_FLOAT,GL_FLOAT,GL_FLOAT,GL_FLOAT, GL_INT,GL_INT,GL_INT,GL_INT, GL_UNSIGNED_INT, GL_UNSIGNED_INT,GL_UNSIGNED_INT,GL_UNSIGNED_INT};
              const uint ElementStride[]        = {4,8,12,16,4,8,12,16,4,8,12,16};

              glEnableVertexAttribArray( index );
              std::cout << "glEnableVertexAttribArray( " << index  << ");\n";
              glVertexAttribPointer(index, ElementsPerAttribute[First], ElementType[First], GL_FALSE, sizeof(VertexType), (void*)offset);
              std::cout << "glVertexAttribPointer(" << index << "," << ElementsPerAttribute[First] << "," << ElementType[First] << "," << GL_FALSE << "," <<  0 << ","  << offset << ");\n";
              EnableVertexAttribArray<Second, AllTheRest...>( index+1, offset+ElementStride[First] );
            }


            //===========================================================

            //===========================================================
            // An attempt to loop through all Buffers in the tuple
            // And clear it from GPU memory
            //===========================================================



        public:

            GLuint m_VAO;

            ArrayBuffer<ElementType,GL_ELEMENT_ARRAY_BUFFER> mIndexBuffer;
            ArrayBuffer<VertexType ,GL_ARRAY_BUFFER>         mVertexBuffer;


    };
}

#endif // VERTEXARRAYOBJECT_H
