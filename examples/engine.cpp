#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/gla.h>
#include <locale>

#include <gla/utils/app.h>
#include <gla/engine/cameracontrol.h>
#include <gla/engine/engine.h>

#include <gla/engine/stackgraph.h>

using namespace gla;
using namespace gla::engine;

//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define WINDOW_TITLE "TransformSequence"




/**
 * @brief The Standard_PUN class
 *
 * Standard material for meshs that contain Position, UV and normals with 1 texture
 */
class Standard_PUN : public Material
{
    public:

        Standard_PUN()
        {

        }

        virtual void use() override
        {
        };


};

class TransformSequenceApp : public gla::utils::GLFW_App
{

    // RootApp interface
    public:
        struct u140_Global {
            glm::mat4   ub_CameraMatrix;
            glm::mat4   ub_ProjectionMatrix;

            glm::vec2   ub_Viewport;          // viewport resolution (in pixels)
            glm::vec2   ub_FrameGlobalTime;   // .x = frame time, .y = total time

            glm::vec2   ub_MousePosition;

        };

        u140_Global      u140_Buffer;

        // Some unused callback functions. We don't really need them.
        void OnWindowSize(int width, int height){}
        void OnFramebufferSize(int width, int height){}
        void OnRefresh(){ }
        void OnFocus(int focused){}
        void OnIconify(int iconified){}
        void OnMouseEnter(int entered){}
        void OnScroll(double x, double y){}
        void OnCharacter(unsigned int codepoint){}
        void OnCharacterMods(unsigned int codepoint, int mods){}
        void OnDrop(int count, const char **paths){}
        void OnWindowPosition(int x, int y){}

        void OnClose()
        {
            mQuit=true;
        }

        // Send the keyboard/mouse input to the camera controller
        void OnMouseButton(int button, int action, int mods)
        {
            mCamControl.InsertButton(button, action);
        }

        void OnMousePosition(double x, double y)
        {
            u140_Buffer.ub_MousePosition = {(float)x, (float)y };
            mCamControl.InsertMouse( (float)x, (float)y );
        }

        void OnKey(int key, int scancode, int action, int mods)
        {
            mCamControl.InsertKey( key, action );
        }

        void run()
        {
            double t;


            GPUUniformBuffer GPU_GlobalUniform( sizeof(u140_Global) );

            auto & StandardShader = mShaders["standard"];
            auto BlockIndex       = StandardShader.GetUniformBlockIndex("u140_Global");
                                    StandardShader.BindUniformBuffer(GPU_GlobalUniform, BlockIndex, 1);



            Scene Sc;

            auto root = Sc.GetRootNode();

            auto b = root->CreateChild<MeshNode>();

            root->AddComponent<NodeComponent>();

            mAssets["box"]    = { &mMeshes["box"], nullptr};
            b->AttachModel( &mAssets["box"] );


            while( !mQuit )
            {
                float dt = mTimer.getElapsedTime(); mTimer.reset();
                t += dt;

                glEnable(GL_DEPTH_TEST);
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);

                glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                // The camera controller needs to know how much time has passed since
                // the last frame. This is for the acceleration calculations and such
                mCamControl.Calculate( dt );

                u140_Buffer.ub_CameraMatrix     = mCamera.getMatrix();
                u140_Buffer.ub_ProjectionMatrix = mCamera.getProjectionMatrix();
                u140_Buffer.ub_Viewport         = { WINDOW_WIDTH, WINDOW_HEIGHT};
                u140_Buffer.ub_FrameGlobalTime  = { dt, t};

                GPU_GlobalUniform.CopyData(u140_Buffer);

                // Activate the line shader

                GPUTexture::SetActiveTexture(mTextures["stone"], 0);

                StandardShader.useShader();
                StandardShader.sendUniform(0, "u_TransformMatrix" , glm::scale( mat4(1.0), vec3(10.0,10.0,10.0)) );
                StandardShader.sendUniform(1, "u_Texture"         , 0);

                mMeshes["box"].Render(QUADS);


                PollEvents();
                SwapBuffers();
            }
        }




        TransformSequenceApp() : gla::utils::GLFW_App( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)
        {
            // Some initial setup
            mQuit = false;
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


            // Set up the camera.
            mCamera.perspective(45, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT ,0.2f, 1000.0f);
            mCamera.setPosition( vec3(2.0,  2.0,  3.0) );
            mCamera.setEuler(    vec3(-15,  0.0 , 0.0) * (3.14159f / 180.0f) );

            // attack the camera to the camera controller
            mCamControl.attachCamera(mCamera);

            // compile the shader
            mShaders["standard"] = ShaderProgram( VertexShader("shaders/Standard_PUN.v"),  FragmentShader("shaders/Standard_PUN.f") );

            mMeshes["axis"]     = gla::Solids::createAxes().toGPU();
            mMeshes["box"]      = gla::Solids::createBox().toGPU();

            mTextures["stone"]  = Texture("resources/rocks1024.jpg",4).toGPU();
        }

        bool                      mQuit;
        gla::CameraControl<GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_MOUSE_BUTTON_RIGHT> mCamControl;
        gla::Camera               mCamera;
        gla::Timer_T<double>      mTimer;


        TransformSequence         KeyFrame;


        //--------------- Containers for our GPU stuff--------------
        std::map<std::string, ShaderProgram>  mShaders;
        std::map<std::string, GPUArrayObject> mMeshes;
        std::map<std::string, GPUTexture>     mTextures;
        std::map<std::string, ModelAsset>     mAssets;
        std::map<std::string, Material*>      mMaterials;



};


