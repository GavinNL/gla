#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>


#include <gla/global.h>
#include <gla/camera.h>
//#include <gla/texture.h>
#include <gla/timer.h>
#include <gla/shader.h>
//#include <gla/gputexturearray.h>
#include <gla/vertexarrayobject.h>
#include <gla/framebufferobject.h>
//#include <gla/solids.h>
#include <gla/uniformbuffer.h>
//#include <gla/arraybuffer.h>
#include <gla/solids.h>
#include <locale>

using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
GLFWwindow* SetupOpenGLLibrariesAndCreateWindow();
//=================================================================================

struct Light
{
    vec3    Position;
    float   Linear;
    vec3    Color;     ////unsigned char padding2[4];
    float   Quadractic;
    float   R;
    float   _p1,p2,p3;
};

struct LightBlock140
{
    Light LightInfo[2];
    vec3 CameraPosition;
    float padding;
};



LightBlock140 LightBlock;

int main()
{
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    //===========================================================================
    // GLA code.
    //===========================================================================


    auto Box = gla::Solids::createPlane(20,20).ToGPU();

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram Standard;
    Standard.AttachShaders(  VertexShader("shaders/Standard_PUN.v"),  FragmentShader("shaders/Standard_PUN.f")  );

    ShaderProgram GBuffer;
    GBuffer.AttachShaders(  VertexShader("shaders/GBuffer.v"),  FragmentShader("shaders/GBuffer.f")  );

    ShaderProgram SPass;
    SPass.AttachShaders(  VertexShader("shaders/GBuffer_SPass.v"),  FragmentShader("shaders/GBuffer_SPass.f")  );

    //==========================================================
    // Create the FrameBufferObject
    //==========================================================
    FrameBufferObject mFBO;
    mFBO.Create();
    mFBO.CreateColourAttachment(0, {WINDOW_WIDTH, WINDOW_HEIGHT }, FBOColourFormat::RGB16F);
    mFBO.CreateColourAttachment(1, {WINDOW_WIDTH, WINDOW_HEIGHT }, FBOColourFormat::RGB16F);
    mFBO.CreateColourAttachment(2, {WINDOW_WIDTH, WINDOW_HEIGHT }, FBOColourFormat::RGBA);
    mFBO.CreateDepthAttachment(    {WINDOW_WIDTH, WINDOW_HEIGHT });
    mFBO.Update();


    //mFBO.Update();
    //==========================================================


    InterleavedVAO<vec3,vec2> cpuVAO;

    cpuVAO.InsertVertex( vec3(-1.0f, -1.0f, 0.f), vec2( 0.f, 0.f ));
    cpuVAO.InsertVertex( vec3( 1.0f ,-1.0f, 0.f), vec2( 1.f, 0.f ));
    cpuVAO.InsertVertex( vec3( 1.0f , 1.0f, 0.f), vec2( 1.f, 1.f ));
    cpuVAO.InsertVertex( vec3(-1.0f , 1.0f, 0.f), vec2( 0.f, 1.f ));

    cpuVAO.InsertElement( uvec4(0,1,2,3) );


    auto VAO = cpuVAO.ToGPU();
    //===============================================================

    GPUUniformBuffer MyGPUUniformBuffer( sizeof(LightBlock140) );
    auto BlockIndex = SPass.GetUniformBlockIndex("LightBlock140");
    SPass.Bind();
    std::cout << "Block Index: " << BlockIndex << std::endl;
    std::cout << " Uniform Buffer Size: "<<  SPass.GetUniformBlockSize("LightBlock140") << std::endl;
    std::cout << " Uniform Struct Size: "<<  sizeof(LightBlock140) << std::endl;
    std::cout << " Uniform Struct Light: "<< SPass.GetUniformBlockSize("LightBlock142")  << std::endl;

    std::cout << " Position Offset: "<<  SPass.GetUniformOffset("LightInfo[0].Color") << std::endl;
    SPass.BindUniformBuffer(MyGPUUniformBuffer, BlockIndex, 1);

    //===============================================================
    auto gpuTex1 = Texture("resources/marble.jpg", 3 ).ToGPU();
    auto gpuTex2 = Texture("resources/grass.jpg", 3 ).ToGPU();

    Camera mCamera;
    mCamera.perspective( 90.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);
    Transformation mTransform;

    mTransform.setPosition( {0,-2.0, 0.0});
    //mTransform.setEuler( vec3(0.5,0.2,-1.2));
    std::cout << "Starting" << std::endl;
    mFBO.Unbind();

    gla::Timer_T<float> Tim;

    while (!glfwWindowShouldClose(gMainWindow) )
    {

        glEnable(GL_DEPTH_TEST);
         mFBO.Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //mTransform.rotate( {0.2,1,0}, 0.01);

        GBuffer.Bind();
        gpuTex1.SetActive( 0 );
        gpuTex2.SetActive( 1 );
        GBuffer.UniformData( ShaderProgram::Hash("uTransform"), mTransform.getMatrix());
        GBuffer.UniformData( ShaderProgram::Hash("uCamera"),    mCamera.getProjectionMatrix() * mCamera.getTransform().getMatrix() );
        GBuffer.UniformData( ShaderProgram::Hash("texture_diffuse1"),  0 );
        GBuffer.UniformData( ShaderProgram::Hash("texture_specular1"), 1 );


        // renders the VAO as triangles (assume triangles since our Index buffer is uvec3, if it was uvec4, it would assume quads
        Box.Render();


        if(1)
        {
            mFBO.Unbind();
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            SPass.Bind();

             {
                 const float linear = 0.0f;
                 const float quadratic = 2.8f;
                 vec3 p = -vec3( cos(Tim.getElapsedTime() ) , 0.0f, sin(Tim.getElapsedTime())  )*2.0f;

                 const GLfloat maxBrightness = 1.0f;

                 float radius =  (-linear + sqrt(linear * linear - 4 * quadratic * (1.0f - (256.0 / 5.0) * maxBrightness))) / (2 * quadratic);
                 //SPass.UniformData( "lights[0].Radius"_h, radius  );

                 LightBlock.LightInfo[0].Linear     = linear;
                 LightBlock.LightInfo[0].Quadractic = quadratic;
                 LightBlock.LightInfo[0].Position   = p;
                 LightBlock.LightInfo[0].Color      = vec3(0,1,0);
                 LightBlock.LightInfo[0].R          = radius;
             }

             {
                 const float linear = 0.00f;
                 const float quadratic = 2.8f;
                 vec3 p =  vec3( cos(Tim.getElapsedTime() ) , 0.0f, sin(Tim.getElapsedTime())  )*2.0f;


                 const GLfloat maxBrightness = 1.0f;

                 float radius = (-linear + sqrt(linear * linear - 4 * quadratic * (1.0f - (256.0 / 5.0) * maxBrightness))) / (2 * quadratic);

                 LightBlock.LightInfo[1].Linear     = linear;
                 LightBlock.LightInfo[1].Position   = p;
                 LightBlock.LightInfo[1].Color      = vec3(0,0,1);
                 LightBlock.LightInfo[1].Quadractic = quadratic;
                 LightBlock.LightInfo[1].R          = radius;//vec4(radius);

             }

            LightBlock.CameraPosition = vec3(0.0f);
            MyGPUUniformBuffer.CopyData(LightBlock);
            //SPass.UniformData( "lights[0]"_h, (float*)&Lights[0].Position[0], 18u );
            //SPass.UniformData( "viewPos"_h, vec3(0.0f,0.0f,0.0f)  );

            mFBO.GetAttachment(FBOAttachment::COLOR0).SetActive(0);
            mFBO.GetAttachment(FBOAttachment::COLOR1).SetActive(1);
            mFBO.GetAttachment(FBOAttachment::COLOR2).SetActive(2);
            //mFBO.GetAttachment(FBOAttachment::DEPTH).SetActive(0);
            //gpuTex1.SetActive(0);
            SPass.UniformData( ShaderProgram::Hash("gPosition"),   0 );
            SPass.UniformData( ShaderProgram::Hash("gNormal"),     1 );
            SPass.UniformData( ShaderProgram::Hash("gAlbedoSpec"), 2 );

            // To specify exactly what primitave you want. Use the following.
            VAO.Render();
        }
        glfwSwapBuffers(gMainWindow);
        glfwPollEvents();
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
//    std::cout << sizeof(TestS) << std::endl;
//    exit(0);
    glewExperimental = GL_TRUE;
    if (!glfwInit())
        exit(EXIT_FAILURE);

    auto gMainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello Triangle", NULL, NULL);

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
