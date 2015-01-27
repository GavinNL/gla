#ifndef GLRE_H
#define GLRE_H

#include <glre/camera.h>
#include <glre/shader.h>
#include <glre/transformation.h>
#include <glre/vertexarrayobject.h>


namespace glre {

// Triangular Mesh, with Position, Normal and UV values for each vertex
typedef  glre::VertexArrayObject<
                                 glre::ArrayBuffer_uvec3,    // indices for triangles
                                 glre::ArrayBuffer_vec3,     // position
                                 glre::ArrayBuffer_vec3,     // normal
                                 glre::ArrayBuffer_vec2      // UV
                                 > TriMesh_PNU;

// Triangular Mesh, with Position, Normal, Colour and UV values for each vertex
typedef  glre::VertexArrayObject<
                                 glre::ArrayBuffer_uvec3,    // indices for triangles
                                 glre::ArrayBuffer_vec3,     // position
                                 glre::ArrayBuffer_vec3,     // normal
                                 glre::ArrayBuffer_vec4,     // Colour
                                 glre::ArrayBuffer_vec2      // UV
                                > TriMesh_PNCU;

// Line Object, with Position and Colour and for each vertex
typedef  glre::VertexArrayObject<
                                 glre::ArrayBuffer_uvec2,    // indices for triangles
                                 glre::ArrayBuffer_vec3,     // position
                                 glre::ArrayBuffer_vec4      // colour
                                > Line_PC ;

}

#include <glre/solids.h>
#endif // GLRE_H
