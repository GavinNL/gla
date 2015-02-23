#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <glre/utils/window.h>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


int main(void)
{

    auto W  = glre::utils::Window::create(640,480, "window 1");
    auto W2 = glre::utils::Window::create(640,480, "window 2");

    W->EventsMap["Exit"] = [&] (const glre::utils::Event & E)
    {
        if( E.type == glre::utils::KEY)
        {
            std::cout << " Key pressed: " << E.Key.key << "," << E.Key.scancode << std::endl;
            W->Minimize();
        }
        else if( E.type == glre::utils::MOUSECURSOR)
        {
            std::cout << " Mouse Moved : " << E.MouseCursor.x << "," << E.MouseCursor.y << std::endl;
        }
        else if( E.type == glre::utils::WINDOW)
        {
            if( E.Window.action == glre::utils::FOCUS_GAINED)
                std::cout << " Window gained focus: " << std::endl;
            if( E.Window.action == glre::utils::FOCUS_LOST)
                std::cout << " Window lost focus: " << std::endl;
            if( E.Window.action == glre::utils::MINIMIZED)
                std::cout << " Window minimized: " << std::endl;
            if( E.Window.action == glre::utils::RESTORED)
                std::cout << " Window restored: " << std::endl;
        }
    };


    while( !W->WantsToClose() )
    {
        glre::utils::Window::PollEvents();
    }
    W.reset();

    //glfwTerminate();
    //exit(EXIT_SUCCESS);
    //return 0;

    GLFWwindow* window;


    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
