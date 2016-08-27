#ifndef GLA_EXP_SHADER_H
#define GLA_EXP_SHADER_H

#include "types.h"
#include "handle.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <utility>

namespace gla { namespace experimental
{


template<GLenum SHADER_TYPE>
struct GenShaderUnit
{
    void operator()(GLuint & x)
    {
        x = glCreateShader( SHADER_TYPE );
        std::cout << "Shader Unit Generated: " << x << std::endl;
    }
};

struct DestShaderUnit
{
    void operator()(GLuint & x)
    {
        std::cout << "Destroying Shader Unit: " << x << std::endl;
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

                char errorLog[maxLength];
                glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

                std::cout   << errorLog << std::endl;


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

struct ShaderProgramInfo
{
    unsigned int UseCount = 0;
    std::map<std::string, int>          UniformLocations;
    std::map<std::string, int>          UniformBlockLocations;
    std::unordered_map<size_t, GLuint>  UniformMap;
    std::unordered_map<size_t, GLuint>  BlockMap;

    int NumberOfUniforms;
    int NumberOfUniformBlocks;

};

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
        std::cout << "Shader Program Generated: " << x << std::endl;
    }
};

struct DestShaderProgram
{
    void operator()(GLuint & x)
    {
        glDeleteProgram(x);
        x = 0;
    }
};



class ShaderProgram : public BaseHandle<GLuint, GenShaderProgram, DestShaderProgram>
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
            //GLA_DOUT  << "Uniform locatiom("<<mProgram<<"):,  " << name << ": " <<  x << std::endl;
            return x;
        }

        inline GLint GetUniformLocation(const std::string & name)
        {
            auto x = glGetUniformLocation( Get() , name.c_str());
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
        template<std::size_t I = 0, typename... Tp>
        inline typename std::enable_if<I == sizeof...(Tp), void>::type
        __AttachShader( const std::tuple<Tp...>& t)
        {
        }

        template<std::size_t I = 0, typename... Tp>
        inline typename std::enable_if<I < sizeof...(Tp), void>::type
        __AttachShader( const std::tuple<Tp...>& t)
        {
            glAttachShader( Get(), std::get<I>(t).Get() );
            __AttachShader<I+1>( t );
        }



        template<class... U>
        void AttachShaders(const U&... u)
        {
            Generate();
            //m_Handle.Release();
            //m_Handle.Create();

            __AttachShader( std::make_tuple( std::forward<const U>(u)... ) );

            glLinkProgram( Get() );

            Bind() ; // <<---- Use Program

            // ============ Get number of uniform locations ==================
            //auto & M = m_Handle.__GetMap();

            int   N = GetNumUniforms();
            auto ID = Get();

         //   auto & Info = m_Handle.__GetInfo();
         //   Info.UniformLocations.clear();
            //Info.UniformLocations.assign(N,-1);
            // ============ Get number of uniform locations ==================

/*
            GLA_DOUT  << "Shader Program created: "  << ID << std::endl;
            GLA_DOUT  << "     Number of Uniforms: " << N      << std::endl;
            for(int i=0;i<N;i++)
            {
                auto name = GetUniformName(i);
                auto loc  = GetUniformLocation(name.c_str());

                Info.UniformLocations[ name ] = loc;
                //GLA_DOUT  << "          Uniform(" << i << "): " << GetUniformName(i) << " HashKey: " << Hash(name.c_str()) << "  glUniformLoc: " << loc << std::endl;

                //Info.UniformMap[ Hash(name.c_str()) ] = loc;

                GLA_DOUT  << "          Uniform(" << i << "): " << GetUniformName(i) << std::endl;
            }
*/
        }
        //========================================================

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
                    break;

                case 1|2|4:
                    P.AttachShaders(
                                VertexShader(vertex_shader, true ),
                                FragmentShader(fragment_shader, true ),
                                GeometryShader(geometry_shader, true ));
                    break;

                case 1|2|4|8|16:

                    P.AttachShaders(
                                VertexShader(vertex_shader, true ),
                                FragmentShader(fragment_shader, true ),
                                GeometryShader(geometry_shader, true ),
                                TessellationControlShader(tessellation_control_shader, true ),
                                TessellationEvaluationShader(tessellation_eval_shader, true ));
                    break;

                default:
                        break;
            }

            return P;

        }


        inline GLuint GetUniformBlockIndex( const char * name )
        {
          auto block_index = glGetUniformBlockIndex( Get(), name);
          return block_index;
        }

        inline GLint GetUniformBlockSize(const char * name )
        {
            auto index = GetUniformBlockIndex(name);
            GLint params;

            glGetActiveUniformBlockiv(Get(), index, GL_UNIFORM_BLOCK_DATA_SIZE, &params);


            return params;

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
        inline void Uniform(GLint UniformID, const glm::mat4 & V, uint count=1)
        {
            glUniformMatrix4fv(UniformID, count, GL_FALSE, &V[0][0] );
        }
        inline void Uniform(GLint UniformID, const glm::mat3 & V, uint count=1)
        {
            glUniformMatrix3fv(UniformID, count, GL_FALSE, &V[0][0] );
        }
        inline void Uniform(GLint UniformID, const glm::mat2 & V, uint count=1)
        {
            glUniformMatrix2fv(UniformID, count, GL_FALSE, &V[0][0] );
        }
        inline void Uniform(GLint UniformID, const glm::vec4 & V, uint count=1)
        {
            glUniform4fv(UniformID, count, &V[0] );
        }
        inline void Uniform(GLint UniformID, const glm::vec3 & V, uint count=1)
        {
            glUniform3fv(UniformID, count, &V[0] );
        }
        inline void Uniform(GLint UniformID, const glm::vec2 & V, uint count=1)
        {
            glUniform2fv(UniformID, count, &V[0] );
        }
        inline void Uniform(GLint UniformID, const glm::ivec4 & V, uint count=1)
        {
            glUniform4iv(UniformID, count, &V[0] );
        }
        inline void Uniform(GLint UniformID, const glm::ivec3 & V, uint count=1)
        {
            glUniform3iv(UniformID, count, &V[0] );
        }
        inline void Uniform(GLint UniformID, const glm::ivec2 & V, uint count=1)
        {
            glUniform2iv(UniformID, count, &V[0] );
        }

        inline void Uniform(GLint UniformID, const float & V, uint count=1)
        {
            glUniform1f(UniformID, V );
        }
        inline void Uniform(GLint UniformID, const int & V, uint count=1)
        {
            glUniform1i(UniformID, V );
        }
        inline void Uniform(GLint UniformID, const int * V, uint count=1)
        {
            glUniform1iv( UniformID, count, V);
        }
        inline void Uniform(GLint UniformID, const float * V, uint count=1)
        {
            glUniform1fv( UniformID, count, V);
        }

        template<typename T>
        inline void Uniform( const char * name, const T &V , uint count=1)
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
}
//inline gla::ShaderProgram::HashKey operator "" _h(const char* c, size_t)
//{
//    return gla::ShaderProgram::Hash(c);
//}
#endif // SHADER_H
