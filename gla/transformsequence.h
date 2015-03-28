#ifndef GLRE_TRANSFORMSEQUENCE_H
#define GLRE_TRANSFORMSEQUENCE_H

#include <gla/global.h>

namespace gla
{

template<class T>
struct ParameterKey
{
    float mTime;
    T     mValue;
};

typedef ParameterKey<gla::vec3> vec3key;
typedef ParameterKey<gla::quat> quatkey;


class TransformSequence
{
    public:

        TransformSequence() {};
        TransformSequence(const TransformSequence &  other);                // copy constructor
        TransformSequence(TransformSequence && other);                // move constructor

        TransformSequence & operator=(TransformSequence && other);    // move Operator
        TransformSequence & operator=(TransformSequence &  other);    // copy Operator

        gla::vec3           getPosition(float t);
        gla::vec3           getScale(float t);
        gla::quat           getRotation(float t);

        Transformation       getTransformation(float t);
        mat4                 getTransformationMatrix(float t);

        // Makes all position/scaling/rotation keyframes equal in size and
        // have constant step size. This will increase the size dramatically
        // but could help in speed to retrieve transformations;
        void           MakeEqual();

        std::vector<vec3key>            mPKeys; // position keys
        std::vector<vec3key>            mSKeys; // scaling  keys
        std::vector<quatkey>            mRKeys; // rotation keys
};

}
#endif // TRANSFORMSEQUENCE_H
