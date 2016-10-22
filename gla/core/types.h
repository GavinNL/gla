/*
 * MIT License
 *
 * Copyright (c) [year] [fullname]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef EXP_TYPES
#define EXP_TYPES





#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

#ifndef GLA_DOUT
#define GLA_DOUT std::cout
#endif

namespace gla
{

class NormalizeFlags
{
    public:
    enum Flags : unsigned int
    {
        none = 0,
        _0   = (1<<0),
        _1   = (1<<1),
        _2   = (1<<2),
        _3   = (1<<3),
        _4   = (1<<4),
        _5   = (1<<5),
        _6   = (1<<6),
        _7   = (1<<7),
        _8   = (1<<8),
        _9   = (1<<9),
        _10  = (1<<10),
        _11  = (1<<11),
        _12  = (1<<12),
        _13  = (1<<13),
        _14  = (1<<14),
        _15  = (1<<15),
        _16  = (1<<16),
        _17  = (1<<17),
        _18  = (1<<18),
        _19  = (1<<19)
    };

    NormalizeFlags(unsigned int f)
    {
        _flags = f;
    }

    NormalizeFlags( Flags flags)
    {
        _flags = static_cast<unsigned int>(flags);
    }

    unsigned int _flags;
};

inline NormalizeFlags::Flags operator | (const NormalizeFlags::Flags & left, const NormalizeFlags::Flags & right )
{
    return static_cast<NormalizeFlags::Flags>( static_cast<unsigned int>(left) | static_cast<unsigned int>(right) );
}

enum class DataType
{
    UNKNOWN        = -1,
    BYTE           = GL_BYTE ,
    UNSIGNED_BYTE  = GL_UNSIGNED_BYTE ,
    SHORT          = GL_SHORT ,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT ,
    INT            = GL_INT ,
    UNSIGNED_INT   = GL_UNSIGNED_INT ,
    FLOAT          = GL_FLOAT ,
    DOUBLE         = GL_DOUBLE
};

enum class Primitave
{
    UNKNOWN_PRIMITAVE       = -1,
    LINES                   = GL_LINES,
    LINE_LOOP               = GL_LINE_LOOP,
//    POINT_BIT               = GL_POINT_BIT,
//    CLIENT_VERTEX_ARRAY_BIT = GL_CLIENT_VERTEX_ARRAY_BIT,
    LINE_STRIP              = GL_LINE_STRIP,
//    LINE_BIT                = GL_LINE_BIT,
    TRIANGLES               = GL_TRIANGLES,
    TRIANGLE_STRIP          = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN            = GL_TRIANGLE_FAN,
//    QUADS                   = GL_QUADS,
//    QUAD_STRIP              = GL_QUAD_STRIP,
    PATCHES                 = GL_PATCHES

};


using sizei = GLsizei;

using bvec2 = glm::bvec2 ;
using bvec3 = glm::bvec3 ;
using bvec4 = glm::bvec4 ;
using vec2  = glm::vec2  ;
using vec3  = glm::vec3  ;
using vec4  = glm::vec4  ;
using uvec4 = glm::uvec4 ;
using uvec3 = glm::uvec3 ;
using uvec2 = glm::uvec2 ;
using ivec2 = glm::ivec2 ;
using ivec3 = glm::ivec3 ;
using ivec4 = glm::ivec4 ;
using mat2  = glm::mat2  ;
using mat3  = glm::mat3  ;
using mat4  = glm::mat4  ;
using quat  = glm::quat  ;
using ucol4 = glm::u8vec4;
using ucol3 = glm::u8vec3;
using ucol2 = glm::u8vec2;
using ucol1 = glm::u8vec1;
using col4  = glm::vec4  ;
using col3  = glm::vec3  ;
using col2  = glm::vec2  ;

static std::ostream& print_time()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "[" << std::put_time(std::localtime(&now_c), "%F %T") << "] - ";

    return std::cout;
}



#ifdef GLA_LOG_ALL
    #define GLA_INFO 1
    #define GLA_DEBUG 1
    #define GLA_VERBOSE 1
    #define GLA_TIMER 1
#endif


#ifdef GLA_INFO
    #define GLA_INFO 1
#else
    #define GLA_INFO 0
#endif

#ifdef GLA_DEBUG
    #define GLA_DEBUG 1
#else
    #define GLA_DEBUG 0
#endif

#ifdef GLA_VERBOSE
    #define GLA_VERBOSE 1
#else
    #define GLA_VERBOSE 0
#endif

#ifdef GLA_TIMER
    #define GLA_TIMER 1
#else
    #define GLA_TIMER 0
#endif

#if 0
#include <iostream>
#include <windows.h>

using namespace std;
HANDLE hCon;

enum Color { DARKBLUE = 1, DARKGREEN, DARKTEAL, DARKRED, DARKPINK, DARKYELLOW, GRAY, DARKGRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE };

void SetColor(Color c){
        if(hCon == NULL)
                hCon = GetStdHandle(STD_OUTPUT_HANDLE);

}

#endif

static const std::string default_console = "\033[0m";
static const std::string color           = "\033[0;1m";

static const std::string c_black_b      = "\033[1;30m";
static const std::string c_red_b        = "\033[1;31m";
static const std::string c_green_b      = "\033[1;32m";
static const std::string c_brown_b      = "\033[1;33m";
static const std::string c_blue_b       = "\033[1;34m";
static const std::string c_magenta_b    = "\033[1;35m";
static const std::string c_cyan_b       = "\033[1;36m";
static const std::string c_lightgray_b  = "\033[1;37m";

static const std::string c_black      = "\033[0;30m";
static const std::string c_red        = "\033[0;31m";
static const std::string c_green      = "\033[0;32m";
static const std::string c_brown      = "\033[0;33m";
static const std::string c_blue       = "\033[0;34m";
static const std::string c_magenta    = "\033[0;35m";
static const std::string c_cyan       = "\033[0;36m";
static const std::string c_lightgray  = "\033[0;37m";

#define GLA_LOGD  if(GLA_DEBUG  && (std::cout<<c_red_b))     gla::print_time() <<  "[Debug] - "
#define GLA_LOGI  if(GLA_INFO   && (std::cout<<c_green_b))   gla::print_time() << "[Info] - "
#define GLA_LOGV  if(GLA_VERBOSE&& (std::cout<<c_cyan_b))    gla::print_time() << "[Verbose] - "
#define GLA_LOGT  if(GLA_TIMER&& (std::cout<<c_magenta_b))    gla::print_time() << "[Timer] - "



#define DO_ONCE(  A )       \
    {                          \
        static bool ____iii____=true;    \
        if(____iii____)                  \
        {                      \
            ____iii____=false;

#define END_ONCE \
            A                  \
        }                      \
    }

#define DO_EVERY( seconds  )              \
    {                               \
        static std::chrono::system_clock::time_point start = std::chrono::system_clock::now(); \
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now()-start;\
        if( elapsed_seconds.count() > seconds )   \
        {   start = std::chrono::system_clock::now();                     \


#define END_EVERY              \
                               \
        }                      \
    }




#define START_TIMER()              \
    {                               \
        static std::chrono::system_clock::time_point start = std::chrono::system_clock::now(); \
        {   start = std::chrono::system_clock::now();


#define END_TIMER(Description)              \
                               \
            std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now()-start; \
            GLA_LOGT << Description << " : " << elapsed_seconds.count() << " seconds" << std::endl; \
        }\
    }



} // gla namespace

#endif
