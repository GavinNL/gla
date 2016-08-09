#include <stdio.h>

#include <gla/timer.h>
#include <gla/exper/sampler2darray.h>

#include <gla/exper/sampler2d.h>

#include <gla/exper/buffers.h>

#include <gla/shader.h>
#include <GLFW/glfw3.h>



using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Sampler 2D Array"
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

struct Uniform140
{
    vec4 x;
};

mat4 X;

int main()
{
    //GLA_DOUT  << &X << "   " << &X[0][0] << "   " << &X[0] << std::endl;
    //return 0;
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    { // create a scope around the main GL calls so that glfwTerminate is not called before
      // the gla objects are automatically destroyed.

        //===========================================================================
        // GLA code.
        //===========================================================================

        //---------------------------------------------------------------------------
        // Create two buffers on the CPU to hold position and colour information
        //---------------------------------------------------------------------------
        // Also create an index buffer.
        struct MyVertex
        {
            glm::vec3 p;
            glm::vec2 uv;
        };

        std::vector< MyVertex > VertexBuffer;


        VertexBuffer.push_back( { vec3(-1.0f, -1.0f, 0.f), vec2( 0.f, 0.f )} );
        VertexBuffer.push_back( { vec3( 1.0f ,-1.0f, 0.f), vec2( 1.f, 0.f )} );
        VertexBuffer.push_back( { vec3( 1.0f , 1.0f, 0.f), vec2( 1.f, 1.f )} );
        VertexBuffer.push_back( { vec3(-1.0f , 1.0f, 0.f), vec2( 0.f, 1.f )} );

        // Load teh buffer into the GPU
        gla::experimental::Array_Buffer buff( VertexBuffer );


        gla::experimental::VertexArray VAO;
        VAO.Attach<glm::vec3, glm::vec2>( buff );

        // Load some textures. And force using 3 components (r,g,b)
        gla::experimental::Image Tex1("../resources/textures/rocks.jpg",  3 );
        gla::experimental::Image Tex2("../resources/textures/rocks1024.jpg", 3 );
        gla::experimental::Image Tex3(256,256, 3);


        buff.Release();


        //=========================================================================================
        // Create the Texture array on the GPU.
        //   Note: Any objects that start with GPU mean they are initialized on the GPU
        //=========================================================================================
        gla::experimental::Sampler2DArray TexArray2D;

        // Create the GPUTexture array with 3 layers that hold 256x256 images with 3 components each, and 2 mipmaps.
        TexArray2D.Create( uvec2(256,256), 3, 3 , 2);

        // Set the red channel using a lambda function
        Tex3.r =  IMAGE_EXPRESSION( glm::perlin( vec2(x,y) * 4.0f, vec2(4.0,4.0) )*0.5 + 0.5f )

                // Resize the textures so they match the TextureArray. This will throw an exception if
                // the sizes do not match.
                Tex1.resize( uvec2(256,256) );
        Tex2.resize( uvec2(256,256) );


        // Copy the textures into the appropriate layer. This will throw an exception if
        // the number of colour channels in the TextureArray does not match what's on the Texture
        //   Note: If the Image does not match the Sampler2D dimensions, it will be rescaled
        //         to the appropriate dimensions and channels. If that is not desired
        //          use TexArray2D.SetLayer(Tex1, 0, uvec2(0,0);
        TexArray2D[0] = Tex1;
        TexArray2D[1] = Tex2;
        TexArray2D[2] = Tex3;

        // Alternatively, one can use the following
        //   TexArray2D.SetLayer( Tex1, 0, uvec2(0 ,0) );
        //   TexArray2D.SetLayer( Tex2, 0, uvec2(0 ,0) );
        //   TexArray2D.SetLayer( Tex3, 0, uvec2(0 ,0) );

        //===============================================================


        // we dont need the cpu texture anymore, so we can clear it.
        Tex1.clear();
        Tex2.clear();
        Tex3.clear();



        //---------------------------------------------------------------------------
        // Create a shader
        //---------------------------------------------------------------------------

        // Create the two shaders. The second argument is set to true because we are
        // compiling the shaders straight from a string. If we were compiling from a file
        // we'd just do:  VertexShader vs(Path_to_file);
        ShaderProgram TextureArrayShader;
        TextureArrayShader.AttachShaders(  VertexShader("../resources/shaders/TextureArray.v"),  FragmentShader("../resources/shaders/TextureArray.f")  );

        //==========================================================

        Timer_T<float> Timer;

        std::cout << "Starting main loop" << std::endl;

        auto uTextureArrayID = TextureArrayShader.GetUniformLocation("uTextureArray");
        auto uSpeedID        = TextureArrayShader.GetUniformLocation("uSpeed");

        while (!glfwWindowShouldClose(gMainWindow) )
        {
            TextureArrayShader.Bind();

            // Set the GPu as the current texture 0;
            TexArray2D.SetActive(0);

            vec2 Speed = Timer.getElapsedTime() * vec2(0.15,0.4);


            // Send the Uniforms to the shader.
            TextureArrayShader.Uniform( uTextureArrayID, 0);      //which texture unit our Sampler2D array is bound to
            TextureArrayShader.Uniform( uSpeedID ,        Speed); //a constant variable that we are using in the shader


            VAO.Draw(gla::experimental::Primitave::TRIANGLE_FAN, 4);


            glfwSwapBuffers(gMainWindow);
            glfwPollEvents();
        }
    }

    // Clear the VAO
    // Since we had flagged the array buffers for deletion ,they will now be
    // cleared as well since they are no longer bound to any VAOs
    //VAO.clear();


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

    auto gMainWindow = glfwCreateWindow(640, 480, WINDOW_TITLE, NULL, NULL);

    if (!gMainWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(gMainWindow);

    int width, height;
    glfwGetFramebufferSize(gMainWindow, &width, &height);
    //    GLenum err = glewInit();

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return NULL;
    }


    return(gMainWindow);

}
//=============================================================================
