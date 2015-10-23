#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <gla/gla.h>
#include <locale>

#include <gla/utils/app.h>
#include <gla/utils/cameracontrol.h>

using namespace gla;


//=================================================================================
// Global Variables and Function Prototypes
//=================================================================================
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024
#define WINDOW_TITLE "TransformSequence"


class TransformSequenceApp : public gla::utils::GLFW_App
{
        // RootApp interface
    public:

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
            mCamControl.InsertMouse( (float)x, (float)y );
        }

        void OnKey(int key, int scancode, int action, int mods)
        {
            mCamControl.InsertKey( key, action );
        }

        void run()
        {
            double t;
            while( !mQuit )
            {

                float dt = mTimer.getElapsedTime(); mTimer.reset();
                t += dt;

                glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                // The camera controller needs to know how much time has passed since
                // the last frame. This is for the acceleration calculations and such
                mCamControl.Calculate( dt );



                // Activate the line shader
                LineShader.useShader();

                // Send the Camera Matrix and the Model matrix for the main Axis.
                LineShader.sendUniform(0, "inCameraMatrix", mCamera.getProjectionMatrix() * mCamera.getMatrix()    );
                LineShader.sendUniform(1, "inModelMatrix" , glm::scale( mat4(1.0), vec3(10.0,10.0,10.0))  );

                // Render the axis
                mAxis.Render();


                // Compute the transformation matrix from the KeyFrames. We want the transformation sequence
                // to repeat which is why we fmod the elapsed time with 4.
                // The getTransformationMatrix will interpolate the keyframes to produce a new matrix
                // that represents the transformation
                auto TransformationMatrix = KeyFrame.getTransformationMatrix( (float)fmod(t,4.0f) );

                // Send the model matrix for the secondary axis
                LineShader.sendUniform(1, "inModelMatrix" , TransformationMatrix );

                // render the axis again.
                mAxis.Render();



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
            LineShader.linkProgram( VertexShader("shaders/Line_PC.v"),  FragmentShader("shaders/Line_PC.f") );


            // Set the Key Frames. The first parameter is the time and the second paramter is the value the key should be.
            KeyFrame.setScaleKey(    0.0f, {1.0,1.0,1.0 });
            KeyFrame.setScaleKey(    3.0f, {3.0,3.0,3.0 });
            KeyFrame.setScaleKey(    4.0f, {1.0,1.0,1.0 });

            KeyFrame.setPositionKey( 0.0f, {2.0,0.0,2.0} );
            KeyFrame.setPositionKey( 2.0f, {4.0,0.0,2.0} );
            KeyFrame.setPositionKey( 3.0f, {0.0,0.0,4.0} );
            KeyFrame.setPositionKey( 4.0f, {2.0,0.0,2.0} );

            KeyFrame.setRotKey( 0.0f, quat( vec3(0, 0,0) ) );         // pitch,yaw,roll = 0
            KeyFrame.setRotKey( 2.0f, quat( vec3(0, 3.14159,0 )) );   // face 180 degrees
            KeyFrame.setRotKey( 4.0f, quat( vec3(0, 2*3.14159,0 )) ); // face 360 degrees

            mAxis  = gla::Solids::createAxes().toGPU();
        }

        bool                      mQuit;
        gla::CameraControl<GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_MOUSE_BUTTON_RIGHT> mCamControl;
        gla::Camera               mCamera;
        gla::Timer_T<double>      mTimer;


        TransformSequence         KeyFrame;



        //--------------- Shaders -----------------------------
        ShaderProgram LineShader;


        //--------------- GPU Objects -------------------------
        GPUArrayObject mAxis;


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

// uses the i'th column in vector x to get rid of the
// i'th column in vector y using a givens rotation
void ApplyGivens( glm::vec4 & x, glm::vec4 & y, int i )
{
    if( fabs(y[i]) < 1e-10 ) return;

    float r = 1.f / sqrt(x[i]*x[i] + y[i]*y[i]);
    float c =  x[i] * r;
    float s = -y[i] * r;

    float a = 0.0;

    a = c*x[0]-s*y[0];
    y[0] = s*x[0]+c*y[0];
    x[0] = a ;

    a = c*x[1]-s*y[1];
    y[1] = s*x[1]+c*y[1];
    x[1] = a ;

    a = c*x[2]-s*y[2];
    y[2] = s*x[2]+c*y[2];
    x[2] = a ;

    a = c*x[3]-s*y[3];
    y[3] = s*x[3]+c*y[3];
    x[3] = a ;

}

void AddRow(glm::mat4 & T, glm::vec4 x)
{
    auto y = x;

    ApplyGivens(T[0], y, 0); // std::cout << y << std::endl;
    ApplyGivens(T[1], y, 1); // std::cout << y << std::endl;
    ApplyGivens(T[2], y, 2);
    ApplyGivens(T[3], y, 3);
}


//==============================
static float PYTHAG(float a, float b)
{
    float at = fabs(a), bt = fabs(b), ct, result;

    if (at > bt)       { ct = bt / at; result = at * sqrt(1.0 + ct * ct); }
    else if (bt > 0.0) { ct = at / bt; result = bt * sqrt(1.0 + ct * ct); }
    else result = 0.0;
    return(result);
}


static int SVD(glm::mat3 & a, glm::vec3 & w, glm::mat3 & v)
{
    #define SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
    #define MIN(x,y) ( (x) < (y) ? (x) : (y) )
    #define MAX(x,y) ((x)>(y)?(x):(y))

    int m = 3;
    int n = 3;
    int flag, i, its, j, jj, k, l, nm;
    float c, f, h, s, x, y, z;
    float anorm = 0.0, g = 0.0, scale = 0.0;


    if (m < n)
    {
        fprintf(stderr, "#rows must be > #cols \n");
        return(0);
    }


    glm::vec3 rv1;

/* Householder reduction to bidiagonal form */
    for (i = 0; i < n; i++)
    {
        /* left-hand reduction */
        l = i + 1;
        rv1[i] = scale * g;
        g = s = scale = 0.0;
        if (i < m)
        {
            for (k = i; k < m; k++)
                scale += fabs((float)a[k][i]);
            if (scale)
            {
                for (k = i; k < m; k++)
                {
                    a[k][i] = (float)((float)a[k][i]/scale);
                    s += ((float)a[k][i] * (float)a[k][i]);
                }
                f = (float)a[i][i];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                a[i][i] = (float)(f - g);
                if (i != n - 1)
                {
                    for (j = l; j < n; j++)
                    {
                        for (s = 0.0, k = i; k < m; k++)
                            s += ((float)a[k][i] * (float)a[k][j]);
                        f = s / h;
                        for (k = i; k < m; k++)
                            a[k][j] += (float)(f * (float)a[k][i]);
                    }
                }
                for (k = i; k < m; k++)
                    a[k][i] = (float)((float)a[k][i]*scale);
            }
        }
        w[i] = (float)(scale * g);

        /* right-hand reduction */
        g = s = scale = 0.0;
        if (i < m && i != n - 1)
        {
            for (k = l; k < n; k++)
                scale += fabs((float)a[i][k]);
            if (scale)
            {
                for (k = l; k < n; k++)
                {
                    a[i][k] = (float)((float)a[i][k]/scale);
                    s += ((float)a[i][k] * (float)a[i][k]);
                }
                f = (float)a[i][l];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                a[i][l] = (float)(f - g);
                for (k = l; k < n; k++)
                    rv1[k] = (float)a[i][k] / h;
                if (i != m - 1)
                {
                    for (j = l; j < m; j++)
                    {
                        for (s = 0.0, k = l; k < n; k++)
                            s += ((float)a[j][k] * (float)a[i][k]);
                        for (k = l; k < n; k++)
                            a[j][k] += (float)(s * rv1[k]);
                    }
                }
                for (k = l; k < n; k++)
                    a[i][k] = (float)((float)a[i][k]*scale);
            }
        }
        anorm = MAX(anorm, (fabs((float)w[i]) + fabs(rv1[i])));
    }

    /* accumulate the right-hand transformation */
    for (i = n - 1; i >= 0; i--)
    {
        if (i < n - 1)
        {
            if (g)
            {
                for (j = l; j < n; j++)
                    v[j][i] = (float)(((float)a[i][j] / (float)a[i][l]) / g);
                    /* double division to avoid underflow */
                for (j = l; j < n; j++)
                {
                    for (s = 0.0, k = l; k < n; k++)
                        s += ((float)a[i][k] * (float)v[k][j]);
                    for (k = l; k < n; k++)
                        v[k][j] += (float)(s * (float)v[k][i]);
                }
            }
            for (j = l; j < n; j++)
                v[i][j] = v[j][i] = 0.0;
        }
        v[i][i] = 1.0;
        g = rv1[i];
        l = i;
    }

    /* accumulate the left-hand transformation */
    for (i = n - 1; i >= 0; i--)
    {
        l = i + 1;
        g = (float)w[i];
        if (i < n - 1)
            for (j = l; j < n; j++)
                a[i][j] = 0.0;
        if (g)
        {
            g = 1.0 / g;
            if (i != n - 1)
            {
                for (j = l; j < n; j++)
                {
                    for (s = 0.0, k = l; k < m; k++)
                        s += ((float)a[k][i] * (float)a[k][j]);
                    f = (s / (float)a[i][i]) * g;
                    for (k = i; k < m; k++)
                        a[k][j] += (float)(f * (float)a[k][i]);
                }
            }
            for (j = i; j < m; j++)
                a[j][i] = (float)((float)a[j][i]*g);
        }
        else
        {
            for (j = i; j < m; j++)
                a[j][i] = 0.0;
        }
        ++a[i][i];
    }

    /* diagonalize the bidiagonal form */
    for (k = n - 1; k >= 0; k--)
    {                             /* loop over singular values */
        for (its = 0; its < 30; its++)
        {                         /* loop over allowed iterations */
            flag = 1;
            for (l = k; l >= 0; l--)
            {                     /* test for splitting */
                nm = l - 1;
                if (fabs(rv1[l]) + anorm == anorm)
                {
                    flag = 0;
                    break;
                }
                if (fabs((float)w[nm]) + anorm == anorm)
                    break;
            }
            if (flag)
            {
                c = 0.0;
                s = 1.0;
                for (i = l; i <= k; i++)
                {
                    f = s * rv1[i];
                    if (fabs(f) + anorm != anorm)
                    {
                        g = (float)w[i];
                        h = PYTHAG(f, g);
                        w[i] = (float)h;
                        h = 1.0 / h;
                        c = g * h;
                        s = (- f * h);
                        for (j = 0; j < m; j++)
                        {
                            y = (float)a[j][nm];
                            z = (float)a[j][i];
                            a[j][nm] = (float)(y * c + z * s);
                            a[j][i] = (float)(z * c - y * s);
                        }
                    }
                }
            }
            z = (float)w[k];
            if (l == k)
            {                  /* convergence */
                if (z < 0.0)
                {              /* make singular value nonnegative */
                    w[k] = (float)(-z);
                    for (j = 0; j < n; j++)
                        v[j][k] = (-v[j][k]);
                }
                break;
            }
            if (its >= 30) {
                fprintf(stderr, "No convergence after 30,000! iterations \n");
                return(0);
            }

            /* shift from bottom 2 x 2 minor */
            x = (float)w[l];
            nm = k - 1;
            y = (float)w[nm];
            g = rv1[nm];
            h = rv1[k];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
            g = PYTHAG(f, 1.0);
            f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;

            /* next QR transformation */
            c = s = 1.0;
            for (j = l; j <= nm; j++)
            {
                i = j + 1;
                g = rv1[i];
                y = (float)w[i];
                h = s * g;
                g = c * g;
                z = PYTHAG(f, h);
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y = y * c;
                for (jj = 0; jj < n; jj++)
                {
                    x = (float)v[jj][j];
                    z = (float)v[jj][i];
                    v[jj][j] = (float)(x * c + z * s);
                    v[jj][i] = (float)(z * c - x * s);
                }
                z = PYTHAG(f, h);
                w[j] = (float)z;
                if (z)
                {
                    z = 1.0 / z;
                    c = f * z;
                    s = h * z;
                }
                f = (c * g) + (s * y);
                x = (c * y) - (s * g);
                for (jj = 0; jj < m; jj++)
                {
                    y = (float)a[jj][j];
                    z = (float)a[jj][i];
                    a[jj][j] = (float)(y * c + z * s);
                    a[jj][i] = (float)(z * c - y * s);
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k] = (float)x;
        }
    }

    return(1);
}

//==============================

int main()
{
    mat4 T(0.0f);

    vec4 x(0,2,3,4);
    vec4 y(2,1,5,-4);

    if(1)
    {
        vec4 x(0,0,0,0);
        vec4 y(2,1,5,-4);
        std::cout << "Testing Givens Rotation on two vectors:" << x << " ,  " << y << std::endl;
        ApplyGivens(x,y,0);
        std::cout << x << std::endl;
        std::cout << y << std::endl;

        std::cout << "--------------------------" << std::endl;
    }

    if(1)
    {
        mat4 T(0.0f);
        vec4 w(1,1,3,4);
        vec4 x(2,1,5,-4);
        vec4 y(-1,4,-3,-4);
        vec4 z(3,-4,3,-2);
        std::cout << "Testing Adding a vector to a matrix" << x << " ,  " << y << std::endl;
        AddRow(T,w);
        std::cout << T << std::endl;
        AddRow(T,x);
        std::cout << T << std::endl;
        AddRow(T,y);
        std::cout << T << std::endl;
        AddRow(T,z);
        std::cout << T << std::endl;
    }

    if(1)
    {
        std::cout << "Testing SVD of a 3x3 matrix" << std::endl;
        glm::mat3 M;
        M[0] = vec3(1,1,3);
        M[1] = vec3(2,1,5);
        M[2] = vec3(-1,4,-3);

        std::cout << M << std::endl;
        std::cout << "--------------------------" << std::endl;
        vec3 Z;
        glm::mat3 V(0.0f);
        SVD(M,Z,V);
        std::cout << "U = " << std::endl;
        std::cout << M*glm::transpose(M) << std::endl;
        std::cout << "Z = " << Z << std::endl;
        std::cout << "V = "<< std::endl;
        std::cout << V*glm::transpose(V) << std::endl;
    }


    //std::cout << T << std::endl;
    //std::cout << "----------------" << std::endl;
    //AddRow(T,y);
    //std::cout << T << std::endl;
    //std::cout << x << std::endl;
    //std::cout << y << std::endl;



   TransformSequenceApp MyApp;

   MyApp.run();

    return 0;

}
