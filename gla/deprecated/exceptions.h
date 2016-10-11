#ifndef GLA_EXCEPTIONS_H
#define GLA_EXCEPTIONS_H

#include <exception>
#include <string>

namespace gla
{

    inline void GetGLError()
    {
        auto err = glGetError();
        switch(err)
        {
            case GL_INVALID_OPERATION:
                throw std::runtime_error("ERROR: GL_INVALID_OPERATION\n");
                break;
            case GL_INVALID_ENUM:
                throw std::runtime_error("ERROR: GL_INVALID_ENUM\n");
                break;
            case GL_INVALID_VALUE:
                throw std::runtime_error("ERROR: GL_INVALID_VALUE\n");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                throw std::runtime_error("ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n");
                break;
            case GL_OUT_OF_MEMORY:
                throw std::runtime_error("ERROR: GL_OUT_OF_MEMORY\n");
                break;
            case GL_STACK_UNDERFLOW:
                throw std::runtime_error("ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n");
                break;
            case GL_STACK_OVERFLOW:
                throw std::runtime_error("ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n");
                break;

            default:
                break;
        }
    }

}
#endif // EXCEPTIONS_H
