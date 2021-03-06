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

#ifndef EXP_ARRAY_BUFFER
#define EXP_ARRAY_BUFFER


#include "bufferbase.h"
#include "draw.h"
#include <type_traits>
#include <cstdint>

namespace gla {

template<typename T, typename ...Tail>
struct type_size
{

};

template<typename T>
struct type_size<T>
{
    static const int max   = sizeof(T);
    static const int min   = sizeof(T);
    static const int sum   = sizeof(T);
};

template<typename T, typename Head>
struct type_size<T,Head>
{
    static const int max   = sizeof(T)>sizeof(Head) ? sizeof(T) : sizeof(Head);
    static const int min   = sizeof(T)<sizeof(Head) ? sizeof(T) : sizeof(Head);
    static const int sum   = sizeof(T)+sizeof(Head);
};

template<typename T, typename Head, typename ...Tail>
struct type_size<T,Head,Tail...>
{
    static const int max = sizeof(T) > type_size<Head,Tail...>::max ? sizeof(T) : type_size<Head,Tail...>::max;
    static const int min = sizeof(T) < type_size<Head,Tail...>::min ? sizeof(T) : type_size<Head,Tail...>::min;
    static const int sum = sizeof(T) + type_size<Head,Tail...>::sum;

};

template<typename ...VecTypes>
struct EnableAttributes_T
{
    static void Enable(GLuint i, std::int32_t offset, std::uint32_t NormalizeFlags, int Stride_Leave_as_Default=-1)
    {
    }
};

template<>
struct EnableAttributes_T<>
{
    static void Enable(GLuint i, std::int32_t offset, std::uint32_t NormalizeFlags, int Stride_Leave_as_Default=-1)
    {
        i;offset;NormalizeFlags;Stride_Leave_as_Default; // to supress errors
    }
};


struct Instance_base
{

};

template<typename glm_type, GLuint location=-1, GLuint div=-1>
struct Attr : public Instance_base
{
    using type                  = glm_type;
    static const GLuint index   = location;
    static const GLuint divisor = div;
};



template<typename FirstType, typename ...VecTypes>
struct EnableAttributes_T<FirstType, VecTypes...>
{
    static void Enable(GLuint i, std::int32_t offset, std::uint32_t NormalizeFlags, int Stride_Leave_as_Default=-1)
    {
        #define BUFFER_OFFSET(idx) (static_cast<char*>(0) + (idx))


        const int total_size     = type_size<FirstType, VecTypes...>::sum;

        static_assert(
            ( !std::is_same< FirstType, double       >::value ) ||
            ( !std::is_same< FirstType, glm::dvec2   >::value ) ||
            ( !std::is_same< FirstType, glm::dvec3   >::value ) ||
            ( !std::is_same< FirstType, glm::dvec4   >::value ) ||
            ( !std::is_same< FirstType, float        >::value ) ||
            ( !std::is_same< FirstType, glm::vec2    >::value ) ||
            ( !std::is_same< FirstType, glm::vec3    >::value ) ||
            ( !std::is_same< FirstType, glm::vec4    >::value ) ||
            ( !std::is_same< FirstType, std::int32_t >::value ) ||
            ( !std::is_same< FirstType, glm::ivec2   >::value ) ||
            ( !std::is_same< FirstType, glm::ivec3   >::value ) ||
            ( !std::is_same< FirstType, glm::ivec4   >::value ) ||
            ( !std::is_same< FirstType, std::uint32_t>::value ) ||
            ( !std::is_same< FirstType, glm::uvec2   >::value ) ||
            ( !std::is_same< FirstType, glm::uvec3   >::value ) ||
            ( !std::is_same< FirstType, glm::uvec4   >::value ) ||
            ( !std::is_same< FirstType, std::int16_t >::value ) ||
            ( !std::is_same< FirstType, glm::i16vec2 >::value ) ||
            ( !std::is_same< FirstType, glm::i16vec3 >::value ) ||
            ( !std::is_same< FirstType, glm::i16vec4 >::value ) ||
            ( !std::is_same< FirstType, std::uint16_t>::value ) ||
            ( !std::is_same< FirstType, glm::u16vec2 >::value ) ||
            ( !std::is_same< FirstType, glm::u16vec3 >::value ) ||
            ( !std::is_same< FirstType, glm::u16vec4 >::value ) ||
            ( !std::is_same< FirstType, std::int8_t  >::value ) ||
            ( !std::is_same< FirstType, glm::i8vec2  >::value ) ||
            ( !std::is_same< FirstType, glm::i8vec3  >::value ) ||
            ( !std::is_same< FirstType, glm::i8vec4  >::value ) ||
            ( !std::is_same< FirstType, std::uint8_t >::value ) ||
            ( !std::is_same< FirstType, glm::u8vec2  >::value ) ||
            ( !std::is_same< FirstType, glm::u8vec3  >::value ) ||
            ( !std::is_same< FirstType, glm::u8vec4  >::value )  ,
            "Unknown data type. Valid types are:  float ,glm::vec2 ,glm::vec3 ,glm::vec4 ,std::int32_t ,glm::ivec2 ,glm::ivec3 ,glm::ivec4 ,std::uint32_t ,glm::uvec2 ,glm::uvec3 ,glm::uvec4 ,std::int16_t ,glm::i16vec2 ,glm::i16vec3 ,glm::i16vec4 ,std::uint16_t ,glm::u16vec2 ,glm::u16vec3 ,glm::u16vec4 ,std::int8_t ,glm::i8vec2 ,glm::i8vec3 ,glm::i8vec4 ,std::uint8_t ,glm::u8vec2 ,glm::u8vec3 ,glm::u8vec4" );


        Stride_Leave_as_Default = Stride_Leave_as_Default==-1 ? total_size : Stride_Leave_as_Default;


        // compile-time if statements, will be optimized
        if( std::is_same< FirstType, float>::value )          glVertexAttribPointer(  i, 1 ,          GL_FLOAT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::vec2>::value )      glVertexAttribPointer(  i, 2 ,          GL_FLOAT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::vec3>::value )      glVertexAttribPointer(  i, 3 ,          GL_FLOAT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::vec4>::value )      glVertexAttribPointer(  i, 4 ,          GL_FLOAT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));

