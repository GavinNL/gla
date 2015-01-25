
#include <glre/global.h>

#include <vector>

namespace glre
{

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

            //std::vector<V2> & getUVBuffer()       { return UV;     };
            //std::vector<V3> & getPosBuffer()      { return Pos;    };
            //std::vector<V3> & getNormalBuffer()   { return Normal; };
            //std::vector<V3> & getColorBuffer()    { return Colour; };

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
            GLuint m_Buffers[NUMBER_OF_BUFFERS];

            ArrayBuffer_V3  mPosition;
            ArrayBuffer_V3  mNormal;
            ArrayBuffer_V2  mUV;
            ArrayBuffer_V4  mColour;
            ArrayBuffer_uV3 mIndex;

            //std::vector<V2>  UV;
            //std::vector<V3>  Pos;
            //std::vector<V3>  Normal;
            //std::vector<V4>  Colour;
            //std::vector<uV3> Indices;


    };
}
