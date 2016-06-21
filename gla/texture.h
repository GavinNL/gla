#ifndef GLA_TEXTURE_H
#define GLA_TEXTURE_H

#include <gla/global.h>
#include <gla/types.h>
#include <gla/exceptions.h>
#include <iostream>
#include <string.h>
#include <functional>
#include <memory>
#include <vector>

#include <gla/handle.h>
#ifdef GLA_IMAGE_USE_STB
    #include <stb/stb_image.h>
#elif GLA_IMAGE_USE_FREEIMAGE
    #include <freeimage.h>
#else
    #include <gla/stb_image_headeronly.h>
    #include <gla/stb_image_write_headeronly.h>
#endif


/**
 * \page page_Testing
 *  this is a test poage
 *
 */
namespace gla {

    class ChannelRef;
    class TextureBase;

    class GPUTextureInfo
    {
        int temp;
    };

    inline std::vector<unsigned int> & GetActiveTextures()
    {
        static auto s = std::make_shared< std::vector<unsigned int> > ();
        static bool init = false;
        if(!init)
        {
            s->resize(128);
            init = true;
        }
        return *s;
    }

    //===========================================================================
    // GPUTexture
    //   - A 2D texture stored on the GPU
    //===========================================================================
    struct TextureHandler
    {
        inline static void Create  (GLuint & h) { glGenTextures(1, &h); }
        inline static void Release (GLuint & h) { glDeleteTextures(1, &h);  }
        inline static void Bind    (GLuint & h) { glBindTexture(GL_TEXTURE_2D, h); }
        inline static void Unbind  (GLuint & h) { glBindTexture(GL_TEXTURE_2D, 0);  }
    };

    struct TextureInfo
    {
        unsigned int       UseCount = 0;
        uvec2              Size = {0,0};
        TexInternalFormat  InternalFormat;
        TexFormat          Format;
        bool               MipMaps;
        TexFilter          MinFilter;
        TexFilter          MagFilter;
        TexWrap            S_Wrap;
        TexWrap            T_Wrap;
        DataType           Type;
    };


    class GPUTexture
    {
    public:
        using HandleType = gla::Handle<GLuint, TextureHandler, TextureInfo >;

        HandleType m_Handle;


    public:

        void Bind()     { m_Handle.Bind();    }
        void Unbind()   { m_Handle.Unbind();  }
        void Release()  { m_Handle.Release(); }
       // using Base = gla::Handle<GLuint, TextureHandler, TextureInfo >;

        /**
         * @brief GPUTexture
         * @param size size of the texture to create on the GPU.
         */
        //GPUTexture_New() : Base()
        //{
        //
        //}
        //
        //GPUTexture_New( const uvec2 & size)
        //{
        //    Create(size, TexColourFormat::RGBA, true);
        //}

        GPUTexture()
        {

        }

        GPUTexture( const uvec2 & size,
                    bool              MipMaps=false,
                    TexInternalFormat InternalFormat=TexInternalFormat::RGBA,
                    TexFormat         Format=TexFormat::RGBA,
                    DataType          Type  = DataType::UNSIGNED_BYTE
                    )
        {
            Create(size, MipMaps, InternalFormat, Format, Type);
        }

        inline bool Create(const uvec2 & size,
                           bool MipMaps=true,

                           TexInternalFormat InternalFormat=TexInternalFormat::RGBA,
                           TexFormat texformat = TexFormat::RGBA,
                           DataType datatype = DataType::UNSIGNED_BYTE
                           )
        {

            return( Create(size, InternalFormat, texformat, datatype, 0, MipMaps) );
        }

        /**
         * @brief create Creates a blank texture
         * @param size The size of the texture to create on the GPU.
         */
        inline bool Create(const uvec2                         &size,
                           TexInternalFormat   InternalFormat =  TexInternalFormat::RGBA,
                           TexFormat           Format         =  TexFormat::RGBA,
                           DataType            Type           =  DataType::UNSIGNED_BYTE,
                           void*               Data           =  0,
                           bool                MipMaps        =  true)
        {
            m_Handle.Create();
            m_Handle.Bind();

            if( !m_Handle.GetID() )
            {
                return false;
            }

            auto & I = m_Handle.__GetInfo( );

            glTexImage2D(GL_TEXTURE_2D, 0, (GLuint)InternalFormat, size.x, size.y, 0, (GLuint)Format, (GLuint)Type, Data);
            if(MipMaps)
            {
                glGenerateMipmap(GL_TEXTURE_2D);
            }

            I.Format  = Format;
            I.MipMaps = MipMaps;
            I.Size    = size;
            I.Type    = Type;
            I.InternalFormat = InternalFormat;

            SetTextureWrap( TexWrap::REPEAT, TexWrap::REPEAT);
            SetFilter(TexFilter::LINEAR_MIPMAP_LINEAR, TexFilter::LINEAR_MIPMAP_LINEAR);

            m_Handle.Unbind();
            return true;
        }