        if( std::is_same< FirstType, double>::value )         glVertexAttribLPointer( i, 1 ,          GL_DOUBLE/*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::dvec2>::value )     glVertexAttribLPointer( i, 2 ,          GL_DOUBLE/*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::dvec3>::value )     glVertexAttribLPointer( i, 3 ,          GL_DOUBLE/*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::dvec4>::value )     glVertexAttribLPointer( i, 4 ,          GL_DOUBLE/*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
                                                                                                               /*                           */
                                                                                                               /*                           */
        if( std::is_same< FirstType, std::int32_t>::value )   glVertexAttribIPointer( i, 1 ,            GL_INT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::ivec2>::value )     glVertexAttribIPointer( i, 2 ,            GL_INT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::ivec3>::value )     glVertexAttribIPointer( i, 3 ,            GL_INT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::ivec4>::value )     glVertexAttribIPointer( i, 4 ,            GL_INT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, std::uint32_t>::value )  glVertexAttribIPointer( i, 1 ,   GL_UNSIGNED_INT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::uvec2>::value )     glVertexAttribIPointer( i, 2 ,   GL_UNSIGNED_INT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::uvec3>::value )     glVertexAttribIPointer( i, 3 ,   GL_UNSIGNED_INT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::uvec4>::value )     glVertexAttribIPointer( i, 4 ,   GL_UNSIGNED_INT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
                                                                                                               /*                           */
        if( std::is_same< FirstType, std::int16_t>::value )   glVertexAttribIPointer( i, 1 ,          GL_SHORT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i16vec2>::value )   glVertexAttribIPointer( i, 2 ,          GL_SHORT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i16vec3>::value )   glVertexAttribIPointer( i, 3 ,          GL_SHORT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i16vec4>::value )   glVertexAttribIPointer( i, 4 ,          GL_SHORT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, std::uint16_t>::value )  glVertexAttribIPointer( i, 1 , GL_UNSIGNED_SHORT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u16vec2>::value )   glVertexAttribIPointer( i, 2 , GL_UNSIGNED_SHORT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u16vec3>::value )   glVertexAttribIPointer( i, 3 , GL_UNSIGNED_SHORT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u16vec4>::value )   glVertexAttribIPointer( i, 4 , GL_UNSIGNED_SHORT /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
                                                                                                               /*                           */
        if( std::is_same< FirstType, std::int8_t>::value )    glVertexAttribIPointer( i, 1 ,           GL_BYTE /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i8vec2>::value )    glVertexAttribIPointer( i, 2 ,           GL_BYTE /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i8vec3>::value )    glVertexAttribIPointer( i, 3 ,           GL_BYTE /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i8vec4>::value )    glVertexAttribIPointer( i, 4 ,           GL_BYTE /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, std::uint8_t>::value )   glVertexAttribIPointer( i, 1 ,  GL_UNSIGNED_BYTE /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u8vec2>::value )    glVertexAttribIPointer( i, 2 ,  GL_UNSIGNED_BYTE /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u8vec3>::value )    glVertexAttribIPointer( i, 3 ,  GL_UNSIGNED_BYTE /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u8vec4>::value )    glVertexAttribIPointer( i, 4 ,  GL_UNSIGNED_BYTE /*, ((NormalizeFlags>>i)&01) */, Stride_Leave_as_Default, BUFFER_OFFSET(offset));

        ///

        if( std::is_same< FirstType, double>::value )         GLA_LOGV << "glVertexAttribPointer(" << i << ", 1 ,          GL_DOUBLE," << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::dvec2>::value )     GLA_LOGV << "glVertexAttribPointer(" << i << ", 2 ,          GL_DOUBLE," << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::dvec3>::value )     GLA_LOGV << "glVertexAttribPointer(" << i << ", 3 ,          GL_DOUBLE," << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::dvec4>::value )     GLA_LOGV << "glVertexAttribPointer(" << i << ", 4 ,          GL_DOUBLE," << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, float>::value )          GLA_LOGV << "glVertexAttribPointer(" << i << ", 1 ,          GL_FLOAT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::vec2>::value )      GLA_LOGV << "glVertexAttribPointer(" << i << ", 2 ,          GL_FLOAT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::vec3>::value )      GLA_LOGV << "glVertexAttribPointer(" << i << ", 3 ,          GL_FLOAT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::vec4>::value )      GLA_LOGV << "glVertexAttribPointer(" << i << ", 4 ,          GL_FLOAT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, std::int32_t>::value )   GLA_LOGV << "glVertexAttribIPointer(" << i << ", 1 ,            GL_INT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::ivec2>::value )     GLA_LOGV << "glVertexAttribIPointer(" << i << ", 2 ,            GL_INT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::ivec3>::value )     GLA_LOGV << "glVertexAttribIPointer(" << i << ", 3 ,            GL_INT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::ivec4>::value )     GLA_LOGV << "glVertexAttribIPointer(" << i << ", 4 ,            GL_INT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, std::uint32_t>::value )  GLA_LOGV << "glVertexAttribIPointer(" << i << ", 1 ,   GL_UNSIGNED_INT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::uvec2>::value )     GLA_LOGV << "glVertexAttribIPointer(" << i << ", 2 ,   GL_UNSIGNED_INT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::uvec3>::value )     GLA_LOGV << "glVertexAttribIPointer(" << i << ", 3 ,   GL_UNSIGNED_INT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::uvec4>::value )     GLA_LOGV << "glVertexAttribIPointer(" << i << ", 4 ,   GL_UNSIGNED_INT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, std::int16_t>::value )   GLA_LOGV << "glVertexAttribIPointer(" << i << ", 1 ,          GL_SHORT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::i16vec2>::value )   GLA_LOGV << "glVertexAttribIPointer(" << i << ", 2 ,          GL_SHORT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::i16vec3>::value )   GLA_LOGV << "glVertexAttribIPointer(" << i << ", 3 ,          GL_SHORT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::i16vec4>::value )   GLA_LOGV << "glVertexAttribIPointer(" << i << ", 4 ,          GL_SHORT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, std::uint16_t>::value )  GLA_LOGV << "glVertexAttribIPointer(" << i << ", 1 , GL_UNSIGNED_SHORT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::u16vec2>::value )   GLA_LOGV << "glVertexAttribIPointer(" << i << ", 2 , GL_UNSIGNED_SHORT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::u16vec3>::value )   GLA_LOGV << "glVertexAttribIPointer(" << i << ", 3 , GL_UNSIGNED_SHORT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::u16vec4>::value )   GLA_LOGV << "glVertexAttribIPointer(" << i << ", 4 , GL_UNSIGNED_SHORT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, std::int8_t>::value )    GLA_LOGV << "glVertexAttribIPointer(" << i << ", 1 ,           GL_BYTE, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::i8vec2>::value )    GLA_LOGV << "glVertexAttribIPointer(" << i << ", 2 ,           GL_BYTE, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::i8vec3>::value )    GLA_LOGV << "glVertexAttribIPointer(" << i << ", 3 ,           GL_BYTE, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::i8vec4>::value )    GLA_LOGV << "glVertexAttribIPointer(" << i << ", 4 ,           GL_BYTE, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, std::uint8_t>::value )   GLA_LOGV << "glVertexAttribIPointer(" << i << ", 1 ,  GL_UNSIGNED_BYTE, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::u8vec2>::value )    GLA_LOGV << "glVertexAttribIPointer(" << i << ", 2 ,  GL_UNSIGNED_BYTE, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::u8vec3>::value )    GLA_LOGV << "glVertexAttribIPointer(" << i << ", 3 ,  GL_UNSIGNED_BYTE, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;
        if( std::is_same< FirstType, glm::u8vec4>::value )    GLA_LOGV << "glVertexAttribIPointer(" << i << ", 4 ,  GL_UNSIGNED_BYTE, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset) << " );" << std::endl;




        if( std::is_same< FirstType, glm::mat4>::value)
        {
            glVertexAttribPointer( i  , 4 , GL_FLOAT, GL_FALSE , Stride_Leave_as_Default, BUFFER_OFFSET(offset)  );
            glVertexAttribPointer( i+1, 4 , GL_FLOAT, GL_FALSE , Stride_Leave_as_Default, BUFFER_OFFSET(offset + sizeof(GLfloat) * 4 * 1  ) );
            glVertexAttribPointer( i+2, 4 , GL_FLOAT, GL_FALSE , Stride_Leave_as_Default, BUFFER_OFFSET(offset + sizeof(GLfloat) * 4 * 2  ) );
            glVertexAttribPointer( i+3, 4 , GL_FLOAT, GL_FALSE , Stride_Leave_as_Default, BUFFER_OFFSET(offset + sizeof(GLfloat) * 4 * 3  ) );


            GLA_LOGV << "glVertexAttribPointer(" << i   << ", 4 ,          GL_FLOAT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset)                             << ");" << std::endl;
            GLA_LOGV << "glVertexAttribPointer(" << i+1 << ", 4 ,          GL_FLOAT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset + sizeof(GLfloat) * 4 * 1  ) << ");" << std::endl;
            GLA_LOGV << "glVertexAttribPointer(" << i+2 << ", 4 ,          GL_FLOAT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset + sizeof(GLfloat) * 4 * 2  ) << ");" << std::endl;
            GLA_LOGV << "glVertexAttribPointer(" << i+3 << ", 4 ,          GL_FLOAT, " << ((NormalizeFlags>>i)&01) << ", " << Stride_Leave_as_Default << ", " << (std::intptr_t)BUFFER_OFFSET(offset + sizeof(GLfloat) * 4 * 3  ) << ");" << std::endl;
        }


        glEnableVertexAttribArray(i);

        EnableAttributes_T<VecTypes...>::Enable(i+1, offset + sizeof(FirstType) , NormalizeFlags , Stride_Leave_as_Default);
    }
};


