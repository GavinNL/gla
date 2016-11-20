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

#ifndef GLA_EXP_SHADER_H
#define GLA_EXP_SHADER_H

#include "types.h"
#include "handle.h"

#include <type_traits>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <utility>

namespace gla {

template<GLenum SHADER_TYPE>
struct GenShaderUnit
{
    void operator()(GLuint & x)
    {
        x = glCreateShader( SHADER_TYPE );
        GLA_LOGD << "Shader Unit Generated: " << x << std::endl;
    }
};

struct DestShaderUnit
{
    void operator()(GLuint & x)
    {
        GLA_LOGD << "Destroying Shader Unit: " << x << std::endl;
        glDeleteShader( x );
        x = 0;
        //glDeleteFramebuffers( 1, &x );
        //x = 0;
    }
};



template<GLenum SHADER_TYPE>
class ShaderUnit : public BaseHandle<GLuint, GenShaderUnit<SHADER_TYPE>, DestShaderUnit>
{
    using HandleType = BaseHandle<GLuint, GenShaderUnit<SHADER_TYPE>, DestShaderUnit>;

    public:

        ShaderUnit(const std::string & Path_Or_SourceCode, bool isSourceCode=false) : HandleType()
        {
            if(isSourceCode)
            {
                compileFromString(Path_Or_SourceCode);
                return;
            }

            compileFromFile(Path_Or_SourceCode);
        }


        void compileFromFile(  const std::string & shader_path)
        {
            std::ifstream v(shader_path, std::ifstream::in);

            if( !v )
            {
                throw std::runtime_error(std::string("Cannot find file: ") + shader_path);
            }

            std::string V( (std::istreambuf_iterator<char>(v)),
                            std::istreambuf_iterator<char>() );

            //GLA_DOUT  << "========================================" << std::endl;
            //GLA_DOUT  << " Compiling: " << shader_path << std::endl;
            compileFromString( V );
            //return
        }

        void compileFromString(const std::string & shader_code)
        {
            //=====================================================
            // Compile the vertex shader
            //=====================================================
            //GLuint V = glCreateShader( SHADER_TYPE );
            HandleType::Generate();

            GLuint V = HandleType::Get();
            const char * code = shader_code.c_str();

            glShaderSource( V, 1, &code, NULL);
            glCompileShader(V);

            bool check = checkShader(V);

            if(check)
            {
               // std::cout  << "Shader compiled sucessfully:" << V << std::endl;
            } else {
                HandleType::Release();
            }

            //=====================================================
        }

        static bool checkShader(GLuint id)
        {
            GLint isCompiled = 0;
            glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
            if(isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

                std::string errorLog(maxLength, ' ');
                //char errorLog[maxLength];
                glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

                //std::cout   << errorLog << std::endl;
                throw std::runtime_error( std::string(errorLog) );

                return false;
            } else {
                return true;
            }
        }


};

using VertexShader                 =  ShaderUnit<GL_VERTEX_SHADER  >        ;
using FragmentShader               =  ShaderUnit<GL_FRAGMENT_SHADER>        ;
using GeometryShader               =  ShaderUnit<GL_GEOMETRY_SHADER>        ;
using TessellationControlShader    =  ShaderUnit<GL_TESS_CONTROL_SHADER>    ;
using TessellationEvaluationShader =  ShaderUnit<GL_TESS_EVALUATION_SHADER> ;

//struct ShaderProgramInfo
//{
//    unsigned int UseCount = 0;
//    std::map<std::string, int>          UniformLocations;
//    std::map<std::string, int>          UniformBlockLocations;
//    std::unordered_map<size_t, GLuint>  UniformMap;
//    std::unordered_map<size_t, GLuint>  BlockMap;

//    int NumberOfUniforms;
//    int NumberOfUniformBlocks;

//};

struct ShaderProgramHandler
{
    inline static void Create  (GLuint & h) { h = glCreateProgram(); }
    inline static void Release (GLuint & h) { glDeleteProgram(h);    }
    inline static void Bind    (GLuint & h) { glUseProgram(h);       }
    inline static void Unbind  (GLuint & h) { glUseProgram(0);       }
};

struct GenShaderProgram
{
    void operator()(GLuint & x)
    {
        x = glCreateProgram();
        GLA_LOGD << "Shader Program Generated: " << x << std::endl;
    }
};

struct DestShaderProgram
{
    void operator()(GLuint & x)
    {
        glDeleteProgram(x);
        GLA_LOGD << "Shader Program Destroyed: " << x << std::endl;
        x = 0;
    }
};

struct ShaderProgramInfo
{
    bool hasFragmentShader = false;
    bool hasVertexShader   = false;
    bool hasGeometryShader = false;
    bool hasTEShader       = false;
    bool hasTCShader       = false;
};

class ShaderProgram : public BaseHandle<GLuint, GenShaderProgram, DestShaderProgram,ShaderProgramInfo>
{


public:

