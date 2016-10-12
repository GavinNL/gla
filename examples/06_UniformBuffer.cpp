//#include <GL/glew.h>    // include GLEW and new version of GL on Windows

#include <stdio.h>

#include "glad.h"

#include <gla/gla.h>
//#include <gla/timer.h>
//#include <gla/exper/sampler2darray.h>
//#include <gla/exper/vertexarray.h>
//#include <gla/exper/uniformbuffer.h>
//#include <gla/exper/shader.h>


#include <glm/gtc/noise.hpp>

#include <GLFW/glfw3.h> // GLFW helper library

#include <locale>

//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE  "Uniform Buffers"
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

using namespace gla::core;

int main()
{
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
            vec3 p;
            vec2 uv;
        };

        std::vector< MyVertex > VertexBuffer;

        VertexBuffer.push_back( { vec3(-1.0f, -1.0f, 0.f), vec2( 0.f, 0.f )} );
        VertexBuffer.push_back( { vec3( 1.0f ,-1.0f, 0.f), vec2( 1.f, 0.f )} );
        VertexBuffer.push_back( { vec3( 1.0f , 1.0f, 0.f), vec2( 1.f, 1.f )} );
        VertexBuffer.push_back( { vec3(-1.0f , 1.0f, 0.f), vec2( 0.f, 1.f )} );

        // Load teh buffer into the GPU
        ArrayBuffer buff( VertexBuffer );


        VertexArray VAO;
        VAO.Attach<vec3, vec2>( buff );

        // Load some textures. And force using 3 components (r,g,b)
        Image Tex1("./resources/textures/rocks.jpg",  3 );
        Image Tex2("./resources/textures/rocks1024.jpg", 3 );
        Image Tex3(256,256, 3);


        buff.Release();


        //=========================================================================================
        // Create the Texture array on the GPU.
        //   Note: Any objects that start with GPU mean they are initialized on the GPU
        //=========================================================================================
        Sampler2DArray TexArray2D;

        // Create the GPUTexture array with 3 layers that hold 256x256 images with 3 components each, and 2 mipmaps.
        TexArray2D.Create( uvec2(256,256), 3, 3 , 2);

        // Set the red channel using a lambda function
        Tex3.r =  IMAGE_EXPRESSION( perlin( vec2(x,y) * 4.0f, vec2(4.0,4.0) )*0.5 + 0.5f )

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
        ShaderProgram UniformBufferShader;
        UniformBufferShader.AttachShaders(  VertexShader("./resources/shaders/UniformBuffer.v"),  FragmentShader("./resources/shaders/UniformBuffer.f")  );

        //==========================================================


        Timer_T<float> Timer;


        // This is the structure that we will create a buffer for
        // Note: the alignment of structs and the alignment of uniform blocks in the shader
        // are NOT the same. vec3 are are aligned to vec4, so you will need to pad
        // the data with an extra by te
        struct UniformBufferStruct140 {
            vec3          Dir_Speed; // x,y = direction, z = speed
            float         padding1;  // we need this padding because vec3 in the shader is aligned to vec4, that is the amount of space
            vec4          colour;
        };

        UniformBufferStruct140 UniformData; // allocate the memory on the CPU

        // Here we create a uniform buffer on the gpu, we pass the size of the struct
        // into the input parameter.
        //  GPUUniformBuffer MyGPUUniformBuffer(sizeof(UniformBufferStruct140));

        UniformBuffer uniform_buffer;

        uniform_buffer.Reserve( sizeof(UniformBufferStruct140) );

        // We first need to get the block index of the "Uniform140" block in the shader.
        auto BlockIndex = UniformBufferShader.GetUniformBlockIndex("Uniform140");


        GLA_DOUT  << "Block Index                     : " << BlockIndex << std::endl;
        GLA_DOUT  << "Max Buffer Bind Points          : " << UniformBuffer::Get_MaxUniformBufferBindings() << std::endl;
        GLA_DOUT  << "Max Buffer block size           : " << UniformBuffer::Get_MaxUniformBlockSize() << std::endl;
        GLA_DOUT  << "Max Combined Uniform block size : " << UniformBuffer::Get_MaxCombinedUniformBlocks() << std::endl;

        GLA_DOUT  << "Uniform Block  Size: " << UniformBufferShader.GetUniformBlockSize("Uniform140") << std::endl;
        GLA_DOUT  << "Uniform Struct Size: " << sizeof(UniformBufferStruct140) << std::endl;

        // Bind the UniformBuffer to the UniformBlock in the shader.
        // Both must be bound to the same location.
        uniform_buffer.SetBindPoint(1);
        UniformBufferShader.BindUniformBlock(BlockIndex, 1);
        //--------------------------------------------------------------------

        while (!glfwWindowShouldClose(gMainWindow) )
        {

            // Set the GPU as the current texture 0;
            int TextureIndex = 0;
            TexArray2D.SetActive(TextureIndex);

            auto t = Timer.getElapsedTime();

            UniformData.Dir_Speed = vec3{ cos(t), sin(t), cos(t)*sin(t) };
            UniformData.colour    = vec4{ cos(t)*cos(t), 0,0,1};

            // Copy the data over
            uniform_buffer( UniformData );

            // Here we send Uniform data to the shader
            //  The first argument is an user defined index. Depending on the number of uniforms you have, you should always
            //  start at 0 and then increase sequentially.
            //  The second parameter is the name of the  uniform in the shader
            //  And the third parameter is the value we want to send. In our case we want to send 0, because we set our texture to be in texture unit 0.
            //
            //  sendUniform will only query the shader the first time and then store the shader uniform location in an array at index 0 (the first parameter)
            //  the next time we call sendUniform(0, "uSampler", X), it will use the cached value.
            //UniformBufferShader.sendUniform(0, "uTextureArray", TextureIndex);
            UniformBufferShader.Uniform( UniformBufferShader.GetUniformLocation("uTextureArray"), TextureIndex);

            VAO.Draw(Primitave::TRIANGLE_FAN, 4);


            glfwSwapBuffers(gMainWindow);
            glfwPollEvents();
        }

        // Clear the VAO
        // Since we had flagged the array buffers for deletion ,they will now be
        // cleared as well since they are no longer bound to any VAOs
        VAO.Release();

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
