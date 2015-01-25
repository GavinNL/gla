#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

#include <glre/global.h>
#include <vector>
#include <tuple>
#include <array>

namespace glre
{

    template <class IndexBuffer, class ...Args>
    class VertexArrayObject
    {
        public:
            VertexArrayObject() : m_VAO(0) {};

            //bool LoadMesh(const std::string& Filename);

            void Render(Primitave PrimitaveType = TRIANGLES)
            {
                glBindVertexArray(m_VAO);
               // std::cout << "array bound\n";

            //    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
            //        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

            //        assert(MaterialIndex < m_Textures.size());

            //        if (m_Textures[MaterialIndex]) {
            //            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
            //        }
                    int N=3;  // number of vertices int he primitave
                    switch(PrimitaveType)
                    {
                        case TRIANGLES:
                            N = 3;
                            break;
                        case LINES:
                            N = 2;
                            break;
                        case QUADS:
                            N = 4;
                            break;
                    }

                    glDrawElementsBaseVertex(PrimitaveType,
                                              mIndex_T.size() * N,
                                              GL_UNSIGNED_INT,
                                              0,
                                              0);
                 //   std::cout << "array drawn\n";
                //}

                // Make sure the VAO is not changed from the outside
                glBindVertexArray(0);
            }

            bool sendToGPU()
            {
                if( m_VAO ) clearGPU();

                glGenVertexArrays(1, &m_VAO);
                glBindVertexArray(    m_VAO);

                sendBuffersToGPU( ArrayObjects );

                //===================================================================================================
                mIndex_T.sendToGPU();
                //===================================================================================================

            }

            void clearGPU()
            {
                clearBuffersFromGPU( ArrayObjects );
            };


            template<class T>
            void insertElement(const T & obj)
            {
                mIndex_T.insert( obj );
            }

            template<std::size_t ArrayIndex, class T>
            void insertVertexAttribute(const T & obj)
            {
                std::get<ArrayIndex>(ArrayObjects).insert( obj );
            }


            //===========================================================
            // An attempt to loop through all Buffers in the tuple
            //===========================================================
             template<std::size_t> struct int_{};

            template <class Tuple, size_t Pos>
            void sendBuffersToGPU( Tuple& t, int_<Pos> )
            {

              auto & buff              = std::get< std::tuple_size<Tuple>::value-Pos >(t);
              int index                = std::tuple_size<Tuple>::value-Pos;
              int ElementsPerAttribute = ( (int)sizeof(buff[0]) / (int)sizeof(buff[0][0]));

              buff.sendToGPU();
              glEnableVertexAttribArray(index);
              glVertexAttribPointer(index, ElementsPerAttribute, GL_FLOAT, GL_FALSE, 0, 0);

              sendBuffersToGPU( t, int_<Pos-1>());
            }

            template <class Tuple>
            void sendBuffersToGPU( Tuple& t, int_<1> )
            {
                auto & buff              = std::get< std::tuple_size<Tuple>::value-1 >(t);
                int index                = std::tuple_size<Tuple>::value-1;
                int ElementsPerAttribute = ( (int)sizeof(buff[0]) / (int)sizeof(buff[0][0]));

                buff.sendToGPU();
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, ElementsPerAttribute, GL_FLOAT, GL_FALSE, 0, 0);

            }

            template <class... Args1>
            void sendBuffersToGPU( std::tuple<Args1...>& t)
            {
              sendBuffersToGPU( t, int_<sizeof...(Args1)>());
            }
            //===========================================================

            //===========================================================
            // An attempt to loop through all Buffers in the tuple
            // And clear it from GPU memory
            //===========================================================


            template <class Tuple, size_t Pos>
            void clearBuffersFromGPU( Tuple& t, int_<Pos> )
            {

              auto & buff              = std::get< std::tuple_size<Tuple>::value-Pos >(t);
              buff.clearGPU();
              clearBuffersFromGPU( t, int_<Pos-1>());
            }

            template <class Tuple>
            void clearBuffersFromGPU( Tuple& t, int_<1> )
            {
                auto & buff              = std::get< std::tuple_size<Tuple>::value-1 >(t);
                buff.clearGPU();
            }

            template <class... Args1>
            void clearBuffersFromGPU( std::tuple<Args1...>& t)
            {
              clearBuffersFromGPU( t, int_<sizeof...(Args1)>());
            }
            //===========================================================


        public:
    //        bool InitFromScene(const aiScene* pScene, const std::string& Filename);

    //        void InitMesh(const aiMesh* paiMesh,
    //                    std::vector& Positions,
    //                    std::vector& Normals,
    //                    std::vector& TexCoords,
    //                    std::vector& Indices);

            //bool InitMaterials(const aiScene* pScene, const std::string& Filename);
            void Clear(){};


            #define INVALID_MATERIAL 0xFFFFFFFF

            GLuint m_VAO;

            IndexBuffer      mIndex_T;
            std::tuple<Args...> ArrayObjects;


    };
}

#endif // VERTEXARRAYOBJECT_H
