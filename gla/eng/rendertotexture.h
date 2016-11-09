#ifndef GLA_RENDERTOTEXTURE_H
#define GLA_RENDERTOTEXTURE_H

#include<gla/core/framebuffer.h>
#include <map>

namespace gla
{

    /**
     * @brief The RenderToTexture class
     */
class RenderToTexture
{
public:
    enum Target : GLenum {
        COLOR0  = GL_COLOR_ATTACHMENT0,
        COLOR1  = GL_COLOR_ATTACHMENT1,
        COLOR2  = GL_COLOR_ATTACHMENT2,
        COLOR3  = GL_COLOR_ATTACHMENT3,
        COLOR4  = GL_COLOR_ATTACHMENT4,
        COLOR5  = GL_COLOR_ATTACHMENT5,
        COLOR6  = GL_COLOR_ATTACHMENT6,
        COLOR7  = GL_COLOR_ATTACHMENT7,
        COLOR8  = GL_COLOR_ATTACHMENT8,
        COLOR9  = GL_COLOR_ATTACHMENT9,
        COLOR10 = GL_COLOR_ATTACHMENT10,
        COLOR11 = GL_COLOR_ATTACHMENT11,
        COLOR12 = GL_COLOR_ATTACHMENT12,
        COLOR13 = GL_COLOR_ATTACHMENT13,
        COLOR14 = GL_COLOR_ATTACHMENT14,
        COLOR15 = GL_COLOR_ATTACHMENT15,
        COLOR16 = GL_COLOR_ATTACHMENT16,
        COLOR17 = GL_COLOR_ATTACHMENT17,
        COLOR18 = GL_COLOR_ATTACHMENT18,
        COLOR19 = GL_COLOR_ATTACHMENT19,
        COLOR20 = GL_COLOR_ATTACHMENT20,
        COLOR21 = GL_COLOR_ATTACHMENT21,
        COLOR22 = GL_COLOR_ATTACHMENT22,
        COLOR23 = GL_COLOR_ATTACHMENT23,
        COLOR24 = GL_COLOR_ATTACHMENT24,
        COLOR25 = GL_COLOR_ATTACHMENT25,
        COLOR26 = GL_COLOR_ATTACHMENT26,
        COLOR27 = GL_COLOR_ATTACHMENT27,
        COLOR28 = GL_COLOR_ATTACHMENT28,
        COLOR29 = GL_COLOR_ATTACHMENT29,
        COLOR30 = GL_COLOR_ATTACHMENT30,
        COLOR31 = GL_COLOR_ATTACHMENT31,
        DEPTH   = GL_DEPTH_ATTACHMENT,
        STENCIL = GL_STENCIL_ATTACHMENT
    };

    enum TextureType {
        rgb,
        rgba,
        vec3_16f,
        vec3_32f,
        vec4_16f,
        vec4_32f,
        depth_16f,
        depth_24f,
        depth_32f,
        stencil
    };

    public:
        RenderToTexture(){}

        ~RenderToTexture()
        {
            m_FBO.Unbind();
        }

        void Bind()  { m_FBO.Bind();   }
        void Unbind(){ m_FBO.UnBind(); }

        /**
         * @brief Create
         * @param Targ - the target attchment
         * @param size - the size of the texture to generate
         * @param texture_type - the type of value you want to store in the texture
         *
         * Create a texture to render to.
         */
        void CreateTexture(Target Targ,  glm::uvec2 size,  TextureType texture_type )
        {
            if( !m_FBO)
            {
                GLA_LOGV << "Frame buffer not created. Creating one now!" << std::endl;
                m_FBO.Generate();
            }

            m_FBO.Bind();
            Sampler2D S;

            switch(texture_type)
            {
                case rgb      :
                    S = FrameBuffer::CreateBufferTexture_RGB(size); break;
                case rgba     :
                    S = FrameBuffer::CreateBufferTexture_RGBA(size); break;
                case vec3_16f :
                    S = FrameBuffer::CreateBufferTexture_Vec3_16f(size); break;
                case vec3_32f :
                    S = FrameBuffer::CreateBufferTexture_Vec3_32f(size); break;
                case vec4_16f :
                    S = FrameBuffer::CreateBufferTexture_Vec4_16f(size); break;
                case vec4_32f :
                    S = FrameBuffer::CreateBufferTexture_Vec4_32f(size); break;
                case depth_16f:
                    S = FrameBuffer::CreateBufferTexture_Depth16F(size); break;
                case depth_24f:
                    S = FrameBuffer::CreateBufferTexture_Depth24F(size); break;
//                case depth_32f:
//                    S = FrameBuffer::CreateBufferTexture_Depth32F(size); break;
                case stencil  :
                    GLA_LOGD << "No stencil buffer implementation yet."; break;
            }

            m_FBO.Attach(S, static_cast<FrameBuffer::Attachment>(Targ) );

            m_Samplers[Targ] = S;

            Update();
            m_FBO.UnBind();
        }

        void Update()
        {
            std::vector<FrameBuffer::Attachment> D;

            for(auto & T : m_Samplers)
            {
               if(T.first != DEPTH && T.first != STENCIL) D.push_back( static_cast<FrameBuffer::Attachment>(T.first ) );
            }
            std::cout << D.size() << std::endl;
            m_FBO.Use( D );

            auto ch = m_FBO.Check();
            if( ch != FrameBuffer::COMPLETE )
            {
                std::cout << "ERROR!!!: " << std::hex << static_cast<int>(ch) << std::endl;
            }

            m_FBO.Unbind();
        }

        Sampler2D & operator[] (Target t)
        {
            return m_Samplers.at(t);
        }

    protected:
        FrameBuffer                 m_FBO;
        std::map<Target, Sampler2D> m_Samplers;
};

}

#endif

