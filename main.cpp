#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <glre/glre.h>
#include <glre/utils/window.h>
#include <glre/utils/cameracontroller.h>
#include <locale>

#include <rgui/rgui.h>
#include <glre/skeleton.h>

using namespace glre;

int main ()
{
    rgui::Timer T1;
    Skeleton Sk;
    Sk.load("resources/boblampclean.md5mesh");
    std::cout << "Mesh loading time: " << T1.getElapsedTime() << std::endl;
    return 0;

    rgui::json::Value JSON;
    JSON.fromFile("resources/ExtraInfo.json");

    int width  = (uint)JSON["main"]["width" ].as<float>();
    int height = (uint)JSON["main"]["height"].as<float>();

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit () )
    {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }


  auto w1 = glre::utils::Window::create(width,height, "Test window");


        auto R = rgui::Root::getInstance();
             R->init();
        auto I = R->createInterface( (uint)JSON["main"]["width"].as<float>(),
                                     (uint)JSON["main"]["height"].as<float>(),
                                     "MainInterface",
                                     JSON["main"]["skin"].as<std::string>() );


        auto M = I->getRootWidget()->loadFromJSON( JSON["main"]["GUI"] );
        for(auto m : M )
                std::cout << m.second.lock()->getName() << std::endl;




        w1->EventsMap["GUI"] = [&] (const glre::utils::Event & E) {

                        switch(E.type)
                        {
                            case glre::utils::MOUSECURSOR:
                                I->injectMousePosition(rgui::LEFT_BUTTON, E.MouseCursor.x, E.MouseCursor.y);
                                break;
                            case glre::utils::TEXT:
                                I->injectCharacters( E.Text.codepoint );
                                break;
                            case glre::utils::KEY:
                                I->injectKey( rgui::FromGLFW[ E.Key.key ] , E.Key.action);
                            case glre::utils::MOUSEBUTTON:

                                const rgui::MouseButton MB[8] = {rgui::LEFT_BUTTON, rgui::RIGHT_BUTTON, rgui::MIDDLE_BUTTON,rgui::NONE,rgui::NONE,rgui::NONE,rgui::NONE, rgui::NONE};

                                I->injectMouseButton( MB[E.MouseButton.button], E.MouseButton.action, E.MouseButton.x, E.MouseButton.y);
                            break;

                        }
        };

        glre::Camera Cam;
        Cam.perspective(45,640.0/480.0,0.2f,1000.0f);


        w1->EventsMap["CAM"] = [&] (const glre::utils::Event & E) {

                        switch(E.type)
                        {
                            case glre::utils::KEY:
                                switch( E.Key.key )
                                {
                                    case GLFW_KEY_W:
                                        Cam.mAcceleration[2] = E.Key.action ?  10.f : 0.0;
                                        break;
                                    case GLFW_KEY_S:
                                        Cam.mAcceleration[2] = E.Key.action ? -10.f : 0.0;
                                        break;
                                    case GLFW_KEY_A:
                                        Cam.mAcceleration[0] = E.Key.action ?  10.f : 0.0;
                                        break;
                                    case GLFW_KEY_D:
                                        Cam.mAcceleration[0] = E.Key.action ? -10.f : 0.0;
                                        break;
                                }

                            case glre::utils::MOUSECURSOR:
                                if( w1->isMouseButtonPressed(glre::utils::MOUSE::RIGHT_MOUSE_BUTTON))
                                {
                                    Cam.rotate(   vec3( -(float)E.MouseCursor.dy*0.001,  (float)E.MouseCursor.dx*0.001, 0.0 ) );
                                }

                                break;

                            case glre::utils::MOUSEBUTTON:

                                switch( E.MouseButton.button )
                                {

                                    case GLFW_MOUSE_BUTTON_RIGHT:

                                        if( E.MouseButton.action == GLFW_PRESS)
                                        {
                                            w1->SetCursorMode( glre::utils::MOUSE::CURSOR_DISABLED);
                                        }
                                        else
                                        {
                                            w1->SetCursorMode(  glre::utils::MOUSE::CURSOR_NORMAL );
                                        }
                                        break;

                                }

                            break;
                        }
        };

  // get version info
  const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
  const GLubyte* version  = glGetString (GL_VERSION);  // version as a string
  printf ("Renderer: %s\n", renderer);
  printf ("OpenGL version supported %s\n", version);





  auto DropDownCallback = [&] ( const rgui::Button::Callback & C, GLuint ID, uint w, uint h)
  {
        M["w3"].lock()->setRawRect( 0, 0, 1, 1, ID, w, h );
  };




  //=============================================================================
  // Load the models
  //=============================================================================
  auto DragonGPU = loadModel( "resources/boblampclean.md5mesh",true).CreateGPUObject();
  auto    Axis   = glre::createAxes().CreateGPUObject();


  //=============================================================================
  // Load the Textures
  //=============================================================================
  GPUTexture Tex  = glre::LoadTexture("resources/dragontexture.png").CreateGPUTexture();
  //GPUTexture Tex  = glre::LoadTexture("resources/boblampclean.md5mesh").CreateGPUTexture();
  GPUTexture Tex2 = glre::LoadTexture("resources/marble.jpg"       ).CreateGPUTexture();
  GPUTexture Tex3 = glre::LoadTexture("resources/SpiderTex.jpg"    ).CreateGPUTexture();


  //=============================================================================
  // Load the Shaders
  //=============================================================================
  glre::ShaderProgram LineShader( VertexShader("shaders/Line_PC.v")   , FragmentShader("shaders/Line_PC.f")   );
  glre::ShaderProgram S(          VertexShader("shaders/Basic_PNCU.v"), FragmentShader("shaders/Basic_PNCU.f"));
  //=============================================================================



  //=============================================================================
  //  Set the callback methods for to see the texture in the GGUI
  //=============================================================================
  std::dynamic_pointer_cast<rgui::ComboBox>(M["w2"].lock())->insertItem("Dragon")->addCallback( "texture", std::bind( DropDownCallback, std::placeholders::_1, Tex.getID(),   Tex.size()[0],  Tex.size()[1])  );
  std::dynamic_pointer_cast<rgui::ComboBox>(M["w2"].lock())->insertItem("Marble")->addCallback( "texture", std::bind( DropDownCallback, std::placeholders::_1, Tex2.getID(), Tex2.size()[0], Tex2.size()[1])  );
  std::dynamic_pointer_cast<rgui::ComboBox>(M["w2"].lock())->insertItem("Spider")->addCallback( "texture", std::bind( DropDownCallback, std::placeholders::_1, Tex3.getID(), Tex3.size()[0], Tex3.size()[1])  );
  //=============================================================================






  //std::dynamic_pointer_cast<rgui::Button>(M["b1"].lock())->addCallback( "change", [&] (const rgui::Button::Callback & C){DragonCPU = loadModel("resources/suzanne.obj", true);}  );


  GLuint LineShaderCamMatrixId   = LineShader.getUniformLocation("inCameraMatrix");
  GLuint LineShaderModelMatrixID = LineShader.getUniformLocation("inModelMatrix");

  GLuint camMatrixId             = S.getUniformLocation("inCameraMatrix");
  GLuint modelMatrixID           = S.getUniformLocation("inModelMatrix");
  GLuint projectionMatrixID      = S.getUniformLocation("inProjectionMatrix");
  GLuint lightPositionID         = S.getUniformLocation("uLightPosition");
  GLuint uSampler0ID             = S.getUniformLocation("uSampler");

  glre::mat4 Pv = Cam.getProjectionMatrix();

  // tell GL to only draw onto a pixel if the shape is closer to the viewer


  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  Cam.setPosition( vec3(0.5 ,1.0, 0.5) );
  Transformation DragonTransform;
  DragonTransform.setScale( vec3(0.1,0.1,0.1) );
  Timer_T<float> tim;
  float dt = 0;
  while( !w1->WantsToClose() )
  {

       glEnable(GL_CULL_FACE);
       glEnable(GL_DEPTH_TEST);
       glDepthFunc(GL_LESS);

       glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
       Cam.calculate(dt);
       //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
       auto CameraMatrix = Cam.getMatrix();


       Tex.setActiveTexture(0);
           S.useShader();
           DragonTransform.yaw( dt * 3.14159 / 10.0 );
           S.sendUniform_mat4(camMatrixId       , CameraMatrix       );
           S.sendUniform_mat4(modelMatrixID     , DragonTransform.getMatrix()      );
           S.sendUniform_mat4(projectionMatrixID, Pv                 );
           S.sendUniform_vec3(lightPositionID   , vec3(50,50,50)     );
           S.sendUniform_Sampler2D(uSampler0ID, 0);
       DragonGPU.Render();

       LineShader.useShader();
           LineShader.sendUniform_mat4(LineShaderCamMatrixId,   Pv * CameraMatrix  );
           LineShader.sendUniform_mat4(LineShaderModelMatrixID, glm::scale( mat4(1.0), vec3(5.0,5.0,5.0)) );
       Axis.Render();


       glDisable(GL_CULL_FACE);
       I->draw();

       glre::utils::Window::PollEvents();
       w1->SwapBuffers();

       dt = tim.getElapsedTime();
       tim.reset();
  }


  /* OTHER STUFF GOES HERE NEXT */

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}
