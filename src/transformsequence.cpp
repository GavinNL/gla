#include "glre/transformsequence.h"

#include <algorithm>
namespace glre
{

TransformSequence::TransformSequence(TransformSequence &&other)
{
    mPKeys = std::move(other.mPKeys);
    mRKeys = std::move(other.mRKeys);
    mSKeys = std::move(other.mSKeys);

}

TransformSequence::TransformSequence(const TransformSequence &other)
{
    mPKeys = other.mPKeys;
    mRKeys = other.mRKeys;
    mSKeys = other.mSKeys;
}

TransformSequence& TransformSequence::operator=(TransformSequence &&other)
{
    mPKeys = std::move(other.mPKeys);
    mRKeys = std::move(other.mRKeys);
    mSKeys = std::move(other.mSKeys);

    return *this;
}

TransformSequence &TransformSequence::operator=(TransformSequence &other)
{
    mPKeys = other.mPKeys;
    mRKeys = other.mRKeys;
    mSKeys = other.mSKeys;
    return *this;
}

vec3 TransformSequence::getPosition(float t)
{
    if( mPKeys.size() ==0 ) return vec3(0,0,0);

    auto it = std::lower_bound(mPKeys.begin(), mPKeys.end(), t,
              [](vec3key const & x, float d)
              { return x.mTime < d; });

    vec3 p = it->mValue;
    if(it==mPKeys.end()) return p;

    float  s = it->mTime;
    float DT = (it++)->mTime - it->mTime;

           s = ( t - s ) / DT;

    p += s*( it->mValue - p );
    return p;


}

vec3 TransformSequence::getScale(float t)
{
    if( mSKeys.size() ==0 ) return vec3(1,1,1);
    auto it = std::lower_bound(mSKeys.begin(), mSKeys.end(), t,
              [](vec3key const & x, float d)
              { return x.mTime < d; });

    vec3 p = it->mValue;
    if(it==mSKeys.end()) return p;

    float  s = it->mTime;
    float DT = (it++)->mTime - it->mTime;

           s = ( t - s ) / DT;

    p += s*( it->mValue - p );
    return p;
}

glre::quat TransformSequence::getRotation(float t)
{
    if( mRKeys.size() ==0 ) return quat();
    auto it = std::lower_bound(mRKeys.begin(), mRKeys.end(), t,
              [](quatkey const & x, float d)
              { return x.mTime < d; });

    glre::quat p = it->mValue;
    if(it == mRKeys.end()) return p;

    float  s = it->mTime;
    float DT = (it++)->mTime - it->mTime;

    glre::quat p2 = it->mValue;
           s = ( t - s ) / DT;

           return glm::slerp(p,p2,s);
}

mat4 TransformSequence::getTransformationMatrix(float t)
{
    return glm::mat4_cast( getRotation(t) ) * glm::translate( glm::scale( mat4(1.0), getScale(t)), getPosition(t) );
}

}
