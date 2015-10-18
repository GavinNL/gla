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
        Material();

        ~Material();

        /**
         * @brief init
         *
         * Initializes the material. Use this method to compile the shader and get all the uniform locations
         */
        virtual void init()
        {

        }

        virtual void use(){};

        /**
         * @brief onApply
         * @param pEntity
         *
         * This method will be called
         */
        virtual void onApply( GPUArrayObject & pEntity)
        {

        }

        /**
         * @brief sendUniforms Sends uniforms to the shader
         * @param pEntity A reference to the entity that is to be drawn
         *
         * A virtual method that must be overrideen to make use of. The input paramters
         * pEntity and pSceneNode can be use to get various information about the object
         * such as transformations, etc.
         *
         * This will be sent just before the object is rendered.
         */
        virtual void sendUniforms( GPUArrayObject & pEntity /*, ScenenNode & pSceneNode*/){};

    protected:
        ShaderProgram   mShader;

};

}
#endif // MATERIAL_H
