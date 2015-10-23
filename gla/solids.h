#ifndef SOLIDS_H
#define SOLIDS_H

#include <gla/global.h>
#include <gla/vertexarrayobject.h>

namespace gla
{


    //TriMesh_PNCU createPrism(float width, float height, float depth);
    //
//    iTriMesh_PNCU     loadModel(const std::string & path, bool sendToGPU=false);
//    iTriMesh_PNCUBI   loadModelWithBones(const std::string & path);
    //
    VertexArrayObject_N createAxes();
    VertexArrayObject_N createBox( vec3 size);
    VertexArrayObject_N createPlane(int x_segments, int z_segments);

//    TriStripMesh_PNCU createPlane(int x_segments, int z_segments, bool sendToGPU);


    class Solids
    {

        public:

            static VertexArrayObject_N createBox( const vec3 & s = vec3(1.0,1.0,1.0), bool UseQuadsInstead = true )
            {
                // always use quads for the time being
                if( true )
                {
                    VertexArrayObject Box(QUADS);

                    Box.createBuffer<vec3>();
                    Box.createBuffer<vec2>();
                    Box.createBuffer<vec3>();

                    Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(2, vec3(0.0,  0.0, 1.0) );  Box.insert(1, vec2(0.0,0.0) );
                    Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(2, vec3(0.0,  0.0, 1.0) );  Box.insert(1, vec2(1.0,0.0) );
                    Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(2, vec3(0.0,  0.0, 1.0) );  Box.insert(1, vec2(1.0,1.0) );
                    Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(2, vec3(0.0,  0.0, 1.0) );  Box.insert(1, vec2(0.0,1.0) );


                    Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(2, vec3(0.0,  0.0, -1.0) ); Box.insert(1, vec2(0.0,1.0) );
                    Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(2, vec3(0.0,  0.0, -1.0) ); Box.insert(1, vec2(1.0,1.0) );
                    Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(2, vec3(0.0,  0.0, -1.0) ); Box.insert(1, vec2(1.0,0.0) );
                    Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(2, vec3(0.0,  0.0, -1.0) ); Box.insert(1, vec2(0.0,0.0) );


                    Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(2, vec3(-1.0f, 0.0, 0.0) ); Box.insert(1, vec2(0.0,0.0) );
                    Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(2, vec3(-1.0f, 0.0, 0.0) ); Box.insert(1, vec2(1.0,0.0) );
                    Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(2, vec3(-1.0f, 0.0, 0.0) ); Box.insert(1, vec2(1.0,1.0) );
                    Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(2, vec3(-1.0f, 0.0, 0.0) ); Box.insert(1, vec2(0.0,1.0) );


                    Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(2, vec3(1.0f, 0.0, 0.0) ); Box.insert(1, vec2(0.0,1.0) );
                    Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(2, vec3(1.0f, 0.0, 0.0) ); Box.insert(1, vec2(1.0,1.0) );
                    Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(2, vec3(1.0f, 0.0, 0.0) ); Box.insert(1, vec2(1.0,0.0) );
                    Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(2, vec3(1.0f, 0.0, 0.0) ); Box.insert(1, vec2(0.0,0.0) );

                    Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(2, vec3(0.0f,-1.0, 0.0) );  Box.insert(1, vec2(0.0,0.0) );
                    Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(2, vec3(0.0f,-1.0, 0.0) );  Box.insert(1, vec2(1.0,0.0) );
                    Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(2, vec3(0.0f,-1.0, 0.0) );  Box.insert(1, vec2(1.0,1.0) );
                    Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(2, vec3(0.0f,-1.0, 0.0) );  Box.insert(1, vec2(0.0,1.0) );

                    Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(2, vec3(0.0f, 1.0, 0.0) );  Box.insert(1, vec2(0.0,1.0) );
                    Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(2, vec3(0.0f, 1.0, 0.0) );  Box.insert(1, vec2(1.0,1.0) );
                    Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(2, vec3(0.0f, 1.0, 0.0) );  Box.insert(1, vec2(1.0,0.0) );
                    Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(2, vec3(0.0f, 1.0, 0.0) );  Box.insert(1, vec2(0.0,0.0) );


                    Box.getBuffer<vec3>(0).addOffset( -s*0.5f);
                    return std::move(Box);
                }
                VertexArrayObject Box(TRIANGLES);


                Box.createBuffer<vec3>();
                Box.createBuffer<vec3>();
                Box.createBuffer<vec2>();

                Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(0.0,0.0) );
                Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(1.0,0.0) );
                Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(0.0,0.0) );
                Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(0.0,1.0) );

                Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(1.0,0.0) );
                Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(0.0,0.0) );
                Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(0.0,1.0) );
                Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(0.0,0.0) );

                //========

                Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
                Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(0.0,1.0) );
                Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
                Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,0.0) );

                Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,0.0) );
                Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
                Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,0.0) );
                Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );

                //============

                Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
                Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(1.0,0.0) );
                Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
                Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(0.0,1.0) );

                Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(1.0,0.0) );
                Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
                Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(0.0,1.0) );
                Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
                Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );

                Box.getBuffer<vec3>(0).addOffset( -s*0.5f);
                return std::move(Box);

            }

            static  VertexArrayObject_N createAxes()
            {
                VertexArrayObject_N Axis(LINES);

                auto Positions = std::make_shared<v3ArrayBuffer>();
                auto Colours   = std::make_shared<v4ArrayBuffer>();

                // vertices
                Positions->insert( vec3(0.0, 0.0, 0.0) );
                Positions->insert( vec3(1.0, 0.0, 0.0) );
                Positions->insert( vec3(0.0, 0.0, 0.0) );
                Positions->insert( vec3(0.0, 1.0, 0.0) );
                Positions->insert( vec3(0.0, 0.0, 0.0) );
                Positions->insert( vec3(0.0, 0.0, 1.0) );


                // Colours
                Colours->insert( col4(1.0, 0.0, 0.0, 1.0) );
                Colours->insert( col4(1.0, 0.0, 0.0, 1.0) );
                Colours->insert( col4(0.0, 1.0, 0.0, 1.0) );
                Colours->insert( col4(0.0, 1.0, 0.0, 1.0) );
                Colours->insert( col4(0.0, 0.0, 1.0, 1.0) );
                Colours->insert( col4(0.0, 0.0, 1.0, 1.0) );


                Axis.insertBuffer(Positions);
                Axis.insertBuffer(Colours);

                return( std::move(Axis) );
            }

            static VertexArrayObject_N createPlane(int x_segments, int z_segments)
            {
                VertexArrayObject_N      Mesh;

                float X = (float)x_segments;
                float Z = (float)z_segments;

                Mesh.createBuffer<vec3>(); // Position
                Mesh.createBuffer<vec3>(); // Normal
                Mesh.createBuffer<vec2>(); // UV

                float z = 0.0;
                while(z < Z)
                {
                    for(float x = 0; x <= X; x += 1.0f)
                    {

                        Mesh.insert<vec3>(0,  vec3(x  , 0.0, z)      );  Mesh.insert<vec3>(1,  vec3(0.0, 1.0, 0.0) );  Mesh.insert<vec2>(2,  vec2(x,z)     );
                        Mesh.insert<vec3>(0,  vec3(x  , 0.0, z+1.0f) );  Mesh.insert<vec3>(1,  vec3(0.0, 1.0, 0.0) );  Mesh.insert<vec2>(2,  vec2(x,z+1.0) );
                    }

                    Mesh.insert<vec3>( 0, vec3(X  , 0.0, z+1.0f) );  Mesh.insert<vec3>( 1, vec3(0.0, 1.0, 0.0) );  Mesh.insert<vec2>( 2, vec2(X,z+1.0)  );
                    z += 1.0;

                    for( float x = X; x > 0; x -= 1.0f )
                    {
                        Mesh.insert<vec3>(0, vec3(x  , 0.0, z) );     Mesh.insert<vec3>(1, vec3(0.0, 1.0, 0.0));   Mesh.insert<vec2>(2, vec2(x,z)     );
                        Mesh.insert<vec3>(0, vec3(x  , 0.0, z+1.0f)); Mesh.insert<vec3>(1, vec3(0.0, 1.0, 0.0));   Mesh.insert<vec2>(2, vec2(x,z+1.0) );
                    }
                }

                Mesh.getBuffer<vec3>(0).addOffset( -vec3( X/2.0, 0.0, Z/2.0) );

                return Mesh;
            }

    };

}

#endif // SOLIDS_H
