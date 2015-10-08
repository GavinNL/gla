#ifndef GLA_TRANSFORMSEQUENCE_H
#define GLA_TRANSFORMSEQUENCE_H

#include <gla/core/types.h>
#include <gla/core/transformation.h>
#include <functional>
#include <algorithm>

namespace gla
{


template<class Element, typename _scalar>
struct DefaultInterpolator {
    void operator()( Element & Out, _scalar x1, Element & y1, _scalar x2, Element & y2, _scalar x)
    {
        float t = static_cast<float>(x-x1) / static_cast<float>(x2-x1);
        Out = y1*(1.0f-t) + y2*t;
    }
};



template<class T, typename IntegerType, class _Interpolator=DefaultInterpolator<T,IntegerType> >
class PWLVector
{
    public:
        typedef std::pair< IntegerType, T> KeyFrame;


        void insert(IntegerType pTime, T pValue)
        {
            auto it = std::lower_bound(mFrames.begin(), mFrames.end(), pTime,
                      [](KeyFrame const & x, IntegerType d)
                      { return x.first < d; });

            if( it == mFrames.end() )
            {
                mFrames.push_back( KeyFrame(pTime, pValue) );
                return;
            }

            mFrames.insert( it, KeyFrame(pTime,pValue) );

        }


        T get(IntegerType pTime)
        {
            if( mFrames.size() == 0) return T();
            if( mFrames.size() == 1) return mFrames[0].second;

            auto it = std::lower_bound(mFrames.begin(), mFrames.end(), pTime,
                      [](KeyFrame const & x, IntegerType d)
                      { return x.first < d; });


            if( it==mFrames.end() )
                return mFrames[ mFrames.size()-1].second;


            auto i = it-1;

            _Interpolator LERP;

            T ret;
            //std::cout << "Interpolating: " << int(i-mFrames.begin()) << ", " << int(it-mFrames.begin()) << "\n";
            LERP(ret, i->first, i->second, it->first, it->second, pTime);
            return ret;

        }


        void erase(IntegerType Start)
        {

        }

        std::vector< KeyFrame > mFrames;
};



struct TransformationInterpolator {
    void operator()( Transformation & Out, unsigned int x1, Transformation & y1, unsigned int x2, Transformation & y2, unsigned int x)
    {
        float t = static_cast<float>(x-x1) / static_cast<float>(x2-x1);
        Transformation::interpolate(Out, y1, y2, t);
    }
};

struct QuatInterpolator {
    void operator()( quat & Out, unsigned int x1, quat & y1, unsigned int x2, quat & y2, unsigned int x)
    {
        float t = static_cast<float>(x-x1) / static_cast<float>(x2-x1);
        Out = glm::slerp(y1, y2, t);
    }
};

typedef PWLVector<gla::Transformation, unsigned int, TransformationInterpolator> TransformationTimeLine;
typedef PWLVector<gla::quat, unsigned int, QuatInterpolator> QuatTimeLine;
typedef PWLVector<gla::vec3, unsigned int>                   Vec3TimeLine;


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
typedef ParameterKey<gla::Transformation> TranKey;

/**
 * @brief The TransformSequence class holds a sequence of transformations for animation purposes
 *
 *
 */
class TransformSequence
{
    public:

        TransformSequence() {};
        TransformSequence(const TransformSequence &  other);          // copy constructor
        TransformSequence(TransformSequence && other);                // move constructor

        TransformSequence & operator=(TransformSequence && other);    // move Operator
        TransformSequence & operator=(TransformSequence &  other);    // copy Operator

        mat4                 getTransformationMatrix(float t)
        {
            return glm::translate(mat4(1.0f), getPosition(t) ) * glm::mat4_cast(getRotation(t)) * glm::scale( mat4(1.0), getScale(t));
            //return glm::mat4_cast( getRotation(t) ) * glm::translate( glm::scale( mat4(1.0), getScale(t)), getPosition(t) );
        }

        inline gla::vec3     getPosition(float t) { return mFrame_position.get( static_cast<unsigned int>(t * 1000) ); }
        inline gla::quat     getRotation(float t) { return mFrame_rot.get( static_cast<unsigned int>(t * 1000) ); }
        inline gla::vec3     getScale(float t)    { return    mFrame_scale.get( static_cast<unsigned int>(t * 1000) ); }

        inline void setPositionKey(float t, const gla::vec3 & p){ mFrame_position.insert( static_cast<unsigned int>(t*1000), p); } ;
        inline void setScaleKey(   float t, const gla::vec3 & s){    mFrame_scale.insert( static_cast<unsigned int>(t*1000), s); } ;
        inline void setRotKey(     float t, const gla::quat & q){      mFrame_rot.insert( static_cast<unsigned int>(t*1000), q); } ;


        void getTransformation(Transformation & Output, float t);


        Vec3TimeLine      mFrame_position; // position keys
        Vec3TimeLine      mFrame_scale; // scaling  keys
        QuatTimeLine      mFrame_rot; // rotation keys

};

}
#endif // TRANSFORMSEQUENCE_H