        void PasteSubImage( const gla::uvec2 & xy, const TextureBase & T, int level=0);


        /**
         *  Sets the Min and Mag filter for this texture
         *
         * @param Min The MIN filter to use
         * @param Mag The MAG filter to use
         */
        inline void SetFilter( TexFilter Min, TexFilter Mag)
        {
            m_Handle.Bind();

            auto & I = m_Handle.__GetInfo();

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLuint)Min);

            if( Mag == TexFilter::LINEAR_MIPMAP_LINEAR)  Mag = TexFilter::LINEAR;
            if( Mag == TexFilter::LINEAR_MIPMAP_NEAREST) Mag = TexFilter::NEAREST;

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLuint)Mag);

            I.MagFilter = Mag;
            I.MinFilter = Mag;

            m_Handle.Unbind();
        }

        inline void SetTextureWrap( TexWrap S_direction, TexWrap T_direction)
        {
            m_Handle.Bind();
            auto & I = m_Handle.__GetInfo();

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLuint)S_direction);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLuint)T_direction);

            I.S_Wrap = S_direction;
            I.T_Wrap = T_direction;

            m_Handle.Unbind();
        }

        void SetActive( unsigned int unit)
        {
            glActiveTexture(GL_TEXTURE0+unit);
            m_Handle.Bind();
            return;
            //static unsigned int CurrentlyBoundTextureUnits[128] = {0};
            auto CurrentlyBoundTextureUnits = gla::GetActiveTextures();

            if( CurrentlyBoundTextureUnits[unit] != m_Handle.GetID() )
            {
                glActiveTexture(GL_TEXTURE0+unit);
                m_Handle.Bind();
                CurrentlyBoundTextureUnits[unit] = m_Handle.GetID();
            }
        }

        //=============================================================
        /**
         * @brief get_MAX_TEXTURE_SIZE
         * @return the maximum dimension of the textures.
         */
        static GLuint get_MAX_TEXTURE_SIZE()
        {
            static GLint  max = 0;
            if(max!=0) return max;
            glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
            return max;
        }

        static GLuint get_MAX_TEXTURE_IMAGE_UNITS()
        {
            static GLint  max = 0;
            if(max!=0) return max;
            //glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max);
            return max;
        }

        friend class FrameBufferObject;

    };

#if 0
    class GPUTexture
    {
        public:

            GPUTexture() : mDim(0,0), mTextureID(0)
            {

            }

            /**
             * @brief GPUTexture
             * @param size size of the texture to create on the GPU.
             */
            GPUTexture( const uvec2 & size)
            {
                //create(size);
            }



            inline bool Create(const uvec2 & size, TEXTURECOLOURFORMAT InternalFormat=RGBA, int MipMaps=-1)
            {
                return( create(size, InternalFormat, InternalFormat, UNSIGNED_BYTE, 0, MipMaps) );
            }

            /**
             * @brief create Creates a blank texture
             * @param size The size of the texture to create on the GPU.
             */
            inline bool create(const uvec2                         &size,
                               TEXTURECOLOURFORMAT InternalFormat =  RGBA,
                               TEXTURECOLOURFORMAT Format         =  RGBA,
                               FUNDAMENTAL_TYPE    Type           =  UNSIGNED_BYTE,
                               void*               Data           =  0,
                               int                 MipMaps        = -1)
            {
                glGenTextures(1,            &mTextureID);
                glBindTexture(GL_TEXTURE_2D, mTextureID);

                if( !mTextureID )
                {
                    return false;
                }

                if(MipMaps==-1)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, size.x, size.y, 0, Format, Type, Data);
                    glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
                } else {
                    glTexStorage2D(GL_TEXTURE_2D, MipMaps, InternalFormat, size.x, size.y);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x, size.y, Format, Type, Data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                }

                mDim = size;

                mColourFormat = InternalFormat == RED ? 0 :
                                InternalFormat == RG  ? 1 :
                                InternalFormat == RGB ? 2 :
                                InternalFormat == BGR ? 3 :
                                InternalFormat == RGBA? 4 :
                                InternalFormat == BGRA? 5 :
                                InternalFormat == DEPTH_COMPONENT  ? 6 : 7;


                auto id = mTextureID;
                mInfo = std::shared_ptr<GPUTextureInfo>( new GPUTextureInfo, [=](GPUTextureInfo* a){ delete a; glDeleteTextures(1, &id); GLA_DOUT  << "Deleting GPUTexture: " << id << std::endl; } );
                return true;
            }

            void pasteSubImage( const gla::uvec2 & xy, const TextureBase & T, int level=0);

            inline bool create(const uvec2 & size, TEXTURECOLOURFORMAT Format = RGBA)
            {
                 return create(size, Format, Format, UNSIGNED_BYTE, 0);
            }

            /**
             *  Sets the Min and Mag filter for this texture
             *
             * @param Min The MIN filter to use
             * @param Mag The MAG filter to use
             */
            inline void setFilter( TEXTUREFILTERTYPE Min, TEXTUREFILTERTYPE Mag, bool BindFirst=true)
            {
                if(BindFirst) bind();

                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Min);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Mag);
            }

            inline void setTextureWrap( TEXTUREWRAPTYPE S_direction, TEXTUREWRAPTYPE T_direction, bool BindFirst=true)
            {
                if(BindFirst) bind();

                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S_direction);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T_direction);
            }


            inline void bind() const
            {
                glBindTexture(GL_TEXTURE_2D, mTextureID);
            };

             // Keep track of the texture that is currently bound
            static GLuint BindTexture(const GPUTexture & T, bool force = false)
            {

               // static GLuint _Current=0;
               // if( force || _Current != T.getID() )
               // {
                    glBindTexture(GL_TEXTURE_2D,  T.getID());
               //     _Current = T.getID();
               //     return _Current;
               // }
                return T.getID();//_Current;
            }


            void SetActive( unsigned int unit)
            {
                //static unsigned int CurrentlyBoundTextureUnits[128] = {0};
                auto CurrentlyBoundTextureUnits = gla::GetActiveTextures();

                if( CurrentlyBoundTextureUnits[unit] != getID() )
                {
                    glActiveTexture(GL_TEXTURE0+unit);
                    bind();
                    CurrentlyBoundTextureUnits[unit] = getID();
                }
            }

