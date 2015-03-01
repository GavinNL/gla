#ifndef GLRE_EXCEPTIONS_H
#define GLRE_EXCEPTIONS_H

#include <exception>
#include <string>

namespace glre
{

    class GLRE_EXCEPTION : public std::exception
    {
        public:
            GLRE_EXCEPTION(){};
            GLRE_EXCEPTION(const std::string & desc) { _what = desc; };
            virtual const char * what() const throw() { return _what.c_str(); }
            std::string _what;
    };

    class GPU_OUT_OF_MEMORY : public GLRE_EXCEPTION
    {
    };

}
#endif // EXCEPTIONS_H
