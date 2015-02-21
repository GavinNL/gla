#ifndef GLRE_EXCEPTIONS_H
#define GLRE_EXCEPTIONS_H

#include <exception>

namespace glre
{

    class GPU_OUT_OF_MEMORY : public std::exception
    {
        public:
            virtual const char * what() const throw() { return "OpenGL has returned a GL_OUT_OF_MEMORY code."; }
    };

}
#endif // EXCEPTIONS_H
