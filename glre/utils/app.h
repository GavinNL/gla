#ifndef GLRE_UTILS_APP_H
#define GLRE_UTILS_APP_H

#include "glre/utils/window.h"
#include "glre/timer.h"

namespace glre  {
namespace utils {

class App {

    public:


        void CreateWindow( unsigned int width, unsigned height, const char * title )
        {
            mWindow = glre::utils::Window::create(width, height, title);
        };

        ~App()
        {
            glfwTerminate();
        }

        virtual void init( ) { };

        void start()
        {
            this->init();

            Timer_T <double> T;

            double dt = 0.0;
            double t  = 0.0;

            while( !mWindow->WantsToClose() )
            {
                glre::utils::Window::PollEvents();

                this->render(dt, t);

                mWindow->SwapBuffers();

                dt = T.getElapsedTime();
                t += dt;
                T.reset();
            }


        }

        std::shared_ptr<Window> getWindow() { return mWindow; }

        virtual bool render(double dt, double t){ return false; };
        virtual void clean() {  };


    protected:
        std::shared_ptr<Window> mWindow;

};

};
};

#endif // APP_H