template <typename... GLM_Vec_Types>
static void EnableAttributes( NormalizeFlags normalizeFlags = NormalizeFlags::none, int start_attrib=0 )
{
    gla::EnableAttributes_T<GLM_Vec_Types...>::Enable(start_attrib, 0, normalizeFlags._flags);
}

class ArrayBuffer : public Buffer
{
    public:
        static BufferBindTarget const BindTarget = BufferBindTarget::ARRAY_BUFFER;

        ArrayBuffer() : Buffer() {}
        ArrayBuffer(std::size_t size_in_bytes) : Buffer(size_in_bytes)
        {}

        template<typename VertexData>
        ArrayBuffer( const std::vector<VertexData> & data, BufferUsage usage = BufferUsage::STATIC_DRAW) : Buffer(data, usage)
        {}

        /**
         * @brief Bind
         * Binds this handle as the current GL_ARRAY_BUFFER
         */
        void Bind() const
        {
            Buffer::Bind( *this, BindTarget);
        }

        /**
         * @brief Unbind
         * Unbinds any current GL_ARRAY_BUFFER
         */
        void Unbind() const
        {
            Buffer::Unbind(  BindTarget );
        }

        GL_id_type CurrentlyBound() const
        {
            GLint id;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id );
            return id;
        }


        template<bool BindFirst=true>
        /**
         * @brief Draw
         * @param p - the pimitave type (LINES, TRIANGLES, TRIANGLE_FAN, etc)
         * @param first - the index of the first
         * @param NumberOfVertices - number of vertices to render.
         *
         * Use the BindFirst=false template paramter to not automatically bind before each call.
         * This is useful if are making a more optimized rendering engine.
         */
        void Draw( Primitave p,  std::size_t NumberOfVertices, std::size_t first_vertex) const
        {
            if(BindFirst) Bind();
            gla::DrawArrays( p,  NumberOfVertices,  first_vertex );
        }

        /**
         * @brief EnableAttributes
         * @param NormalizeFlags - flags which indicate which attributes should be normalized.
         *                         eg:  NormalizeFlags::_0 |  NormalizeFlags::_3
         *
         * Tells OpenGL how the attributes in the array_buffer should be read as, eg:
         *
         * Given an array_buffer, where each vertex is composed of position(vec3), uv_coords(vec2), normal(vec3), and colour(vec4)
         * where the normal must be normalized
         *
         * Then we can tell openGL to read the array buffer by calling the following function.
         *
         * EnableAttributes<glm::vec3, glm::vec2, glm::vec3, glm::vec4>(NormalizeFlags::_2);
         */
        template <typename... GLM_Vec_Types>
        void EnableAttributes( NormalizeFlags normalizeFlags = NormalizeFlags::none ) const
        {
            Bind();
            gla::EnableAttributes<GLM_Vec_Types...>(normalizeFlags);
        }


        template <typename... GLM_Vec_Types>
        void EnableAttributes( int StartAttrib, NormalizeFlags normalizeFlags = NormalizeFlags::none ) const
        {
            Bind();
            gla::EnableAttributes<GLM_Vec_Types...>(normalizeFlags,StartAttrib);
        }

        template <typename GLM_Vec_Type>
        void EnableDivisor( int AttribIndex, GLuint divisor ) const
        {
            Bind();
            EnableAttributes<GLM_Vec_Type>( AttribIndex );
            glVertexAttribDivisor(AttribIndex, divisor);
            GLA_LOGV << "glVertexAttribDivisor(" << AttribIndex   << ", " << divisor << ");" << std::endl;
        }
};




}

#endif
