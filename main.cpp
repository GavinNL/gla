#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <glre/glre.h>
#include <glre/utils/window.h>
#include <glre/utils/app.h>
#include <locale>

#include <rgui/rgui.h>


using namespace glre;


class MyApp : public glre::utils::App
{
    public:

    //===================================================================================================================
    // Init - load the json file
    //===================================================================================================================
    virtual void init()
    {
        INFO.fromFile("resources/ExtraInfo.json");


        CreateWindow( (uint)INFO["main"]["width"].as<float>(),  (uint)INFO["main"]["height"].as<float>() , "GLRE Window" );
        setupGUI();
        setupOpenGL();
    }


    //===================================================================================================================
    // Setup Rgui and the callbacks.
    //===================================================================================================================
    void setupGUI()
    {
        auto R = rgui::Root::getInstance();
             R->init();

        auto size = mWindow->size();
        mGuiInterface = R->createInterface(  size.x, size.y,
                                             "MainInterface",
                                             INFO["main"]["skin"].as<std::string>() );

        auto M = mGuiInterface->getRootWidget()->loadFromJSON( INFO["main"]["GUI"] );
        for(auto m : M )  std::cout << m.second.lock()->getName() << std::endl;

        //--------------------------------------------------------------------------------------------------------------
        // Set the callback for RGUI.
        //--------------------------------------------------------------------------------------------------------------
        mWindow->EventsMap["GUI"] = [&] (const glre::utils::Event & E)
        {
            switch(E.type)
            {
                case glre::utils::MOUSECURSOR:
                    mGuiInterface->injectMousePosition(rgui::LEFT_BUTTON, E.MouseCursor.x, E.MouseCursor.y);
                    break;
                case glre::utils::TEXT:
                    mGuiInterface->injectCharacters( E.Text.codepoint );
                    break;
                case glre::utils::KEY:
                    mGuiInterface->injectKey( rgui::FromGLFW[ E.Key.key ] , E.Key.action);
                case glre::utils::MOUSEBUTTON:

                    const rgui::MouseButton MB[8] = {rgui::LEFT_BUTTON, rgui::RIGHT_BUTTON, rgui::MIDDLE_BUTTON,rgui::NONE,rgui::NONE,rgui::NONE,rgui::NONE, rgui::NONE};

                    mGuiInterface->injectMouseButton( MB[E.MouseButton.button], E.MouseButton.action, E.MouseButton.x, E.MouseButton.y);
                break;

             }
        };

        //--------------------------------------------------------------------------------------------------------------
        // Set the callback to control the camera
        //--------------------------------------------------------------------------------------------------------------
        mWindow->EventsMap["CAM"] = [&] (const glre::utils::Event & E)
        {
            float speed = 10.0f;
            switch(E.type)
            {
                case glre::utils::KEY:
                    if(E.Key.mods == GLFW_MOD_SHIFT) speed = 160.f;
                    switch( E.Key.key )
                    {
                        case GLFW_KEY_W:
                            mCamera.mAcceleration[2] = E.Key.action ? speed : 0.0;
                            break;
                        case GLFW_KEY_S:
                            mCamera.mAcceleration[2] = E.Key.action ? -speed  : 0.0;
                            break;
                        case GLFW_KEY_A:
                            mCamera.mAcceleration[0] = E.Key.action ? speed  : 0.0;
                            break;
                        case GLFW_KEY_D:
                            mCamera.mAcceleration[0] = E.Key.action ? -speed  : 0.0;
                            break;
                    }

                case glre::utils::MOUSECURSOR:
                    if( mWindow->isMouseButtonPressed(glre::utils::MOUSE::RIGHT_MOUSE_BUTTON))
                        mCamera.rotate(   vec3( -(float)E.MouseCursor.dy*0.001,  (float)E.MouseCursor.dx*0.001, 0.0 ) );
                    break;

                case glre::utils::MOUSEBUTTON:
                    switch( E.MouseButton.button )
                    {
                        case GLFW_MOUSE_BUTTON_RIGHT:
                            if( E.MouseButton.action == GLFW_PRESS)
                                mWindow->SetCursorMode( glre::utils::MOUSE::CURSOR_DISABLED);
                            else
                                mWindow->SetCursorMode(  glre::utils::MOUSE::CURSOR_NORMAL );
                            break;

                    }

                break;
            }
        };

    }


    void setupOpenGL()
    {
        glEnable (GL_DEPTH_TEST); // enable depth-testing
        glDepthFunc (GL_LESS);    // depth-testing interprets a smaller value as "closer"
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Load the shaders
        LineShader.linkProgram(  VertexShader("shaders/Line_PC.v")   , FragmentShader("shaders/Line_PC.f")    );
        BasicSahder.linkProgram( VertexShader("shaders/Basic_PNCU.v"), FragmentShader("shaders/Basic_PNCU.f") );

        // load the objects
        Axis = glre::createAxes().toGPU();

        // setup the camera
        mCamera.perspective(45, 640.0/480.0 ,0.2f, 1000.0f);
    }

    //===================================================================================================================
    // The main render loop
    //===================================================================================================================
    virtual bool render(double dt, double t)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        mCamera.calculate(dt);

