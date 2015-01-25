#include <glre/shader.h>
#include <iostream>
#include <vector>

namespace glre
{

Shader::Shader() : mVertexShader(0), mFragmentShader(0), mProgram(0)
{
}

Shader::~Shader()
{
    glDeleteProgram(mProgram);
}

GLuint Shader::compileShader(std::string &vertex, std::string &fragment)
{
    mVertexCode   = vertex;
    mFragmentCode = fragment;


    return( compileShader() );
}

GLuint Shader::compileShader()
{

    //=====================================================
    // Compile the vertex shader
    //=====================================================
    GLuint V = glCreateShader( GL_VERTEX_SHADER );

    auto vv = mVertexCode.c_str();
    glShaderSource(V, 1, &vv, NULL);
    glCompileShader(V);

    checkShader(V);
    mVertexShader = V;
    //=====================================================


    //=====================================================
    // Compile the fragment shader
    //=====================================================
    GLuint F = glCreateShader( GL_FRAGMENT_SHADER);

    auto ff = mFragmentCode.c_str();
    glShaderSource(F, 1, &ff, NULL);
    glCompileShader(F);

    checkShader(F);
    mFragmentShader = F;
    //=====================================================

    GLuint shader = glCreateProgram();
    glAttachShader (shader, F);
    glAttachShader (shader, V);

    glLinkProgram (shader);
    glUseProgram (shader);

    mProgram = shader;
    std::cout << "SHADER CREATED: " << shader << std::endl;
    return shader;
}


bool Shader::checkShader(GLuint shader)
{
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        for(auto a : errorLog)           std::cout << a ;
        std::cout << std::endl;
        //Provide the infolog in whatever manor you deem best.
        //Exit with failure.
        glDeleteShader(shader); //Don't leak the shader.
    }
}

}