//            static void SetActiveTexture( GPUTexture & T, unsigned int unit)
//            {
//                //static unsigned int CurrentlyBoundTextureUnits[128] = {0};
//                auto CurrentlyBoundTextureUnits = gla::GetActiveTextures();

//                if( CurrentlyBoundTextureUnits[unit] != T.getID() )
//                {
//                    glActiveTexture(GL_TEXTURE0+unit);
//                    BindTexture(T, true);
//                    CurrentlyBoundTextureUnits[unit] = T.getID();
//                }
//            }

            /**
             *  Clears the TextureBase from the GPU. This is not done automatically when the destructor is called.
             *
             */
            void clear()
            {
                if(mTextureID) glDeleteTextures(1, &mTextureID);
                mTextureID = 0;
                mDim = {0,0};
            }

            /**
             * @brief toCPU
             * @return Returns a CPU copy of the texture
             */
            //TextureBase toCPU();

            inline uvec2  size()  const { return mDim; }
            inline GLuint getID() const { return mTextureID;  }

            inline TEXTURECOLOURFORMAT getColourFormat()
            {
                const TEXTURECOLOURFORMAT Form[] = {RED, RG, RGB,BGR, RGBA,BGRA, DEPTH_COMPONENT, DEPTH_STENCIL};
                return Form[mColourFormat];
            }

            inline FUNDAMENTAL_TYPE getIntegralType()
            {
                const FUNDAMENTAL_TYPE Ty[] = {BYTE, UNSIGNED_BYTE, SHORT, UNSIGNED_SHORT, INT, UNSIGNED_INT, FLOAT, DOUBLE};
                return Ty[mFundamentalType];
            }

            //=============================================================
            /**
             * @brief get_MAX_TEXTURE_SIZE
             * @return the maximum dimension of the textures.
             */
            static GLuint get_MAX_TEXTURE_SIZE()
            {
                static GLint  max = 0;
                if(max!=0) return max;
                glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
                return max;
            }

            static GLuint get_MAX_TEXTURE_IMAGE_UNITS()
            {
                static GLint  max = 0;
                if(max!=0) return max;
                //glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
                glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max);
                return max;
            }

       private:
            uvec2         mDim;
            GLuint        mTextureID;
            unsigned char mColourFormat;
            unsigned char mFundamentalType;

            std::shared_ptr<GPUTextureInfo> mInfo;

       friend class FrameBufferObject;
    };