        void Bind()     { glUseProgram( Get() );    }
        void Unbind()   { glUseProgram(0); }


        inline static GLuint Invalid_Block_Index()
        {
            return GL_INVALID_INDEX;
        }

        inline GLint GetUniformLocation(const GLchar *name)
        {            
            auto x = glGetUniformLocation( Get(), name);
            if( x== -1)
            {
                throw std::runtime_error( std::string("Unknown Uniform name: ") + std::string(name) );
            }
            //GLA_DOUT  << "Uniform locatiom("<<mProgram<<"):,  " << name << ": " <<  x << std::endl;
            return x;
        }

        inline GLint GetUniformLocation(const std::string & name)
        {
            auto x = glGetUniformLocation( Get() , name.c_str());
            if( x== -1)
            {
                throw std::runtime_error( std::string("Unknown Uniform name: ") + name );
            }
            //GLA_DOUT  << "Uniform locatiom("<<mProgram<<"):,  " << name << ": " <<  x << std::endl;
            return x;
        }

        inline int GetNumUniforms()
        {
            int total = -1;
            glGetProgramiv( Get() , GL_ACTIVE_UNIFORMS, &total );
            return total;
        }

        std::string GetUniformName(int i)
        {
            int name_len=-1, num=-1;

            GLenum type = GL_ZERO;

            char name[100];

            const auto id = Get();
            glGetActiveUniform( id , GLuint(i), sizeof(name)-1, &name_len,  &num,  &type,  name );
            name[name_len] = 0;

            return std::string(name);
        }


        //=====================================================================


        template<class S1>
        void AttachShaders(const S1 & s1)
        {
            Regenerate();
            glAttachShader( Get(), s1.Get() );
            glLinkProgram( Get() );
            Bind() ; // <<---- Use Program
        }

        template<class S1,class S2>
        void AttachShaders(const S1 & s1,const S2 & s2)
        {
            Regenerate();
            glAttachShader( Get(), s1.Get() );
            glAttachShader( Get(), s2.Get() );
            glLinkProgram( Get() );
            Bind() ; // <<---- Use Program
        }

        template<class S1,class S2,class S3>
        void AttachShaders(const S1 & s1,const S2 & s2,const S3 & s3)
        {
            Regenerate();
            glAttachShader( Get(), s1.Get() );
            glAttachShader( Get(), s2.Get() );
            glAttachShader( Get(), s3.Get() );
            glLinkProgram( Get() );
            Bind() ; // <<---- Use Program
        }

        template<class S1,class S2,class S3, class S4>
        void AttachShaders(const S1 & s1,const S2 & s2,const S3 & s3,const S4 & s4)
        {
            Regenerate();
            glAttachShader( Get(), s1.Get() );
            glAttachShader( Get(), s2.Get() );
            glAttachShader( Get(), s3.Get() );
            glAttachShader( Get(), s4.Get() );
            glLinkProgram( Get() );
            Bind() ; // <<---- Use Program
        }

