#ifndef GLA_SKELETON_H
#define GLA_SKELETON_H


#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <gla/transformation.h>
#include <gla/transformsequence.h>

#include <gla/bone.h>

#include <map>

namespace gla {



class Skeleton
{
public:
        //===========================================================================================
        // Skeleton Node class (bone)
        //===========================================================================================

        //===========================================================================================

        void load(const std::string & path);


        // Fills MatrixArray[0->size-1] with the appropriate bone transfomration matrix
        // for the time.
        // MatrixArray - must be at least the size of the number of bones.
        int GetBoneTransformationMatrices( gla::mat4 * MatrixArray,
                                           int size,
                                           float time,
                                           const std::string & AnimationName = std::string(""));


        Bone                                                        mRootBone;
        uint                                                        mNumBones;


        std::map<std::string, std::vector< TransformSequence > >    mBoneAnimation;  // A map of animations sequences.




private:
        void _ConstructBoneTree( const aiNode * node, Bone & rootnode, const aiMesh * mesh, int spaces=0);

        void _ConstructTransformationSequences( const aiScene * rootscene, const aiNode * node, const aiNodeAnim *animnode, Bone & rootnode);

        const aiNodeAnim* _FindAnimationNode(const aiAnimation* pAnimation, const std::string & NodeName);

        // Reads all the key information from the aiNodeAnim and creates
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
