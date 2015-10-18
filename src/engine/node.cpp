#include "gla/engine/node.h"
#include <algorithm>

namespace gla {
namespace engine {


SceneNode::SceneNode() : mChildren(), mParent(nullptr), mName("")
{

}

SceneNode::~SceneNode()
{

}

SceneNode & SceneNode::AttachChild(uPtr child)
{
    child->mParent = this;
    mChildren.push_back( std::move(child) );
    return *this;
}

SceneNode::uPtr SceneNode::DetachChild(const SceneNode & node)
{
    auto found = std::find_if( mChildren.begin(),
                               mChildren.end(),
                               [&node](uPtr & child){
                                     return child.get() == &node;
                                 }
                               );

    if( found != mChildren.end() )
    {
        auto result     = std::move( *found );
        result->mParent = nullptr;
        mChildren.erase(found);

        return result;
    }

    // no child was found;
    return nullptr;
}


gla::Transformation  SceneNode::GetGlobalTransform() const
{
    gla::Transformation result;
    for(SceneNode * p = mParent; p != nullptr; p->mParent)
    {
        result *= p->mTransform;
    }
    return result;
}

void SceneNode::onStart()
{
    onStartCurrent();
    onStartChildren();
}

void SceneNode::update(float dt)
{
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::draw(Transformation t )
{
    t *= this->mTransform;
    drawCurrent(t);
    drawChildren(t);
}

void SceneNode::onStartCurrent()
{
    // do nothing by default
}

void SceneNode::onStartChildren()
{
    for(uPtr & child : mChildren)
    {
        child->onStart();
    }
}

void SceneNode::updateCurrent(float dt)
{
    // do nothing by default
}

void SceneNode::updateChildren(float dt)
{
    for(uPtr & child : mChildren)
    {
        child->update(dt);
    }
}

void SceneNode::drawCurrent(Transformation t)
{
    // do nothing by default
}

void SceneNode::drawChildren(Transformation t)
{
    for(uPtr & child : mChildren)
    {
        child->draw(t);
    }
}

} // engine
} // glre
