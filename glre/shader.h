#ifndef SHADER_H
#define SHADER_H

#include <glre/global.h>

namespace glre
{

class Shader
{
    public:
        Shader();
        ~Shader();

        void   setSource(std::string & vertex, std::string & fragment);

        inline GLuint getUniformLocation(const GLchar *name) { return glGetUniformLocation(mProgram, name); }

        inline void sendMatrix(GLuint location, const M4 & M)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, &M[0][0]);
        };

        GLuint compileFromFile(const std::string & vertex_path, const std::string & fragment_path);
        GLuint compileShader(const std::string & vertex, const std::string & fragment);
        GLuint compileShader();
        bool   checkShader(GLuint shader);

        inline void useShader() { glUseProgram(mProgram);}
    private:
        std::string mVertexCode;
        std::string mFragmentCode;

        GLuint mVertexShader;
        GLuint mFragmentShader;

        GLuint mProgram;

};

}
#endif // SHADER_H
