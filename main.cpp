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

// Creates a SDL window and openGL rendering context so we can do some rendring
SDL_Window* setupSDL(uint width, uint height);


rgui::Root      *R;
rgui::pInterface I;
std::map<std::string, std::function<void(const SDL_Event&)> >  SDLEvents;


int main(int argc, char **argv)
{
    //=============================================================================
    // Load a JSON file
    //=============================================================================
    rgui::json::Value JSON;
    JSON.fromFile("resources/ExtraInfo.json");

    //=============================================================================

    auto mSDLWindowHandle = setupSDL( (uint)JSON["main"]["width"].as<float>(), (uint)JSON["main"]["height"].as<float>() );

    glre::Camera Cam;
    glre::vec3   gCameraAcceleration;
    bool         mQuit = false;

    //=============================================================================
    // Set up rgui and create the widgets
    //=============================================================================
    R = rgui::Root::getInstance();
    R->init();


    I = R->createInterface( (uint)JSON["main"]["width"].as<float>(), (uint)JSON["main"]["height"].as<float>(), "MainInterface");

    I->loadSkin( JSON["main"]["skin"].as<std::string>() );


    std::cout << JSON["main"]["GUI"].type() << std::endl;
    for(auto m : JSON["main"]["GUI"].getValueMap() )
    {
        std::cout << m.first << std::endl;
    }

    //auto M = I->getRootWidget()->loadFromJSON( JSON["main"]["GUI"] );

    std::cout << "GUI loaded\n";
    //M["W2"].lock()->setRawRect(0,0,1.0,1.0,5,64.0,64.0);

    //================================================================================




    //================================================================================


    Texture Tex("resources/dragontexture.png", true);


    Line_PC      Axis   = createAxes(true);
    TriMesh_PNCU Dragon = loadModel( JSON["main"]["mode"].as<std::string>() , true);


    ShaderProgram LineShader( VertexShader("shaders/Line_PC.v")   , FragmentShader("shaders/Line_PC.f")   );
    ShaderProgram S(          VertexShader("shaders/Basic_PNCU.v"), FragmentShader("shaders/Basic_PNCU.f"));


    Cam.setPosition( vec3(0.5 ,1.0,0.5) );

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
                {
                    float f = 80.0f;
                    if( E.key.keysym.mod & KMOD_LSHIFT ) f = 1000.0;
                    if( E.key.keysym.sym == SDLK_s) gCameraAcceleration[2] = -f;
                    if( E.key.keysym.sym == SDLK_w) gCameraAcceleration[2] =  f;
                    if( E.key.keysym.sym == SDLK_d) gCameraAcceleration[0] = -f;
                    if( E.key.keysym.sym == SDLK_a) gCameraAcceleration[0] =  f;
                }
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

    //================================================================================================================
    // GUI
    //================================================================================================================
    SDLEvents["GUI"] = [&] (const SDL_Event & E) {
        rgui::SDL_InputHandler::HandleInput(I,E);
    };
//    //================================================================================================================

    GLuint camMatrixId             = S.getUniformLocation("inCameraMatrix");
    GLuint modelMatrixID           = S.getUniformLocation("inModelMatrix");
    GLuint uSampler0ID             = S.getUniformLocation("uSampler");

    GLuint LineShaderCamMatrixId   = LineShader.getUniformLocation("inCameraMatrix");
    GLuint LineShaderModelMatrixID = LineShader.getUniformLocation("inModelMatrix");

    Timer_T<float> tim;
    float dt = 0;

    vec3 mSpeed;

    float AR  = JSON["main"]["width"].as<float>() / JSON["main"]["height"].as<float>();
    float FOV = JSON["main"]["fov"].as<float>();

    Cam.perspective(FOV, AR, 0.2f, 1000.0f);
    mat4 Pv = Cam.getProjectionMatrix();


    while( !mQuit )
    {
        // Do all the input handling for the interface
        SDL_Event e;
        while( SDL_PollEvent( &e ) != 0 ) for(auto a : SDLEvents) a.second( e);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        //===================================================
        // draw stuff here.
        //===================================================
        S.useShader();

        //===================================================
        // Move the camera
        //===================================================
        quat q = Cam.reverse();
        vec3 acc = (q * gCameraAcceleration);//Cam.getDirection(gCameraAcceleration);

        Cam.translate(  (mSpeed + (dt*0.5f)*acc)*dt*80.f  );
        mSpeed += ( acc*dt  - glm::length(mSpeed)*mSpeed );

        //===================================================


        auto CameraMatrix = Cam.getMatrix();
        Tex.setActiveTexture(0);
        S.sendUniform_mat4(camMatrixId  , Pv * CameraMatrix  );
        S.sendUniform_mat4(modelMatrixID, mat4(0.01));
        S.sendUniform_Sampler2D(uSampler0ID, 0);

        Dragon.Render();

        LineShader.useShader();
        LineShader.sendUniform_mat4(LineShaderCamMatrixId,   Pv * CameraMatrix  );
        LineShader.sendUniform_mat4(LineShaderModelMatrixID, glm::scale( mat4(1.0), vec3(5.0,5.0,5.0)) );
        Axis.Render(LINES);

        //=======================================
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        I->draw();
        // Draw the interface.
        SDL_GL_SwapWindow (mSDLWindowHandle);


        dt = tim.getElapsedTime();
        tim.reset();
    }



    SDL_Quit();


    return 0;
}


































































// Creates a SDL window and openGL rendering context so we can do some rendring.
// Your application can use other rendering contexts. I prefer SDL.
SDL_Window* setupSDL(uint width , uint height)
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
                                              width, height,
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


