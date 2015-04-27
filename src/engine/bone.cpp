#include "gla/engine/bone.h"

namespace gla {

Bone::Bone()
{

}

Bone::~Bone()
{

}

Bone::Bone(const Bone &other)
{
    mName            = other.mName;
    mTransformMatrix = other.mTransformMatrix;
    mChildren        = other.mChildren;
    mNumChildren     = other.mNumChildren;
    for(int i=0;i<other.mChildren.size();i++)
    {
        mChildren.push_back( new Bone( *other.mChildren[i] ) );
    }
}

Bone::Bone(Bone &&other)
{
    mName            = other.mName;
    mTransformMatrix = other.mTransformMatrix;
    mChildren        = std::move(other.mChildren);
    mNumChildren     = other.mNumChildren;
}

Bone& Bone::operator =(const Bone &other)
{

}

Bone *Bone::find(const std::string &name)
{
    for(int i=0;i<mChildren.size(); i++)
    {
        if( mChildren[i]->mName == name) return mChildren[i];
    }

    return 0;
}


}
