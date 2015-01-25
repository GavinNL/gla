
#include <glre/global.h>
#include <vector>
#include <tuple>

namespace glre
{

    //template <class IndexBuffer, class... Args>
    class Mesh
    {
        public:
            Mesh();

           // ~Mesh();

            //bool LoadMesh(const std::string& Filename);

            void Render();

            void insertVertex( const V3 & v, const V3 & n, const V2 & uv);
            void insertFace(const uV3 & triangle);
            void insertFace(uint v1, uint v2, uint v3) { insertFace( uV3(v1,v2,v3) ); }

            bool sendToGPU();
            void clearGPU();

            ArrayBuffer_V3  & getPosition() { return mPosition;};
            ArrayBuffer_V3  & getNormal  () { return mNormal;  };
            ArrayBuffer_V2  & getUV      () { return mUV;      };
            ArrayBuffer_V4  & getColour  () { return mColour;  };
            ArrayBuffer_uV3 & getIndex   () { return mIndex;   };

        private:
    //        bool InitFromScene(const aiScene* pScene, const std::string& Filename);

    //        void InitMesh(const aiMesh* paiMesh,
    //                    std::vector& Positions,
    //                    std::vector& Normals,
    //                    std::vector& TexCoords,
    //                    std::vector& Indices);

            //bool InitMaterials(const aiScene* pScene, const std::string& Filename);
            void Clear();


            #define INVALID_MATERIAL 0xFFFFFFFF

            #define INDEX_BUFFER 0
            #define POS_VB 1
            #define NORMAL_VB 2
            #define TEXCOORD_VB 3
            #define COLOUR_VB 4
            #define NUMBER_OF_BUFFERS 5

            GLuint m_VAO;

            ArrayBuffer_V3  mPosition;
            ArrayBuffer_V3  mNormal;
            ArrayBuffer_V2  mUV;
            ArrayBuffer_V4  mColour;
            ArrayBuffer_uV3 mIndex;

            //std::tuple<Args> ArrayTuples;
    };
}
