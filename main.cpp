#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/gla.h>
#include <gla/utils/sdlapp.h>
#include <locale>
#include <tuple>
#include <gla/types.h>
#include <gla/handle.h>

using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
//=================================================================================

struct MyApp : public SDL_App
{

    bool quit=false;

    MyApp() : SDL_App(640,480, "My Test App"){}

    virtual void OnKey(SDL_KeyboardEvent &e)
    {
        std::cout << "Key: " << e.keysym.sym << std::endl;
        std::cout << "scan: " << e.keysym.scancode<< std::endl;
        switch(e.keysym.sym)
        {
            case SDLK_ESCAPE:
                quit = true;
            break;
        }

    }

    void Init()
    {
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


            using MyVertex       = std::tuple<vec3, vec4>;
            using MyVertexBuffer = gla::ArrayBuffer< MyVertex >;
            MyVertexBuffer Buf;


            Buf.Insert(  std::make_tuple(vec3(-1.0f, -1.0f, 0.f), vec4(1.f, 0.f, 0.f, 1.0f)  )  );
            Buf.Insert(  std::make_tuple(vec3( 1.0f ,-1.0f, 0.f), vec4(0.f, 1.f, 0.f, 1.0f)  )  );
            Buf.Insert(  std::make_tuple(vec3( 0.0f , 1.0f, 0.f), vec4(0.f, 0.f, 1.f, 1.0f)  )  );

            //auto G = Buf.toGPU(ARRAY_BUFFER);
            auto G = Buf.ToGPU();

            //---------------------------------------------------------------------------
            // Create a shader
            //---------------------------------------------------------------------------

            // Create the two shaders. The second argument is set to true because we are
            // compiling the shaders straight from a string. If we were compiling from a file
            // we'd just do:  VertexShader vs(Path_to_file);
            ShaderProgram TriangleShader;
            TriangleShader.AttachShaders(  VertexShader("shaders/HelloTriangle.v"),  FragmentShader("shaders/HelloTriangle.f")  );


            gla::EnableVertexAttribute<vec3, vec4>(G);
            //==========================================================

        while(!quit)
        {
            // Set the triangle shader to be the one that we will use
            TriangleShader.Bind();

            G.Render(Primitave::TRIANGLES, 0, 3);

            PollEvents();
            SwapBuffers();
        }

    }
};





int main()
{

    MyApp S;
    S.Init();

    return 0;
}


