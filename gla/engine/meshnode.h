#ifndef GLA_ENGINE_MESHNODE_H
#define GLA_ENGINE_MESHNODE_H

#include <gla/engine/scenenode.h>
#include <gla/engine/material.h>

namespace gla {
namespace engine {


struct ModelAsset
{
    Mesh     *mMesh;
    Material *mMaterial;
};


class MeshNode : public SceneNode
{
public:
    MeshNode() : mModel(nullptr) {};

    void         AttachModel(ModelAsset * M){ mModel = M;      }
    void         DetachModel()              { mModel = nullptr;}
    ModelAsset*  GetModel() const           { return mModel;   }

    ModelAsset * mModel;
};

}}

#endif // MESHNODE_H
