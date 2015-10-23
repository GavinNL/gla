#ifndef GLA_ENGINE_SCENE_NODE_H
#define GLA_ENGINE_SCENE_NODE_H

#include <gla/core/core.h>
#include <gla/engine/common.h>

#include <vector>
#include <memory>
#include <typeindex>

namespace gla {
namespace engine {

class SceneNode;

class NodeComponent
{
    public:
        using uPtr             = std::unique_ptr<NodeComponent>;

        virtual void draw( Transformation t = Transformation() ) {};
        virtual void onStart() {};

        SceneNode * mParent;
};



class SceneNode
{
    public:
        using uPtr              = std::unique_ptr<SceneNode>;
        using GroupComponentMap = std::map<std::type_index, std::vector<NodeComponent*> >;


         SceneNode();
        virtual ~SceneNode();


         template<class T>
         T*                     CreateChild()
         {
            auto f = new T();
            std::unique_ptr<T> uptr( f );
            AttachChild( std::move(uptr) );
            return f;
         }


         SceneNode* AddComponent(NodeComponent * component)
         {
            component->mParent = this;
            std::type_index id( typeid(*component) );

            mGroupComponents[id].push_back(component);
         }

         template<class Derived, class... Args>
         inline SceneNode* AddComponent(Args&&... args)
         {
             return AddComponent( new Derived(args...) );
         }

         SceneNode&             AttachChild(SceneNode::uPtr child);
         uPtr                   DetachChild(const SceneNode & child);
         gla::Transformation    GetGlobalTransform() const;
         void                   onStart();
         void                   update(float dt);
         virtual void           draw(Transformation t = Transformation());

         gla::Transformation    mTransform;
         GroupComponentMap      mGroupComponents;



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
};




}
}

#endif // NODE_H
