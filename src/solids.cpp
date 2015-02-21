#include "glre/solids.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


glre::TriMesh_PNCU glre::loadModel(const std::string & path, bool sendToGPU)
{
    Assimp::Importer Importer;

    const aiScene * pScene = Importer.ReadFile(path.c_str(),
                                               aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    std::cout << "Loading mesh: " << path << std::endl;
    glre::TriMesh_PNCU ReturnMesh;
    int count = 0;
    if (pScene)
     {
            // std::cout << "-Number of Meshs: "     << pScene->mNumMeshes << std::endl;
            // std::cout << "-Number of Materials: " << pScene->mNumMaterials << std::endl;

            for (unsigned int i = 0 ; i < pScene->mNumMeshes ; i++)
            {
                const aiMesh* paiMesh = pScene->mMeshes[i];

                if(!paiMesh) continue;

                //std::cout << "--Mesh " << i << std::endl;
                //std::cout << "--- Num Vertices  " << paiMesh->mNumVertices << std::endl;
                //std::cout << "--- Num Triangles " << paiMesh->mNumFaces    << std::endl;

                for (unsigned int j = 0 ; j < paiMesh->mNumVertices ; j++)
                {

                    const aiVector3D* pPos      = &(paiMesh->mVertices[j]);
                    const aiVector3D* pNormal   = paiMesh->HasNormals() ? &(paiMesh->mNormals[j]) : &Zero3D;
                    const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][j]) : &Zero3D;

                    ReturnMesh.insertVertex(  { vec3(      pPos->x,      pPos->y,    pPos->z     ),
                                                vec3(   pNormal->x,   pNormal->y, pNormal->z     ),
                                                vec4(      1.0    ,          1.0,        1.0,1.0 ),
                                                vec2( pTexCoord->x, pTexCoord->y                 ) } );
                }

                //std::cout << "--Mesh " << i << " completed " << std::endl;
                for (unsigned int j = 0 ; j < paiMesh->mNumFaces ; j++)
                {
                    const aiFace& Face = paiMesh->mFaces[j];
                    //std::cout << Face.mNumIndices << std::endl;
                    if(Face.mNumIndices != 3) continue;
                    ReturnMesh.insertElement(  uvec3( Face.mIndices[0]+count, Face.mIndices[1]+count, Face.mIndices[2]+count ) );
                }
                count+=paiMesh->mNumVertices;

            }
            if(sendToGPU) ReturnMesh.sendToGPU();
            return ReturnMesh;
        }
        else {
            printf("Error parsing '%s': '%s'\n", path.c_str(), Importer.GetErrorString());
            return ReturnMesh;
        }

}

//glre::TriMesh_PNCU glre::createPrism(float width, float height, float depth)
//{
//    TriMesh_PNCU M;

//    vec3 i(1.0,0.0,0.0);
//    vec3 j(0.0,1.0,0.0);
//    vec3 k(0.0,0.0,1.0);

//    // Positions                                   Normals

//    const vec3 v0 = 0.5f*( i-j+k );
//    const vec3 v1 = 0.5f*( i-j-k );
//    const vec3 v2 = 0.5f*(-i-j-k );
//    const vec3 v3 = 0.5f*(-i-j-k );

//    const vec3 v4 = 0.5f*( i+j+k );
//    const vec3 v5 = 0.5f*( i+j-k );
//    const vec3 v6 = 0.5f*(-i+j-k );
//    const vec3 v7 = 0.5f*(-i+j-k );

//    // Bottom face
//    M.insertVertexAttribute<0>( v3  );  M.insertVertexAttribute<0>( -j );
//    M.insertVertexAttribute<0>( v2  );  M.insertVertexAttribute<0>( -j );
//    M.insertVertexAttribute<0>( v1  );  M.insertVertexAttribute<0>( -j );
//    M.insertVertexAttribute<0>( v0  );  M.insertVertexAttribute<0>( -j );
//    M.insertElement( uvec3(0,1,2) );
//    M.insertElement( uvec3(0,2,3) );

