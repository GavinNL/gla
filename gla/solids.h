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
  //  VertexArrayObject_N createAxes();
  //  VertexArrayObject_N createBox( vec3 size);
  //  VertexArrayObject_N createPlane(int x_segments, int z_segments);

//    TriStripMesh_PNCU createPlane(int x_segments, int z_segments, bool sendToGPU);


    class Solids
    {

        public:

            static gla::InterleavedVAO<vec3, vec2, vec3> createSphere(float radius = 1.0, unsigned int rings=20, unsigned int sectors=20)
            {
                gla::InterleavedVAO<vec3, vec2, vec3> Sphere;


                float const R = 1./(float)(rings-1);
                float const S = 1./(float)(sectors-1);
                int r, s;

                for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                        float const y = sin( -M_PI_2 + M_PI * r * R );
                        float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
                        float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

                        Sphere.InsertVertex(radius*vec3(x,y,z) ,
                                             vec2(s*S, r*R),
                                             vec3(x,y,z) );
                }

                for(r = 0; r < rings-1; r++)
                for(s = 0; s < sectors-1; s++)
                {
                        Sphere.InsertElement( uvec4(
                                                  (r+1) * sectors + s,
                                              (r+1) * sectors + (s+1),
                                              r * sectors + (s+1),
                                              r * sectors + s)
                                              );

                }

                GLA_DOUT  << "Returning from Sphere\n";
                return Sphere;
            }


            static gla::InterleavedVAO<vec3, vec2, vec3> createBox( const vec3 & s = vec3(1.0,1.0,1.0), bool UseQuadsInstead = true )
            {
                // always use quads for the time being
                if( true )
                {
                    //VertexArrayObject Box(QUADS);
                    gla::InterleavedVAO<vec3, vec2, vec3> Box(Primitave::QUADS);

                    //Box.createBuffer<vec3>();
                    //Box.createBuffer<vec2>();
                    //Box.createBuffer<vec3>();

                    Box.InsertVertex( vec3(0.0,0.0,s.z) -s*0.5f, vec2(0.0,0.0) , vec3(0.0,  0.0, 1.0) );
                    Box.InsertVertex( vec3(s.x,0.0,s.z) -s*0.5f, vec2(1.0,0.0) , vec3(0.0,  0.0, 1.0) );
                    Box.InsertVertex( vec3(s.x,s.y,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(0.0,  0.0, 1.0) );
                    Box.InsertVertex( vec3(0.0,s.y,s.z) -s*0.5f, vec2(0.0,1.0) , vec3(0.0,  0.0, 1.0) );

                    Box.InsertVertex( vec3(0.0,s.y,0.0) -s*0.5f, vec2(0.0,1.0) , vec3(0.0,  0.0, -1.0) );
                    Box.InsertVertex( vec3(s.x,s.y,0.0) -s*0.5f, vec2(1.0,1.0) , vec3(0.0,  0.0, -1.0) );
                    Box.InsertVertex( vec3(s.x,0.0,0.0) -s*0.5f, vec2(1.0,0.0) , vec3(0.0,  0.0, -1.0) );
                    Box.InsertVertex( vec3(0.0,0.0,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(0.0,  0.0, -1.0) );

                    Box.InsertVertex( vec3(0.0,0.0,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(-1.0f, 0.0, 0.0) );
                    Box.InsertVertex( vec3(0.0,0.0,s.z) -s*0.5f, vec2(1.0,0.0) , vec3(-1.0f, 0.0, 0.0) );
                    Box.InsertVertex( vec3(0.0,s.y,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(-1.0f, 0.0, 0.0) );
                    Box.InsertVertex( vec3(0.0,s.y,0.0) -s*0.5f, vec2(0.0,1.0) , vec3(-1.0f, 0.0, 0.0) );

                    Box.InsertVertex( vec3(s.x,s.y,0.0) -s*0.5f, vec2(0.0,1.0) , vec3(1.0f, 0.0, 0.0) );
                    Box.InsertVertex( vec3(s.x,s.y,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(1.0f, 0.0, 0.0) );
                    Box.InsertVertex( vec3(s.x,0.0,s.z) -s*0.5f, vec2(1.0,0.0) , vec3(1.0f, 0.0, 0.0) );
                    Box.InsertVertex( vec3(s.x,0.0,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(1.0f, 0.0, 0.0) );

                    Box.InsertVertex( vec3(0.0,0.0,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(0.0f,-1.0, 0.0) );
                    Box.InsertVertex( vec3(s.x,0.0,0.0) -s*0.5f, vec2(1.0,0.0) , vec3(0.0f,-1.0, 0.0) );
                    Box.InsertVertex( vec3(s.x,0.0,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(0.0f,-1.0, 0.0) );
                    Box.InsertVertex( vec3(0.0,0.0,s.z) -s*0.5f, vec2(0.0,1.0) , vec3(0.0f,-1.0, 0.0) );

                    Box.InsertVertex( vec3(0.0,s.y,s.z) -s*0.5f, vec2(0.0,1.0) , vec3(0.0f, 1.0, 0.0) );
                    Box.InsertVertex( vec3(s.x,s.y,s.z) -s*0.5f, vec2(1.0,1.0) , vec3(0.0f, 1.0, 0.0) );
                    Box.InsertVertex( vec3(s.x,s.y,0.0) -s*0.5f, vec2(1.0,0.0) , vec3(0.0f, 1.0, 0.0) );
                    Box.InsertVertex( vec3(0.0,s.y,0.0) -s*0.5f, vec2(0.0,0.0) , vec3(0.0f, 1.0, 0.0) );


                    return Box;
                }
               // VertexArrayObject Box(TRIANGLES);
               //
               //
               // Box.createBuffer<vec3>();
               // Box.createBuffer<vec3>();
               // Box.createBuffer<vec2>();
               //
               // Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(0.0,0.0) );
               // Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(1.0,0.0) );
               // Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(0.0,0.0) );
               // Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(1, vec3(0.0,  0.0, 1.0) ); Box.insert(2, vec2(0.0,1.0) );
               //
               // Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(1.0,0.0) );
               // Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(0.0,0.0) );
               // Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(0.0,1.0) );
               // Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(0.0,  0.0,-1.0) ); Box.insert(2, vec2(0.0,0.0) );
               //
               // //========
               //
               // Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
               // Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(0.0,1.0) );
               // Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
               // Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(1, vec3(-1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,0.0) );
               //
               // Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,0.0) );
               // Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
               // Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,0.0) );
               // Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(1, vec3(1.0f, 0.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
               //
               // //============
               //
               // Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
               // Box.insert(0, vec3(s.x,0.0,0.0) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(1.0,0.0) );
               // Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(0.0,0.0,0.0) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
               // Box.insert(0, vec3(s.x,0.0,s.z) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(0.0,0.0,s.z) ); Box.insert(1, vec3(0.0f,-1.0, 0.0) ); Box.insert(2, vec2(0.0,1.0) );
               //
               // Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(s.x,s.y,0.0) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(1.0,0.0) );
               // Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
               // Box.insert(0, vec3(0.0,s.y,s.z) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(0.0,1.0) );
               // Box.insert(0, vec3(s.x,s.y,s.z) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(1.0,1.0) );
               // Box.insert(0, vec3(0.0,s.y,0.0) ); Box.insert(1, vec3(0.0f, 1.0, 0.0) ); Box.insert(2, vec2(0.0,0.0) );
               //
               // Box.getBuffer<vec3>(0).addOffset( -s*0.5f);
               // return std::move(Box);

            }

            static  gla::InterleavedVAO<vec3, vec4> createAxes()
            {
                //VertexArrayObject_N Axis(LINES);
                gla::InterleavedVAO<vec3, vec4> Axis;

                //auto Positions = std::make_shared<v3ArrayBuffer>();
                //auto Colours   = std::make_shared<v4ArrayBuffer>();

                // vertices
                Axis.InsertVertex( vec3(0.0, 0.0, 0.0) , col4(1.0, 0.0, 0.0, 1.0));
                Axis.InsertVertex( vec3(1.0, 0.0, 0.0) , col4(1.0, 0.0, 0.0, 1.0));
                Axis.InsertVertex( vec3(0.0, 0.0, 0.0) , col4(0.0, 1.0, 0.0, 1.0));
                Axis.InsertVertex( vec3(0.0, 1.0, 0.0) , col4(0.0, 1.0, 0.0, 1.0));
                Axis.InsertVertex( vec3(0.0, 0.0, 0.0) , col4(0.0, 0.0, 1.0, 1.0));
                Axis.InsertVertex( vec3(0.0, 0.0, 1.0) , col4(0.0, 0.0, 1.0, 1.0));


                Axis.InsertElement( uvec2{0,1});
                Axis.InsertElement( uvec2{2,3});
                Axis.InsertElement( uvec2{4,5});
                // Colours
                ///Colours->insert( col4(1.0, 0.0, 0.0, 1.0) );
                ///Colours->insert( col4(1.0, 0.0, 0.0, 1.0) );
                ///Colours->insert( col4(0.0, 1.0, 0.0, 1.0) );
                ///Colours->insert( col4(0.0, 1.0, 0.0, 1.0) );
                ///Colours->insert( col4(0.0, 0.0, 1.0, 1.0) );
                ///Colours->insert( col4(0.0, 0.0, 1.0, 1.0) );

                //Axis.insertBuffer(Positions);
                //Axis.insertBuffer(Colours);

                return( std::move(Axis) );
            }

            static gla::InterleavedVAO<vec3, vec2, vec3> createPlane(int x_segments, int z_segments, vec2 uv_scale=vec2(1.0, 1.0 ) )
            {
                gla::InterleavedVAO<vec3, vec2, vec3> Plane;

                    for(float x = 0.f; x < x_segments; x+=1)
                    {
                        for(float z = 0.f; z < z_segments; z+=1)
                        {
                            Plane.InsertVertex( vec3(x,0.0,z) - vec3(x_segments,0,z_segments)*0.5f , vec2(x,z)*uv_scale, vec3(0,1,0) );
                        }
                    }

                    for(int x = 0; x < x_segments-1; ++x)
                    {
                        for(int z = 0; z < z_segments-1; ++z)
                        {
                          //  Plane.InsertVertex( vec3(x,0.0,z) , vec2(x,z)*uv_scale, vec3(0,1,0) );

                            int i = z*x_segments + x;
                            Plane.InsertElement(
                                        uvec4(i,i+1,i+x_segments+1,i+x_segments)
                                        );
                        }
                    }
                    return Plane;

            }

    };

}

#endif // SOLIDS_H
