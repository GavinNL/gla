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

struct Uniform140
{
    vec4 x;
};

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



    //---------------------------------------------------------------------------
    // Create a shader
    //
    // This shader is the same as the shader from TextureArray.f
    // But instead of sending the time parameter as a uniform, we will
    // bind a uniform buffer
    //---------------------------------------------------------------------------

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram UniformBufferShader;
    UniformBufferShader.linkProgram(  VertexShader("shaders/UniformBuffer.v"),  FragmentShader("shaders/UniformBuffer.f")  );

   //==========================================================

    Timer_T<float> Timer;


    // This is the structure that we will create a buffer for
    // Note: the alignment of structs and the alignment of uniform blocks in the shader
    // are NOT the same. vec3 are
    struct UniformBufferStruct140 {
        vec3 Dir_Speed;          // x,y = direction, z = speed
        unsigned char padding1;  // we need this padding because vec3 in the shader is aligned to vec4, that is the amount of space
        vec4 colour;
    };
    UniformBufferStruct140 UniformData; // allocate the memory on the CPU


    // Here we create a uniform buffer on the gpu, we pass the size of the struct
    // into the input parameter.
    GPUUniformBuffer MyGPUUniformBuffer(sizeof(UniformBufferStruct140));


    // We first need to get the block index of the "Uniform140" block in the shader.
    auto BlockIndex = UniformBufferShader.GetUniformBlockIndex("Uniform140");

    std::cout << "Block Index           : " << BlockIndex << std::endl;
    std::cout << "Max Buffer Bind Points: " << GPUUniformBuffer::Get_MAX_UNIFORM_BUFFER_BINDINGS() << std::endl;
    std::cout << "Max Buffer block size : " << GPUUniformBuffer::Get_GL_MAX_UNIFORM_BLOCK_SIZE()   << std::endl;


    //--------------------------------------------------------------------
    // Bind the GPUUniformBuffer to the UniformBlock in the shader.
    // The third argument is the BindPoint we want to bind to and has
    // to be between 0 and GPUUniformBuffer::GetGet_MAX_UNIFORM_BUFFER_BINDINGS()
    // We are going to bind it to 1.
    //--------------------------------------------------------------------
    UniformBufferShader.BindUniformBuffer(MyGPUUniformBuffer, BlockIndex, 1);
    //--------------------------------------------------------------------

    while (!glfwWindowShouldClose(gMainWindow) )
    {


        // Set the GPU as the current texture 0;
        int TextureIndex = 0;
        GPUTArray.setActiveTexture(TextureIndex);

        auto t = Timer.getElapsedTime();

        UniformData.Dir_Speed = { cos(t), sin(t), cos(t)*sin(t) };
        UniformData.colour    = { cos(t)*cos(t), 0,0,1};

        MyGPUUniformBuffer.CopyData(UniformData);
        //X.CopyData(Udata);
        //X.GetData().x.x = Speed.x;
        //X.GetData().x.y = Speed.y;
        //X.UpdateBuffer();


        // Here we send Uniform data to the shader
        //  The first argument is an user defined index. Depending on the number of uniforms you have, you should always
        //  start at 0 and then increase sequentially.
        //  The second parameter is the name of the  uniform in the shader
        //  And the third parameter is the value we want to send. In our case we want to send 0, because we set our texture to be in texture unit 0.
        //
        //  sendUniform will only query the shader the first time and then store the shader uniform location in an array at index 0 (the first parameter)
        //  the next time we call sendUniform(0, "uSampler", X), it will use the cached value.
        UniformBufferShader.sendUniform(0, "uTextureArray", TextureIndex);

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
