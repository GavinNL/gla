#ifndef GLRE_H
#define GLRE_H

#include <glre/camera.h>
#include <glre/shader.h>
#include <glre/transformation.h>
#include <glre/vertexarrayobject.h>
#include <glre/texture.h>
#include <glre/timer.h>

namespace glre {

// Triangular Mesh, with Position, Normal and UV values for each vertex
typedef glre::VertexArrayObject< glre::Vertex_PNCU, glre::uvec3, F3, F3,F4, F2> TriMesh_PNCU;
typedef glre::VertexArrayObject< glre::Vertex_PC,   glre::uvec2, F3, F4 >       Line_PC;

//typedef  glre::VertexArrayObject<
//                                 glre::ArrayBuffer_uvec3,    // indices for triangles
//                                 glre::ArrayBuffer_vec3,     // position
//                                 glre::ArrayBuffer_vec3,     // normal
//                                 glre::ArrayBuffer_vec2      // UV
//                                 > TriMesh_PNU;

// Triangular Mesh, with Position, Normal, Colour and UV values for each vertex
//typedef  glre::VertexArrayObject<
//                                 glre::ArrayBuffer_uvec3,    // indices for triangles
//                                 glre::ArrayBuffer_vec3,     // position
//                                 glre::ArrayBuffer_vec3,     // normal
//                                 glre::ArrayBuffer_vec4,     // Colour
//                                 glre::ArrayBuffer_vec2      // UV
//                                > TriMesh_PNCU;

// Line Object, with Position and Colour and for each vertex
//typedef  glre::VertexArrayObject<
//                                 glre::ArrayBuffer_uvec2,    // indices for triangles
//                                 glre::ArrayBuffer_vec3,     // position
//                                 glre::ArrayBuffer_vec4      // colour
//                                > Line_PC ;

}

#include <glre/solids.h>
#endif // GLRE_H
