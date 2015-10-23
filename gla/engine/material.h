#ifndef GLA_MATERIAL_H
#define GLA_MATERIAL_H

#include <gla/core/shader.h>
#include <gla/core/vertexarrayobject.h>

namespace gla {


/**
 * @brief The Base class for all materials
 *
 * The Material class is used on various geometry to give it it's associated look. When inheriting from
 * this class, do not unload the shader. It will be automatically done for you.
 */
class Material
{
    public:
        Material(){};

        virtual ~Material(){};

        /**
         * @brief init
         *
         * Initializes the material. Use this method to compile the shader and get all the uniform locations
         */
        virtual void init()
        {

        }

        /**
         * @brief use
         *
         * override this method and use it to send uniform data to the shader
         */
        virtual void use()
        {

        };


};

}
#endif // MATERIAL_H
