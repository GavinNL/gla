#ifndef GLRE_MODELLOADER_H
#define GLRE_MODELLOADER_H

#include <glre/global.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glre/skeleton.h>
#include <string>

namespace glre {

class ModelLoader
{
    public:
        ModelLoader();
        ~ModelLoader();

        static glre::VertexArrayObject_N loadModel(const std::string & path);

    private:

        // Load a mesh from  Assimp
        //  IndexOffset is used if you want to add multiple meshes to
        //  the same vertex array object. You will need to keep track
        static glre::VertexArrayObject_N _extract_Mesh( aiMesh * InputMesh, bool FlipYZ = false);


        // Extracts the skeleton from the Assimp Mesh
        static Skeleton      _extractSkeleton( aiMesh * InputMesh );

};

}
#endif // MODELLOADER_H
