#include "glre/modelloader.h"
#include <iostream>

namespace glre {





ModelLoader::ModelLoader()
{

}

ModelLoader::~ModelLoader()
{

}

void ModelLoader::loadModel(const std::string &path)
{
    Assimp::Importer Importer;

    const aiScene * pScene = Importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs );

    if(pScene)
    {
        std::vector<iTriMesh_PNCU> mMeshes;

        for(int i=0;i<pScene->mNumMeshes;i++)
        {
            mMeshes.push_back( _extractPNCU_Mesh(pScene->mMeshes[i]) );


            if( pScene->mMeshes[i]->hasBones() )
            {
                _extractSkeleton( pScene->mMeshes[i]->mBones[i]-> )    ;
            }

            _extractSkeleton( pScene->mMesh[i] );
        }
    }
}

iTriMesh_PNCU ModelLoader::_extractPNCU_Mesh(aiMesh *InputMesh, bool FlipYZ)
{
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

        std::cout << "Loading mesh: " << InputMesh->mName.C_Str() << std::endl;

        glre::iTriMesh_PNCU ReturnMesh;

        for( unsigned int j = 0 ; j < InputMesh->mNumVertices ; j++ )
        {

            const aiVector3D* pPos      = &(InputMesh->mVertices[j]);
            const aiVector3D* pNormal   = InputMesh->HasNormals()        ? &(InputMesh->mNormals[j])          : &Zero3D;
            const aiVector3D* pTexCoord = InputMesh->HasTextureCoords(0) ? &(InputMesh->mTextureCoords[0][j]) : &Zero3D;

            if(FlipYZ)
            {
                ReturnMesh.insertVertex(  { vec3(      pPos->x,      pPos->z,    pPos->y     ),
                                            vec3(   pNormal->x,   pNormal->z, pNormal->y     ),
                                            vec4(      1.0    ,          1.0,        1.0,1.0 ),
                                            vec2( pTexCoord->x, pTexCoord->y                 ) } );
            }
            else
            {
                ReturnMesh.insertVertex(  { vec3(      pPos->x,      pPos->y,    pPos->z     ),
                                            vec3(   pNormal->x,   pNormal->y, pNormal->z     ),
                                            vec4(      1.0    ,          1.0,        1.0,1.0 ),
                                            vec2( pTexCoord->x, pTexCoord->y                 ) } );
            }
        }

        for (unsigned int j = 0 ; j < InputMesh->mNumFaces ; j++)
        {
            const aiFace& Face = InputMesh->mFaces[j];

            if( Face.mNumIndices != 3 ) continue;

            ReturnMesh.insertElement(  uvec3( Face.mIndices[0], Face.mIndices[1], Face.mIndices[2] ) );
        }

        return std::move(ReturnMesh);



}

Skeleton ModelLoader::_extractSkeleton(aiMesh *InputMesh)
{


}


}
