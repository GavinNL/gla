#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/global.h>
#include <gla/camera.h>
#include <gla/texture.h>
#include <gla/timer.h>
#include <gla/shader.h>
//#include <gla/gputexturearray.h>
#include <gla/vertexarrayobject.h>
//#include <gla/framebufferobject.h>
//#include <gla/solids.h>
//#include <gla/uniformbuffer.h>
//#include <gla/arraybuffer.h>
#include <locale>

using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

int main()
{
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    //===========================================================================
    // GLA code.
    //===========================================================================

    InterleavedVAO<vec3,vec2> cpuVAO;

    cpuVAO.InsertVertex( vec3(-1.0f, -1.0f, 0.f), vec2( 0.f, 0.f ));
    cpuVAO.InsertVertex( vec3( 1.0f ,-1.0f, 0.f), vec2( 1.f, 0.f ));
    cpuVAO.InsertVertex( vec3( 1.0f , 1.0f, 0.f), vec2( 1.f, 1.f ));
    cpuVAO.InsertVertex( vec3(-1.0f , 1.0f, 0.f), vec2( 0.f, 1.f ));

    cpuVAO.InsertElement( uvec4(0,1,2,3) );


    auto VAO = cpuVAO.ToGPU();


    // Load some textures
    Texture cpuTex1("resources/rocks1024.jpg", 3 );
    Texture cpuTex2("resources/rocks1024.jpg", 3 );

    // Do some manipulation on them using their individual channels;
    cpuTex1.g = cpuTex1.r*cpuTex1.b;


    // resize the texture
    cpuTex2.resize( {512,512} );


    //---------------------------------------------------------------------------
    // Do some texture manipulation for fun.
    //---------------------------------------------------------------------------
    // Set the values of the blue channel based on the position of the
    // pixel values.  the input arguments const vec2 & x ranges from 0..1
    // and the lambda must return a float between 0 and 1
    cpuTex1.b = [] (float x, float y) { return (float)(0.5f * glm::perlin( glm::vec2(x,y)*4.0f ) + 0.5);  };
    cpuTex1.r = [] (float x, float y) { return (float)(0.5f * glm::perlin( glm::vec2(x,y)*8.0f ) + 0.5);  };


    // Send Texture 1 to the GPU.
    //GPUTexture gpuTex = cpuTex1.toGPU();
    GPUTexture gpuTex = cpuTex1.ToGPU();


    // Paste Tex2 into Tex1 with an offset
    //gpuTex.pasteSubImage( {25,25}, cpuTex2);
    gpuTex.PasteSubImage( {25,25}, cpuTex2);


    // we dont need the cpu texture anymore, so we can clear it.
    cpuTex1.clear();
    cpuTex2.clear();



    //---------------------------------------------------------------------------
    // Create a shader
    //---------------------------------------------------------------------------

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram TriangleShader;
    TriangleShader.AttachShaders( VertexShader("shaders/Textures.v"),  FragmentShader("shaders/Textures.f") );
    //TriangleShader.linkProgram(  VertexShader("shaders/Textures.v"),  FragmentShader("shaders/Textures.f")  );

    // get the uniform location ID
    int SamplerLocation = TriangleShader.GetUniformLocation("uSampler");
    //==========================================================

    while (!glfwWindowShouldClose(gMainWindow) )
    {

        // Set the GPu as the current texture 0;
        gpuTex.SetActive( 0);

        //VAO.bind();

        // Here we send Uniform data to the shader
        //  The first argument is an user defined index. Depending on the number of uniforms you have, you should always
        //  start at 0 and then increase sequentially.
        //  The second parameter is the name of the  uniform in the shader
        //  And the third parameter is the value we want to send. In our case we want to send 0, because we set our texture to be in texture unit 0.
        //
        //  sendUniform will only query the shader the first time and then store the shader uniform location in an array at index 0 (the first parameter)
        //  the next time we call sendUniform(0, "uSampler", X), it will use the cached value.
        //TriangleShader.sendUniform(0, "uSampler", 0);
        TriangleShader.UniformData("uSampler"_h, 0);

        VAO.Render();


        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }

    // Clear the VAO
    // Since we had flagged the array buffers for deletion ,they will now be
    // cleared as well since they are no longer bound to any VAOs
    VAO.Release();
    std::cout << "Released:" << std::endl;

    glfwDestroyWindow(gMainWindow);
    glfwTerminate();
    return 0;
}



//=============================================================================
// Set up GLFW and GLEW
//=============================================================================
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow()
{
    glewExperimental = GL_TRUE;
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
    GLenum err = glewInit();

    return(gMainWindow);

}
//=============================================================================
