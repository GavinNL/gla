#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/gla.h>
#include <locale>
#include <tuple>
#include <gla/types.h>

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








template< std::size_t I, typename... Types >
inline typename std::tuple_element<I, std::tuple<Types...> >::type&
    attr( MemoryAlignedTuple<Types...>& t )
    {
        using element = typename std::tuple_element<I, std::tuple<Types...> >::type;

        return *reinterpret_cast<element*>(t.data + tupleoffset<I, Types...>());
    }


int main()
{


    MemoryAlignedTuple<vec2, float , double> X;

    gla::Timer_T<double> Tm;

    std::cout << sizeof(X) << std::endl;
    std::cout << sizeof( std::tuple<vec2, float, double>) << std::endl;
    std::cout << (sizeof(vec2)+sizeof(float)+sizeof(double)) << std::endl;
    std::cout << "=====" << std::endl;
    std::cout << tupleoffset<0, vec2, float, double>() << std::endl;
    std::cout << tupleoffset<1, vec2, float, double>() << std::endl;
    std::cout << tupleoffset<2, vec2, float, double>() << std::endl;

    std::cout << "Total size: " << tuplesize<2, vec2, float, double>() << std::endl;
    std::cout << "Total size: " << sizeof(X) << std::endl;

    std::cout << Tm.getElapsedTime() << std::endl;
   // return 0;
    //std::cout << &X << "   " << &X[0][0] << "   " << &X[0] << std::endl;
    //return 0;
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    //===========================================================================
    // GLA code.
    //===========================================================================

    //---------------------------------------------------------------------------
    // Create two buffers on the CPU to hold position and colour information
    //---------------------------------------------------------------------------
    VertexArrayObject cpuVAO;


    // Create two buffers, they will return the position
    // of the buffer. Eg, position will be the 0'th attribute in the shader
    // colour will be the 1'st attribute
    int positionIndex = cpuVAO.createBuffer<vec3>();  // this will be 0
    int texIndex      = cpuVAO.createBuffer<vec2>();  // this will be 1


    // Also create an index buffer.
    u4ArrayBuffer cpuIndex;

    cpuVAO.insert(positionIndex, vec3(-1.0f, -1.0f, 0.f));
    cpuVAO.insert(positionIndex, vec3( 1.0f ,-1.0f, 0.f));
    cpuVAO.insert(positionIndex, vec3( 1.0f , 1.0f, 0.f));
    cpuVAO.insert(positionIndex, vec3(-1.0f , 1.0f, 0.f));

    cpuVAO.insert(texIndex, vec2( 0.f, 0.f ) );
    cpuVAO.insert(texIndex, vec2( 1.f, 0.f ) );
    cpuVAO.insert(texIndex, vec2( 1.f, 1.f ) );
    cpuVAO.insert(texIndex, vec2( 0.f, 1.f ) );

    cpuVAO.insertElement( uvec4(0,1,2,3) );


    auto VAO = cpuVAO.toGPU();

    // Load some textures. And force using 3 components (r,g,b)
    Texture cpuTex1("resources/moss1024.jpg", 3 );
    Texture cpuTex2("resources/rocks1024.jpg", 3 );
    Texture cpuTex3(256,256, 3);



    //=========================================================================================
    // Create the Texture array on the GPU.
    //   Note: Any objects that start with GPU mean they are initialized on the GPU
    //=========================================================================================
    GPUTextureArray GPUTArray;

    // Create the GPUTexture array with 3 layers that hold 256x256 images with 3 components each, and 2 mipmaps.
    GPUTArray.create( uvec2(256,256), 3, 2 , 3);

    // Set the red channel using a lambda function
    cpuTex3.r = [] (float x, float y) {  return glm::perlin( vec2(x,y) * 2.0f, vec2(2.0,2.0) )*0.5 + 0.5f;  };

    // Resize the textures so they match the TextureArray. This will throw an exception if
    // the sizes do not match.
    cpuTex1.resize( uvec2(256,256) );
    cpuTex2.resize( uvec2(256,256) );


    // Copy the textures into the appropriate layer. This will throw an exception if
    // the number of colour channels in the TextureArray does not match what's on the Texture
    GPUTArray.SetLayer( cpuTex1 , 0);
    GPUTArray.SetLayer( cpuTex2 , 1);
    GPUTArray.SetLayer( cpuTex3 , 2);

    //===============================================================


    // we dont need the cpu texture anymore, so we can clear it.
    cpuTex1.clear();
    cpuTex2.clear();
    cpuTex3.clear();



//    gla::ArrayBuffer_T< std::tuple<vec3, vec2> > Buf;
    gla::InterleavedVAO<vec3, vec2> Buf;


    Buf.insertVertex( vec3(-1.0f, -1.0f, 0.f), vec2( 0.f, 0.f )  );
    Buf.insertVertex( vec3( 1.0f ,-1.0f, 0.f), vec2( 1.f, 0.f )  );
    Buf.insertVertex( vec3( 1.0f , 1.0f, 0.f), vec2( 1.f, 1.f )  );
    Buf.insertVertex( vec3(-1.0f , 1.0f, 0.f), vec2( 0.f, 1.f )  );

//    cpuVAO.insert(texIndex, vec2( 0.f, 0.f ) );
//    cpuVAO.insert(texIndex, vec2( 1.f, 0.f ) );
//    cpuVAO.insert(texIndex, vec2( 1.f, 1.f ) );
//    cpuVAO.insert(texIndex, vec2( 0.f, 1.f ) );

    VAO = Buf.toGPU();
    //---------------------------------------------------------------------------
    // Create a shader
    //---------------------------------------------------------------------------

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram TextureArrayShader;
    TextureArrayShader.linkProgram(  VertexShader("shaders/TextureArray.v"),  FragmentShader("shaders/TextureArray.f")  );

   //==========================================================

    Timer_T<float> Timer;



    while (!glfwWindowShouldClose(gMainWindow) )
    {

        // Set the GPu as the current texture 0;
        GPUTArray.setActiveTexture(0);

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
        TextureArrayShader.UniformData( ShaderProgram::Hash("uTextureArray"), 0);
        TextureArrayShader.UniformData( ShaderProgram::Hash("uSpeed"),        Speed);

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
