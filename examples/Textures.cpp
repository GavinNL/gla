#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/gla.h>
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

    //---------------------------------------------------------------------------
    // Create two buffers on the CPU to hold position and colour information
    //---------------------------------------------------------------------------
    v3ArrayBuffer cpuPositions;
    v2ArrayBuffer cpuTexCoords;

    // Also create an index buffer.
    u4ArrayBuffer cpuIndex;

    cpuPositions.insert( vec3(-1.0f, -1.0f, 0.f));
    cpuPositions.insert( vec3( 1.0f ,-1.0f, 0.f));
    cpuPositions.insert( vec3( 1.0f , 1.0f, 0.f));
    cpuPositions.insert( vec3(-1.0f , 1.0f, 0.f));

    cpuTexCoords.insert( vec2( 0.f, 0.f ) );
    cpuTexCoords.insert( vec2( 1.f, 0.f ) );
    cpuTexCoords.insert( vec2( 1.f, 1.f ) );
    cpuTexCoords.insert( vec2( 0.f, 1.f ) );

    cpuIndex.insert( uvec4(0,1,2,3) );

    // Copy the CPU buffers to the GPU.
    GPUArrayBuffer gpuPositions = cpuPositions.toGPU(ARRAY_BUFFER);   // same as GL_ARRAY_BUFFER, but placed in a enum
    GPUArrayBuffer gpuTexCoords = cpuTexCoords.toGPU(ARRAY_BUFFER);   // same as GL_ARRAY_BUFFER, but placed in a enum
    GPUArrayBuffer gpuIndex     =     cpuIndex.toGPU(ELEMENT_ARRAY_BUFFER);   // same as GL_ARRAY_BUFFER, but placed in a enum

    // The data in the CPU buffers are no longer needed. We can clear their memory
    cpuPositions.clear();
    cpuTexCoords.clear();
    cpuIndex.clear();

    // Create a VertexArrayObject with Positions as the first attribute, colours as the second attribute
    // Use the gpuIndex as the index buffer
    // and use TRIANGLES as the render method.
    GPUArrayObject VAO(  QUADS,
                        {gpuPositions, gpuTexCoords},
                         gpuIndex);


    /* NOTE: We can also create a VAO without a Index buffer, simple call
     * GPUArrayObject VAO( TRIANGLES, {gpuPositions, gpuColours}); */


    // GPUArrayBuffers are bound to the VAO now, we can clear the buffers, but they wont
    // be removed from GPU memory until we clear the VOA
    gpuPositions.clear();
    cpuTexCoords.clear();
    gpuIndex.clear();

    //---------------------------------------------------------------------------
    // Load a texture
    //---------------------------------------------------------------------------
    TextureC cpuTex ("resources/rocks1024.jpg", 3 );
    TextureC cpuTex2("resources/rocks1024.jpg", 3 );

    cpuTex.g = cpuTex.r*cpuTex.b*cpuTex.b*cpuTex.b*cpuTex.b;
    // resize the texture
    cpuTex2.resize( {512,512} );


    //---------------------------------------------------------------------------
    // Do some texture manipulation for fun.
    //---------------------------------------------------------------------------
    // cpuTex.r = cpuTex.g;   // copy the green channel into the red channel
    // cpuTex.g = 0;          // set the green channel to zero;

    // Set the values of the blue channel based on the position of the
    // pixel values.  the input arguments const vec2 & x ranges from 0..1
    // and the lambda must return a float between 0 and 1
    cpuTex.b = [] (float x, float y) { return (float)(0.5f * glm::perlin( glm::vec2(x,y)*4.0f ) + 0.5);  };
    cpuTex.r = [] (float x, float y) { return (float)(0.5f * glm::perlin( glm::vec2(x,y)*8.0f ) + 0.5);  };


    // Set the green channel to be the difference of the red channel and the blue channel
    //cpuTex.g = cpuTex.r - cpuTex.b;


    //---------------------------------------------------------------------------
    // Finally send the texture to the GPU
    //---------------------------------------------------------------------------
    GPUTexture gpuTex = cpuTex.toGPU();
    gpuTex.pasteSubImage( {25,25}, cpuTex2);
    // we dont need the cpu texture anymore, so we can clear it.
    cpuTex.clear();


    //---------------------------------------------------------------------------
    // Create a shader
    //---------------------------------------------------------------------------

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram TriangleShader;
    TriangleShader.linkProgram(  VertexShader("shaders/Textures.v"),  FragmentShader("shaders/Textures.f")  );

    // get the uniform location ID
    int SamplerLocation = TriangleShader.getUniformLocation("uSampler");
    //==========================================================

    while (!glfwWindowShouldClose(gMainWindow) )
    {

        // Set the GPu as the current texture 0;
        gpuTex.setActiveTexture(0);

        VAO.bind();
            TriangleShader.sendUniform_Sampler2D( SamplerLocation ,0 ); // the uniform to the shader
        VAO.Render(QUADS);



        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }

    // Clear the VAO
    // Since we had flagged the array buffers for deletion ,they will now be
    // cleared as well since they are no longer bound to any VAOs
    VAO.clear();


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
