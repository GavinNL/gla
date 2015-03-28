#ifndef GLRE_TEXTURE_H
#define GLRE_TEXTURE_H

#include <gla/global.h>
#include <iostream>
#include <string.h>
#include <functional>

namespace gla {

    class Texture;
    class TextureChannel;


    class ChannelReference
    {
        public:
            ChannelReference(Texture* pTexture, int pIndex)
            {
                mTexture = pTexture;
                mIndex   = pIndex;
            };

            inline uvec2 size() const;

            inline unsigned char & operator()(int i, int j);

            TextureChannel    operator+( ChannelReference & c);
            TextureChannel    operator-( ChannelReference & c);
            void              operator+=( ChannelReference & c);
            void              operator-=( ChannelReference & c);

            void              operator+=( unsigned char c);
            void              operator-=( unsigned char c);
            TextureChannel    operator+(  unsigned char c);
            TextureChannel    operator-(  unsigned char c);

            ChannelReference& operator=(        unsigned char c);
            ChannelReference& operator=(        ChannelReference & c);
            ChannelReference& operator=(  const TextureChannel   & c);


            /**
             * @brief operator = sets the value of the channel using a lambda expression
             * @param F a function or lambda that takes in a const uvec2 and returns a float
             *
             * You can set the colour values of a texture channel using a labmda function. The
             * lambda function must take in a const & uvec2 and and return a float
             * between 0 and 1
             */
            ChannelReference&       operator=( std::function<float(const vec2 &)> F);


            private:
                Texture *   mTexture;
                int         mIndex;
    };

    //===========================================================================
    // GPUTexture
    //   - A 2D texture stored on the GPU
    //===========================================================================
    class GPUTexture
    {
        public:


            typedef enum {
                NEAREST                = GL_NEAREST,
                LINEAR                 = GL_LINEAR,
                NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
                LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
                NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
                LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
            } FilterType;


            GPUTexture() : mDim(0,0), mTextureID(0)
            {

            }

