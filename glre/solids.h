#ifndef SOLIDS_H
#define SOLIDS_H

#include <glre/glre.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace glre
{


TriMesh_PNCU createPrism(float width, float height, float depth);

TriMesh_PNCU loadModel(const std::string & path);

}

#endif // SOLIDS_H
