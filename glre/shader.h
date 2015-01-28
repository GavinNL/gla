#ifndef SHADER_H
#define SHADER_H

#include <glre/global.h>
#include <iostream>
namespace glre
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
            if(isSourceCode) compileFromString(Path_Or_SourceCode);

            compileFromFile(Path_Or_SourceCode);
        }

        GLuint compileFromFile(  const std::string & shader_path)
        {
            std::ifstream v(shader_path);

            std::string V((std::istreambuf_iterator<char>(v)),
                             std::istreambuf_iterator<char>());
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

            std::cout << shader_code << std::endl;

            glShaderSource(V, 1, &code, NULL);
            glCompileShader(V);

            bool check = checkShader(V);

            if(check)
            {
                std::cout << "Shader compiled sucessfully:" << V << std::endl;
            }
            mShaderCode = shader_code;
            mShaderID   = V;
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

                //The maxLength includes the NULL character
                //std::vector<char> errorLog(maxLength);
                char errorLog[maxLength];
                glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

                std::cout << errorLog << std::endl;

                //Provide the infolog in whatever manor you deem best.
                //Exit with failure.
                glDeleteShader(id); //Don't leak the shader.
                return false;
            } else {
                return true;
            }
        }

        GLuint      mShaderID;
        std::string mShaderCode;
};

typedef glre::ShaderUnit<GL_VERTEX_SHADER  > VertexShader;
typedef glre::ShaderUnit<GL_FRAGMENT_SHADER> FragmentShader;


class ShaderProgram
{
    public:
         ShaderProgram(const VertexShader & VS, const FragmentShader & FS);
         ShaderProgram();
        ~ShaderProgram();


        inline GLuint getUniformLocation(const GLchar *name) { return glGetUniformLocation(mProgram, name); }

        GLuint linkProgram(const VertexShader & VS, const FragmentShader & FS);

        inline void useShader() { glUseProgram(mProgram);}

        // Sending data to the shader
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
