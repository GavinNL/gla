#include <gla/glad.h>
#include <gla/exper/array_buffer.h>
#include <gla/exper/sampler2d.h>
#include <gla/shader.h>

#include <glm/gtc/noise.hpp>
#include <GLFW/glfw3.h>

//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

#ifdef GLAD_DEBUG
// logs every gl call to the console
void pre_gl_call(const char *name, void *funcptr, int len_args, ...) {
    printf("Calling: %s (%d arguments)\n", name, len_args);
}
#endif


int main()
{
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

#ifdef GLAD_DEBUG
    // before every opengl call call pre_gl_call
    glad_set_pre_callback(pre_gl_call);

    // post callback checks for glGetError by default

    // don't use the callback for glClear
    // (glClear could be replaced with your own function)
    glad_debug_glClear = glad_glClear;
#endif

    { // adding an extra scope here because we want all gla objects automatically destroyed when they go out of scope
      // calling glfwTerminate before destroying the openGL objects will cause a segfault, so putting
      // this scope here will make the gla objects destroy themselves before terminate gets called.

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


        gla::experimental::Image Img;

        Img.loadFromPath("../resources/textures/rocks1024.jpg" );


        #define EXPRESSION( A ) [] (float x, float y) { return A; };

        Img.r = [] (float x, float y) { return (float)(0.5f * glm::perlin( glm::vec2(x,y)*8.0f ) + 0.5);  };
        Img.g = EXPRESSION( 2*x*x + y );

        //Img.a = [] (float x, float y) { return (float)(0.5f * glm::perlin( glm::vec2(x,y)*8.0f ) + 0.5);  };
        gla::experimental::Sampler2D Sampler(Img);


        gla::ShaderProgram TriangleShader;
        TriangleShader.AttachShaders(  gla::VertexShader("../resources/shaders/Textures.v"),  gla::FragmentShader("../resources/shaders/Textures.f")  );




        //====
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec2 uv;
        };

        std::vector< MyVertex > CpuBuffer;

        CpuBuffer.push_back( { glm::vec3(-1.0f, -1.0f, 0.f), glm::vec2(0.f, 1.f)  }  );
        CpuBuffer.push_back( { glm::vec3( 1.0f ,-1.0f, 0.f), glm::vec2(1.f, 1.f)  }  );
        CpuBuffer.push_back( { glm::vec3( 0.0f , 1.0f, 0.f), glm::vec2(.5f, .0f)  }  );

        gla::experimental::Array_Buffer G( CpuBuffer );

        G.Bind();
        G.EnableAttributes<glm::vec3, glm::vec2>( {false,false } );
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);



        Sampler.SetActive(0);
        Sampler.Bind();
        //====

        while (!glfwWindowShouldClose(gMainWindow) )
        {

            Sampler.SetActive( 0 );
            // Set the triangle shader to be the one that we will use
            TriangleShader.Bind();

            TriangleShader.Uniform( TriangleShader.GetUniformLocation("uSampler"), 0 );
            G.Bind();

            // Can use any one of the following to render the triangle, they are all equivelant.
            // as long as both buffers have the same number of items in it. In our case 3.
            //G.Render(Primitave::TRIANGLES, 0, 3);
            glDrawArrays( GL_TRIANGLES, 0,  3 );


            glfwSwapBuffers(gMainWindow);
            glfwPollEvents();
        }
    }


    glfwDestroyWindow(gMainWindow);
    glfwTerminate();

    return 0;
}


//=============================================================================
// Set up GLFW and GLEW
//=============================================================================
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow()
{
    //    glewExperimental = GL_TRUE;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    auto gMainWindow = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

    if (!gMainWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(gMainWindow);

    int width, height;
    glfwGetFramebufferSize(gMainWindow, &width, &height);
    //    GLenum err = glewInit();

    return(gMainWindow);

}
//=============================================================================

