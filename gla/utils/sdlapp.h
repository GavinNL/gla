#ifndef SDLAPP_H
#define SDLAPP_H

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#define APPLICATION_TITLE "XML Loading"
#define WIDTH  800
#define HEIGHT 600


bool mQuit = false;

// Creates a SDL window and openGL rendering context so we can do some rendring

struct SDL_App
{
    SDL_App() : SDL_App(640,480,"No app name"){}
    ~SDL_App();
    SDL_App(unsigned int with, unsigned int height, const char * windowtitle );


    virtual void OnMouseButton(SDL_MouseButtonEvent & e){}
    virtual void OnMousePosition(SDL_MouseMotionEvent & e){}
    virtual void OnKey(SDL_KeyboardEvent & e){}
    virtual void OnText(SDL_TextInputEvent & e ){}
    virtual void OnScroll(SDL_MouseWheelEvent & e){}

    virtual void OnWindowPosition(int x, int y){}
    virtual void OnWindowSize(int width, int height){}
    virtual void OnFramebufferSize(int width, int height){}
    virtual void OnClose(){}
    virtual void OnRefresh(){}
    virtual void OnFocus(int focused){}
    virtual void OnIconify(int iconified){}
    virtual void OnMouseEnter(int entered){}
    virtual void OnCharacter(unsigned int codepoint){}
    virtual void OnCharacterMods(unsigned int codepoint, int mods){}
    virtual void OnDrop(int count, const char** paths){}

    void SwapBuffers();
    void PollEvents();
    private:
        bool setupSDL(unsigned int w, unsigned int h, const char windowtitle[]);

        SDL_Window   *m_Window;
        SDL_GLContext m_Context;
};


inline SDL_App::SDL_App(unsigned int with, unsigned int height, const char * windowtitle )
{
    setupSDL(with, height, windowtitle);
}

inline SDL_App::~SDL_App()
{
    SDL_GL_DeleteContext(m_Context);

    // Destroy our window
    SDL_DestroyWindow(m_Window);

    // Shutdown SDL 2
    SDL_Quit();
}


inline void SDL_App::SwapBuffers()
{
    SDL_GL_SwapWindow (m_Window);
}

// Creates a SDL window and openGL rendering context so we can do some rendring
inline bool SDL_App::setupSDL(unsigned int w, unsigned int h, const char windowtitle[])
{
    //====================================================================
    //Use OpenGL 3.2 core
    //
    // Set up the basic openGL and SDL windows so we can do openGL calls
    //====================================================================
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Initialize SDL
    static bool isInit=false;
    if(!isInit)
    {
        if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
        {
            printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
            return true;
        }
        isInit = true;
    }


    m_Window = SDL_CreateWindow( windowtitle,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              w, h,
                                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN  );

    m_Context = SDL_GL_CreateContext( m_Window );

    if( m_Context == NULL )
    {
        printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
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
    glewError = glewInit();

    if( glewError != GLEW_OK )
    {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
    }
    //printf("Glew initialized\n");
    //=============================================================================


    SDL_StartTextInput();
    return true;
}



// Handles the keyboard and mouse inputs for the GUI Interface.
inline void SDL_App::PollEvents()
{
    SDL_Event e;

    while( SDL_PollEvent( &e ) != 0 )
    {
        switch(e.type)
        {
        // Text input for when we are typing into text boxes. This is different from Keypresses
        case SDL_TEXTINPUT:
            this->OnText( e.text );
            //MainInterface->injectCharacters( std::string(e.text.text) );
            break;
            // Key press and release. Keycode is the same as the SDL scancode, so you can do a direct static_cast from one to the other
            // If you are not using SDL, then you will have to manually convert the keycodes
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            this->OnKey( e.key );
            break;
        case SDL_MOUSEWHEEL:
            this->OnScroll( e.wheel );
            // Mouse motion and button presses.
        case SDL_MOUSEMOTION:
            this->OnMousePosition( e.motion );
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            this->OnMouseButton( e.button );
            break;
        }
    }
}





























































#endif // SDLAPP_H