        template<class S1,class S2,class S3, class S4, class S5>
        void AttachShaders(const S1 & s1,const S2 & s2,const S3 & s3,const S4 & s4,const S5 & s5)
        {
            Regenerate();
            glAttachShader( Get(), s1.Get() );
            glAttachShader( Get(), s2.Get() );
            glAttachShader( Get(), s3.Get() );
            glAttachShader( Get(), s4.Get() );
            glAttachShader( Get(), s5.Get() );
            glLinkProgram( Get() );
            Bind() ; // <<---- Use Program
        }

        static ShaderProgram LoadFromString(const std::string & source_code, const std::map<std::string, std::string> & Replacements = std::map<std::string, std::string>() )
        {
            auto vertex_shader               = get_shader_sub_code( source_code, "vertex")                  ;
            auto fragment_shader             = get_shader_sub_code( source_code, "fragment")                ;
            auto geometry_shader             = get_shader_sub_code( source_code, "geometry")                ;
            auto tessellation_control_shader = get_shader_sub_code( source_code, "tessellation_control")    ;
            auto tessellation_eval_shader    = get_shader_sub_code( source_code, "tessellation_evaluation") ;

            int mask = 0;
            if( vertex_shader               != "") mask |= 1;
            if( fragment_shader             != "") mask |= 2;
            if( geometry_shader             != "") mask |= 4;
            if( tessellation_control_shader != "") mask |= 8;
            if( tessellation_eval_shader    != "") mask |= 16;

            ShaderProgram P;

            switch(mask)
            {
                case 1|2:
                    P.AttachShaders(
                                VertexShader(vertex_shader, true ),
                                FragmentShader(fragment_shader, true ) );
                    P.SharedData().hasFragmentShader = true;
                    P.SharedData().hasVertexShader = true;
                    break;

                case 1|2|4:
                    P.AttachShaders(
                                VertexShader(vertex_shader, true ),
                                FragmentShader(fragment_shader, true ),
                                GeometryShader(geometry_shader, true ));
                    P.SharedData().hasFragmentShader = true;
                    P.SharedData().hasVertexShader = true;
                    P.SharedData().hasGeometryShader = true;
                    break;

                case 1|2|4|8|16:

                    P.AttachShaders(
                                VertexShader(vertex_shader, true ),
                                FragmentShader(fragment_shader, true ),
                                GeometryShader(geometry_shader, true ),
                                TessellationControlShader(tessellation_control_shader, true ),
                                TessellationEvaluationShader(tessellation_eval_shader, true ));
                    P.SharedData().hasFragmentShader = true;
                    P.SharedData().hasVertexShader = true;
                    P.SharedData().hasGeometryShader = true;
                    P.SharedData().hasTEShader = true;
                    P.SharedData().hasTCShader = true;
                    break;

                default:
                        break;
            }

            GLA_LOGD << "Shader Loaded Successfully:" << std::endl;
            GLA_LOGV << source_code << std::endl;

            auto s = P.GetNumUniforms();
            for(int i=0;i< s ; i++)
            {
                auto name  = P.GetUniformName(i);

                GLA_LOGI << "Uniform [" << name << "] - location: " << P.GetUniformLocation( name.c_str() ) << std::endl;
            }
            //std::cout << "Shader Loaded Successfully: " << path.c_str() << std::endl;
            return P;
        }

