#ifndef GLA_TRANSFORMSEQUENCE_H
#define GLA_TRANSFORMSEQUENCE_H

#include <gla/core/global.h>

namespace gla
{

template<class T>
struct ParameterKey
{
    float mTime;
    T     mValue;
    ParameterKey(): mTime(0){};
    ParameterKey(float t, T val) : mTime(t), mValue(val)
    {}
};

typedef ParameterKey<gla::vec3> vec3key;
typedef ParameterKey<gla::quat> quatkey;


/**
 * @brief The TransformSequence class holds a sequence of transformations for animation purposes
 *
 *
 */
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

        void setPositionKey(float t, const gla::vec3 & p);
        void setScaleKey(float t, const gla::vec3 & s);
        void setRotKey(float t, const gla::quat & q);


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
