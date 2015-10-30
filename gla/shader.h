#ifndef GLA_SHADER_H
#define GLA_SHADER_H

#include <gla/types.h>
#include <gla/uniformbuffer.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>

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

            std::string V( (std::istreambuf_iterator<char>(v)),
                            std::istreambuf_iterator<char>() );

            std::cout << "========================================" << std::endl;
            std::cout << " Compiling: " << shader_path << std::endl;

            compileFromString( V );
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
                std::cout << "Shader compiled sucessfully:" << V << std::endl;
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

                std::cout << errorLog << std::endl;

                glDeleteShader(id); //Don't leak the shader.
                return false;
            } else {
                return true;
            }
        }

        GLuint      mShaderID;

};

typedef gla::ShaderUnit<GL_VERTEX_SHADER  > VertexShader;
typedef gla::ShaderUnit<GL_FRAGMENT_SHADER> FragmentShader;
typedef gla::ShaderUnit<GL_GEOMETRY_SHADER> GeometryShader;


struct ShaderInfo
{
    std::map<std::string, int> UniformLocations;
    std::map<std::string, int> UniformBlockLocations;
    int NumberOfUniforms;
    int NumberOfUniformBlocks;
};

class ShaderProgram
{
    public:
         ShaderProgram(const VertexShader & VS, const FragmentShader & FS);
         ShaderProgram();
        ~ShaderProgram();


        ShaderProgram(const ShaderProgram & other)
        {
            mProgram = other.mProgram;
            mUniformLocations = other.mUniformLocations;
        }

        ShaderProgram & operator=(const ShaderProgram & other)
        {
            mProgram = other.mProgram;
            mUniformLocations = other.mUniformLocations;
            return(*this);
        }

        inline void DeleteShader()
        {
            if(mProgram) glDeleteProgram(mProgram);
            mUniformLocations.clear();
        }


        inline GLuint getUniformLocation(const GLchar *name)
        {
            auto x = glGetUniformLocation(mProgram, name);
            //std::cout << "Uniform locatiom("<<mProgram<<"):,  " << name << ": " <<  x << std::endl;
            return x;
        }

        inline int GetNumUniforms()
        {
            int total = -1;
            glGetProgramiv( mProgram, GL_ACTIVE_UNIFORMS, &total );
            return total;
        }

        std::string GetUniformName(int i)
        {
            int name_len=-1, num=-1;

            GLenum type = GL_ZERO;

            char name[100];

            glGetActiveUniform( mProgram, GLuint(i), sizeof(name)-1, &name_len,  &num,  &type,  name );
            name[name_len] = 0;
            GLuint location = glGetUniformLocation( mProgram, name );

            return std::string(name);
        }

        GLuint linkProgram(const VertexShader & VS, const FragmentShader & FS)
        {

            GLuint shader=0;

            if( VS.mShaderID && FS.mShaderID )
            {
                GLuint shader = glCreateProgram();
                glAttachShader( shader, VS.mShaderID );
                glAttachShader( shader, FS.mShaderID );

                glLinkProgram(shader);
                glUseProgram (shader);

                mProgram = shader;


                Info = std::shared_ptr<ShaderInfo>( new ShaderInfo, [=](ShaderInfo* a){ delete a; glDeleteProgram(mProgram); std::cout << "Deleting Program: " << mProgram << std::endl; } );

                // ============ Get number of uniform locations ==================
                int N = GetNumUniforms();
                mUniformLocations.clear();
                mUniformLocations.assign(N,-1);
                // ============ Get number of uniform locations ==================


                std::cout << "Shader Program created: "  << shader << std::endl;
                std::cout << "     Number of Uniforms: " << N << std::endl;
                for(int i=0;i<N;i++)
                {
                    auto name = GetUniformName(i);
                    auto loc  = getUniformLocation(name.c_str());

                    Info->UniformLocations[ name ] = loc;
                    std::cout << "          Uniform(" << i << "): " << GetUniformName(i) << " HashKey: " << Hash(name.c_str()) << std::endl;

                    mUniformMap[ Hash(name.c_str()) ] = loc;
                    //std::cout << "          Uniform(" << i << "): " << GetUniformName(i) << std::endl;
                }


                return shader;
            } else {
                std::cout << "Unable to link shader. Vertex or Fragment shader is not compiled properly." << std::endl;
                return shader;
            }

            return shader;
        }

        inline void useShader()
        {
            static int CurrentlyBoundShader = 0;
            if(CurrentlyBoundShader==mProgram) return;

            CurrentlyBoundShader = mProgram;
            glUseProgram(mProgram);
        }

        // Sending data to the shader

