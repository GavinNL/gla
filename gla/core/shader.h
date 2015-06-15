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
typedef gla::ShaderUnit<GL_GEOMETRY_SHADER> GeometryShader;


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
            glDeleteProgram(mProgram);
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
                std::cout << "Shader Program created: "  << shader << std::endl;

                // ============ Get number of uniform locations ==================
                int N = GetNumUniforms();
                mUniformLocations.clear();
                mUniformLocations.assign(N,-1);
                // ============ Get number of uniform locations ==================

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

        //

    private:
        std::vector<int> mUniformLocations;
        GLuint mProgram;

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


}
#endif // SHADER_H
