#ifndef GLA_EXCEPTIONS_H
#define GLA_EXCEPTIONS_H

#include <exception>
#include <string>

namespace gla
{

    class GLA_EXCEPTION : public std::exception
    {
        public:
            GLA_EXCEPTION(){};
            GLA_EXCEPTION(const std::string & desc) { _what = desc; };
            virtual const char * what() const throw() { return _what.c_str(); }
            std::string _what;
    };

    class GPU_OUT_OF_MEMORY : public GLA_EXCEPTION
    {
    };

}
#endif // EXCEPTIONS_H
