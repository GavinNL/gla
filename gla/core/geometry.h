#ifndef EXP_GEOMETRY_
#define EXP_GEOMETRY_

#include "types.h"
#include <vector>
#include <cmath>

namespace gla { namespace experimental {


struct Vertex_pun
{
    glm::vec3 p;
    glm::vec2 u;
    glm::vec3 n;
};


struct Mesh
{
    std::vector<Vertex_pun>   vertices;
    std::vector<unsigned int> indices;
};

static Mesh createSphere(float radius = 1.0, unsigned int rings=20, unsigned int sectors=20)
{

    Mesh Sphere;

    float const R = 1.0f/(float)(rings-1);
    float const S = 1.0f/(float)(sectors-1);
    unsigned int r, s;

    for(r = 0; r < rings; r++)
        for(s = 0; s < sectors; s++) {
            float const y = std::sinf( -3.141592653589f*0.5f + 3.141592653589f * r * R );
            float const x = std::cosf(2*3.141592653589f * s * S) * std::sinf( 3.141592653589f * r * R );
            float const z = std::sinf(2*3.141592653589f * s * S) * std::sinf( 3.141592653589f * r * R );

            Sphere.vertices.push_back(  {radius*vec3(x,y,z) ,
                                   vec2(s*S, r*R),
                                   vec3(x,y,z) } );
    }

    for(r = 0; r < rings-1; r++)
    for(s = 0; s < sectors-1; s++)
    {
        Sphere.indices.push_back(  (r+1) * sectors + s ); //0
        Sphere.indices.push_back(  (r+1) * sectors + (s+1)  ); //1
        Sphere.indices.push_back(   r * sectors + (s+1) ); //2

        Sphere.indices.push_back(  (r+1) * sectors + s ); //0
        Sphere.indices.push_back(   r * sectors + (s+1) ); //2
        Sphere.indices.push_back(    r * sectors + s ); //3
    }

    GLA_DOUT  << "Returning from Sphere\n";
    return Sphere;
}


static std::vector<Vertex_pun > createBox( const glm::vec3 & s = glm::vec3(1.0,1.0,1.0) )
{
    using namespace glm;
    // always use quads for the time being

    {
        //VertexArrayObject Box(QUADS);
        std::vector< Vertex_pun > Box;

        Box.push_back( { vec3(0.0,0.0,s.z) -s*0.5f, vec2(0.0,0.0) , vec3(0.0,  0.0, 1.0)} ); // 0
        Box.push_back( { vec3(s.x,0.0,s.z) -s*0.5f, vec2(1.0,0.0) , vec3(0.0,  0.0, 1.0)} ); // 1
        Box.push_back( { vec3(s.x,s.y,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(0.0,  0.0, 1.0)} ); // 2

        Box.push_back( { vec3(0.0,0.0,s.z) -s*0.5f, vec2(0.0,0.0) , vec3(0.0,  0.0, 1.0)} ); // 0
        Box.push_back( { vec3(s.x,s.y,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(0.0,  0.0, 1.0)} ); // 2
        Box.push_back( { vec3(0.0,s.y,s.z) -s*0.5f, vec2(0.0,1.0) , vec3(0.0,  0.0, 1.0)} ); // 3



        Box.push_back( { vec3(0.0,s.y,0.0) -s*0.5f, vec2(0.0,1.0) , vec3(0.0,  0.0, -1.0)} ); // 0
        Box.push_back( { vec3(s.x,s.y,0.0) -s*0.5f, vec2(1.0,1.0) , vec3(0.0,  0.0, -1.0)} ); // 1
        Box.push_back( { vec3(s.x,0.0,0.0) -s*0.5f, vec2(1.0,0.0) , vec3(0.0,  0.0, -1.0)} ); // 2

        Box.push_back( { vec3(0.0,s.y,0.0) -s*0.5f, vec2(0.0,1.0) , vec3(0.0,  0.0, -1.0)} ); // 0
        Box.push_back( { vec3(s.x,0.0,0.0) -s*0.5f, vec2(1.0,0.0) , vec3(0.0,  0.0, -1.0)} ); // 2
        Box.push_back( { vec3(0.0,0.0,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(0.0,  0.0, -1.0)} ); // 3



        Box.push_back( {vec3(0.0,0.0,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(-1.0f, 0.0, 0.0)} ); // 0
        Box.push_back( {vec3(0.0,0.0,s.z) -s*0.5f, vec2(1.0,0.0) , vec3(-1.0f, 0.0, 0.0)} ); // 1
        Box.push_back( {vec3(0.0,s.y,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(-1.0f, 0.0, 0.0)} ); // 2

        Box.push_back( {vec3(0.0,0.0,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(-1.0f, 0.0, 0.0)} ); // 0
        Box.push_back( {vec3(0.0,s.y,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(-1.0f, 0.0, 0.0)} ); // 2
        Box.push_back( {vec3(0.0,s.y,0.0) -s*0.5f, vec2(0.0,1.0) , vec3(-1.0f, 0.0, 0.0)} ); // 3



        Box.push_back( {vec3(s.x,s.y,0.0) -s*0.5f, vec2(0.0,1.0) , vec3(1.0f, 0.0, 0.0)} ); // 0
        Box.push_back( {vec3(s.x,s.y,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(1.0f, 0.0, 0.0)} ); // 1
        Box.push_back( {vec3(s.x,0.0,s.z) -s*0.5f, vec2(1.0,0.0) , vec3(1.0f, 0.0, 0.0)} ); // 2

        Box.push_back( {vec3(s.x,s.y,0.0) -s*0.5f, vec2(0.0,1.0) , vec3(1.0f, 0.0, 0.0)} ); // 0
        Box.push_back( {vec3(s.x,0.0,s.z) -s*0.5f, vec2(1.0,0.0) , vec3(1.0f, 0.0, 0.0)} ); // 2
        Box.push_back( {vec3(s.x,0.0,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(1.0f, 0.0, 0.0)} ); // 3



        Box.push_back( {vec3(0.0,0.0,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(0.0f,-1.0, 0.0)} ); // 0
        Box.push_back( {vec3(s.x,0.0,0.0) -s*0.5f, vec2(1.0,0.0) , vec3(0.0f,-1.0, 0.0)} ); // 1
        Box.push_back( {vec3(s.x,0.0,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(0.0f,-1.0, 0.0)} ); // 2

        Box.push_back( {vec3(0.0,0.0,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(0.0f,-1.0, 0.0)} ); // 0
        Box.push_back( {vec3(s.x,0.0,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(0.0f,-1.0, 0.0)} ); // 2
        Box.push_back( {vec3(0.0,0.0,s.z) -s*0.5f, vec2(0.0,1.0) , vec3(0.0f,-1.0, 0.0)} ); // 3



        Box.push_back( {vec3(0.0,s.y,s.z) -s*0.5f, vec2(0.0,1.0) , vec3(0.0f, 1.0, 0.0)} ); // 0
        Box.push_back( {vec3(s.x,s.y,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(0.0f, 1.0, 0.0)} ); // 1
        Box.push_back( {vec3(s.x,s.y,0.0) -s*0.5f, vec2(1.0,0.0) , vec3(0.0f, 1.0, 0.0)} ); // 2

        Box.push_back( {vec3(0.0,s.y,s.z) -s*0.5f, vec2(0.0,1.0) , vec3(0.0f, 1.0, 0.0)} ); // 0
        Box.push_back( {vec3(s.x,s.y,0.0) -s*0.5f, vec2(1.0,0.0) , vec3(0.0f, 1.0, 0.0)} ); // 2
        Box.push_back( {vec3(0.0,s.y,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(0.0f, 1.0, 0.0)} ); // 3


        return Box;
    }
}

}}

#endif
