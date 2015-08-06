#include <gla/core/shader.h>
#include <gla/core/texture.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>

namespace gla
{

ShaderProgram::ShaderProgram(const VertexShader & VS, const FragmentShader & FS) : mProgram(0)
{
    linkProgram(VS, FS);

}

ShaderProgram::ShaderProgram() : mProgram(0)
{
}

ShaderProgram::~ShaderProgram()
{
    //glDeleteProgram(mProgram);
    #warning "TODO: Figure out what to do about unloading shaders."
}

void ShaderProgram::destroy()
{
    glDeleteProgram(mProgram);
}


GLuint ShaderProgram::linkProgram(const VertexShader & VS, const FragmentShader & FS)
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
        return shader;
    } else {
        std::cout << "Unable to link shader. Vertex or Fragment shader is not compiled properly." << std::endl;
        return shader;
    }

    return shader;
}




}
