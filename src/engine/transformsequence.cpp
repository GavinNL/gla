#include "gla/engine/transformsequence.h"

#include <algorithm>
namespace gla
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


void TransformSequence::setPositionKey(float t, const gla::vec3 & p)
{
    auto it = std::lower_bound(mPKeys.begin(), mPKeys.end(), t,
              [](vec3key const & x, float d)
              { return x.mTime < d; });

    mPKeys.insert(it, vec3key(t,p ) );
}

void TransformSequence::setScaleKey(float t, const gla::vec3 & s)
{
    auto it = std::lower_bound(mSKeys.begin(), mSKeys.end(), t,
              [](vec3key const & x, float d)
              { return x.mTime < d; });

    mSKeys.insert(it, vec3key(t,s));
}

void TransformSequence::setRotKey(float t, const gla::quat & q)
{
    auto it = std::lower_bound(mRKeys.begin(), mRKeys.end(), t,
              [](quatkey const & x, float d)
              { return x.mTime < d; });

    mRKeys.insert(it, quatkey(t,q));
}

vec3 TransformSequence::getPosition(float t)
{
    if( mPKeys.size() == 0 ) return vec3(0,0,0);

    auto it = std::lower_bound(mPKeys.begin(), mPKeys.end(), t,
              [](vec3key const & x, float d)
              { return x.mTime < d; });

    vec3 p = (it-1)->mValue;

    if(it==mPKeys.end()) return p;

    float  s = (it-1)->mTime;
    float DT = (it)->mTime - (it-1)->mTime;

           s = ( t-s ) / DT;


    p += s * ( it->mValue - p );

    return p;


}

vec3 TransformSequence::getScale(float t)
{
    if( mSKeys.size() == 0 ) return vec3(1.f,1.f,1.f);

    auto it = std::lower_bound(mSKeys.begin(), mSKeys.end(), t,
              [](vec3key const & x, float d)
              { return x.mTime < d; });

    vec3 p = (it-1)->mValue;

    if(it==mSKeys.end()) return p;

    float  s = (it-1)->mTime;
    float DT = (it  )->mTime - (it-1)->mTime;

           s = ( t-s ) / DT;


    p += s * ( it->mValue - p );

    return p;
}

gla::quat TransformSequence::getRotation(float t)
{
    if( mRKeys.size() == 0 ) return quat();

    auto it = std::lower_bound(mRKeys.begin(), mRKeys.end(), t,
              [](quatkey const & x, float d)
              { return x.mTime < d; });

    quat p = (it-1)->mValue;

    if(it==mRKeys.end()) return p;

    float  s = (it-1)->mTime;
    float DT = (it  )->mTime - (it-1)->mTime;

           s = ( t-s ) / DT;


    auto p2 = it->mValue;

           return glm::slerp(p,p2,s);
}

mat4 TransformSequence::getTransformationMatrix(float t)
{
    return glm::mat4_cast( getRotation(t) ) * glm::translate( glm::scale( mat4(1.0), getScale(t)), getPosition(t) );
}

}