#endif
    //=========================================================================






    /*! @brief TextureBase class for holding image information in rgba format.
     *
     * The TextureBase class holds image information on the CPU. It also contains
     * methods and functions to manipulate the pixel information.
     *
     * Example #1:
     *
     *   \code{.cpp}
     *      TextureBase T(256,256);          // Create a blank texture 256x256
     *      T.a = T.r + T.b;             // Set the alpha channel to be the sum of the red and blue channels
     *
     *      GPUTexture TexGpu = T.toGPU();    // send the texture to the GPU so that it can be used in openGL calls.
     *    \endcode
     *
     * Example #2:
     *
     *   \code{.cpp}
     *      TextureBase T(256,256);          // Create a blank texture 256x256
     *
     *      // set the red channel of the texture using a lambda function
     *      // The lambda function needs to return a float ranging between 0 and 1 and input a const vec2&
     *      T.r = [] (const vec2 & r) { return( cos(glm::length( r-vec2(0.5f))*3.14159); };
     *
     *
     *      // Set teh pixel values of the texture using a lambda function
     *      // The lambda function needs to return a vec4 with values ranging between 0 and 1
     *      T   = [] (const vec2 & r)
     *          {
     *                 float red   =  cos(glm::length( r-vec2(0.5f))*3.14159 );
     *                 float green =  sin(glm::length( r-vec2(0.5f))*3.14159 );
     *                 return( vec4(red,green,1.0,1.0)) ;
     *          };
     *
     *      GPUTexture TexGpu = T.toGPU();    // send the texture to the GPU so that it can be used in openGL calls.
     *    \endcode
     */
    class TextureBase
    {

        protected:

            TextureBase() : mData(0)
            {

            }

            TextureBase( const uvec2 & size, unsigned int components=4) : mData(0)
            {
                mDim.x = size.x;
                mDim.y = size.y;
                mComponents = components;
                mData  = new unsigned char[ mDim[0] * mDim[1] * components];
                memset(mData,0, mDim[0] * mDim[1] * components);
            }

            TextureBase(uint w, uint h, unsigned int components=4) : mData(0)
            {
                mDim.x = w;
                mDim.y = h;

                mComponents = components;
                mData  = new unsigned char[ mDim[0] * mDim[1] * components];
                memset(mData,0, mDim[0] * mDim[1] * components);
            }

            explicit TextureBase(TextureBase & T) :  mData(0)
            {

                mDim  = T.mDim;
                mComponents = T.mComponents;
                mData = new unsigned char[mDim[0]*mDim[1]*T.mComponents];
                memcpy( mData, (void*)T.mData, mDim[0] * mDim[1] * T.mComponents);

            }

            TextureBase(TextureBase && T) : mData(0)
            {
                if(mData) clear();

                mData   = T.mData;
                mDim    = T.mDim;
                mComponents = T.mComponents;

                T.mDim  = { 0, 0 };
                T.mData = 0;
                T.mComponents = 0;
                //GLA_DOUT  << " TextureBase::Move constructor\n";

            }

    public:
            TextureBase & operator=(TextureBase && T)
            {
                if(mData) clear();

                mData       = T.mData;
                mDim        = T.mDim;
                mComponents = T.mComponents;

                T.mDim        = {0,0};
                T.mData       = 0;
                T.mComponents = 0;

                //GLA_DOUT  << " TextureBase::Move operator\n";
                return *this;
            }

            /**
             * Frees all memory associated with this texture including GPU
             * data. The OpenGL TextureBase id will be freed.
             */
            ~TextureBase()
            {
                clear();
            }


            /**
             * @brief get_MAX_TEXTURE_SIZE
             * @return the maximum dimension of the textures.
             */
            static GLuint get_MAX_TEXTURE_SIZE()
            {
                GLint max;
                glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);

                return max;
            }

            //===========================================================================
            // Loading functions
            //===========================================================================

            /**
             * Load an image from a raw buffer. The buffer is NOT raw pixels
             * it is the binary data of an actual encoded image such as jpg or png
             *
             * @param Buffer Pointer to the buffer
             * @param buffersize The number of bytes in the buffer
             */
            void loadFromMemory( unsigned char * Buffer, int buffersize)
            {
                int x, y, comp;
                GLA_uc           * img = ImageLoader::GLA_load_from_memory( (GLA_uc*) Buffer, buffersize, &x, &y, &comp, 0 );

                if( img )
                {
                    _handleRawPixels(img, static_cast<unsigned int>( x ), static_cast<unsigned int>( y ) );
                    mComponents = comp;
                }
                else
                {
                    GLA_DOUT  << "from Memory error: " << x << "," << y << "\n";
                }

            }

            /**
             *  Load an image from a file.
             *
             * @param Path a path to the file
             */
            void loadFromPath( const std::string & path, unsigned int ForceNumberChannels=0)
            {
                int x, y, comp;

                //==========================================================
                // Load the TextureBase from an image file.
                //==========================================================
                unsigned char * img = ImageLoader::GLA_load(path.c_str(), &x, &y, &comp, ForceNumberChannels );

                if( img )
                {
                    _handleRawPixels(img, static_cast<unsigned int>( x ), static_cast<unsigned int>( y ) );
                    mComponents = ForceNumberChannels!=0 ? ForceNumberChannels : comp;
                    GLA_DOUT  << "TextureBase loaded with #components = " << mComponents << std::endl;

                } else {
                    //GLA_DOUT  << "Error loading texture: " << path << std::endl;
                    throw std::runtime_error( std::string("Error loading texture: ") + path);
                }

            }


            //===========================================================================

            /**
             * Sends the texture to the GPU and returns a GPUTexture object. This does
             * not clear the texture from CPU memory. Once the texture is sent to the
             * gpu, you can clear the memory from the cpu if you do not need it again.
             *
             * @return A GPUTexture
             */
            GPUTexture ToGPU(bool MipMaps=true)
            {
                GPUTexture GPU;

                TexFormat format;
                TexInternalFormat iFormat;

                switch( mComponents )
                {
                    case 1: iFormat = TexInternalFormat::RED;  format = TexFormat::RED;  break;
                    case 2: iFormat = TexInternalFormat::RG;   format = TexFormat::RG;   break;
                    case 3: iFormat = TexInternalFormat::RGB;  format = TexFormat::RGB;  break;
                    case 4: iFormat = TexInternalFormat::RGBA; format = TexFormat::RGBA; break;
                }

                if( GPU.Create( mDim, iFormat, format, DataType::UNSIGNED_BYTE, (void*)mData), MipMaps )
                {
                    GPU.SetFilter(     TexFilter::LINEAR, TexFilter::LINEAR);
                    GPU.SetTextureWrap(TexWrap::REPEAT, TexWrap::REPEAT);

                    return GPU;
                }

                return GPU;
            }