//    M.insertVertexAttribute<0>( v4  );  M.insertVertexAttribute<0>( j );
//    M.insertVertexAttribute<0>( v5  );  M.insertVertexAttribute<0>( j );
//    M.insertVertexAttribute<0>( v6  );  M.insertVertexAttribute<0>( j );
//    M.insertVertexAttribute<0>( v7  );  M.insertVertexAttribute<0>( j );
//    M.insertElement( uvec3(0,1,2) );
//    M.insertElement( uvec3(0,2,3) );

//    M.insertVertexAttribute<0>( v1  );  M.insertVertexAttribute<0>( i );
//    M.insertVertexAttribute<0>( v5  );  M.insertVertexAttribute<0>( i );
//    M.insertVertexAttribute<0>( v4  );  M.insertVertexAttribute<0>( i );
//    M.insertVertexAttribute<0>( v0  );  M.insertVertexAttribute<0>( i );
//    M.insertElement( uvec3(0,1,2) );
//    M.insertElement( uvec3(0,2,3) );

//    M.insertVertexAttribute<0>( v2  );  M.insertVertexAttribute<0>( -i );
//    M.insertVertexAttribute<0>( v3  );  M.insertVertexAttribute<0>( -i );
//    M.insertVertexAttribute<0>( v7  );  M.insertVertexAttribute<0>( -i );
//    M.insertVertexAttribute<0>( v6  );  M.insertVertexAttribute<0>( -i );
//    M.insertElement( uvec3(0,1,2) );
//    M.insertElement( uvec3(0,2,3) );

//    M.insertVertexAttribute<0>( v0  );  M.insertVertexAttribute<0>( k );
//    M.insertVertexAttribute<0>( v4  );  M.insertVertexAttribute<0>( k );
//    M.insertVertexAttribute<0>( v7  );  M.insertVertexAttribute<0>( k );
//    M.insertVertexAttribute<0>( v3  );  M.insertVertexAttribute<0>( k );
//    M.insertElement( uvec3(0,1,2) );
//    M.insertElement( uvec3(0,2,3) );

//    M.insertVertexAttribute<0>( v1  );  M.insertVertexAttribute<0>( k );
//    M.insertVertexAttribute<0>( v2  );  M.insertVertexAttribute<0>( k );
//    M.insertVertexAttribute<0>( v6  );  M.insertVertexAttribute<0>( k );
//    M.insertVertexAttribute<0>( v5  );  M.insertVertexAttribute<0>( k );
//    M.insertElement( uvec3(0,1,2) );
//    M.insertElement( uvec3(0,2,3) );

//}


glre::Line_PC glre::createAxes(bool sendToGPU)
{
    Line_PC      Axis;

   Axis.insertVertex( { vec3(0.0, 0.0, 0.0) , col4(1.0, 0.0, 0.0, 1.0) } );
   Axis.insertVertex( { vec3(1.0, 0.0, 0.0) , col4(1.0, 0.0, 0.0, 1.0) } );
   Axis.insertVertex( { vec3(0.0, 0.0, 0.0) , col4(0.0, 1.0, 0.0, 1.0) } );
   Axis.insertVertex( { vec3(0.0, 1.0, 0.0) , col4(0.0, 1.0, 0.0, 1.0) } );
   Axis.insertVertex( { vec3(0.0, 0.0, 0.0) , col4(0.0, 0.0, 1.0, 1.0) } );
   Axis.insertVertex( { vec3(0.0, 0.0, 1.0) , col4(0.0, 0.0, 1.0, 1.0) } );

    Axis.insertElement( uvec2(0, 1) );
    Axis.insertElement( uvec2(2, 3) );
    Axis.insertElement( uvec2(4, 5) );

    if(sendToGPU) Axis.sendToGPU();
    return Axis;
}


