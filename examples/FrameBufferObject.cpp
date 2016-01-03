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
//#include <gla/uniformbuffer.h>
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

int main()
{
    GLFWwindow * gMainWindow = SetupOpenGLLibrariesAndCreateWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    //===========================================================================
    // GLA code.
    //===========================================================================


    auto Box = gla::Solids::createBox().ToGPU();

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
    mFBO.CreateColourAttachment(0, {WINDOW_WIDTH, WINDOW_HEIGHT}, FBOColourFormat::RGB16F);
    mFBO.CreateColourAttachment(1, {WINDOW_WIDTH, WINDOW_HEIGHT}, FBOColourFormat::RGB16F);
    mFBO.CreateColourAttachment(2, {WINDOW_WIDTH, WINDOW_HEIGHT}, FBOColourFormat::RGBA);
    mFBO.CreateDepthAttachment({WINDOW_WIDTH, WINDOW_HEIGHT});
    mFBO.Update();

/*
    auto position = GPUTexture( {WINDOW_WIDTH, WINDOW_HEIGHT}, false, TexInternalFormat::RGB16F, TexFormat::RGB, DataType::UNSIGNED_BYTE) ;
    auto normal   = GPUTexture( {WINDOW_WIDTH, WINDOW_HEIGHT}, false, TexInternalFormat::RGB16F, TexFormat::RGB, DataType::UNSIGNED_BYTE) ;
    auto albedo   = GPUTexture( {WINDOW_WIDTH, WINDOW_HEIGHT}, false, TexInternalFormat::RGBA,   TexFormat::RGBA, DataType::UNSIGNED_BYTE) ;
    auto depth    = GPUTexture( {WINDOW_WIDTH, WINDOW_HEIGHT}, false, TexInternalFormat::DEPTH_COMPONENT,  TexFormat::DEPTH_COMPONENT,  DataType::UNSIGNED_BYTE) ;

    mFBO.Create();
    mFBO.Bind();

    position.SetFilter(TexFilter::NEAREST, TexFilter::NEAREST);
    normal.SetFilter(TexFilter::NEAREST, TexFilter::NEAREST);
    albedo.SetFilter(TexFilter::NEAREST, TexFilter::NEAREST);
    depth.SetFilter(TexFilter::NEAREST, TexFilter::NEAREST);

   // mFBO.Create(FBOAttachment::COLOR0, {WINDOW_WIDTH, WINDOW_HEIGHT} );
    glFramebufferTexture2D(GL_FRAMEBUFFER, (GLuint)FBOAttachment::COLOR0, GL_TEXTURE_2D, position.m_Handle.GetID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, (GLuint)FBOAttachment::COLOR1, GL_TEXTURE_2D, normal  .m_Handle.GetID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, (GLuint)FBOAttachment::COLOR2, GL_TEXTURE_2D, albedo  .m_Handle.GetID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, (GLuint)FBOAttachment::DEPTH,  GL_TEXTURE_2D, depth   .m_Handle.GetID(), 0);

    GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw gla::GLA_EXCEPTION("Framebuffer status was not complete.");
    }
*/
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

    auto gpuTex1 = Texture("resources/rocks1024.jpg", 3 ).ToGPU();
    auto gpuTex2 = Texture("resources/grass.jpg", 3 ).ToGPU();

    Camera mCamera;
    mCamera.perspective( 90.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);
    Transformation mTransform;

    mTransform.setPosition( {0,0, -1.5});
    mTransform.setEuler( vec3(0.5,0.2,-1.2));
    std::cout << "Starting" << std::endl;
    mFBO.Unbind();

    gla::Timer_T<float> Tim;
    while (!glfwWindowShouldClose(gMainWindow) )
    {

        glEnable(GL_DEPTH_TEST);
        mFBO.Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mTransform.rotate( {0.2,1,0}, 0.01);

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

            mFBO.GetAttachment(FBOAttachment::DEPTH).SetActive(0);
            //gpuTex1.SetActive(0);
            SPass.UniformData( ShaderProgram::Hash("uSampler"),  0 );

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
