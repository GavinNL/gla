#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <iostream>

#include <map>
#include <string>

#include <deque>
#include <memory>

#include <fstream>
#include <streambuf>
#include <functional>

#include <glre/glre.h>
#include <rgui/rgui.h>

#include <rgui/SDL_InputHandler.h>

#include <bitset>
using namespace glre;

#define WIDTH  1024
#define HEIGHT 768

#define APPLICATION_TITLE "This is a test"
bool mQuit = false;

// Creates a SDL window and openGL rendering context so we can do some rendring
SDL_Window* setupSDL();


rgui::Root      *R;
rgui::pInterface I;


std::map<std::string,
        std::function<void(const SDL_MouseMotionEvent&)> >  MouseMotionEvents;

std::map<std::string,
        std::function<void(const SDL_MouseButtonEvent&)> >  MouseButtonEvents;

std::map<std::string,
        std::function<void(const SDL_Event&)> >  SDLEvents;
// Handles the keyboard and mouse inputs for the GUI Interface.
void handleInputs()
{
    SDL_Event e;

    while( SDL_PollEvent( &e ) != 0 )
    {
        SDL_InputHandler::HandleInput(I, e);

        for(auto a : SDLEvents) a.second( e);

            switch(e.type)
            {

            case SDL_KEYUP:
                if( e.key.keysym.sym == SDLK_ESCAPE) mQuit=true;
                break;
            }
    }
}

#define STB_PERLIN_IMPLEMENTATION
#include <glre/stb/stb_perlin.h>



template <class T>
class Array3D
{
    public:
    Array3D(unsigned int size)
    {
        data = new T[size*size*size];
        X = Y = Z = size;
    }
    ~Array3D()
    {
        delete [] data;
        data = 0;
    }

    inline T & operator()( const unsigned int i)
    {
        return data[i];
    }

    inline T & operator()( const uvec3 & p)
    {
        return data[ X*Y*p[2] + X*p[1] + p[0] ];
    }

    inline T & operator()( const ivec3 & p)
    {
        return data[ X*Y*p[2] + X*p[1] + p[0] ];
    }

    inline T & operator()( int x, int y, int z)
    {
        return data[ X*(Y*z + y) + x];
    }

    T * data;
    unsigned int X;
    unsigned int Y;
    unsigned int Z;
};

//=============================

struct VertexNormal
{
    vec3 v;
    vec3 n;
    int index;
    short EdgeFlags;  // Which edge of the cube needs to have a quad created
};

// This is a table of vertex offset values relative to the lower/left/front vertex of a cube.
const ivec3 Offsets[8] = {{0,0,0},
                        {1,0,0},
                        {0,0,1},
                        {1,0,1},
                        {0,1,0},
                        {1,1,0},
                        {0,1,1},
                        {1,1,1} };

// This is a table of vertices that define an edge of a cube. There are 12 edges.
// For example, EdgeList[0] = {0,1}, which means, vertex 0 and vertex 1 (in the above Offsets table)
// define an edge.
const int EdgeList[12][2] = { {0,1}, {1,3}, {0,2}, {2,3}, {4,5}, {5,7}, {6,7}, {4,6}, {0,4}, {1,5}, {3,7}, {2,6} };


class Chunk : public Array3D<unsigned char>
{

public:
    Chunk(unsigned int size) : Array3D<unsigned char>(size), Vertices(size)
    {
        for(int i=0;i<(size*size*size);i++) Vertices.data[i] = 0;

        Size = size;
    }

    // checks a particular cell to determine if a mesh vertex needs to be created
    // in there. A vertex will need to be created if any of the edges that
    // define the cube have differeing voxel solid values (ie, one inside and one is outside)
    int surfaceVertexExists( const ivec3 & p)
    {
        Chunk & C = *this;

        bool inside = false;

        short EdgeFlags = 0;
        // loop through all the edges
        for(int i=0;i<12;i++)
        {
            const unsigned int e1 = EdgeList[i][0];
            const unsigned int e2 = EdgeList[i][1];

            // will be 1 if items are different
            short flag = ( C( p + Offsets[e1]) < 128) ^ (C( p + Offsets[e2]) < 128 ) ? 1 : 0;
            //std::cout << flag << std::endl;
            EdgeFlags |= flag<<i;

            //inside |= ( C( p + Offsets[e1]) < (signed char)0) ^ (C( p + Offsets[e2]) < (signed char)0);
        }

        //if(EdgeFlags)
        //std::cout << std::bitset<16>(EdgeFlags) << std::endl;
        return EdgeFlags;
    }

