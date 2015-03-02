#include "glre/skeleton.h"

#include <algorithm>

namespace glre {

//Skeleton::Skeleton()
//{

//}

//Skeleton::~Skeleton()
//{

//}

void Skeleton::load(const std::string &path)
{
    Assimp::Importer Importer;
    const aiScene    * pScene = Importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if(pScene)
    {
        std::cout << "=============================================================\n";
        std::cout << "-Number of Meshs: "     << pScene->mNumMeshes     << std::endl;
        std::cout << "-Number of Materials: " << pScene->mNumMaterials  << std::endl;
        std::cout << "-Number of Textures:"   << pScene->mNumTextures   << std::endl;
        std::cout << "-Number of Animations:" << pScene->mNumAnimations << "\n";
        std::cout << "=============================================================\n";

        _ConstructNodeTree( pScene->mRootNode, mRootNode);

        if( pScene->HasAnimations() )
        {

        }
//        if( pScene->HasAnimations( ) )
//        {

//            auto RootNode    =             pScene    ->  mRootNode;
//            std::string Name = std::string(RootNode  ->  mName.data);

//            for(int i=0; i < pScene->mNumAnimations ; i++)
//            {
//                auto  AnimationNode = _FindAnimationNode( pScene->mAnimations[i], Name );
//                if(AnimationNode)
//                {
//                    std::cout << "Loading Animation: (" << Name << ")" << std::endl;
//                }
//            }

//            _PrintNodeHeirch(RootNode);

//        }
    }
}

void Skeleton::_ConstructNodeTree(const aiNode *node, Skeleton::Node &rootnode)
{
    rootnode.mChildren.resize( node->mNumChildren );

    aiMatrix4x4 T = node->mTransformation;

     rootnode.mTransformMatrix = glre::mat4( T.a1, T.a2, T.a3, T.a4,
                                             T.b1, T.b2, T.b3, T.b4,
                                             T.c1, T.c2, T.c3, T.c4,
                                             T.d1, T.d2, T.d3, T.d4);
     rootnode.mName = std::string( node->mName.data );
    std::cout << rootnode.mName << std::endl;

    for(int i=0 ; i < node->mNumChildren ; i++)
    {
        rootnode.mChildren[i] = new Node();
        _ConstructNodeTree( node->mChildren[i], *rootnode.mChildren[i] );
    }
    std::cout << "----" << std::endl;
}


void Skeleton::_ConstructTransformationSequences(const aiScene * rootscene, const aiNode *node, const aiNodeAnim *animnode, Skeleton::Node &rootnode)
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

TransformSequence Skeleton::_GetTransformationSequence(const aiNodeAnim *node)
{
    TransformSequence T;


    for(int i=0;i< node->mNumPositionKeys; i++)
    {
        vec3key P;
        P.mTime = node->mPositionKeys[i].mTime;
        P.mValue = glre::vec3( node->mPositionKeys[i].mValue.x, node->mPositionKeys[i].mValue.y, node->mPositionKeys[i].mValue.z);

        T.mPKeys.push_back( P );
    }

    for(int i=0; i < node->mNumScalingKeys; i++)
    {
        vec3key S;
        S.mTime = node->mScalingKeys[i].mTime;
        S.mValue = glre::vec3( node->mScalingKeys[i].mValue.x, node->mScalingKeys[i].mValue.y, node->mScalingKeys[i].mValue.z);

        T.mSKeys.push_back( S );
    }

    for(int i=0; i < node->mNumRotationKeys; i++)
    {
        quatkey Q;
        Q.mTime = node->mRotationKeys[i].mTime;
        Q.mValue = glre::quat( node->mRotationKeys[i].mValue.w, node->mRotationKeys[i].mValue.x, node->mRotationKeys[i].mValue.y, node->mRotationKeys[i].mValue.z);

        T.mRKeys.push_back( Q );
    }

    return T;
}


}