        //-----------------------------------------------------------------------------
        // Create static variables of some of the uniform locations.
        // This shouldn't be done in practice, it's just convenient for now.
        //-----------------------------------------------------------------------------
        static GLuint LineShaderCamMatrixId   = LineShader.getUniformLocation("inCameraMatrix");
        static GLuint LineShaderModelMatrixID = LineShader.getUniformLocation("inModelMatrix");
        //-----------------------------------------------------------------------------


        LineShader.useShader();
            LineShader.sendUniform_mat4(LineShaderCamMatrixId,   mCamera.getProjectionMatrix() * mCamera.getMatrix()  );
            LineShader.sendUniform_mat4(LineShaderModelMatrixID, glm::scale( mat4(1.0), vec3(5.0,5.0,5.0)) );
        Axis.Render();


        // Draw the RGUI interface
        glDisable(GL_CULL_FACE);
        mGuiInterface->draw();

        return true;
    };

    private:
        rgui::json::Value INFO;
        rgui::pInterface  mGuiInterface;

        glre::Camera      mCamera;

        //===================================================================================================================
        // Shaders
        //===================================================================================================================
        glre::ShaderProgram LineShader;
        glre::ShaderProgram BasicSahder;

        //===================================================================================================================
        // OpenGL Objects
        //===================================================================================================================
        glre::GPUArrayObject Axis;
};

int main ()
{

    MyApp A;
    A.start();

    return 0;
    rgui::json::Value JSON;
    JSON.fromFile("resources/ExtraInfo.json");

    int width  = (uint)JSON["main"]["width" ].as<float>();
    int height = (uint)JSON["main"]["height"].as<float>();


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
        Cam.perspective(45, 640.0/480.0 ,0.2f, 1000.0f);


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

  int maxTexLayers;
  glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTexLayers);
  printf ("Renderer: %s\n", renderer);
  printf ("OpenGL version supported %s\n", version);
  printf ("Max texture layers %d\n", maxTexLayers);



  auto DropDownCallback = [&] ( const rgui::Button::Callback & C, GLuint ID, uint w, uint h)
  {
        M["w3"].lock()->setRawRect( 0, 0, 1, 1, ID, w, h );
  };


  //=============================================================================
  // Load the models
  //=============================================================================
  //auto DragonGPU = loadModel( "resources/dragon.obj",true).CreateGPUObject();
  //auto DragonGPU = glre::ModelLoader::loadModel( JSON["main"]["model"].as<std::string>() ).toGPU();
  auto DragonGPU = glre::createBox( vec3(2.0,2.0,8.0) ).toGPU();
  auto    Axis   = glre::createAxes().toGPU();


  //=============================================================================
  // Load the Textures
  //=============================================================================
  GPUTexture Tex  = glre::LoadTexture("resources/dragontexture.png").toGPU();
  //GPUTexture Tex  = glre::LoadTexture("resources/boblampclean.md5mesh").toGPU();
  GPUTexture Tex2 = glre::LoadTexture("resources/marble.jpg"       ).toGPU();
  GPUTexture Tex3 = glre::LoadTexture("resources/SpiderTex.jpg"    ).toGPU();

  Texture T(1024,1024);
  for( uint i = 0; i < 1024; i++)
      for( uint j = 0; j < 1024; j++)
      {
          float f = glm::simplex( 5.f * vec2(i,j) / (1024.0f) );
          T(i,j).r = (unsigned char)(128 + 128*f );
          T(i,j).a = 255;
      }

  auto  Tex4 = T.toGPU();
  //=============================================================================
  // Load the Shaders
  //=============================================================================
  glre::ShaderProgram LineShader( VertexShader("shaders/Line_PC.v")   , FragmentShader("shaders/Line_PC.f")   );
  glre::ShaderProgram S(          VertexShader("shaders/Basic_PNCU.v"), FragmentShader("shaders/Basic_PNCU.f"));
  //=============================================================================



  //=============================================================================
  //  Set the callback methods for to see the texture in the GGUI
  //=============================================================================
  std::dynamic_pointer_cast<rgui::ComboBox>(M["w2"].lock())->insertItem("Dragon")->addCallback( "texture", std::bind( DropDownCallback, std::placeholders::_1, Tex.getID(),   Tex.size()[0],   Tex.size()[1])  );
  std::dynamic_pointer_cast<rgui::ComboBox>(M["w2"].lock())->insertItem("Marble")->addCallback( "texture", std::bind( DropDownCallback, std::placeholders::_1, Tex2.getID(), Tex2.size()[0],  Tex2.size()[1])  );
  std::dynamic_pointer_cast<rgui::ComboBox>(M["w2"].lock())->insertItem("Spider")->addCallback( "texture", std::bind( DropDownCallback, std::placeholders::_1, Tex3.getID(), Tex3.size()[0],  Tex3.size()[1])  );
  std::dynamic_pointer_cast<rgui::ComboBox>(M["w2"].lock())->insertItem("Noise")->addCallback(  "texture", std::bind( DropDownCallback, std::placeholders::_1, Tex4.getID(), Tex4.size()[0],  Tex4.size()[1])  );
  //=============================================================================


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
  glDepthFunc (GL_LESS);    // depth-testing interprets a smaller value as "closer"
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

