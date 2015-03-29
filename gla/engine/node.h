#ifndef GLA_ENGINE_NODE_H
#define GLA_ENGINE_NODE_H

#include <gla/gla.h>

namespace gla {
namespace engine {

class Node
{
    public:
         Node();
        ~Node();

    private:
        gla::Transformation mTransform;

};


}
}

#endif // NODE_H
