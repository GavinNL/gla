//#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>


#include <gla/global.h>
#include <gla/camera.h>
#include <gla/timer.h>
#include <gla/shader.h>
#include <gla/vertexarrayobject.h>
#include <gla/framebufferobject.h>
#include <gla/solids.h>
#include <gla/uniformbuffer.h>
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

    // Create the two shaders. The second argument is set to true because we are
    // compiling the shaders straight from a string. If we were compiling from a file
    // we'd just do:  VertexShader vs(Path_to_file);
    ShaderProgram ShadowMapPass;
    ShadowMapPass.AttachShaders(  VertexShader("../resources/shaders/Shadow_Map_Pass.v"),  FragmentShader("../resources/shaders/Shadow_Map_Pass.f")  );

    ShaderProgram ObjectPass;
    ObjectPass.AttachShaders(  VertexShader("../resources/shaders/Standard_PUN_shadows.v"),  FragmentShader("../resources/shaders/Standard_PUN_shadows.f")  );

    //==========================================================
    // Create the FrameBufferObject for shadow map
    //==========================================================
    FrameBufferObject mFBO;
    mFBO.Create();
    mFBO.CreateDepthAttachment(  {WINDOW_WIDTH, WINDOW_HEIGHT });
    //glDrawBuffer(GL_NONE);
    mFBO.Update();

    //==========================================================

    auto Sphere = gla::Solids::createBox().ToGPU();
    auto Plane  = gla::Solids::createPlane(10,10, vec2(0.1,0.1)).ToGPU();


    gla::InterleavedVAO<vec3, vec2> P;
    P.InsertVertex( glm::vec3(-1, 1,0) ,glm::vec2(0,0) );
    P.InsertVertex( glm::vec3(-1,-1,0) ,glm::vec2(0,1) );
    P.InsertVertex( glm::vec3( 1,-1,0) ,glm::vec2(1,1) );
    P.InsertVertex( glm::vec3( 1, 1,0) ,glm::vec2(1,0) );

    auto Flat = P.ToGPU();

    //===============================================================

    auto gpuTex1 = Texture("../resources/textures/marble.jpg", 3 ).ToGPU();
    auto gpuTex2 = Texture("../resources/textures/rocks.jpg",  3 ).ToGPU();

    Camera mCamera;

    mCamera.Perspective( 90.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);

    mCamera.SetPosition( vec3(0,3,2));
    Transform mTransform;

    mTransform.SetPosition( vec3{0, 1.0, -2.0 });


    gla::Timer_T<float> Tim;

    while (!glfwWindowShouldClose(gMainWindow) )
    {
        glEnable(GL_DEPTH_TEST);
    #if 1
         mFBO.Bind();

         glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         float t  = Tim.getElapsedTime() * 2.0*3.14159 / 3.0f ;

         glm::vec3 lightInvDir = glm::vec3(sin(t)*4.0f, 1.0 ,cos(t)*4.0f-2.0f);



         // Compute the MVP matrix from the light's point of view
         glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
         glm::mat4 depthViewMatrix       = glm::lookAt(lightInvDir, glm::vec3(0,0 ,-2), glm::vec3(0,1,0));
         glm::mat4 depthModelMatrix      = glm::mat4(1.0);
         glm::mat4 depthMVP              = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;


         ShadowMapPass.Bind();

         ShadowMapPass.Uniform( ShadowMapPass.GetUniformLocation("depthMVP")  ,  depthMVP          );


         ShadowMapPass.Uniform( ShadowMapPass.GetUniformLocation("uModel") ,  mTransform.GetMatrix()        );
         Sphere.Render();

         ShadowMapPass.Uniform( ShadowMapPass.GetUniformLocation("uModel") ,  mat4()        );
         Plane.Render();

         mFBO.Unbind();
#else
         mFBO.Bind();
         glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

         gpuTex1.SetActive( 0 );
         gpuTex2.SetActive( 1 );

         glm::vec3 lightInvDir = glm::vec3(0.0f, 4 ,0);
         glm::mat4 depthViewMatrix       = glm::lookAt(lightInvDir, glm::vec3(0,0,-2), glm::vec3(0,1,0));

         ObjectPass.Bind();
         ObjectPass.Uniform( ObjectPass.GetUniformLocation("uViewMatrix")  ,  depthViewMatrix           );
         ObjectPass.Uniform( ObjectPass.GetUniformLocation("uProjMatrix")  ,  mCamera.GetProjectionMatrix() );

         ObjectPass.Uniform( ObjectPass.GetUniformLocation("uModelMatrix") ,  mTransform.GetMatrix()        );
         ObjectPass.Uniform( ObjectPass.GetUniformLocation("uTexture")     ,  0 );
         Sphere.Render();

         ObjectPass.Uniform( ObjectPass.GetUniformLocation("uModelMatrix") ,  mat4()        );
         ObjectPass.Uniform( ObjectPass.GetUniformLocation("uTexture")     ,  2 );
         Plane.Render();
#endif
        //======================

        mFBO.Unbind();
        glViewport( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        gpuTex1.SetActive( 0 );
        gpuTex2.SetActive( 1 );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto depthtexture = mFBO.GetAttachment( FBOAttachment::DEPTH );

        depthtexture.SetActive(2);

        ObjectPass.Bind();


        glm::mat4 biasMatrix = mat4( 0.5, 0.0, 0.0, 0.0,
                              0.0, 0.5, 0.0, 0.0,
                              0.0, 0.0, 0.5, 0.0,
                              0.5, 0.5, 0.5, 1.0 ) * depthMVP;;


        ObjectPass.Uniform( ObjectPass.GetUniformLocation("uShadowMap")     ,  2 );
        ObjectPass.Uniform( ObjectPass.GetUniformLocation("uViewMatrix")  ,  mCamera.GetMatrix()           );
        ObjectPass.Uniform( ObjectPass.GetUniformLocation("uProjMatrix")  ,  mCamera.GetProjectionMatrix() );
        ObjectPass.Uniform( ObjectPass.GetUniformLocation("uDepthBias")  ,   biasMatrix );


        ObjectPass.Uniform( ObjectPass.GetUniformLocation("uModelMatrix") ,  mTransform.GetMatrix()        );
        ObjectPass.Uniform( ObjectPass.GetUniformLocation("uTexture")     ,  0 );

        Sphere.Render();

        ObjectPass.Uniform( ObjectPass.GetUniformLocation("uModelMatrix") ,  mat4()        );
        ObjectPass.Uniform( ObjectPass.GetUniformLocation("uTexture")     ,  1 );
        Plane.Render();

      //  Flat.Render();

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
//    GLA_DOUT  << sizeof(TestS) << std::endl;
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
