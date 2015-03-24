#ifndef GLRE_ENGINE_NODE_H
#define GLRE_ENGINE_NODE_H

#include <glre/glre.h>

namespace glre {
namespace engine {

class Node
{
    public:
         Node();
        ~Node();

    private:
        glre::Transformation mTransform;

};


}
}

#endif // NODE_H
