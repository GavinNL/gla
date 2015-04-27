#include "gla/engine/transformsequence.h"

#include <algorithm>
namespace gla
{

TransformSequence::TransformSequence(TransformSequence &&other)
{
    mFrame_position = std::move(other.mFrame_position);
    mFrame_rot      = std::move(other.mFrame_rot);
    mFrame_scale    = std::move(other.mFrame_scale);

}

TransformSequence::TransformSequence(const TransformSequence &other)
{
    mFrame_position = other.mFrame_position;
    mFrame_rot      = other.mFrame_rot;
    mFrame_scale    = other.mFrame_scale;
}

TransformSequence& TransformSequence::operator=(TransformSequence &&other)
{
    mFrame_position = std::move(other.mFrame_position);
    mFrame_rot      = std::move(other.mFrame_rot);
    mFrame_scale    = std::move(other.mFrame_scale);

    return *this;
}

TransformSequence &TransformSequence::operator=(TransformSequence &other)
{
    mFrame_position = other.mFrame_position;
    mFrame_rot      = other.mFrame_rot;
    mFrame_scale    = other.mFrame_scale;
    return *this;
}


}
