#ifndef GLA_EXCEPTIONS_H
#define GLA_EXCEPTIONS_H

#include <exception>
#include <string>

namespace gla
{

    class GLA_EXCEPTION : public std::exception
    {
        public:
            GLA_EXCEPTION(){}
            GLA_EXCEPTION(const std::string & desc) { _what = desc; }
            virtual const char * what() const throw() { return _what.c_str(); }
            std::string _what;
    };

    class GPU_OUT_OF_MEMORY : public GLA_EXCEPTION
    {
    };


    inline void GetGLError()
    {
        auto err = glGetError();
        switch(err)
        {
            case GL_INVALID_OPERATION:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_OPERATION\n");
                break;
            case GL_INVALID_ENUM:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_ENUM\n");
                break;
            case GL_INVALID_VALUE:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_VALUE\n");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n");
                break;
            case GL_OUT_OF_MEMORY:
                throw gla::GLA_EXCEPTION("ERROR: GL_OUT_OF_MEMORY\n");
                break;
            case GL_STACK_UNDERFLOW:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n");
                break;
            case GL_STACK_OVERFLOW:
                throw gla::GLA_EXCEPTION("ERROR: GL_INVALID_FRAMEBUFFER_OPERATION\n");
                break;

            default:
                break;
        }
    }

}
#endif // EXCEPTIONS_H