        inline void sendUniform_Sampler2D(GLint location, GLint TextureNumber=0)
        {
            glUniform1i(location, TextureNumber);
        }

        inline void sendUniform_mat4(GLuint location, const mat4 & M, uint count=1 )
        {
            glUniformMatrix4fv(location, count, GL_FALSE, &M[0][0] );
        }

        inline void sendUniform_vec4(GLuint location, const vec4 & V, uint count=1 )
        {
            glUniform4fv(location, count, &V[0]);
        }

        inline void sendUniform_vec3(GLuint location, const vec3 & V, uint count=1 )
        {
            glUniform3fv(location, count, &V[0]);
        }

        inline unsigned int GetUniformBlockIndex(const char * name)
        {
          unsigned int block_index = glGetUniformBlockIndex(mProgram, name);
          return block_index;
        }



        inline void BindUniformBuffer(GPUUniformBuffer & buffer, GLuint BlockIndex, GLuint BindPoint)
        {
            glUniformBlockBinding(mProgram, BlockIndex, BindPoint);
            buffer.BindBase(BindPoint);
        }


        using HashKey = size_t;

        static constexpr HashKey Hash(const char * UniformName, HashKey k=0)
        {
            return *UniformName==0 ? k : Hash(UniformName+1, *UniformName * 101 + *UniformName);
        }

        /*
        template<typename T>
        inline void UniformData(HashKey UniformNameHash, const T & V, uint count=1)
        {
            if( std::is_same<T, const gla::mat4&>::value)  glUniformMatrix3fv(mUniformMap[UniformNameHash], count, GL_FALSE, static_cast<const float*>(&V) );
            if( std::is_same<T, const gla::mat3&>::value)  glUniformMatrix2fv(mUniformMap[UniformNameHash], count, GL_FALSE, static_cast<const float*>(&V) );
            if( std::is_same<T, const gla::mat2&>::value)  glUniform3fv(mUniformMap[UniformNameHash], count, (void*)&V );

            if( std::is_same<T, const gla::vec4&>::value)  glUniform4fv(mUniformMap[UniformNameHash], count, (void*)&V );
            if( std::is_same<T, const gla::vec3&>::value)  glUniform3fv(mUniformMap[UniformNameHash], count, (void*)&V );
            if( std::is_same<T, const gla::vec2&>::value)  glUniform2iv(mUniformMap[UniformNameHash], count, (void*)&V );

            if( std::is_same<T, const gla::ivec4&>::value) glUniform4iv(mUniformMap[UniformNameHash], count, (void*)&V );
            if( std::is_same<T, const gla::ivec3&>::value) glUniform3iv(mUniformMap[UniformNameHash], count, (void*)&V );
            if( std::is_same<T, const gla::ivec2&>::value) glUniform2fv(mUniformMap[UniformNameHash], count, (void*)&V );

            if( std::is_same<T, const float&>::value)      glUniform1iv(mUniformMap[UniformNameHash], count, (void*)&V );
            if( std::is_same<T, const int&>::value)        glUniform1iv(mUniformMap[UniformNameHash], count, (void*)&V );
        }
        */


        inline void UniformData(HashKey UniformNameHash, const gla::mat4 & V, uint count=1)
        {
            glUniformMatrix4fv(mUniformMap[UniformNameHash], count, GL_FALSE, &V[0][0] );
        }
        inline void UniformData(HashKey UniformNameHash, const gla::mat3 & V, uint count=1)
        {
            glUniformMatrix3fv(mUniformMap[UniformNameHash], count, GL_FALSE, &V[0][0] );
        }
        inline void UniformData(HashKey UniformNameHash, const gla::mat2 & V, uint count=1)
        {
            glUniformMatrix2fv(mUniformMap[UniformNameHash], count, GL_FALSE, &V[0][0] );
        }
        inline void UniformData(HashKey UniformNameHash, const gla::vec4 & V, uint count=1)
        {
            glUniform4fv(mUniformMap[UniformNameHash], count, &V[0] );
        }
        inline void UniformData(HashKey UniformNameHash, const gla::vec3 & V, uint count=1)
        {
            glUniform3fv(mUniformMap[UniformNameHash], count, &V[0] );
        }
        inline void UniformData(HashKey UniformNameHash, const gla::vec2 & V, uint count=1)
        {
            glUniform2fv(mUniformMap[UniformNameHash], count, &V[0] );
        }

