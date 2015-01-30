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


#include <bitset>
using namespace glre;

#define WIDTH  1024
#define HEIGHT 768

#define APPLICATION_TITLE "This is a test"

// Creates a SDL window and openGL rendering context so we can do some rendring
SDL_Window* setupSDL();


std::map<std::string, std::function<void(const SDL_Event&)> >  SDLEvents;


//=============================


int main(int argc, char **argv)
{
    auto mSDLWindowHandle = setupSDL();




    glre::Camera Cam;
    glre::vec3   gCameraAcceleration;
    bool         mQuit = false;



//================================================================================
    Texture Tex;
    Tex.loadFromPath("marble.jpg");
    Tex.sendToGPU();



    TriMesh_PNCU Dragon = glre::loadModel("dragon.blend");
    Dragon.sendToGPU();
    //=============================================================================


    Line_PC      Axis = glre::createAxes();




    ShaderProgram LineShader( VertexShader("shaders/Line_PC.v"), FragmentShader("shaders/Line_PC.f") );
    ShaderProgram S( VertexShader("shaders/Basic_PNCU.v"), FragmentShader("shaders/Basic_PNCU.f"));


    Axis.sendToGPU();



    mat4 Pv = glm::perspective(120.f, (float)WIDTH/(float)HEIGHT, 0.1f,1000.0f);
    Transformation Tr, Cr;
    vec3 mSpeed;



    //================================================================================================================
    // Camera Movements
    //================================================================================================================
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

            if( E.key.keysym.sym == SDLK_s) gCameraAcceleration[2] =  0.00;
            if( E.key.keysym.sym == SDLK_w) gCameraAcceleration[2] =  0.00;
            if( E.key.keysym.sym == SDLK_d) gCameraAcceleration[0] =  0.00;
            if( E.key.keysym.sym == SDLK_a) gCameraAcceleration[0] =  0.00;

                break;
            case SDL_KEYDOWN:
                if( E.key.keysym.sym == SDLK_s) gCameraAcceleration[2] = -1.000;
                if( E.key.keysym.sym == SDLK_w) gCameraAcceleration[2] =  1.000;
                if( E.key.keysym.sym == SDLK_d) gCameraAcceleration[0] = -1.000;
                if( E.key.keysym.sym == SDLK_a) gCameraAcceleration[0] =  1.000;

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
    //================================================================================================================

    //================================================================================================================
    // Quit
    //================================================================================================================
    SDLEvents["Exit"] = [&] (const SDL_Event & E) {
        switch(E.type)
        {
            case SDL_KEYUP:
                if( E.key.keysym.sym == SDLK_ESCAPE) mQuit=true;
                break;
            default:

            break;
        }
    };
    //================================================================================================================

    GLuint camMatrixId   = S.getUniformLocation("inCameraMatrix");
    GLuint modelMatrixID = S.getUniformLocation("inModelMatrix");


    while( !mQuit )
    {
        // Do all the input handling for the interface
        SDL_Event e;
        while( SDL_PollEvent( &e ) != 0 ) for(auto a : SDLEvents) a.second( e);




        // If you were doing your own 3d application, this is where you would draw your
        // own 3d rendering calls.
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        //=======================================
        // draw stuff here.
        //=======================================
        S.useShader();

        //===================================================
        // Move the camera
        //===================================================
        quat q = Cam.getOrientation();
        q.x *= -1.0; q.y *= -1.0; q.z *= -1.0;

        Cam.translate(  q * gCameraAcceleration * (-0.01f));
        //mSpeed += q*gCameraAcceleration*t;

        //===================================================


        auto CameraMatrix = Cam.getMatrix();

        S.sendUniform_mat4(camMatrixId, Pv * CameraMatrix  );
        S.sendUniform_mat4(modelMatrixID, mat4(1.0));

        //std::cout << "rendering dragon\n";
        Dragon.Render();

        // LineShader.useShader();
        // LineShader.sendUniform_mat4(LineShader.getUniformLocation("inCameraMatrix"), Pv * CameraMatrix  );
        // LineShader.sendUniform_mat4(LineShader.getUniformLocation("inModelMatrix") , glm::scale( mat4(1.0), vec3(5.0,5.0,5.0)) );
        // Axis.Render(LINES);

        //=======================================
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


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


