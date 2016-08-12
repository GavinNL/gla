#ifndef EXP_GEOMETRY_
#define EXP_GEOMETRY_

#include "types.h"
#include <vector>

namespace gla { namespace experimental {


struct Vertex_pun
{
    glm::vec3 p;
    glm::vec2 u;
    glm::vec3 n;
};






static std::vector<Vertex_pun > createBox( const glm::vec3 & s = glm::vec3(1.0,1.0,1.0) )
{
    using namespace glm;
    // always use quads for the time being



    {
        //VertexArrayObject Box(QUADS);
        std::vector<Vertex_pun> Box;

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
