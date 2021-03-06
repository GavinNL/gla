#ifndef SamplerFILTER_H
#define SMPALER2DFILTER_H

#include "../core/vertexarray.h"
#include "../core/framebuffer.h"
#include "../core/shader.h"
#include "rendertotexture.h"


namespace gla
{

class SamplerFilter
{
    public:

        void Init()
        {
            create_geometry();
            load_shaders();
        }

        // draws directly to screen.
        void Null( Sampler Input)
        {
            const glm::uvec2 InputSize  = uvec2( Input.SharedData().m_Size.x, Input.SharedData().m_Size.y);

            m_RTT.Unbind();

            GLboolean depth;
            glGetBooleanv(GL_DEPTH_TEST, &depth);

            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            const glm::vec2 texel_size = 1.0f / vec2( InputSize );

//            GLA_LOGV << "Null Shader" << std::endl
//                     << "Input id    : " << Input.Get() << std::endl
//                     << "Input  size : " << InputSize.x << ", " << InputSize.y << std::endl
//                     << "texel_size  : " << texel_size.x << ", " << texel_size.y << std::endl;

            Input.SetActive(0);
            m_GaussianShader.Bind();
            m_GaussianShader.Uniform( m_GaussianShader.GetUniformLocation("u_Texture")  , 0          );

            m_vao.Draw(Primitave::TRIANGLES, 6);

            m_vao.Unbind();
            m_NullShader.Unbind();
            m_RTT.Unbind();

            if(depth) glEnable(GL_DEPTH_TEST);
        }

        void Null( Sampler Input, Sampler Output)
        {
            const glm::uvec2 OutputSize = uvec2(Output.SharedData().m_Size);
            const glm::uvec2 InputSize  = uvec2(Input.SharedData().m_Size );

            GLint viewport_dim[4];
            glGetIntegerv(GL_VIEWPORT, viewport_dim);

            m_RTT.Attach(Output);
            m_RTT.Bind();

            GLboolean depth;
            glGetBooleanv(GL_DEPTH_TEST, &depth);

            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glViewport(0, 0, OutputSize.x, OutputSize.y);

//            GLA_LOGV << "Null Shader" << std::endl
//                     << "Input id    : " << Input.Get() << std::endl
//                     << "Output id   : " << Input.Get() << std::endl
//                     << "Input  size : " << InputSize.x << ", " << InputSize.y << std::endl
//                     << "Output_size : " << OutputSize.x << ", " << OutputSize.y << std::endl
//                     << "texel_size  : " << texel_size.x << ", " << texel_size.y << std::endl;

            Input.SetActive(0);
            m_GaussianShader.Bind();
            m_GaussianShader.Uniform( m_GaussianShader.GetUniformLocation("u_Texture")  , 0          );

            m_vao.Draw(Primitave::TRIANGLES, 6);

            m_vao.Unbind();
            m_NullShader.Unbind();
            m_RTT.Unbind();

            if(depth) glEnable(GL_DEPTH_TEST);
            glViewport(viewport_dim[0], viewport_dim[1], viewport_dim[2], viewport_dim[3]);
        }

        void Gaussian( Sampler Input, Sampler Output, const glm::vec2 dir)
        {
            const glm::uvec2 OutputSize = uvec2(Output.SharedData().m_Size);
            const glm::uvec2 InputSize  = uvec2(Input.SharedData().m_Size );

            GLint viewport_dim[4];
            glGetIntegerv(GL_VIEWPORT, viewport_dim);

            m_RTT.Attach(Output);
            m_RTT.Bind();

            GLboolean depth;
            glGetBooleanv(GL_DEPTH_TEST, &depth);

            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glViewport(0, 0, OutputSize.x, OutputSize.y);

            const glm::vec2 texel_size = 1.0f / vec2( InputSize );

//            GLA_LOGV << "Guassian Blurr" << std::endl
//                     <<   "Input id:  " << Input.Get() << std::endl
//                     <<   "Output id: " << Input.Get() << std::endl
//                     << "Input  size : " << InputSize.x << ", " << InputSize.y << std::endl
//                     << "Output_size : " << OutputSize.x << ", " << OutputSize.y << std::endl
//                     << "texel_size : " << texel_size.x << ", " << texel_size.y << std::endl
//                     << "dir        : " << dir.x << ", " << dir.y << std::endl;

            Input.SetActive(0);
            m_GaussianShader.Bind();
            m_GaussianShader.Uniform( m_GaussianShader.GetUniformLocation("u_Texture")  , 0          );
            m_GaussianShader.Uniform( m_GaussianShader.GetUniformLocation("u_Direction"), dir        );
            m_GaussianShader.Uniform( m_GaussianShader.GetUniformLocation("u_TexelSize"), texel_size );


            m_vao.Draw(Primitave::TRIANGLES, 6);

            m_vao.Unbind();
            m_GaussianShader.Unbind();
            m_RTT.Unbind();

            if(depth) glEnable(GL_DEPTH_TEST);
            glViewport(viewport_dim[0], viewport_dim[1], viewport_dim[2], viewport_dim[3]);
        }

    protected:
        void create_geometry()
        {
            if( m_vao)
                return;

            struct v3fv2f
            {
                glm::vec3 p;
                glm::vec2 uv;
            };


            std::vector< v3fv2f > VertexBuffer;

            VertexBuffer.push_back( { glm::vec3(-1.0,  1.0, 0.0) , glm::vec2(0.0,1.0) }); // 3
            VertexBuffer.push_back( { glm::vec3(-1.0, -1.0, 0.0) , glm::vec2(0.0,0.0) }); // 0
            VertexBuffer.push_back( { glm::vec3( 1.0, -1.0, 0.0) , glm::vec2(1.0,0.0) }); // 1

            VertexBuffer.push_back( { glm::vec3(-1.0,  1.0, 0.0) , glm::vec2(0.0,1.0) }); // 0
            VertexBuffer.push_back( { glm::vec3( 1.0, -1.0, 0.0) , glm::vec2(1.0,0.0) }); // 2
            VertexBuffer.push_back( { glm::vec3( 1.0,  1.0, 0.0) , glm::vec2(1.0,1.0) }); // 2

            ArrayBuffer  PlaneBuff( VertexBuffer );

            m_vao.Attach<glm::vec3, glm::vec2>(PlaneBuff);

        }

        void load_shaders()
        {
            if( !m_GaussianShader )
            {
                const std::string src =
                #include "sampler2dfilter_gaussian.inc"
                m_GaussianShader = ShaderProgram::LoadFromString( src );
            }
            if( !m_NullShader )
            {
                const std::string src =
                #include "sampler2d_filter_null.inc"
                m_NullShader = ShaderProgram::LoadFromString( src );
            }
        }


    VertexArray     m_vao;
    RenderToTexture m_RTT;
    ShaderProgram   m_GaussianShader;
    ShaderProgram   m_NullShader;
};

}


#endif
