#ifndef SOLIDS_H
#define SOLIDS_H

#include <glre/global.h>

namespace glre
{


    //TriMesh_PNCU createPrism(float width, float height, float depth);
    //
    iTriMesh_PNCU loadModel(const std::string & path, bool sendToGPU=false);
    //
    Line_PC createAxes(bool sendToGPU=false);

    TriStripMesh_PNCU createPlane(int x_segments, int z_segments, bool sendToGPU);

}

#endif // SOLIDS_H