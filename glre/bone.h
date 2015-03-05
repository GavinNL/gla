#ifndef GLRE_BONE_H
#define GLRE_BONE_H

#include <glre/global.h>

namespace glre {

class Bone
{
    public:
        Bone();
        Bone(const Bone & other);
        Bone( Bone && other);

        ~Bone();

        Bone & operator = (const Bone & other);

        Bone * find(const std::string & name);

        std::string                     mName;               // The Bone name
        int                             mIndex;              // The Bone index, -1 if there is no bone
        glre::mat4                      mTransformMatrix;    // The initial transformation matrix;
        std::vector<Bone*>              mChildren;
        uint                            mNumChildren;

};

}

#endif // BONE_H