std::ostream& operator <<(std::ostream & out, const glm::vec3 & v)
{
    out << "{" << v[0] << "," << v[1] << "," << v[2] << "}";
    return out;
}

std::ostream& operator <<(std::ostream & out, const glm::vec4 & v)
{
    out << "{" << v[0] << "," << v[1] << "," << v[2] << "," << v[3] << "}";
    return out;
}

std::ostream& operator <<(std::ostream & out, const glm::mat4 & v)
{
    out << v[0] << std::endl;
    out << v[1] << std::endl;
    out << v[2] << std::endl;
    out << v[3] << std::endl;

    return out;
}

std::ostream& operator <<(std::ostream & out, const glm::mat3 & v)
{
    out << v[0] << std::endl;
    out << v[1] << std::endl;
    out << v[2] << std::endl;

    return out;
}

//==============================

struct SNode
{
    Transformation local;
    Transformation world;
};

class SceneGraph : public StackGraph<SNode>
{
public:
    /**
     * @brief transformChild
     * @param id
     * @param parentTransform
     * Transforms a node with a given transform and updates the world transform of the node
     * and all child nodes
     */
    void transformChild(SceneGraph::NodeID id, const Transformation& parentTransform)
    {
        std::cout << "Transforming child: " << id << std::endl;
        Get(id).world = parentTransform * Get(id).local;

        SceneGraph::NodeID child = GetFirstChild(id);
        while (IsValid(child))
        {
            transformChild(child, Get(id).world);
            child = GetNextSibling(child);
        }
    }

    /**
     * @brief updateLocal
     * @param id
     * Updates the world transform of the node and all children as well
     */
    void updateLocal(SceneGraph::NodeID id)
    {
        std::cout << "Updating local : " << id << std::endl;
        SceneGraph::NodeID parentId = GetParent(id);
        std::cout << "   parent : " << parentId << std::endl;
        Transformation parentTransform;

        if (IsValid(parentId))
            parentTransform = Get(parentId).world;

        transformChild(id, parentTransform);
    }

    /**
     * @brief updateGlobal
     * @param id
     * Updates the local transformation based on any changes to the world transformation
     */
    void updateWorld(SceneGraph::NodeID id)
    {
        SceneGraph::NodeID parentId           = GetParent(id);

        Transformation parentTransform;

        if (IsValid(parentId))
            parentTransform = Get(parentId).world;//data.global[parentId];

        Get(id).local = Get(id).world / parentTransform;
        transformChild(id, parentTransform);
    }
};


int main()
{
//    auto q = quat(0,0,0,1);
//    auto n = quat();
//    std::cout << q.x << q.y << q.z << q.w << std::endl;
//    std::cout << n.x << n.y << n.z << n.w << std::endl;
//    vec3 p = vec3(1,2,3) + quat(0,0,0,1) * (vec3(1,1,1) * vec3(1,1,1));
//    std::cout << p << std::endl;
//
//    return 0;
    SceneGraph sg;


    auto R = sg.Create();
    auto c = sg.Create(R); sg.Get(c).local = Transformation({1,1,1});
    sg.updateLocal(R);
    std::cout << "Updating local:" << std::endl;
    std::cout << sg.Get(c).world.mPosition << std::endl;
    sg.transformChild( R, Transformation({1,2,3}));
    std::cout << sg.Get(c).world.mPosition;
    return 0;
    TransformSequenceApp MyApp;

    MyApp.run();

    return 0;

}
