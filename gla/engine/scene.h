#ifndef GLA_ENGINE_SCENE_H
#define GLA_ENGINE_SCENE_H


#include <gla/gla.h>
#include <gla/engine/node.h>


namespace gla {
namespace engine {


/**
 * @brief The Scene class
 *
 * The Scene class is used to hold an entire set of objects to be rendered. It contains
 * the bare minimum required to draw a scene.
 *
 */
class Scene
{
    public:
        Scene();
        ~Scene();


        Camera& getCamera() { return mCamera; };

    protected:
        SceneNode    mRootNode;
        Camera       mCamera;
};

};
};
#endif // SCENE_H
