#ifndef GLRE_ENGINE_SCENE_H
#define GLRE_ENGINE_SCENE_H


#include <glre/glre.h>
#include <glre/engine/node.h>


namespace glre {
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
