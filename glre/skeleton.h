#ifndef GLRE_SKELETON_H
#define GLRE_SKELETON_H


#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glre/transformation.h>
#include <glre/transformsequence.h>

namespace glre {



class Skeleton
{
public:


        class Node
        {
            public:
                Node()
                {
                    //std::cout << "Node constructed\n";
                };

                ~Node()
                {
                    //std::cout << "Node destroyed\n";
                }

                Node(const Node & other)
                {
                    mName            = other.mName;
                    mTransformMatrix = other.mTransformMatrix;
                    mChildren        = other.mChildren;
                    mNumChildren     = other.mNumChildren;
                    for(int i=0;i<other.mChildren.size();i++)
                    {
                        mChildren.push_back( new Node( *other.mChildren[i] ) );
                    }
                }

                Node( Node && other)
                {
                    mName            = other.mName;
                    mTransformMatrix = other.mTransformMatrix;
                    mChildren        = std::move(other.mChildren);
                    mNumChildren     = other.mNumChildren;
                }

                Node & operator = (const Node & other)
                {

                }
                Node * find(const std::string & name)
                {
                    for(int i=0;i<mChildren.size(); i++)
                    {
                        if( mChildren[i]->mName == name) return mChildren[i];
                    }

                    return 0;
                }

            std::string                     mName;
            glre::mat4                      mTransformMatrix;    // The initial transformation matrix;
            TransformSequence               mKeyFrames;    // Contains the keyframes for position, scaling and transformation.
            std::vector<Node*>              mChildren;
            uint                            mNumChildren;

        };

        void load(const std::string & path);

        Node mRootNode;




private:
        void _ConstructNodeTree( const aiNode * node, Node & rootnode);

        void _ConstructTransformationSequences( const aiScene * rootscene, const aiNode * node, const aiNodeAnim *animnode, Node & rootnode);

        void _PrintNodeHeirch(aiNode * N, int spacing = 0)
        {
            for(int i=0;i<spacing;i++)
                std::cout << " ";



            std::cout << std::string( N->mName.data) << " " <<  N->mNumMeshes << "   Transform: ";
            for(int i=0;i<16;i++)
            {
                std::cout << *(N->mTransformation[i]) << ", ";
            }
            std::cout << std::endl;
            for(int i=0;i<N->mNumChildren;i++)
            {
                _PrintNodeHeirch( N->mChildren[i], spacing+3);
            }
        }

        const aiNodeAnim* _FindAnimationNode(const aiAnimation* pAnimation, const std::string & NodeName)
        {
            //std::cout << "_FindAnimations: " << NodeName << "   #Channels: " << pAnimation->mNumChannels << std::endl;

            //std::cout << "Num channels: " << pAnimation->mNumChannels << std::endl;
            for (uint i = 0 ; i < pAnimation->mNumChannels ; i++)
            {
                const aiNodeAnim* pNodeAnim           = pAnimation->mChannels[i];
                //std::cout << std::string(pNodeAnim->mNodeName.data) << " ==? " << NodeName << std::endl;

                //std::cout << " Duration     : " << pAnimation->mDuration << std::endl;
                //std::cout << " TicksPerSec  : " << pAnimation->mTicksPerSecond<< std::endl;
                //_PrintAnimationInfo(pNodeAnim);
                auto T = _GetTransformationSequence(pNodeAnim);

                if (std::string(pNodeAnim->mNodeName.data) == NodeName)
                {
                    return pNodeAnim;
                }
            }

            return NULL;
        }



        // Reads alll the key information from the aiNodeAnim and creates
        // a transformation sequence class
        TransformSequence _GetTransformationSequence(const aiNodeAnim * node);


        void _PrintAnimationInfo(const aiNodeAnim * node)
        {
            std::cout << " Position Keys: " << node->mNumPositionKeys << std::endl;
            std::cout << "   Scale Keys : " << node->mNumScalingKeys << std::endl;
            std::cout << "Rotation Keys : " << node->mNumRotationKeys << std::endl;
        }

};

}
#endif // SKELETON_H
