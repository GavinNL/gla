#ifndef GLRE_H
#define GLRE_H

#include <glre/camera.h>
#include <glre/shader.h>
#include <glre/transformation.h>
#include <glre/vertexarrayobject.h>


// Triangular Mesh, with Position, Normal and UV values for each vertex
typedef  glre::VertexArrayObject<
                                 glre::ArrayBuffer_uV3,    // indices for triangles
                                 glre::ArrayBuffer_V3,     // position
                                 glre::ArrayBuffer_V3,     // normal
                                 glre::ArrayBuffer_V2      // UV
                                 > TriMesh_PNU;

// Triangular Mesh, with Position, Normal, Colour and UV values for each vertex
typedef  glre::VertexArrayObject<
                                 glre::ArrayBuffer_uV3,    // indices for triangles
                                 glre::ArrayBuffer_V3,     // position
                                 glre::ArrayBuffer_V3,     // normal
                                 glre::ArrayBuffer_V4,     // Colour
                                 glre::ArrayBuffer_V2      // UV
                                > TriMesh_PNCU;

// Line Object, with Position and Colour and for each vertex
typedef  glre::VertexArrayObject<
                                 glre::ArrayBuffer_uV2,    // indices for triangles
                                 glre::ArrayBuffer_V3,     // position
                                 glre::ArrayBuffer_V4     // colour
                                > Line_PC ;

#endif // GLRE_H
