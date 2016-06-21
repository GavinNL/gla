#ifndef GLA_GLOBAL_H
#define GLA_GLOBAL_H


#include <gla/types.h>


namespace gla
{

    template <typename T>
    struct TBox
    {
        TBox() : pos(0), dim(0) {}
        T pos;
        T dim;
    };




    typedef TBox<gla::vec3>  fBox;
    typedef TBox<gla::ivec3> iBox;
    typedef TBox<gla::uvec3> uBox;

    typedef TBox<gla::vec2>  fRect;
    typedef TBox<gla::ivec2> iRect;
    typedef TBox<gla::uvec2> uRect;


}


#endif // GLOBAL_H
