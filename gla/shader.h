#ifndef GLA_SHADER_H
#define GLA_SHADER_H

#include <gla/types.h>
//#include <gla/uniformbuffer.h>
#include <gla/handle.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <utility>

namespace gla
{

template<uint SHADER_TYPE>
class ShaderUnit
{
    public:
        ShaderUnit() : mShaderID(0)
        {

        }

        ShaderUnit(const std::string & Path_Or_SourceCode, bool isSourceCode=false)
        {
            if(isSourceCode)
            {
                compileFromString(Path_Or_SourceCode);
                return;
            }

            compileFromFile(Path_Or_SourceCode);
        }

        void destroy()
        {
            glDeleteShader( mShaderID );
            mShaderID = 0;
        }

        GLuint compileFromFile(  const std::string & shader_path)
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

            return compileFromString( V );
        }

        GLuint compileFromString(const std::string & shader_code)
        {
            //=====================================================
            // Compile the vertex shader
            //=====================================================
            GLuint V = glCreateShader( SHADER_TYPE );

            const char * code = shader_code.c_str();

            glShaderSource( V, 1, &code, NULL);
            glCompileShader(V);

            bool check = checkShader(V);

            if(check)
            {
                GLA_DOUT  << "Shader compiled sucessfully:" << V << std::endl;
            } else {

            }

            mShaderID   = V;

            return V;
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

                glDeleteShader(id); //Don't leak the shader.
                return false;
            } else {
                return true;
            }
        }

        explicit operator bool() const
        {
            return mShaderID!=0;
        }

        GLuint      mShaderID;

};

typedef gla::ShaderUnit<GL_VERTEX_SHADER  >        VertexShader;
typedef gla::ShaderUnit<GL_FRAGMENT_SHADER>        FragmentShader;
typedef gla::ShaderUnit<GL_GEOMETRY_SHADER>        GeometryShader;
typedef gla::ShaderUnit<GL_TESS_CONTROL_SHADER>    TessellationControlShader;
typedef gla::ShaderUnit<GL_TESS_EVALUATION_SHADER> TessellationEvaluationShader;

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

class ShaderProgram
{
public:
    using HandleType = gla::Handle<GLuint, ShaderProgramHandler, ShaderProgramInfo >;

    HandleType m_Handle;


public:

    void Bind()     { m_Handle.Bind();    }
    void Unbind()   { m_Handle.Unbind();  }
    void Release()  { m_Handle.Release(); }

        inline static GLuint Invalid_Block_Index()
        {
            return GL_INVALID_INDEX;
        }

        inline GLint GetUniformLocation(const GLchar *name)
        {            
            auto x = glGetUniformLocation(m_Handle.GetID(), name);
            //GLA_DOUT  << "Uniform locatiom("<<mProgram<<"):,  " << name << ": " <<  x << std::endl;
            return x;
        }

        inline GLint GetUniformLocation(const std::string & name)
        {
            auto x = glGetUniformLocation(m_Handle.GetID(), name.c_str());
            //GLA_DOUT  << "Uniform locatiom("<<mProgram<<"):,  " << name << ": " <<  x << std::endl;
            return x;
        }

        inline int GetNumUniforms()
        {
            int total = -1;
            glGetProgramiv( m_Handle.GetID(), GL_ACTIVE_UNIFORMS, &total );
            return total;
        }

        std::string GetUniformName(int i)
        {
            int name_len=-1, num=-1;

            GLenum type = GL_ZERO;

            char name[100];

            const auto id = m_Handle.GetID();
            glGetActiveUniform( id , GLuint(i), sizeof(name)-1, &name_len,  &num,  &type,  name );
            name[name_len] = 0;
            GLuint location = glGetUniformLocation( id , name );

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
            glAttachShader( m_Handle.GetID(), std::get<I>(t).mShaderID );
            __AttachShader<I+1>( t );
        }