        static ShaderProgram Load(const std::string & path, const std::map<std::string, std::string> & Replacements = std::map<std::string, std::string>())
        {
            std::ifstream v(path, std::ifstream::in);

            if( !v )    throw std::runtime_error(std::string("Cannot find file: ") + path);

            std::string source_code( (std::istreambuf_iterator<char>(v))   , std::istreambuf_iterator<char>() );

            for(auto & m : Replacements)
            {
                source_code = FindAndReplace( source_code, m.first, m.second);
            }

            auto vertex_shader               = get_shader_sub_code( source_code, "vertex")                  ;
            auto fragment_shader             = get_shader_sub_code( source_code, "fragment")                ;
            auto geometry_shader             = get_shader_sub_code( source_code, "geometry")                ;
            auto tessellation_control_shader = get_shader_sub_code( source_code, "tessellation_control")    ;
            auto tessellation_eval_shader    = get_shader_sub_code( source_code, "tessellation_evaluation") ;

            int mask = 0;
            if( vertex_shader               != "") mask |= 1;
            if( fragment_shader             != "") mask |= 2;
            if( geometry_shader             != "") mask |= 4;
            if( tessellation_control_shader != "") mask |= 8;
            if( tessellation_eval_shader    != "") mask |= 16;

            ShaderProgram P;

            switch(mask)
            {
                case 1|2:
                    P.AttachShaders(
                                VertexShader(vertex_shader, true ),
                                FragmentShader(fragment_shader, true ) );
                    P.SharedData().hasFragmentShader = true;
                    P.SharedData().hasVertexShader = true;
                    break;

                case 1|2|4:
                    P.AttachShaders(
                                VertexShader(vertex_shader, true ),
                                FragmentShader(fragment_shader, true ),
                                GeometryShader(geometry_shader, true ));
                    P.SharedData().hasFragmentShader = true;
                    P.SharedData().hasVertexShader = true;
                    P.SharedData().hasGeometryShader = true;
                    break;

                case 1|2|4|8|16:

                    P.AttachShaders(
                                VertexShader(vertex_shader, true ),
                                FragmentShader(fragment_shader, true ),
                                GeometryShader(geometry_shader, true ),
                                TessellationControlShader(tessellation_control_shader, true ),
                                TessellationEvaluationShader(tessellation_eval_shader, true ));
                    P.SharedData().hasFragmentShader = true;
                    P.SharedData().hasVertexShader = true;
                    P.SharedData().hasGeometryShader = true;
                    P.SharedData().hasTEShader = true;
                    P.SharedData().hasTCShader = true;
                    break;

                default:
                        break;
            }

            GLA_LOGD << "Shader Loaded Successfully: " << path.c_str() << std::endl;

            auto s = P.GetNumUniforms();
            for(int i=0;i< s ; i++)
            {
                auto name  = P.GetUniformName(i);

                try {
                GLA_LOGI << "Uniform [" << name << "] - location: " << P.GetUniformLocation( name.c_str() ) << std::endl;
                } catch (std::exception & e )
                {

                }
            }
            //std::cout << "Shader Loaded Successfully: " << path.c_str() << std::endl;
            return P;

        }


        inline GLuint GetUniformBlockIndex( const char * name )
        {
          auto block_index = glGetUniformBlockIndex( Get(), name);
          return block_index;
        }

        inline std::size_t GetUniformBlockSize(const char * name )
        {
            auto index = GetUniformBlockIndex(name);
            GLint params;

            glGetActiveUniformBlockiv(Get(), index, GL_UNIFORM_BLOCK_DATA_SIZE, &params);


            return static_cast<std::size_t>(params);

        }

        inline unsigned int GetUniformOffset(const char * name )
        {
            //auto index = GetUniformBlockIndex(name);
            GLuint index;
            GLint params;
            const char * names[] = {name};

            glGetUniformIndices(Get(), 1, names, &index);

            glGetActiveUniformBlockiv(Get(), index, GL_UNIFORM_OFFSET, &params);

            return params;

        }


        inline void BindUniformBlock(const char * BlockName, GLuint UniformBufferBindPoint)
        {
            BindUniformBlock( GetUniformBlockIndex(BlockName), UniformBufferBindPoint);
        }


