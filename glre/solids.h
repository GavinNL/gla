#ifndef SOLIDS_H
#define SOLIDS_H

#include <glre/global.h>

namespace glre
{


    //TriMesh_PNCU createPrism(float width, float height, float depth);
    //
    TriMesh_PNCU loadModel(const std::string & path, bool sendToGPU=false);
    //
    Line_PC createAxes(bool sendToGPU=false);

}

#endif // SOLIDS_H
