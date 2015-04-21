#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/gla.h>
#include <gla/utils/window.h>
#include <gla/utils/app.h>
#include <locale>

#include <rgui/rgui.h>


using namespace gla;


class MyApp : public gla::utils::App
{
    public:

    //===================================================================================================================
    // Init - load the json file
    //===================================================================================================================
    virtual void init()
    {
        INFO.fromFile("resources/ExtraInfo.json");

        CreateWindow( (uint)INFO["main"]["width"].as<float>(),  (uint)INFO["main"]["height"].as<float>() , "Texture Array" );

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

        auto size     = mWindow->size();
        mGuiInterface = R->createInterface(  size.x, size.y,
                                             "MainInterface",
                                             INFO["main"]["skin"].as<std::string>() );

        auto M = mGuiInterface->getRootWidget()->loadFromJSON( INFO["main"]["GUI"] );
        for(auto m : M )  std::cout << m.second.lock()->getName() << std::endl;

        //--------------------------------------------------------------------------------------------------------------
        // Set the callback for RGUI.
        //--------------------------------------------------------------------------------------------------------------
        mWindow->EventsMap["GUI"] = [&] (const gla::utils::Event & E)
        {
            switch(E.type)
            {
                case gla::utils::MOUSECURSOR:
                    mGuiInterface->injectMousePosition(rgui::LEFT_BUTTON, E.MouseCursor.x, E.MouseCursor.y);
                    break;

                case gla::utils::TEXT:
                    mGuiInterface->injectCharacters( E.Text.codepoint );
                    break;

                case gla::utils::KEY:
                    mGuiInterface->injectKey( rgui::FromGLFW[ E.Key.key ] , E.Key.action);
                    break;
                case gla::utils::MOUSEBUTTON:
                    const rgui::MouseButton MB[8] = {rgui::LEFT_BUTTON, rgui::RIGHT_BUTTON, rgui::MIDDLE_BUTTON,rgui::NONE,rgui::NONE,rgui::NONE,rgui::NONE, rgui::NONE};
                    mGuiInterface->injectMouseButton( MB[E.MouseButton.button], E.MouseButton.action, E.MouseButton.x, E.MouseButton.y);
                    break;
                break;

             }
        };


        //--------------------------------------------------------------------------------------------------------------
        // Set the callback to control the camera
        //--------------------------------------------------------------------------------------------------------------
        mWindow->EventsMap["CAM"] = [&] (const gla::utils::Event & E)
        {
            float speed = 10.0f;
            switch(E.type)
            {
                case gla::utils::KEY:
                    if(E.Key.mods == GLFW_MOD_SHIFT)
                    {
                        speed = 160.f;
                        mCamera.mMaxSpeed = vec3(20.0f);
                    }
                    else
                        mCamera.mMaxSpeed = vec3(5.0f);
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

                case gla::utils::MOUSECURSOR:
                    if( mWindow->isMouseButtonPressed(gla::utils::MOUSE::RIGHT_MOUSE_BUTTON))
                        mCamera.rotate(   vec3( -(float)E.MouseCursor.dy*0.001,  (float)E.MouseCursor.dx*0.001, 0.0 ) );
                    break;

                case gla::utils::MOUSEBUTTON:
                    switch( E.MouseButton.button )
                    {
                        case GLFW_MOUSE_BUTTON_RIGHT:
                            if( E.MouseButton.action == GLFW_PRESS)
                                mWindow->SetCursorMode( gla::utils::MOUSE::CURSOR_DISABLED);
                            else
                                mWindow->SetCursorMode(  gla::utils::MOUSE::CURSOR_NORMAL );
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

        // load the objects
        Axis  = gla::createAxes().toGPU();
        Box   = gla::createBox( vec3(5.0,5.0,5.0) ).toGPU();

        // setup the camera
        mCamera.perspective(45, 640.0/480.0 ,0.2f, 1000.0f);

        mCamera.setPosition( vec3(5.0,1.0,5.0) );
        mCamera.yaw( -45.0f* 3.14159/180.0f );

        TextureArrayShader.linkProgram( VertexShader("shaders/PNU_TextureArray.v"), FragmentShader("shaders/PNU_TextureArray.f") );

        TArray.create( uvec2(256,256), 3, 1 );

        TextureRGBA T1("resources/greyrock.png");
        TextureRGBA T2("resources/rocks.jpg");
        TextureRGBA T3(256,256);

        T3.r = [] (vec2 x) {
            return glm::perlin( x * 2.0f )*0.5 + 0.5f;
        };

        T3.a = 255;


       // T1.resize( uvec2(32,32) );
        T1.resize( uvec2(256,256) );
        T2.resize( uvec2(256,256) );

        TArray.SetLayer( T1 , 0);
        TArray.SetLayer( T2 , 1);
        TArray.SetLayer( T3 , 2);

        uvec2 s( (uint)INFO["main"]["width"].as<float>(),  (uint)INFO["main"]["height"].as<float>() );
        FBO.create( s );

        auto DropDownCallback = [&] ( const rgui::Button::Callback & C, GLuint ID, uvec2 dim)
        {
             // M["w3"].lock()->setRawRect( 0, 0, 1, 1, ID, dim.x, dim.y );
            mGuiInterface->getWidgetByName<rgui::Widget>("RootWidget_w1_w3")->setRawRect(0, 0, 1, 1,ID, dim.x, dim.y);
        };

        auto gT = FBO.getRenderTexture();

        mGuiInterface->getWidgetByName<rgui::ComboBox>("RootWidget_w1_TextureComboBox")->insertItem("Dragon")->addCallback( "texture", std::bind( DropDownCallback, std::placeholders::_1,  gT.getID(),  gT.size() ) );

    }

    //===================================================================================================================
    // The main render loop
    //===================================================================================================================
    virtual bool render(double dt, double t)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_BLEND_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthFunc(GL_LESS);


        FBO.unbind();
        //glViewport(0, mWindow->size().y, mWindow->size().x, -mWindow->size().y);
        //glViewport(0, 0, 32, 32);

        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        mCamera.calculate(dt);

        //-----------------------------------------------------------------------------
        // Create static variables of some of the uniform locations.
        // This shouldn't be done in practice, it's just convenient for now.
        //-----------------------------------------------------------------------------
        static GLuint TA_ShaderCamMatrixId   = TextureArrayShader.getUniformLocation("inCameraMatrix");
        static GLuint TA_ShaderModelMatrixID = TextureArrayShader.getUniformLocation("inModelMatrix");
        static GLuint TA_ShaderTextureArray  = TextureArrayShader.getUniformLocation("uTextureArray");

        static GLuint LineShaderCamMatrixId   = LineShader.getUniformLocation("inCameraMatrix");
        static GLuint LineShaderModelMatrixID = LineShader.getUniformLocation("inModelMatrix");
        //-----------------------------------------------------------------------------


        TArray.setActiveTexture(0);
        TextureArrayShader.useShader();
            TextureArrayShader.sendUniform_mat4(TA_ShaderCamMatrixId,   mCamera.getProjectionMatrix() * mCamera.getMatrix()  );
            TextureArrayShader.sendUniform_mat4(TA_ShaderModelMatrixID, glm::scale( mat4(1.0), vec3(1.0,1.0,1.0)) );
            TextureArrayShader.sendUniform_Sampler2D(TA_ShaderTextureArray, 0);
        Box.Render();


        LineShader.useShader();
            LineShader.sendUniform_mat4(LineShaderCamMatrixId,   mCamera.getProjectionMatrix() * mCamera.getMatrix()   );
            LineShader.sendUniform_mat4(LineShaderModelMatrixID, glm::scale( mat4(1.0), vec3(5.0,5.0,5.0)) );
        Axis.Render();


        // Draw the RGUI interface
        //FBO.unbind();
        //glViewport( 0, 0 , mWindow->size().x,mWindow->size().y );
        //glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glDisable(GL_CULL_FACE);
        mGuiInterface->draw();

        return true;
    };

    private:
        rgui::json::Value       INFO;
        rgui::pInterface        mGuiInterface;

        gla::Camera            mCamera;


        //===================================================================================================================
        // Textures
        //===================================================================================================================
        gla::GPUTextureArray  TArray;
        gla::GPUTexture        Tex;
        gla::FrameBufferObject FBO;

        //===================================================================================================================
        // Shaders
        //===================================================================================================================
        gla::ShaderProgram   LineShader;
        gla::ShaderProgram   TextureArrayShader;

        //===================================================================================================================
        // OpenGL Objects
        //===================================================================================================================
        gla::GPUArrayObject  Axis;
        gla::GPUArrayObject  Box;
};




///======================================



int main ()
{

//    for(uvec3 &x : LoopRange<uvec3>(uvec3(2,2,2)))
//    {
//        std::cout << x.x << "," << x.y << "," << x.z << "\n";
//    }

     MyApp A;
     A.start();

     return 0;
}

