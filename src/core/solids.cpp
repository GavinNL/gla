#include "gla/core/solids.h"
#include "gla/core/vertexarrayobject.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>


gla::iTriMesh_PNCU gla::loadModel(const std::string & path, bool FlipYZ)
{
    Assimp::Importer Importer;

    const aiScene * pScene = Importer.ReadFile(path.c_str(),
                                               aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    std::cout << "Loading mesh: " << path << std::endl;
    gla::iTriMesh_PNCU ReturnMesh;
    int count = 0;


    if (pScene)
     {

            for (unsigned int i = 0 ; i < pScene->mNumMeshes ; i++)
            {
                const aiMesh* paiMesh = pScene->mMeshes[i];

                if(!paiMesh) continue;

                //std::cout << "--Mesh " << i << std::endl;
                //std::cout << "--- Num Vertices  " << paiMesh->mNumVertices << std::endl;
                //std::cout << "--- Num Triangles " << paiMesh->mNumFaces    << std::endl;

                for (unsigned int j = 0 ; j < paiMesh->mNumVertices ; j++)
                {

                    const aiVector3D* pPos      = &(paiMesh->mVertices[j]);
                    const aiVector3D* pNormal   = paiMesh->HasNormals() ? &(paiMesh->mNormals[j]) : &Zero3D;
                    const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][j]) : &Zero3D;

                    if(FlipYZ)
                    {
                        ReturnMesh.insertVertex(  { vec3(      pPos->x,      pPos->z,    pPos->y     ),
                                                    vec3(   pNormal->x,   pNormal->z, pNormal->y     ),
                                                    vec4(      1.0    ,          1.0,        1.0,1.0 ),
                                                    vec2( pTexCoord->x, pTexCoord->y                 ) } );
                    } else
                    {
                        ReturnMesh.insertVertex(  { vec3(      pPos->x,      pPos->y,    pPos->z     ),
                                                    vec3(   pNormal->x,   pNormal->y, pNormal->z     ),
                                                    vec4(      1.0    ,          1.0,        1.0,1.0 ),
                                                    vec2( pTexCoord->x, pTexCoord->y                 ) } );
                    }
                }

                //std::cout << "--Mesh " << i << " completed " << std::endl;
                for (unsigned int j = 0 ; j < paiMesh->mNumFaces ; j++)
                {
                    const aiFace& Face = paiMesh->mFaces[j];
                    //std::cout << Face.mNumIndices << std::endl;
                    if(Face.mNumIndices != 3) continue;
                    ReturnMesh.insertElement(  uvec3( Face.mIndices[0]+count, Face.mIndices[1]+count, Face.mIndices[2]+count ) );
                }
                count+=paiMesh->mNumVertices;

            }

            return ReturnMesh;
        }
        else {
            printf("Error parsing '%s': '%s'\n", path.c_str(), Importer.GetErrorString());
            return ReturnMesh;
        }

}

gla::VertexArrayObject_N gla::createBox( vec3 s )
{
    VertexArrayObject Box(TRIANGLES);

    //auto Positions = std::make_shared<v3ArrayBuffer>();
    //auto Normals   = std::make_shared<v3ArrayBuffer>();
    //auto UV        = std::make_shared<v2ArrayBuffer>();


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

gla::VertexArrayObject_N gla::createAxes()
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

gla::VertexArrayObject_N gla::createPlane(int x_segments, int z_segments)
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

gla::TriStripMesh_PNCU gla::createPlane(int x_segments, int z_segments, bool sendToGPU)
{
    TriStripMesh_PNCU      Mesh;

    float X = (float)x_segments;
    float Z = (float)z_segments;

    float z = 0.0;
    while(z < Z)
    {
        for(float x = 0; x <= X; x += 1.0f)
        {
            Mesh.insertVertex( {  vec3(x  , 0.0, z)      , vec3(0.0, 1.0, 0.0),  col4(1.0,1.0,1.0,1.0),  vec2(x,z)      } );
            Mesh.insertVertex( {  vec3(x  , 0.0, z+1.0f) , vec3(0.0, 1.0, 0.0),  col4(1.0,1.0,1.0,1.0),  vec2(x,z+1.0)  } );
        }

        Mesh.insertVertex( {  vec3(X  , 0.0, z+1.0f) , vec3(0.0, 1.0, 0.0),  col4(1.0,1.0,1.0,1.0),  vec2(X,z+1.0)  } );
        z += 1.0;
        //if(z > Z ) break;


        for( float x = X; x > 0; x -= 1.0f )
        {
            Mesh.insertVertex( {  vec3(x  , 0.0, z)      , vec3(0.0, 1.0, 0.0),  col4(1.0,1.0,1.0,1.0),  vec2(x,z)      } );
            Mesh.insertVertex( {  vec3(x  , 0.0, z+1.0f) , vec3(0.0, 1.0, 0.0),  col4(1.0,1.0,1.0,1.0),  vec2(x,z+1.0)  } );
        }
    }

    for(int i=0;i<Mesh.mVertexBuffer.cpuBufferSize();i++  )
    {
        Mesh.mVertexBuffer[i].p -= vec3( X/2.0, 0.0, Z/2.0);
    }
    //if(sendToGPU) Mesh.sendToGPU();

    return Mesh;
}