        template<class... U>
        void AttachShaders(const U&... u)
        {

            m_Handle.Release();


            m_Handle.Create();

            __AttachShader( std::make_tuple( std::forward<const U>(u)... ) );

            glLinkProgram( m_Handle.GetID() );

            m_Handle.Bind(); // <<---- Use Program
            //glUseProgram(shader);

            //mProgram = shader;

            //auto p = mProgram;
            //Info   = std::shared_ptr<ShaderInfo>( new ShaderInfo, [=](ShaderInfo* a){ delete a; glDeleteProgram(p); GLA_DOUT  << "-------------Deleting Program:----------------- " << p << std::endl; } );

            // ============ Get number of uniform locations ==================
            //auto & M = m_Handle.__GetMap();

            int   N = GetNumUniforms();
            auto ID = m_Handle.GetID();

            auto & Info = m_Handle.__GetInfo();
            Info.UniformLocations.clear();
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



        inline GLuint GetUniformBlockIndex(const char * name)
        {
          auto block_index = glGetUniformBlockIndex( m_Handle.GetID(), name);
          return block_index;
        }

        inline GLint GetUniformBlockSize(const char * name )
        {
            auto index = GetUniformBlockIndex(name);
            GLint params;

            glGetActiveUniformBlockiv(m_Handle.GetID(), index, GL_UNIFORM_BLOCK_DATA_SIZE, &params);


            return params;

        }

        inline unsigned int GetUniformOffset(const char * name )
        {
            //auto index = GetUniformBlockIndex(name);
            GLuint index;
            GLint params;
            const char * names[] = {name};

            glGetUniformIndices(m_Handle.GetID(), 1, names, &index);

            glGetActiveUniformBlockiv(m_Handle.GetID(), index, GL_UNIFORM_OFFSET, &params);

            return params;

        }


        inline void BindUniformBlock(const char * BlockName, GLuint UniformBufferBindPoint)
        {
            BindUniformBlock( GetUniformBlockIndex(BlockName), UniformBufferBindPoint);
        }


        // Bind a uniform buffer's bindpoint to a block index in the shader.
        inline void BindUniformBlock(GLuint BlockIndex, GLuint UniformBufferBindPoint)
        {
            glUniformBlockBinding(m_Handle.GetID(), BlockIndex, UniformBufferBindPoint);
        }


      //  inline void BindUniformBuffer(GPUUniformBuffer & buffer, GLuint BlockIndex, GLuint BindPoint)
      //  {
      //      glUniformBlockBinding(m_Handle.GetID(), BlockIndex, BindPoint);
      //      buffer.BindBase(BindPoint);
      //  }


        //========================================================================================
        inline void Uniform(GLint UniformID, const gla::mat4 & V, uint count=1)
        {
            glUniformMatrix4fv(UniformID, count, GL_FALSE, &V[0][0] );
        }
        inline void Uniform(GLint UniformID, const gla::mat3 & V, uint count=1)
        {
            glUniformMatrix3fv(UniformID, count, GL_FALSE, &V[0][0] );
        }
        inline void Uniform(GLint UniformID, const gla::mat2 & V, uint count=1)
        {
            glUniformMatrix2fv(UniformID, count, GL_FALSE, &V[0][0] );
        }
        inline void Uniform(GLint UniformID, const gla::vec4 & V, uint count=1)
        {
            glUniform4fv(UniformID, count, &V[0] );
        }
        inline void Uniform(GLint UniformID, const gla::vec3 & V, uint count=1)
        {
            glUniform3fv(UniformID, count, &V[0] );
        }
        inline void Uniform(GLint UniformID, const gla::vec2 & V, uint count=1)
        {
            glUniform2fv(UniformID, count, &V[0] );
        }
        inline void Uniform(GLint UniformID, const gla::ivec4 & V, uint count=1)
        {
            glUniform4iv(UniformID, count, &V[0] );
        }
        inline void Uniform(GLint UniformID, const gla::ivec3 & V, uint count=1)
        {
            glUniform3iv(UniformID, count, &V[0] );
        }
        inline void Uniform(GLint UniformID, const gla::ivec2 & V, uint count=1)
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
        //===============================================================================================


};



}

//inline gla::ShaderProgram::HashKey operator "" _h(const char* c, size_t)
//{
//    return gla::ShaderProgram::Hash(c);
//}
#endif // SHADER_H