    // gets the gradient at the voxel position.
    // uses central finite difference for interior cells
    // and one sided difference for edges.
    vec3 gradient( const ivec3 & position)
    {
        Chunk & C = *this;
        vec3 grad;

        const ivec3 u[3] = { {1,0,0},{0,1,0},{0,0,1} };

        for(int i=0;i<3;i++)
            if( position[i] >0  && position[i] < Size-1)
            {
                grad[i] = 0.5*C(position+u[i])  - 0.5*C(position-u[i]);
            }
            else if( position[0] == 0)
            {
                grad[0] = -0.666*C(position) + 2.0*C(position+u[i]) - 0.5*C(position+2*u[i]);
            }
            else {
                grad[0] =  0.666*C(position) - 2.0*C(position-u[i]) + 0.5*C(position-2*u[i]);
            }

        return grad;
    }

    // Finds the optimal position of the vertex within the cell
    //   First approximation - Take the weighted average position of all the solid voxels
    vec3 FindOptimalPosition( const ivec3 & index)
    {
        Chunk & C = *this;
        vec3 p;
        float sum;

        for(int i=0;i<8;i++)
        {

             float w = (float)C(index+Offsets[i]);
             if(w < 128) continue;

             ivec3 Pi = index+Offsets[i];
             p   += vec3( (float)Pi[0], (float)Pi[1], (float)Pi[2])*w;
             sum += w;
        }
        return p/sum;
    }



    void ExtractSurface()
    {
        // loop through all cells and determine which cells will have a vertex in it
        int vertexCount = 0;
        Vertex.clear();
        Face.clear();
        for(int z=0;z<Size-1;z++)
            for(int y=0;y<Size-1;y++)
                for(int x=0;x<Size-1;x++)
                {
                    int EdgeFlag = surfaceVertexExists( ivec3(x,y,z) );
                    if( EdgeFlag==0 || EdgeFlag==255) continue;

                    // create a vertex at the center of the cell.
                    auto * V = new VertexNormal();

                    // This needs to be replaced with a better vertex positioning method
                    V->v     = FindOptimalPosition( ivec3(x,y,z) );
                    //{ (float)x+0.5, (float)y+0.5, (float)z+0.5};

                    Vertex.push_back(V->v);

                    V->index        = vertexCount++;

                    V->EdgeFlags    = EdgeFlag;
                    Vertices(x,y,z) = V;

                }



        // loop through all edges that have a sign change and construct a quad from the
        // 4 adjacent cells
        // This triple loop needs to be replaced with a way to only traverse the cubes that have
        // vertices in them.

        for(int z=0;z<Size-1;z++)
            for(int y=0;y<Size-1;y++)
                for(int x=0;x<Size-1;x++)
                {
                    VertexNormal * V = Vertices(x,y,z);

                    // if there is no vertex skip this cell
                    if( !V ) continue;

                    short EdgeFlag = V->EdgeFlags;

                    // check edge 11
                    if( EdgeFlag & (1 << 10) && x<Size-2 && z<Size-2)
                    {
                       // if( Vertices(x,y,z) && Vertices(x+1,y,z) && Vertices(x+1,y,z+1) && Vertices(x,y,z+1))
                        {
                            Face.push_back( uvec3(Vertices(x,y,z)->index, Vertices(x+1,y,z)->index, Vertices(x+1,y,z+1)->index) );
                            Face.push_back( uvec3(Vertices(x+1,y,z+1)->index, Vertices(x,y,z+1)->index, Vertices(x,y,z)->index) );
                        }
                        //    std::cout << "Edge 11 good\n";
                        //else
                        //    std::cout << "Edge 11 error\n";

                    }

                    // check edge 6
                    if( EdgeFlag & (1 << 5)  && x<Size-2 && y<Size-2 )
                    {
                        //if( Vertices(x,y,z) && Vertices(x+1,y,z) && Vertices(x+1,y+1,z) && Vertices(x,y+1,z))
                        {
                            Face.push_back( uvec3(Vertices(x,y,z)->index, Vertices(x+1,y,z)->index,     Vertices(x+1,y+1,z)->index) );
                            Face.push_back( uvec3(Vertices(x+1,y+1,z)->index, Vertices(x,y+1,z)->index, Vertices(x,y,z)->index) );
                        }
                        //    std::cout << "Edge 6 good\n";
                       // else
                        //    std::cout << "Edge 6 error\n";
                    }

                    // check edge 7
                    if( EdgeFlag & (1 << 6)  && z<Size-2 && y<Size-2 )
                    {
                        //if( Vertices(x,y,z) && Vertices(x,y,z+1) && Vertices(x,y+1,z+1) && Vertices(x,y,z+1))
                        {
                            Face.push_back( uvec3(Vertices(x,y,z)->index, Vertices(x,y,z+1)->index, Vertices(x,y+1,z+1)->index) );
                            Face.push_back( uvec3(Vertices(x,y+1,z+1)->index, Vertices(x,y+1,z)->index, Vertices(x,y,z)->index) );
                        }
                        //else
                        //    std::cout << "Edge 7 error\n";
                    }
                }
    }