        inline void UniformData(HashKey UniformNameHash, const gla::ivec4 & V, uint count=1)
        {
            glUniform4iv(mUniformMap[UniformNameHash], count, &V[0] );
        }
        inline void UniformData(HashKey UniformNameHash, const gla::ivec3 & V, uint count=1)
        {
            glUniform3iv(mUniformMap[UniformNameHash], count, &V[0] );
        }
        inline void UniformData(HashKey UniformNameHash, const gla::ivec2 & V, uint count=1)
        {
            glUniform2iv(mUniformMap[UniformNameHash], count, &V[0] );
        }

        inline void UniformData(HashKey UniformNameHash, const float & V, uint count=1)
        {
            glUniform1f(mUniformMap[UniformNameHash], V );
        }
        inline void UniformData(HashKey UniformNameHash, const int & V, uint count=1)
        {
            glUniform1i(mUniformMap[UniformNameHash], V );
        }

        /**
         * @brief sendUniform
         * @param location - a unique index in the array to store the uniform locations. Start at 0 and increment for each new uniform you are going to send
         * @param name - the name of the uniform in the shader
         * @param V
         * @param count - number of elements to send, default is 1
         *
         * This method combines finding the uniform location and sending the uniform into one. This is a specialized function that will only find
         * the uniform location once, then store it in an array.  For example:
         *
         * sendUniformLocation(0, "projectionMatrix", proj);
         * sendUniformLocation(1, "ModelMatrix", model);
         * sendUniformLocation(3, "ModelMatrix", model);  // this shouldn't be done, use 2 instead of 3. Must go sequentally
         */

        inline void sendUniform(GLuint location, const char *name, const gla::mat4 & V, uint count=1 )
        {
            switch( mUniformLocations[location] )
            {
                case -1:
                    mUniformLocations[location] = getUniformLocation(name);
                    std::cout << "Location found: " << mUniformLocations[location] << std::endl;
                default:
                    glUniformMatrix4fv(mUniformLocations[location], count, GL_FALSE, &V[0][0] );
            }
        }


        inline void sendUniform(GLuint location, const char *name, const gla::vec3 & V, uint count=1 )
        {
            switch( mUniformLocations[location] )
            {
                case -1:
                    mUniformLocations[location] = getUniformLocation(name);
                default:
                    glUniform3fv(mUniformLocations[location], count, &V[0]);
            }
        }

        inline void sendUniform(GLuint location, const char *name, const gla::vec4 & V, uint count=1 )
        {
            switch( mUniformLocations[location] )
            {
                case -1:
                    mUniformLocations[location] = getUniformLocation(name);
                default:
                    glUniform4fv(mUniformLocations[location], count, &V[0]);
            }
        }

        inline void sendUniform(GLuint location, const char *name, const gla::vec2 & V, uint count=1 )
        {
            switch( mUniformLocations[location] )
            {
                case -1:
                    mUniformLocations[location] = getUniformLocation(name);
                default:
                    glUniform2fv(mUniformLocations[location], count, &V[0]);
            }
        }

        inline void sendUniform(GLuint location, const char *name, const float * V, uint count=1 )
        {
            switch( mUniformLocations[location] )
            {
                case -1:
                    mUniformLocations[location] = getUniformLocation(name);
                default:
                    glUniform1fv(mUniformLocations[location], count, V);
            }
        }

        inline void sendUniform(GLuint location, const char *name, const int * V, uint count=1 )
        {
            switch( mUniformLocations[location] )
            {
                case -1:
                    mUniformLocations[location] = getUniformLocation(name);
                default:
                    glUniform1iv(mUniformLocations[location], count, V);
            }
        }

        inline void sendUniform(GLuint location, const char *name, const int V )
        {
            switch( mUniformLocations[location] )
            {
                case -1:
                    mUniformLocations[location] = getUniformLocation(name);
                default:
                    glUniform1i(mUniformLocations[location], V);
            }
        }


    private:
        std::vector<int> mUniformLocations;         // TODO(Gavin): To be removed later
        std::vector<int> mUniformBlockLocations;    // TODO(Gavin): To be removed later

        std::unordered_map<HashKey, GLuint> mUniformMap;
        std::unordered_map<HashKey, GLuint> mBlockMap;

        GLuint mProgram;
        std::shared_ptr<ShaderInfo> Info;

};

inline ShaderProgram::ShaderProgram(const VertexShader & VS, const FragmentShader & FS) : mProgram(0)
{
    linkProgram(VS, FS);
}

inline ShaderProgram::ShaderProgram() : mProgram(0)
{
}

inline ShaderProgram::~ShaderProgram()
{
    //glDeleteProgram(mProgram);
//    #warning "TODO: Figure out what to do about unloading shaders."
}

inline ShaderProgram::HashKey operator "" _h(const char* c, size_t)
{
    return ShaderProgram::Hash(c);
}

}
#endif // SHADER_H
