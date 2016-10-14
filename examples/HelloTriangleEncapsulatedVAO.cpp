//#include <GL/glew.h>    // include GLEW and new version of GL on Windows
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
#if 0
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    //===========================================================================
    // GLA code.
    //===========================================================================


    // There are two ways we can create VAOs
    //   Using only vertices
    //   Using vertices and an Index buffer

    // A cpuVAO holds cpuArraybuffers. It's a convenient class to help
    // create a VAO
    VertexArrayObject cpuNonIndexed;
    VertexArrayObject cpuIndexed;


    //===========================================================================
    // Create a non indexed VAO that will hold a triangle.
    //===========================================================================
    {
        // Create two buffers, they will return the position
        // of the buffer. Eg, position will be the 0'th attribute in the shader
        // colour will be the 1'st attribute
        int positionIndex = cpuNonIndexed.createBuffer<vec3>();  // this will be 0
        int colourIndex   = cpuNonIndexed.createBuffer<vec4>();  // this will be 1

        // insert the positions and colours
        cpuNonIndexed.insert(positionIndex, vec3(-1.0f, -1.0f, 0.f) );
        cpuNonIndexed.insert(positionIndex, vec3( 0.0f ,-1.0f, 0.f) );
        cpuNonIndexed.insert(positionIndex, vec3(-0.5f , 0.0f, 0.f) );

        //==attempting to insert a vec4 into a vec3 buffer will not do anything==
        cpuNonIndexed.insert(positionIndex, vec4( 0.0f , 1.0f, 0.f, 1.0) );  // This won't do anything.

        // Insert the colours
        cpuNonIndexed.insert(colourIndex,   vec4(1.f, 0.f, 0.f, 1.0f) );
        cpuNonIndexed.insert(colourIndex,   vec4(0.f, 1.f, 0.f, 1.0f) );
        cpuNonIndexed.insert(colourIndex,   vec4(0.f, 0.f, 1.f, 1.0f) );
    }
    //===========================================================================



    //===========================================================================
    // Create an Indexed VAO that will hold a triangle.
    //   In most cases you will want to use an indexed
    //===========================================================================
    {
        // Create two buffers, they will return the position
        // of the buffer. Eg, position will be the 0'th attribute in the shader
        // colour will be the 1'st attribute
        int positionIndex = cpuIndexed.createBuffer<vec3>();  // this will be 0
        int colourIndex   = cpuIndexed.createBuffer<vec4>();  // this will be 1

        // insert the positions and colours
        cpuIndexed.insert(positionIndex, vec3( 0.0f,  0.0f, 0.f) );
        cpuIndexed.insert(positionIndex, vec3( 1.0f , 0.0f, 0.f) );
        cpuIndexed.insert(positionIndex, vec3( 1.0f , 1.0f, 0.f) );
        cpuIndexed.insert(positionIndex, vec3( 0.0f , 1.0f, 0.f) );

        // Insert the colours
        cpuIndexed.insert(colourIndex,   vec4(1.f, 0.f, 0.f, 1.0f) );
        cpuIndexed.insert(colourIndex,   vec4(0.f, 1.f, 0.f, 1.0f) );
        cpuIndexed.insert(colourIndex,   vec4(0.f, 0.f, 1.f, 1.0f) );
        cpuIndexed.insert(colourIndex,   vec4(1.f, 0.f, 1.f, 1.0f) );

        // Create a quad from vertices 0 1 2 and 3. If we wanted to use quads
        // we can use uvec4. or uvec2 for lines.
        cpuIndexed.insertElement( uvec4(0,1,2,4) );

    }
    //===========================================================================


    // Send the VAOs to the GPU.
    GPUArrayObject gpuNonIndexedVAO = cpuNonIndexed.toGPU();
    GPUArrayObject gpuIndexedVAO    = cpuIndexed.toGPU();

    // we can free the memory from the cpu since it is all on the gpu now.
    cpuNonIndexed.clear();
    cpuIndexed.clear();


    //---------------------------------------------------------------------------
    // Create a shader
    //---------------------------------------------------------------------------
    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram TriangleShader;
    TriangleShader.linkProgram(  VertexShader("shaders/HelloTriangle.v"),  FragmentShader("shaders/HelloTriangle.f")  );


    //==========================================================

    while (!glfwWindowShouldClose(gMainWindow) )
    {


        // Can use any one of the following to render the triangle, they are all equivelant.
        // as long as both buffers have the same number of items in it. In our case 3.
        TriangleShader.useShader();


        // Draw the non indexed vao as triangles. If we do not specify, it will assume that it's triangles
        // since the vast majority of the time, meshes are made out of triangles.
        gpuNonIndexedVAO.Render(TRIANGLES);


        // Draw the indexed VAO. The VAO is assumed to be QUADS because we assigned the index buffer
        // using uvec4.  If we wanted it to be something else, we'd have to specify what it is: eg: gpuIndexedVAO.Render(QUAD_STRIP);
        gpuIndexedVAO.Render();


        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
    }

    // Clear the VAO
    // Since we had flagged the array buffers for deletion ,they will now be
    // cleared as well since they are no longer bound to any VAOs
    gpuNonIndexedVAO.clear();
    gpuIndexedVAO.clear();


    glfwDestroyWindow(gMainWindow);
    glfwTerminate();
#endif
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