            /**
             *  Sets the Min and Mag filter for this texture
             *
             * @param Min The MIN filter to use
             * @param Mag The MAG filter to use
             */
            inline void setFilter( FilterType Min, FilterType Mag)
            {
                bind();

                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Min);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Mag);
            }

            inline void bind()
            {
                glBindTexture(GL_TEXTURE_2D, mTextureID);
            };


            /**
             *  Sets this texture to be the active texture for
             *  the.
             *
             *  @param unit The texture unit to set it as, usually between 0-32, but this is HW dependant. Default is 0.
             *
             */
            inline void setActiveTexture(unsigned int unit=0)
            {
                glActiveTexture(GL_TEXTURE0 + unit);
                glBindTexture(GL_TEXTURE_2D, mTextureID);
            }

            /**
             *  Clears the Texture from the GPU. This is not done automatically when the destructor is called.
             *
             */
            void clear()
            {
                if(mTextureID) glDeleteTextures(1, &mTextureID);
                mDim = {0,0};
            }

            /**
             * @brief toCPU
             * @return Returns a CPU copy of the texture
             */
            Texture toCPU();

            inline uvec2  size()  const { return mDim; }
            inline GLuint getID() const { return mTextureID;  }

            //=============================================================
            /**
             * @brief get_MAX_TEXTURE_SIZE
             * @return the maximum dimension of the textures.
             */
            static GLuint get_MAX_TEXTURE_SIZE()
            {
                GLint  max;
                glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
                return max;
            }

       private:
            uvec2        mDim;
            GLuint       mTextureID;

       friend class Texture;
    };


    //=========================================================================
    // Texture Channel Class.
    //=========================================================================

    /**
     * @brief The TextureChannel class.
     *
     * The TextureChannel class is used to reference individual channels
     * of the texture. The user should not need to create an instance of this class.
     */
    class TextureChannel
    {

    public:
        TextureChannel() : mData(0)
        {

        }

        ~TextureChannel()
        {
            clear();
        }

        TextureChannel(uint w, uint h) : mData(0)
        {
            mDim.x = w;
            mDim.y = h;
            mData  = new unsigned char[ mDim[0] * mDim[1] ];
        }

        TextureChannel(TextureChannel & T) :  mData(0)
        {

            mDim  = T.mDim;

            mData = new unsigned char[mDim[0]*mDim[1]];
            memcpy( mData, (void*)T.mData, mDim[0]*mDim[1]);
        }

        TextureChannel(TextureChannel && T) : mData(0)
        {
            if(mData) clear();

            mData = T.mData;
            mDim  = T.mDim;
            T.mDim = {0,0};
            T.mData = 0;

        }

        TextureChannel & operator=(TextureChannel && T)
        {
            if(mData) clear();

            mData = T.mData;
            mDim  = T.mDim;
            T.mDim = {0,0};
            T.mData = 0;

            return *this;
        }

        void clear()
        {
            if(mData) delete [] mData;

            mData = 0;
            mDim = {0,0};

        }

        inline unsigned char & operator ()(int x, int y) { return mData[ y*mDim[ 0 ] + x ]; }
        inline unsigned char   get(int x, int y) const   { return mData[ y*mDim[ 0 ] + x ]; }

        inline uvec2 size() { return mDim; };


        // Operators
        TextureChannel operator+(TextureChannel & R);
        TextureChannel operator-(TextureChannel & R);
        TextureChannel operator-(unsigned char r);
        TextureChannel operator+(unsigned char r);

        //
        void operator+=(TextureChannel & R);
        void operator-=(TextureChannel & R);
        void operator-=(   unsigned char r);
        void operator+=(   unsigned char r);


        /**
         * @brief operator = sets the value of the channel using a lambda expression
         * @param F a function or lambda that takes in a const & vec2 and returns a float
         *
         * You can set the colour values of a texture channel using a labmda function. The
         * lambda function must take in a const & vec2 and and return a float
         * between 0 and 1. The input vec2 will have component values ranging from 0.1
         */
        TextureChannel&       operator=( std::function<        float(const  vec2 &)> F);

        private:
            unsigned char * mData;
            uvec2           mDim;
    };
    //=========================================================================


    /*! @brief Texture class for holding image information in rgba format.
     *
     * The Texture class holds image information on the CPU. It also contains
     * methods and functions to manipulate the pixel information.
     *
     * Example #1:
     *
     *   \code{.cpp}
     *      Texture T(256,256);          // Create a blank texture 256x256
     *      T.a = T.r + T.b;             // Set the alpha channel to be the sum of the red and blue channels
     *
     *      GPUTexture TexGpu = T.toGPU();    // send the texture to the GPU so that it can be used in openGL calls.
     *    \endcode
     *
     * Example #2:
     *
     *   \code{.cpp}
     *      Texture T(256,256);          // Create a blank texture 256x256
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
    class Texture
    {

        public:

            Texture() : mData(0), r(this,0), g(this,1), b(this,2), a(this,3)
            {

            }

            Texture(uint w, uint h) : mData(0), r(this,0), g(this,1), b(this,2), a(this,3)
            {
                mDim.x = w;
                mDim.y = h;
                mData  = new gla::ucol4[ mDim[0] * mDim[1] ];
            }

            Texture(const std::string & path) :  mData(0), r(this,0), g(this,1), b(this,2), a(this,3)
            {
                loadFromPath(path);
            }

            Texture(Texture & T) :  mData(0), r(this,0), g(this,1), b(this,2), a(this,3)
            {

                mDim  = T.mDim;

                mData = new gla::ucol4[mDim[0]*mDim[1]];
                memcpy( mData, (void*)T.mData, mDim[0]*mDim[1] * 4);

            }

            Texture(Texture && T) : mData(0), r(this,0), g(this,1), b(this,2), a(this,3)
            {
                if(mData) clear();

                mData   = T.mData;
                mDim    = T.mDim;
                T.mDim  = { 0, 0 };
                T.mData = 0;

            }

            Texture & operator=(Texture && T)
            {
                if(mData) clear();

                mData   = T.mData;
                mDim    = T.mDim;
                T.mDim  = {0,0};
                T.mData = 0;

                return *this;
            }

            /**
             * Frees all memory associated with this texture including GPU
             * data. The OpenGL Texture id will be freed.
             */
            ~Texture()
            {
                clear();
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
            void loadFromMemory( unsigned char * Buffer, int buffersize);

            /**
             *  Load an image from a file.
             *
             * @param Path a path to the file
             */
            void loadFromPath( const std::string & path);
            //===========================================================================

            /**
             * Sends the texture to the GPU and returns a GPUTexture object. This does
             * not clear the texture from CPU memory. Once the texture is sent to the
             * gpu, you can clear the memory from the cpu if you do not need it again.
             *
             * @return A GPUTexture
             */
            GPUTexture toGPU();


            /**
             * Clears the image data from the CPU.
             */
            void clear();

            /**
             * @brief get Returns the value of the pixel (x,y)
             * @param x
             * @param y
             * @return a copy of the pixel at location (x,y)
             */
            inline gla::ucol4  get(int x, int y) const { return mData[ y*mDim[0] + x ]; }

            /**
             * @brief size Gets the dimensions of the texture
             * @return
             */
            inline gla::uvec2  size() const { return mDim; };

            /**
             * @brief getRawData gets a pointer to the raw pixel data so that it can be sent to OpenGL
             * @return a pointer to the starting byte;
             */
            inline gla::ucol4* getRawData() const { return mData; };

            /**
             * Access pixel data on the CPU. This is unpredictable if you
             * have cleared the CPU data.
             */
            inline gla::ucol4 & operator ()(int x, int y) { return mData[ y*mDim[0] + x ]; }

            //=====================================
            // Operators
            //=====================================
            Texture        operator+(  Texture & c);
            Texture        operator-(  Texture & c);
            void           operator+=( Texture & c);
            void           operator-=( Texture & c);

            Texture        operator+ (  ChannelReference & c );
            Texture        operator- (  ChannelReference & c );
            void           operator+=(  ChannelReference & c );
            void           operator-=(  ChannelReference & c );

            void           operator+=( unsigned char c );
            void           operator-=( unsigned char c );
            Texture        operator+(  unsigned char c );
            Texture        operator-(  unsigned char c );

            Texture&       operator=(        unsigned char c);
            Texture&       operator=(        ChannelReference & c);
            Texture&       operator=(  const TextureChannel   & c);

            Texture&       operator=( std::function<vec4 (const vec2 &)> F);
            Texture&       operator=( std::function<float(const vec2 &)> F);

            ChannelReference r;
            ChannelReference g;
            ChannelReference b;
            ChannelReference a;

        private:
            void _handleRawPixels(unsigned char * buffer, uint width, uint height);

            gla::ucol4 *mData;

            uvec2        mDim;


    };



    inline unsigned char & ChannelReference::operator()(int i, int j)
    {
        return (*mTexture)(i,j)[mIndex];
    };




    inline uvec2 ChannelReference::size() const
    {
        return mTexture->size();
    };


}
#endif // TEXTURE_H