#if 0
            GPUTexture toGPU(int MipMaps=-1)
            {
                GPUTexture GPU;

                TEXTURECOLOURFORMAT format;

                switch( mComponents )
                {
                    case 1: format = RED;  break;
                    case 2: format = RG;   break;
                    case 3: format = RGB;  break;
                    case 4: format = RGBA; break;
                }

                //GPU.mColourFormat = format;
               // GLA_DOUT  << "Sending to GPU: #components: " << mComponents << std::endl;
                if( GPU.create( mDim, format, format, UNSIGNED_BYTE, (void*)mData), MipMaps )
                {
                    GPU.setFilter(     LINEAR, LINEAR, false );
                    GPU.setTextureWrap(REPEAT, REPEAT, false);

                    return GPU;
                }

                return GPU;
            }

#endif
            inline unsigned int getChannels() const
            {
                return mComponents;
            }

            /**
             * Clears the image data from the CPU.
             */
            void clear()
            {
                if( mData ) free(mData);
                mData = 0;
            }






            ///**
            // * @brief get Returns the value of the pixel (x,y)
            // * @param x
            // * @param y
            // * @return a copy of the pixel at location (x,y)
            // */
            //inline PixelType  get(int x, int y) const { return mData[ y*mDim[0] + x ]; }

            /**
             * @brief size Gets the dimensions of the texture
             * @return
             */
            inline gla::uvec2  size() const { return mDim; };

            /**
             * @brief getRawData gets a pointer to the raw pixel data so that it can be sent to OpenGL
             * @return a pointer to the starting byte;
             */
            inline void* getRawData() const { return (void*)mData; };



            inline void resize( const uvec2 & newSize)
            {
                //GLA_DOUT  << "Resizing from " << mDim.x << ", " << mDim.y << " to " << newSize.x << "," << newSize.y << std::endl;
                TextureBase T( newSize.x, newSize.y , mComponents);
                auto d = T.size();

                for(int j=0; j < d.y; j++)
                for(int i=0; i < d.x; i++)
                for(int z=0; z < mComponents; z++)
                    {
                        // scale the x and y dimeninos to be between 0 and 1
                        float x = (float)i / (float)d.x;
                        float y = (float)j / (float)d.y;


                        // Find the index on the main texture
                        int X = static_cast<int>(x * mDim.x);
                        int Y = static_cast<int>(y * mDim.y);

                        //assert( X<=1024 );
                        // sample the 4 locations
                        float f00 = float( (*this)(X,Y    , z ) );
                        float f01 = float( (*this)(X,Y+1  , z ) );
                        float f10 = float( (*this)(X+1,Y  , z ) );
                        float f11 = float( (*this)(X+1,Y+1, z ) );

                        float s = x * static_cast<float>(mDim.x);
                        float t = y * static_cast<float>(mDim.y);

                        // get the fractional part
                        s = s-floor(s);
                        t = t-floor(t);

                        T(i,j,z) = f00 * (1-s)*(1-t) + f10*s*(1-t) + f01*(1-s)*t + f11*s*t;

                    }

                //GLA_DOUT  << "Resizing complete" << std::endl;

                *this = std::move(T);
            }


            /**
             * Access pixel data on the CPU. This is unpredictable if you
             * have cleared the CPU data.
             */
            inline unsigned char & operator ()(int x, int y, int comp) const
            {
                return mData[ y*(mDim[0] * mComponents) + x*mComponents + comp ];
            }

            //=====================================
            // Operators
            //=====================================
            TextureBase& operator=(  std::function<gla::ucol4(float,float) > F)
            {
                uvec2 S = size();
                float W = 1.0 / (float)S.x;
                float H = 1.0 / (float)S.y;
                for(int y =0; y < S.y; y++)
                    for(int x=0; x < S.x; x++)
                    {
                        auto c = (F( (float)x * W, (float)y*H ));
                        (*this)(x,y,0) = c[0];
                        (*this)(x,y,1) = c[1];
                        (*this)(x,y,2) = c[2];
                        (*this)(x,y,3) = c[3];
                    }

                return *this;
            }

            TextureBase& operator=(  std::function<gla::vec4(float,float) > F)
            {
                uvec2 S = size();
                float W = 1.0 / (float)S.x;
                float H = 1.0 / (float)S.y;
                for(int y =0; y < S.y; y++)
                    for(int x=0; x < S.x; x++)
                    {
                        auto c = (F( (float)x * W, (float)y*H ) );
                        (*this)(x,y,0) = (unsigned char)c[0]*255.f;
                        (*this)(x,y,1) = (unsigned char)c[1]*255.f;
                        (*this)(x,y,2) = (unsigned char)c[2]*255.f;
                        (*this)(x,y,3) = (unsigned char)c[3]*255.f;
                    }

                return *this;
            }


            inline TextureBase operator  + ( const TextureBase & c) const
            {
                uvec2 siz      = glm::min( size(), c.size() );
                unsigned int C = mComponents;
                TextureBase T(    siz.x,siz.y  ,C   );

                for(int y=0;y<siz.y;y++)
                for(int x=0;x<siz.x;x++)
                for(int z=0;z<C;z++)
                {
                    T(x,y,z) = (*this)(x,y,z) + c(x,y,z);
                }

               return T;
            };

            inline TextureBase operator  - ( const TextureBase & c) const
            {
                uvec2 siz      = glm::min( size(), c.size() );
                unsigned int C = mComponents;
                TextureBase T(    siz.x,siz.y  ,C   );

                for(int y=0;y<siz.y;y++)
                for(int x=0;x<siz.x;x++)
                for(int z=0;z<C;z++)
                {
                    T(x,y,z) = (*this)(x,y,z) - c(x,y,z);
                }

               return T;
            };




            inline TextureBase        operator  - ( const float & c) const
            {
               uvec2 siz      = size();
               unsigned int C = mComponents;
               TextureBase T(    siz.x,siz.y ,C    );

                //DOUBLE_LOOP(siz)  {  T(x,y) = LeftOp OPP RightOp ;  }
                for(int y=0;y<siz.y;y++)
                for(int x=0;x<siz.x;x++)
                for(int z=0;z<C;z++)
                {
                    T(x,y,z) = (*this)(x,y,z) - static_cast<unsigned char>(c*255.f);
                }

               return T;
            };

            inline TextureBase        operator  + ( const float & c) const
            {
               uvec2 siz      = size();
               unsigned int C = mComponents;
               TextureBase T(    siz.x,siz.y ,C    );

                //DOUBLE_LOOP(siz)  {  T(x,y) = LeftOp OPP RightOp ;  }
                for(int y=0;y<siz.y;y++)
                for(int x=0;x<siz.x;x++)
                for(int z=0;z<C;z++)
                {
                    T(x,y,z) = (*this)(x,y,z) + static_cast<unsigned char>(c*255.f);
                }

               return T;
            };

            inline TextureBase        operator  * ( const float & c) const
            {
               uvec2 siz      = size();
               unsigned int C = mComponents;
               TextureBase T(    siz.x,siz.y ,C    );

                //DOUBLE_LOOP(siz)  {  T(x,y) = LeftOp OPP RightOp ;  }
                for(int y=0;y<siz.y;y++)
                for(int x=0;x<siz.x;x++)
                for(int z=0;z<C;z++)
                {
                    T(x,y,z) = (*this)(x,y,z) * c;
                }

               return T;
            };

            inline TextureBase        operator  / ( const float & c) const
            {
               uvec2 siz      = size();
               unsigned int C = mComponents;
               TextureBase T(    siz.x,siz.y ,C    );

                //DOUBLE_LOOP(siz)  {  T(x,y) = LeftOp OPP RightOp ;  }
                for(int y=0;y<siz.y;y++)
                for(int x=0;x<siz.x;x++)
                for(int z=0;z<C;z++)
                {
                    T(x,y,z) = (*this)(x,y,z) / c;
                }

               return T;
            };

            #define OPERATOR(OP)                                                 \
            inline TextureBase        operator  OP ( const unsigned char & c) const   \
            {                                                                    \
               uvec2 siz      = size();                                          \
               unsigned int C = mComponents;                                     \
               TextureBase T(    siz.x,siz.y ,C    );                                \
                                                                                 \
                for(int y=0; y<siz.y;y++)                                         \
                for(int x=0; x<siz.x;x++)                                         \
                for(int z=0; z<C    ;z++)                                         \
                {                                                                \
                    T(x,y,z) = (*this)(x,y,z) OP c;                               \
                }                                                                \
                                                                                 \
               return T;                                                         \
            };



              OPERATOR( - )
              OPERATOR( + )
              OPERATOR( * )
              OPERATOR( / )


        public:
            void _handleRawPixels(unsigned char * buffer, uint width, uint height)
            {
                clear();

                mDim = {width, height};

                mData = buffer;;

            }

            unsigned char   *mData;
            int              mComponents;
            uvec2            mDim;


    };


    class Texture;

    //=====================

    class ChannelRef
    {
    public:
        ChannelRef(TextureBase * parent, unsigned int comp) : mParent(parent), mComp(comp)
        {
        }

        inline unsigned char & operator()(int x, int y)
        {
            return (*mParent)(x,y, mComp);
        }

        ChannelRef& operator=(ChannelRef & other)
        {
            int c   = other.mComp;
            uvec2 S = glm::min( mParent->size(), other.mParent->size() );

            printf(" === %d %d ===\n", other.mComp, mComp);

            for(int y =0; y < S.y; y++)
                for(int x=0; x < S.x; x++)
                {
                    (*mParent)(x,y,(int)mComp) = (*other.mParent)(x, y, c);
                }

            return *this;
        }




        ChannelRef& operator=(const Texture & other);


        Texture operator+(ChannelRef & other);
        Texture operator-(ChannelRef & other);
        Texture operator*(ChannelRef & other);
        Texture operator/(ChannelRef & other);


        ChannelRef& operator=(  std::function<float(float,float) > F)
        {
            uvec2 S = mParent->size();
            float W = 1.0 / (float)S.x;
            float H = 1.0 / (float)S.y;
            for(int y =0; y < S.y; y++)
                for(int x=0; x < S.x; x++)
                {
                    (*mParent)(x,y,(int)mComp) = (unsigned char)(F( (float)x * W, (float)y*H ) * 255.f);
                }

            return *this;
        }


        unsigned int    mComp;
        TextureBase     *mParent;
    };



    class Texture : public TextureBase
    {
        public:
            ChannelRef r;
            ChannelRef g;
            ChannelRef b;
            ChannelRef a;


            Texture() : r(this,0),g(this,1),b(this,2),a(this,3)
            {

            }

            Texture( const uvec2 & size, unsigned int components=4) : r(this,0),g(this,1),b(this,2),a(this,3), TextureBase(size,components)
            {
            }

            Texture(uint w, uint h, unsigned int components=4) : r(this,0),g(this,1),b(this,2),a(this,3), TextureBase(w,h,components)
            {
            }

            Texture(const std::string & path, unsigned int ForceNumberChannels=0) : r(this,0),g(this,1),b(this,2),a(this,3)
            {
                loadFromPath(path, ForceNumberChannels);
            }

            explicit Texture(Texture & T) : r(this,0),g(this,1),b(this,2),a(this,3),  TextureBase(T)
            {
            }

            Texture(Texture && T) : r(this,0),g(this,1),b(this,2),a(this,3),  TextureBase(T)
            {
                *this = std::move(T);
            }

            Texture & operator=(Texture && T)
            {
                //*this = std::move(T);

                if(mData) clear();

                mData       = T.mData;
                mDim        = T.mDim;
                mComponents = T.mComponents;

                T.mDim        = {0,0};
                T.mData       = 0;
                T.mComponents = 0;

                //GLA_DOUT  << " Texture::Move operator\n";
                //return *this;

                return *this;

            }
    };


    inline ChannelRef& ChannelRef::operator=(const Texture & other)
    {
        uvec2 S = glm::min( mParent->size(), other.size() );

        for(int y =0; y < S.y; y++)
            for(int x=0; x < S.x; x++)
            {
                (*mParent)(x,y,(int)mComp) = other(x, y, 0);
            }
        return *this;
    }


    inline Texture ChannelRef::operator+(ChannelRef & other)
    {
        int c   = other.mComp;
        uvec2 S = glm::min( mParent->size(), other.mParent->size() );
        Texture T( S, 1);



        for(int y =0; y < S.y; y++)
            for(int x=0; x < S.x; x++)
            {
                T(x,y,0) = (*mParent)(x, y, mComp) + (*other.mParent)(x, y, c);
            }

        return std::move(T);
    }

    inline Texture ChannelRef::operator-(ChannelRef & other)
    {
        int c   = other.mComp;
        uvec2 S = glm::min( mParent->size(), other.mParent->size() );
        Texture T( S, 1);



        for(int y =0; y < S.y; y++)
            for(int x=0; x < S.x; x++)
            {
                T(x,y,0) = (*mParent)(x, y, mComp) - (*other.mParent)(x, y, c);
            }

        return std::move(T);
    }

    inline Texture ChannelRef::operator*(ChannelRef & other)
    {
        int c   = other.mComp;
        uvec2 S = glm::min( mParent->size(), other.mParent->size() );
        Texture T( S, 1);


        for(int y =0; y < S.y; y++)
            for(int x=0; x < S.x; x++)
            {
                float t = (float)(*mParent)(x, y, mComp) * (float)(*other.mParent)(x, y, c);
                T(x,y,0) = (unsigned char)(t / (255.f));
            }

        return std::move(T);
    }

    inline Texture ChannelRef::operator/(ChannelRef & other)
    {
        int c   = other.mComp;
        uvec2 S = glm::min( mParent->size(), other.mParent->size() );
        Texture T( S, 1);


        for(int y =0; y < S.y; y++)
            for(int x=0; x < S.x; x++)
            {
                float t = (float)(*mParent)(x, y, mComp) / (float)(*other.mParent)(x, y, c);
                T(x,y,0) = (unsigned char)(t * (255.f));
            }

        return std::move(T);
    }



    inline Texture operator / (const ChannelRef & left, const Texture & right)
    {

        uvec2 S = glm::min( left.mParent->size(), right.size() );
        Texture T( S, 1);


        for(int y =0; y < S.y; y++)
            for(int x=0; x < S.x; x++)
            {
                T(x,y,0) = (unsigned char) (  (float)(*left.mParent)(x, y, left.mComp) / (float) right(x, y, 0) * (255.f)  );
            }

        return std::move(T);
    }

    inline Texture operator * (const  ChannelRef & left,  const Texture & right)
    {

        uvec2 S = glm::min( left.mParent->size(), right.size() );
        Texture T( S, 1);


        for(int y =0; y < S.y; y++)
            for(int x=0; x < S.x; x++)
            {
                T(x,y,0) = (unsigned char) (  (float)(*left.mParent)(x, y, left.mComp) * (float) right(x, y, 0) * (1.0f/255.f)  );
            }

        return std::move(T);
    }

    inline Texture operator * ( const Texture & left,  const ChannelRef & right)
    {
       return right * left;
    }

    inline Texture operator + (const  ChannelRef & left,  const Texture & right)
    {

        uvec2 S = glm::min( left.mParent->size(), right.size() );
        Texture T( S, 1);


        for(int y =0; y < S.y; y++)
            for(int x=0; x < S.x; x++)
            {
                T(x,y,0) = (*left.mParent)(x, y, left.mComp) + right(x, y, 0);
            }

        return std::move(T);
    }

    inline Texture operator - ( const ChannelRef & left,  const Texture & right)
    {
        uvec2 S = glm::min( left.mParent->size(), right.size() );
        Texture T( S, 1);


        for(int y =0; y < S.y; y++)
            for(int x=0; x < S.x; x++)
            {
                T(x,y,0) = (*left.mParent)(x, y, left.mComp) - right(x, y, 0);
            }

        return std::move(T);
    }


    inline Texture operator - ( const Texture & left,  const ChannelRef & right)
    {
        uvec2 S = glm::min( left.size(), right.mParent->size() );
        Texture T( S, 1);


        for(int y =0; y < S.y; y++)
            for(int x=0; x < S.x; x++)
            {
                T(x,y,0) = - left(x, y, 0) - (*right.mParent)(x, y, right.mComp);
            }

        return std::move(T);
    }

