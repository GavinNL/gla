/*
 * MIT License
 *
 * Copyright (c) [year] [fullname]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EXP_GEOMETRY_
#define EXP_GEOMETRY_

#include "types.h"
#include <vector>
#include <cmath>

namespace gla {


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


static Mesh createCylinder(float radius = 1.0 , unsigned int faces = 20)
{
    Mesh Cyl;
    for(auto i = 0u ; i < faces+1; i++)
    {
        float const s = static_cast<float>(i) / static_cast<float>(faces);
        float const x = std::cos( 2*3.141592653589f * s );
        float const z = std::sin( 2*3.141592653589f * s );
        float const y = 0;
        Cyl.vertices.push_back(  { vec3(x*radius,y,z*radius) , vec2( s, 0 ), vec3( z, 0, -x )} );
    }

    for(auto i = 0u ; i < faces+1; i++)
    {
        float const s = static_cast<float>(i) / static_cast<float>(faces);
        float const x = std::cos( 2*3.141592653589f * s );
        float const z = std::sin( 2*3.141592653589f * s );
        float const y = 1;
        Cyl.vertices.push_back(  { vec3(x*radius,y,z*radius) , vec2( s, 1 ), vec3( z, 0, -x )} );
    }

    for(auto i=0u;i<faces;i++)
    {
        Cyl.indices.push_back( i           );
        Cyl.indices.push_back( i+1         );
        Cyl.indices.push_back( i+1+faces+1 );

        Cyl.indices.push_back( i           );
        Cyl.indices.push_back( i+faces+1+1 );
        Cyl.indices.push_back( i+faces+1   );
    }

    // top cap
    Cyl.vertices.push_back(  { vec3(0,1,0) , vec2( 0.5, 0.5 ), vec3( 0,  1, 0 )} );
    unsigned int S = static_cast<unsigned int>( Cyl.vertices.size()) ;
    for(auto i = 0u ; i < faces; i++)
    {
        float const s = static_cast<float>(i) / static_cast<float>(faces);
        float const x = std::cos( 2*3.141592653589f * s );
        float const z = std::sin( 2*3.141592653589f * s );
        float const y = 1;
        Cyl.vertices.push_back(  { vec3(x*radius,y,z*radius) , vec2( 0.5+x*0.5, 0.5+z*0.5 ), vec3( 0, 1, 0 )} );

        Cyl.indices.push_back( S     );
        Cyl.indices.push_back( S+i+1 );
        Cyl.indices.push_back( S+i+2 );
    }

    // bottom cap
    Cyl.vertices.push_back(  { vec3(0,0,0) , vec2( 0.5, 0.5 ), vec3( 0,  -1, 0 )} );
    S = static_cast<unsigned int>( Cyl.vertices.size()) ;
    for(auto i = 0u ; i < faces; i++)
    {
        float const s = static_cast<float>(i) / static_cast<float>(faces);
        float const x = std::cos( 2*3.141592653589f * s );
        float const z = std::sin( 2*3.141592653589f * s );
        float const y = 0;
        Cyl.vertices.push_back(  { vec3(x*radius,y,z*radius) , vec2( 0.5+x*0.5, 0.5+z*0.5 ), vec3( 0, -1, 0 )} );

        Cyl.indices.push_back( S+i+2 );
        Cyl.indices.push_back( S+i+1 );
        Cyl.indices.push_back( S     );
    }
    return Cyl;
}

static Mesh createSphere(float radius = 1.0, unsigned int rings=20, unsigned int sectors=20)
{
    Mesh Sphere;

    float const R = 1.0f / (float)(rings-1);
    float const S = 1.0f / (float)(sectors-1);
    unsigned int r, s;

    for(r = 0; r < rings; r++)
        for(s = 0; s < sectors; s++) {
            float const y = std::sin( -3.141592653589f*0.5f + 3.141592653589f * r * R );
            float const x = std::cos(2*3.141592653589f * s * S) * std::sin( 3.141592653589f * r * R );
            float const z = std::sin(2*3.141592653589f * s * S) * std::sin( 3.141592653589f * r * R );

            Sphere.vertices.push_back(  {radius*vec3(x,y,z) ,
                                   vec2(s*S, r*R),
                                   vec3(x,y,z) } );
    }

    for(r = 0 ; r < rings   - 1 ; r++)
    for(s = 0 ; s < sectors - 1 ; s++)
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

}

#endif
