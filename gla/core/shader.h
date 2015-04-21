#ifndef GLA_SHADER_H
#define GLA_SHADER_H

#include <gla/core/global.h>
#include <iostream>
#include <fstream>
#include <streambuf>

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


class ShaderProgram
{
    public:
         ShaderProgram(const VertexShader & VS, const FragmentShader & FS);
         ShaderProgram();
        ~ShaderProgram();

         void destroy();

        ShaderProgram(const ShaderProgram & other)
        {
            mProgram = other.mProgram;
        }

        ShaderProgram & operator=(const ShaderProgram & other)
        {
            mProgram = other.mProgram;
            return(*this);
        }


        inline void DeleteShader()
        {
            glDeleteProgram(mProgram);
        }

        inline GLuint getUniformLocation(const GLchar *name)
        {
            auto x = glGetUniformLocation(mProgram, name);
            std::cout << "Uniform locatiom("<<mProgram<<"):,  " << name << ": " <<  x << std::endl;
            return x;
        }

        GLuint linkProgram(const VertexShader & VS, const FragmentShader & FS);

        inline void useShader() { glUseProgram(mProgram);}

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
    private:

        GLuint mProgram;

};

}
#endif // SHADER_H
