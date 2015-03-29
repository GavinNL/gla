#ifndef GLA_ENGINE_SCENE_H
#define GLA_ENGINE_SCENE_H


#include <gla/gla.h>
#include <gla/engine/node.h>


namespace gla {
namespace engine {


class Scene
{
    public:
        Scene();
        ~Scene();

        Node * mRootNode;
};

};
};
#endif // SCENE_H