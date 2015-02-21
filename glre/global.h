#ifndef GLOBAL_H
#define GLOBAL_H

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>


namespace glre
{
    typedef glm::vec2   vec2;
    typedef glm::vec3   vec3;
    typedef glm::vec4   vec4;
    typedef glm::uvec3  uvec3;
    typedef glm::uvec2  uvec2;
    typedef glm::ivec3  ivec3;
    typedef glm::mat4   mat4;
    typedef glm::quat   quat;
    typedef glm::u8vec4 ucol4;
    typedef glm::u8vec3 ucol3;
    typedef glm::vec4    col4;
    typedef glm::vec3    col3;

    struct Vertex_PNCU
    {
        vec3 p; // position
        vec3 n; // normal
        col4 c; // colour
        vec2 u; // uv coords
    };

    struct Vertex_PC
    {
        vec3 p; // position
        col4 c; // colour
    };

    typedef enum
    {
        LINES     = GL_LINES,
        QUADS     = GL_QUADS,
        TRIANGLES = GL_TRIANGLES
    } Primitave;
}

#include <glre/transformation.h>
#include <glre/vertexarrayobject.h>


namespace glre
{

    typedef glre::VertexArrayObject< glre::Vertex_PNCU, glre::uvec3, F3, F3,F4, F2> TriMesh_PNCU;
    typedef glre::VertexArrayObject< glre::Vertex_PC,   glre::uvec2, F3, F4 >       Line_PC;

}


#endif // GLOBAL_H