//, GL_RGB, GL_RGBA, GL_LUMINANCE, and .
#if 0
    inline void GPUTexture::PasteSubImage( const gla::uvec2 & xy, const TextureBase & T, int level)
    {
        if(!mTextureID) return;

        auto d = T.getRawData();
        if(!d) return;

        //have a bigger image bigImage and the
        // subImage sub
       // GLA_DOUT  << "Copying subimage over\n";
        glBindTexture(GL_TEXTURE_2D,   mTextureID);

        // Note, i/j might have the origin int he bottom left corner instead of top left.
        GLenum Format[] = {GL_RED, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA};
       // GLA_DOUT  << "PASTING SUB IMAGE: " << T.getChannels()-1 <<std::endl;
        glTexSubImage2D(GL_TEXTURE_2D, level, xy.x, xy.y, T.size().x, T.size().y, Format[T.getChannels()-1], GL_UNSIGNED_BYTE, d );

      //  GLA_DOUT  << "Error code: " << glGetError() << std::endl;

    }
#endif

    inline void GPUTexture::PasteSubImage( const gla::uvec2 & xy, const TextureBase & T, int level)
    {
        if(!m_Handle.GetID()) return;

        auto d = T.getRawData();
        if(!d) return;

        //have a bigger image bigImage and the
        // subImage sub
       // GLA_DOUT  << "Copying subimage over\n";
        Bind();

        // Note, i/j might have the origin int he bottom left corner instead of top left.
        GLenum Format[] = {GL_RED, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA};
       // GLA_DOUT  << "PASTING SUB IMAGE: " << T.getChannels()-1 <<std::endl;
        glTexSubImage2D(GL_TEXTURE_2D,
                        level,
                        xy.x, xy.y,
                        T.size().x,
                        T.size().y,
                        Format[T.getChannels()-1],
                        (GLenum)m_Handle.GetInfo().Type,
                        d );

      //  GLA_DOUT  << "Error code: " << glGetError() << std::endl;

    }

}


#endif // TEXTURE_H

