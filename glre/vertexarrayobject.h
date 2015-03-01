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
                    glBindVertexArray( _VAO );
                    _isIndexed ? glDrawElementsBaseVertex(PrimitaveType, _size, GL_UNSIGNED_INT, 0,0) : glDrawArrays(PrimitaveType, 0, _size);
                    glBindVertexArray(0);
                    //std::cout << _size << " " << _VAO <<std::endl;
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
                glGenVertexArrays(1, &GPU._VAO);
                glBindVertexArray(    GPU._VAO);

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
}

#endif // VERTEXARRAYOBJECT_H