    std::vector< vec3 >  Vertex;
    std::vector< uvec3>  Face;

    Array3D<VertexNormal*> Vertices;
    int Size;
};

//=============================


int main(int argc, char **argv)
{



    int N = 32;
    Chunk Ch(N);
    float W = (float)N;
    for(int z=0; z<N; z++)
        for(int y=0; y<N; y++)
            for(int x=0; x<N; x++)
            {

                if(1)   // Hemisphere
                {
                    vec3 p = vec3((float)16, 0 ,(float)16) - vec3( (float)x, (float)y, (float)z);
                    p *= p;
                    Ch(x,y,z) = (p[0]+p[1]+p[2]) < 16*16 ? 255 : 0;

                }
                else   // perlin noise
                {
                    Ch(x,y,z) = 128 + (unsigned char)(128.0 * stb_perlin_noise3( (float)(x/W) * 2.0, (float)(y/W)* 2.0, (float)(z/W) * 2.0) );
                }
                //Ch(x,y,z) = z < 10 ?  255 : 0;
                //if ((abs(x-16) < 3) && (abs(y-16)<3) ) Ch(x,y,z) = 255;
                //Ch(i,j,k) = i*i+j*j+k*k < 24*24 ? 0 : 255;

                //std::cout << (int)Ch(i,j,k) << std::endl;
            }
   //Ch(1,1,1) = 127;
    Ch.ExtractSurface();


    auto mSDLWindowHandle = setupSDL();

    //=============================================================================
    // Set up rgui and create the widgets
    //=============================================================================
    R = rgui::Root::getInstance();
    R->init();

    I = R->createInterface(WIDTH, HEIGHT, "MainInterface");
    I->loadSkin("default_skin.zip");


//================================================================================
std::string  json = R"raw(
{
    W1 : {
        widget_type : "window",
        layout_style : "vertical_layout",
        pos : [0,0],
        size: [300,300],

        pitch : {
            widget_type : "slider"
        },

        roll : {
            widget_type : "slider"
        },

        yaw : {
            widget_type : "slider"
        }
    }
}

)raw";