        // Bind a uniform buffer's bindpoint to a block index in the shader.
        inline void BindUniformBlock(GLuint BlockIndex, GLuint UniformBufferBindPoint)
        {
            glUniformBlockBinding(Get(), BlockIndex, UniformBufferBindPoint);
        }


      //  inline void BindUniformBuffer(GPUUniformBuffer & buffer, GLuint BlockIndex, GLuint BindPoint)
      //  {
      //      glUniformBlockBinding(Get(), BlockIndex, BindPoint);
      //      buffer.BindBase(BindPoint);
      //  }


        //========================================================================================
		inline void Uniform(GLint UniformID, const glm::mat4 & V, std::uint32_t count = 1)
        {
            glUniformMatrix4fv(UniformID, count, GL_FALSE, &V[0][0] );
        }
		inline void Uniform(GLint UniformID, const glm::mat3 & V, std::uint32_t count = 1)
        {
            glUniformMatrix3fv(UniformID, count, GL_FALSE, &V[0][0] );
        }
		inline void Uniform(GLint UniformID, const glm::mat2 & V, std::uint32_t count = 1)
        {
            glUniformMatrix2fv(UniformID, count, GL_FALSE, &V[0][0] );
        }
		inline void Uniform(GLint UniformID, const glm::vec4 & V, std::uint32_t count = 1)
        {
            glUniform4fv(UniformID, count, &V[0] );
        }
		inline void Uniform(GLint UniformID, const glm::vec3 & V, std::uint32_t count = 1)
        {
            glUniform3fv(UniformID, count, &V[0] );
        }
		inline void Uniform(GLint UniformID, const glm::vec2 & V, std::uint32_t count = 1)
        {
            glUniform2fv(UniformID, count, &V[0] );
        }
		inline void Uniform(GLint UniformID, const glm::ivec4 & V, std::uint32_t count = 1)
        {
            glUniform4iv(UniformID, count, &V[0] );
        }
		inline void Uniform(GLint UniformID, const glm::ivec3 & V, std::uint32_t count = 1)
        {
            glUniform3iv(UniformID, count, &V[0] );
        }
		inline void Uniform(GLint UniformID, const glm::ivec2 & V, std::uint32_t count = 1)
        {
            glUniform2iv(UniformID, count, &V[0] );
        }

		inline void Uniform(GLint UniformID, const float & V, std::uint32_t count = 1)
        {
            glUniform1f(UniformID, V );
        }
		inline void Uniform(GLint UniformID, const int & V, std::uint32_t count = 1)
        {
            glUniform1i(UniformID, V );
        }
		inline void Uniform(GLint UniformID, const int * V, std::uint32_t count = 1)
        {
            glUniform1iv( UniformID, count, V);
        }
		inline void Uniform(GLint UniformID, const float * V, std::uint32_t count = 1)
        {
            glUniform1fv( UniformID, count, V);
        }

        template<typename T>
		inline void Uniform(const char * name, const T &V, std::uint32_t count = 1)
        {
            Uniform( GetUniformLocation(name), V, count);
        }
        //===============================================================================================



        static std::string FindAndReplace(std::string s,
                                   std::string Find,
                                   std::string Replace)
        {
            try
            {

                auto start = s.find(Find);
                while(start != std::string::npos)
                {
                    s = s.replace( start, Find.length(), Replace);
                    start = s.find(Find);
                }
                return s;


            } catch( std::exception & e)
            {
                return s;
            }

        }

        static std::string get_shader_sub_code(const std::string & full_shader_code, const std::string & tag_value)
        {
            std::string opening = std::string("<") + tag_value + ">";
            std::string closing = std::string("</") + tag_value + ">";


            auto first = full_shader_code.find(opening);
            auto last  = full_shader_code.rfind(closing);


            if( first == std::string::npos || first == std::string::npos) return "";

            return full_shader_code.substr(first+opening.size(), last-first-opening.size());
        }
};



}


#endif // SHADER_H

