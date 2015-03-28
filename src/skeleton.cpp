#include "gla/skeleton.h"

#include <algorithm>
#include <stack>
#include <map>
#include <set>

#include <gla/transformsequence.h>

namespace gla {

void printMatrix(const gla::mat4 & M)
{
    for(int i=0;i<4;i++)
    {
        for(int j=0; j<4; j++)
        {
            printf("%4.2f%c",  M[i][j] , (j==3 ? ' ' : ','));
        }
        printf("\n");
    }

}

void Skeleton::load(const std::string &path)
{
    Assimp::Importer Importer;
    const aiScene    * pScene = Importer.ReadFile( path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if(pScene)
    {
        std::cout << "=============================================================\n";
        std::cout << "-Number of Meshs: "     << pScene->mNumMeshes     << std::endl;
        //==============Loop Through Meshes=================
        for(int i=0 ; i < pScene->mNumMeshes; i ++)
        {
            std::cout << "#Mesh: "     << pScene->mMeshes[i]->mName.C_Str() << std::endl;
            std::cout << "   #Bones: " << pScene->mMeshes[i]->mNumBones << std::endl;

            //for(int j=0 ; j < pScene->mMeshes[i]->mNumBones; j ++)
            //{
            //    std::cout << "      Bones: " << pScene->mMeshes[i]->mBones[j]->mName.C_Str() << std::endl;
            //}

            _ConstructBoneTree( pScene->mRootNode, mRootBone, pScene->mMeshes[i] );
        }
        //==================================================
        std::cout << "-Number of Materials: " << pScene->mNumMaterials  << std::endl;
        std::cout << "-Number of Textures:"   << pScene->mNumTextures   << std::endl;
        std::cout << "-Number of Animations:" << pScene->mNumAnimations << "\n";
        for(int i=0;i<pScene->mNumAnimations;i++)
            std::cout << "     Animations:" << pScene->mAnimations[i]->mName.C_Str() << "\n";
        std::cout << "=============================================================\n";


        //printMatrix( mRootNode.mTransformMatrix );

        mBoneAnimation.clear();

        if( pScene->HasAnimations() )
        {
            for(int i = 0; i < pScene->mNumAnimations;i++)
            {
                std::string AnimationName( pScene->mAnimations[i]->mName.data );

                std::cout << AnimationName << " duration   : " << pScene->mAnimations[i]->mDuration << std::endl;
                std::cout << AnimationName << " tickspersec: " << pScene->mAnimations[i]->mTicksPerSecond << std::endl;
                std::cout << AnimationName << " Num chans  : " << pScene->mAnimations[i]->mNumChannels << std::endl;

                mBoneAnimation[AnimationName].resize( pScene->mAnimations[i]->mNumChannels );

                for(int j=0;j< pScene->mAnimations[i]->mNumChannels;j++)
                {
                    auto AnimationNode = pScene->mAnimations[i]->mChannels[i];

                    mBoneAnimation[AnimationName][ j ] = _GetTransformationSequence(AnimationNode);
                }

            }

        }

    }
}

int Skeleton::GetBoneTransformationMatrices(gla::mat4 *MatrixArray, int size, float time, const std::string & AnimationName)
{
    std::vector<TransformSequence> & Anim = mBoneAnimation[AnimationName];

    for(int i=0; i<size; i++)
    {
        MatrixArray[i] = Anim[i].getTransformationMatrix(time);
    }
}



void Skeleton::_ConstructBoneTree(const aiNode *node, Bone &rootnode, const aiMesh * mesh, int spaces)
{
    rootnode.mChildren.resize( node->mNumChildren );

    aiMatrix4x4 T = node->mTransformation;
    rootnode.mTransformMatrix = gla::mat4(  T.a1, T.a2, T.a3, T.a4,
                                             T.b1, T.b2, T.b3, T.b4,
                                             T.c1, T.c2, T.c3, T.c4,
                                             T.d1, T.d2, T.d3, T.d4 );

    rootnode.mName = std::string( node->mName.data );



    for(int i=0;i<spaces;i++)
    {
        std::cout << ' ';
    }
    std::cout << rootnode.mName << std::endl;
    //printMatrix( rootnode.mTransformMatrix );

    for(int i=0 ; i < node->mNumChildren ; i++)
    {
        rootnode.mChildren[i] = new Bone();
        _ConstructBoneTree( node->mChildren[i], *rootnode.mChildren[i], mesh, spaces+3 );
    }
}


void Skeleton::_ConstructTransformationSequences(const aiScene * rootscene, const aiNode *node, const aiNodeAnim *animnode, Bone &rootnode)
{
//    if(animnode)
//    {
//        auto T = _GetTransformationSequence(animnode);

//        rootnode.mKeyFrames.mPKeys = std::move( T.mPKeys );
//        rootnode.mKeyFrames.mSKeys = std::move( T.mSKeys );
//        rootnode.mKeyFrames.mRKeys = std::move( T.mRKeys );
//        rootnode.mName             = std::string( node->mName.data ) ;
//    }

//    rootnode.mChildren.resize( node->mNumChildren );
//    for(int i=0 ; i < node->mNumChildren ; i++)
//    {
//        _FindAnimationNode(anim)
//        _ConstructTransformationSequences(rootscene, node->mChildren[i], animnode, rootnode.mChildren[i] );
//    }
}

const aiNodeAnim *Skeleton::_FindAnimationNode(const aiAnimation *pAnimation, const std::string &NodeName)
{
    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++)
    {
        const aiNodeAnim* pNodeAnim           = pAnimation->mChannels[i];

        if (std::string(pNodeAnim->mNodeName.data) == NodeName)
        {
            return pNodeAnim;
        }
    }

    return NULL;
}

TransformSequence Skeleton::_GetTransformationSequence(const aiNodeAnim *node)
{
    TransformSequence T;


    for(int i=0;i< node->mNumPositionKeys; i++)
    {
        vec3key P;
        P.mTime = node->mPositionKeys[i].mTime;
        P.mValue = gla::vec3( node->mPositionKeys[i].mValue.x, node->mPositionKeys[i].mValue.y, node->mPositionKeys[i].mValue.z);

        T.mPKeys.push_back( P );
    }

    std::cout << "p: " << T.mPKeys.back().mTime << std::endl;
    for(int i=0; i < node->mNumScalingKeys; i++)
    {
        vec3key S;
        S.mTime = node->mScalingKeys[i].mTime;
        S.mValue = gla::vec3( node->mScalingKeys[i].mValue.x, node->mScalingKeys[i].mValue.y, node->mScalingKeys[i].mValue.z);

        T.mSKeys.push_back( S );
    }
  // std::cout << "s: " << T.mSKeys.back().mTime << std::endl;
    for(int i=0; i < node->mNumRotationKeys; i++)
    {
        quatkey Q;
        Q.mTime = node->mRotationKeys[i].mTime;
        Q.mValue = gla::quat( node->mRotationKeys[i].mValue.w, node->mRotationKeys[i].mValue.x, node->mRotationKeys[i].mValue.y, node->mRotationKeys[i].mValue.z);

        T.mRKeys.push_back( Q );
    }

   // std::cout << "r: " << T.mRKeys.back().mTime << std::endl;
    return T;
}


}