//================================================================================
    Texture Tex;
    Tex.loadFromPath("marble.jpg");
    Tex.sendToGPU();
    I->getRootWidget()->loadFromJSONString(json);
    //=============================================================================


    //=============================================================================
    //
    //=============================================================================
    TriMesh_PNCU M;
    Line_PC      Axis;

    Axis.insertVertexAttribute<0>( vec3(0.0, 0.0, 0.0) );     Axis.insertVertexAttribute<1>( col4(1.0, 0.0, 0.0, 1.0) );
    Axis.insertVertexAttribute<0>( vec3(1.0, 0.0, 0.0) );     Axis.insertVertexAttribute<1>( col4(1.0, 0.0, 0.0, 1.0) );

    Axis.insertVertexAttribute<0>( vec3(0.0, 0.0, 0.0) );     Axis.insertVertexAttribute<1>( col4(0.0, 1.0, 0.0, 1.0) );
    Axis.insertVertexAttribute<0>( vec3(0.0, 1.0, 0.0) );     Axis.insertVertexAttribute<1>( col4(0.0, 1.0, 0.0, 1.0) );

    Axis.insertVertexAttribute<0>( vec3(0.0, 0.0, 0.0) );     Axis.insertVertexAttribute<1>( col4(0.0, 0.0, 1.0, 1.0) );
    Axis.insertVertexAttribute<0>( vec3(0.0, 0.0, 1.0) );     Axis.insertVertexAttribute<1>( col4(0.0, 0.0, 1.0, 1.0) );

    Axis.insertElement( uvec2(0, 1) );
    Axis.insertElement( uvec2(2, 3) );
    Axis.insertElement( uvec2(4, 5) );

    ShaderProgram LineShader( VertexShader("shaders/Line_PC.v"), FragmentShader("shaders/Line_PC.f") );
    ShaderProgram S( VertexShader("shaders/Basic_PNCU.v"), FragmentShader("shaders/Basic_PNCU.f"));


    Axis.sendToGPU();

    for(auto a : Ch.Vertex)
    {
        M.insertVertexAttribute<0>( a-vec3(16,16,16)  );
        M.insertVertexAttribute<1>( vec3(0.0,1.0,0.0) );
        M.insertVertexAttribute<2>( col4(1.0,0.0,0.0,1.0) );
        M.insertVertexAttribute<3>( vec2(0,0)         );
    }

    //std::cout << "Number of vertices: " << M.getPosBuffer().size() << std::endl;

    for(auto a : Ch.Face)
    {
        M.insertElement( a );
    }


    M.sendToGPU();



    mat4 Pv = glm::perspective(120.f, (float)WIDTH/(float)HEIGHT, 0.1f,1000.0f);
    Transformation Tr, Cr;
    vec3 mSpeed;

    Camera Cam;


    SDLEvents["Camera"] = [&] (const SDL_Event & E) {
        switch(E.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                if( E.button.button == SDL_BUTTON_RIGHT) SDL_ShowCursor(0);
                break;
            case SDL_MOUSEBUTTONUP:
                SDL_SetRelativeMouseMode( SDL_FALSE  );
                if( E.button.button == SDL_BUTTON_RIGHT) SDL_ShowCursor(1);
                break;
            case SDL_KEYUP:

            if( E.key.keysym.sym == SDLK_s) mSpeed[2] =  0.00;
            if( E.key.keysym.sym == SDLK_w) mSpeed[2] =  0.00;
            if( E.key.keysym.sym == SDLK_d) mSpeed[0] =  0.00;
            if( E.key.keysym.sym == SDLK_a) mSpeed[0] =  0.00;

                break;
            case SDL_KEYDOWN:
                if( E.key.keysym.sym == SDLK_s) mSpeed[2] = -1.000;
                if( E.key.keysym.sym == SDLK_w) mSpeed[2] =  1.000;
                if( E.key.keysym.sym == SDLK_d) mSpeed[0] = -1.000;
                if( E.key.keysym.sym == SDLK_a) mSpeed[0] =  1.000;

                break;
            case SDL_MOUSEMOTION:
                if( E.motion.state & SDL_BUTTON_RMASK )
                {
                    SDL_SetRelativeMouseMode( SDL_TRUE  );
                    Cam.rotate(   vec3( -(float)E.motion.yrel*0.001,  (float)E.motion.xrel*0.001, 0.0 ) );
                }
            default:

            break;
        }


    };


    GLuint camMatrixId   = S.getUniformLocation("inCameraMatrix");
    GLuint modelMatrixID = S.getUniformLocation("inModelMatrix");

    std::cout << "Cam id:" << camMatrixId << std::endl;
    std::cout << "Mod id:" << modelMatrixID << std::endl;
            //glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    Cr.setPosition( vec3(-0.5 , -0.5, -1.5 ) );
    Cam.setPosition( vec3( 3 , 1.0, 3) );
    //Cam.setOrientation( quat(  0.5*0.5*sqrt(2.0), 0.0, -sqrt(2.0)*0.5*0.5,  0.0) );

    while( !mQuit )
    {
        // Do all the input handling for the interface
        handleInputs();




        // If you were doing your own 3d application, this is where you would draw your
        // own 3d rendering calls.
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        //=======================================
        // draw stuff here.
        //=======================================
        S.useShader();

        Cam.moveTowardOrientation( mSpeed * 0.01f);
        // quat tr = Cr.getOrientation();
        // Cr.translate( quat(tr.w, -tr.x, -tr.y, -tr.z) * mSpeed * 0.01f );

        auto CameraMatrix = Cam.getMatrix();

        S.sendUniform_mat4(camMatrixId, Pv * CameraMatrix  );
        S.sendUniform_mat4(modelMatrixID, mat4(1.0));
        M.Render();

        LineShader.useShader();
        LineShader.sendUniform_mat4(LineShader.getUniformLocation("inCameraMatrix"), Pv * CameraMatrix  );
        LineShader.sendUniform_mat4(LineShader.getUniformLocation("inModelMatrix") , glm::scale( mat4(1.0), vec3(5.0,5.0,5.0)) );
        Axis.Render(LINES);

        //=======================================
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        I->draw();
        // Draw the interface.
        SDL_GL_SwapWindow (mSDLWindowHandle);
    }



    SDL_Quit();


    return 0;
}


































































// Creates a SDL window and openGL rendering context so we can do some rendring.
// Your application can use other rendering contexts. I prefer SDL.
SDL_Window* setupSDL()
{
    //====================================================================
    //Use OpenGL 3.2 core
    //
    // Set up the basic openGL and SDL windows so we can do openGL calls
    //====================================================================
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }


    auto mSDLWindowHandle = SDL_CreateWindow( APPLICATION_TITLE,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              WIDTH, HEIGHT,
                                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN  );

    SDL_GLContext mSDLContext = SDL_GL_CreateContext( mSDLWindowHandle );

    if( mSDLContext == NULL )
    {
        printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }

    /*
      * Initialize GLEW
      */
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
    }
    printf("Glew initialized\n");
    //=============================================================================

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_StartTextInput();
    return mSDLWindowHandle;
}


