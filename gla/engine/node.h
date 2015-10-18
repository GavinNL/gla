#ifndef GLA_ENGINE_SCENE_NODE_H
#define GLA_ENGINE_SCENE_NODE_H

#include <gla/gla.h>
#include <vector>
#include <memory>

namespace gla {
namespace engine {

class SceneNode
{
    public:
        using uPtr = std::unique_ptr<SceneNode>;

         SceneNode();
        ~SceneNode();

         SceneNode&             AttachChild(uPtr child);
         uPtr                   DetachChild(const SceneNode & child);
         gla::Transformation    GetGlobalTransform() const;
         void                   onStart();
         void                   update(float dt);
         virtual void           draw(Transformation t = Transformation());

         gla::Transformation  mTransform;

    protected:

        virtual void onStartCurrent();
                void onStartChildren();

        virtual void updateCurrent(float dt);
                void updateChildren(float dt);

        virtual void drawCurrent(Transformation t);
                void drawChildren(Transformation t);


    private:
        std::vector< uPtr > mChildren;
        SceneNode*          mParent;
        std::string         mName;


};


}
}

#endif // NODE_H
