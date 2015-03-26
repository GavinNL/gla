#include "glre/modelloader.h"
#include <memory.h>
#include <iostream>

namespace glre {



ModelLoader::ModelLoader()
{

}

ModelLoader::~ModelLoader()
{

}

glre::VertexArrayObject_N ModelLoader::loadModel(const std::string &path)
{
    Assimp::Importer Importer;

    const aiScene * pScene = Importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs );

    glre::VertexArrayObject_N Mesh;

    if(pScene)
    {
        if( pScene->HasAnimations() )
        {
            std::cout << "=====Has animations=====\n";
            for(int i=0;i<pScene->mNumAnimations;i++)
            {
                std::cout << "animation: " << pScene->mAnimations[i]->mName.C_Str() << std::endl;
            }
        }

        for(int i=0 ; i < pScene->mNumMeshes ; i++)
        {
            //if(pScene->mMeshes[i]->mNumVertices > 1000)
            {
                auto m = _extract_Mesh( pScene->mMeshes[i], false );

                if(Mesh.numBuffers()==0)
                {
                    Mesh = m;
                }
                else
                {
                    Mesh.mergeWith(m);
                }
            }
        }

    }

    return Mesh;
}


glre::VertexArrayObject_N ModelLoader::_extract_Mesh(aiMesh *InputMesh, bool FlipYZ)
{
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

        std::cout << "Loading mesh: " << InputMesh->mName.C_Str() << std::endl;

        glre::VertexArrayObject_N ReturnMesh;

        if( InputMesh->HasPositions() )
        {
            std::cout << "  Has Positions: " << InputMesh->mNumVertices << std::endl;
            auto B = std::make_shared<v3ArrayBuffer>();
            for( unsigned int j = 0 ; j < InputMesh->mNumVertices ; j++ )  B->insert( vec3( InputMesh->mVertices[j].x, InputMesh->mVertices[j].y, InputMesh->mVertices[j].z  ) );
            ReturnMesh.insertBuffer(B);
        }

        if( InputMesh->HasNormals() )
        {
            std::cout << "  Has Normals: " << InputMesh->mNumVertices << std::endl;
            auto B = std::make_shared<v3ArrayBuffer>();
            for( unsigned int j = 0 ; j < InputMesh->mNumVertices ; j++ )  B->insert( vec3( InputMesh->mNormals[j].x, InputMesh->mNormals[j].y, InputMesh->mNormals[j].z  ) );
            ReturnMesh.insertBuffer(B);
        } else {
            auto B = std::make_shared<v3ArrayBuffer>();
            for( unsigned int j = 0 ; j < InputMesh->mNumVertices ; j++ )  B->insert( vec3( 0.f,1.f,0.0 ) );
            ReturnMesh.insertBuffer(B);
        }

        if(1)
        {
            std::cout << "  Has colour: " << InputMesh->mNumVertices << std::endl;
            auto B = std::make_shared<v4ArrayBuffer>();
            for( unsigned int j = 0 ; j < InputMesh->mNumVertices ; j++ )  B->insert( vec4( 1.0,1.0,1.0,1.0 ) );
            ReturnMesh.insertBuffer(B);
        }

        if( InputMesh->HasTextureCoords(0) )
        {
            std::cout << "  Has UV: " << InputMesh->mNumVertices << std::endl;
            auto B = std::make_shared<v2ArrayBuffer>();
            for( unsigned int j = 0 ; j < InputMesh->mNumVertices ; j++ )  B->insert( vec2( InputMesh->mTextureCoords[0][j].x, InputMesh->mTextureCoords[0][j].y ) );
            ReturnMesh.insertBuffer(B);
        } else {
            auto B = std::make_shared<v2ArrayBuffer>();
            for( unsigned int j = 0 ; j < InputMesh->mNumVertices ; j++ )  B->insert( vec2( 0.f,0.f ) );
            ReturnMesh.insertBuffer(B);
        }

        //======================== Get the Face Indices ============================
        if( InputMesh->HasFaces() )
        {
            std::cout << "  Has Faces: " << InputMesh->mNumFaces << std::endl;
            auto B = std::make_shared<u3ArrayBuffer>( );

            for (unsigned int j = 0 ; j < InputMesh->mNumFaces ; j++)
            {
                const aiFace& Face = InputMesh->mFaces[j];

                if( Face.mNumIndices != 3 ) continue;

                B->insert( uvec3( Face.mIndices[0], Face.mIndices[1], Face.mIndices[2] ) );
            }

            ReturnMesh.setIndexBuffer(B);
        }

        return std::move(ReturnMesh);

}

Skeleton ModelLoader::_extractSkeleton(aiMesh *InputMesh)
{


}


}
