#include <stdio.h>

#include <gla/timer.h>
#include <gla/exper/array_buffer.h>
#include <gla/exper/sampler2darray.h>
#include <gla/exper/array_buffer.h>
#include <gla/exper/sampler2d.h>
#include <gla/shader.h>
#include <GLFW/glfw3.h>
#include <gla/exper/sampler2darray.h>



using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
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

    std::vector< MyVertex > CpuBuffer;


    CpuBuffer.push_back( { vec3(-1.0f, -1.0f, 0.f), vec2( 0.f, 0.f )} );
    CpuBuffer.push_back( { vec3( 1.0f ,-1.0f, 0.f), vec2( 1.f, 0.f )} );
    CpuBuffer.push_back( { vec3( 1.0f , 1.0f, 0.f), vec2( 1.f, 1.f )} );
    CpuBuffer.push_back( { vec3(-1.0f , 1.0f, 0.f), vec2( 0.f, 1.f )} );

    // Load teh buffer into the GPU
    gla::experimental::Array_Buffer buff( CpuBuffer );


    // Load some textures. And force using 3 components (r,g,b)
    gla::experimental::Image cpuTex1("../resources/textures/moss1024.jpg", 3 );
    gla::experimental::Image cpuTex2("../resources/textures/rocks1024.jpg", 3 );
    gla::experimental::Image cpuTex3(256,256, 3);



    //=========================================================================================
    // Create the Texture array on the GPU.
    //   Note: Any objects that start with GPU mean they are initialized on the GPU
    //=========================================================================================
    gla::experimental::Sampler2DArray TexArray2D;

    // Create the GPUTexture array with 3 layers that hold 256x256 images with 3 components each, and 2 mipmaps.
    TexArray2D.Create( uvec2(256,256), 3, 3 , 2);

    // Set the red channel using a lambda function
    cpuTex3.r = [] (float x, float y) {  return glm::perlin( vec2(x,y) * 2.0f, vec2(2.0,2.0) )*0.5 + 0.5f;  };

    // Resize the textures so they match the TextureArray. This will throw an exception if
    // the sizes do not match.
    cpuTex1.resize( uvec2(256,256) );
    cpuTex2.resize( uvec2(256,256) );


    // Copy the textures into the appropriate layer. This will throw an exception if
    // the number of colour channels in the TextureArray does not match what's on the Texture
    TexArray2D[0] = cpuTex1;
    TexArray2D[1] = cpuTex2;
    TexArray2D[2] = cpuTex3;

    //===============================================================


    // we dont need the cpu texture anymore, so we can clear it.
    cpuTex1.clear();
    cpuTex2.clear();
    cpuTex3.clear();



    //---------------------------------------------------------------------------
    // Create a shader
    //---------------------------------------------------------------------------

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram TextureArrayShader;
    TextureArrayShader.AttachShaders(  VertexShader("../resources/shaders/TextureArray.v"),  FragmentShader("resources/shaders/TextureArray.f")  );
    //TextureArrayShader.linkProgram(  VertexShader("shaders/TextureArray.v"),  FragmentShader("shaders/TextureArray.f")  );

   //==========================================================

    Timer_T<float> Timer;

    std::cout << "Starting main loop" << std::endl;

    auto uTextureArrayID = TextureArrayShader.GetUniformLocation("uTextureArray");
    auto uSpeedID        = TextureArrayShader.GetUniformLocation("uSpeed");

    while (!glfwWindowShouldClose(gMainWindow) )
    {

        // Set the GPu as the current texture 0;
        TexArray2D.SetActive(0);

        vec2 Speed = Timer.getElapsedTime() * vec2(0.7,1.2);


        // Here we send Uniform data to the shader
        //  The first argument is an user defined index. Depending on the number of uniforms you have, you should always
        //  start at 0 and then increase sequentially.
        //  The second parameter is the name of the  uniform in the shader
        //  And the third parameter is the value we want to send. In our case we want to send 0, because we set our texture to be in texture unit 0.
        //
        //  sendUniform will only query the shader the first time and then store the shader uniform location in an array at index 0 (the first parameter)
        //  the next time we call sendUniform(0, "uSampler", X), it will use the cached value.
        //TextureArrayShader.sendUniform(0, "uTextureArray", 0);
        //TextureArrayShader.sendUniform(1, "uSpeed", Speed);
        TextureArrayShader.Uniform( uTextureArrayID, 0);
        TextureArrayShader.Uniform( uSpeedID ,        Speed);

        //VAO.Render();

        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }

    // Clear the VAO
    // Since we had flagged the array buffers for deletion ,they will now be
    // cleared as well since they are no longer bound to any VAOs
   // VAO.clear();


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
