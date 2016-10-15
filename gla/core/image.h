/*
 * MIT License
 *
 * Copyright (c) [year] [fullname]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GLA_Image_H
#define GLA_Image_H

#include "types.h"
//#include <gla/global.h>
//#include <gla/types.h>
//#include <gla/exceptions.h>
#include <iostream>
#include <string.h>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstdint>

//#include <gla/handle.h>
#ifdef GLA_IMAGE_USE_STB
    #include <stb/stb_image.h>
#elif GLA_IMAGE_USE_FREEIMAGE
    #include <freeimage.h>
#else
    #include <gla/core/stb_image_headeronly.h>
    #include <gla/core/stb_image_write_headeronly.h>
#endif


/**
 * \page page_Testing
 *  this is a test poage
 *
 */
namespace gla { namespace experimental {

    class ChannelRef;
    class ImageBase;

    class GPUImageInfo
    {
        int temp;
    };

    inline std::vector<unsigned int> & GetActiveImages()
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




    /*! @brief ImageBase class for holding image information in rgba format.
     *
     * The ImageBase class holds image information on the CPU. It also contains
     * methods and functions to manipulate the pixel information.
     *
     * Example #1:
     *
     *   \code{.cpp}
     *      ImageBase T(256,256);          // Create a blank Image 256x256
     *      T.a = T.r + T.b;             // Set the alpha channel to be the sum of the red and blue channels
     *
     *      GPUImage TexGpu = T.toGPU();    // send the Image to the GPU so that it can be used in openGL calls.
     *    \endcode
     *
     * Example #2:
     *
     *   \code{.cpp}
     *      ImageBase T(256,256);          // Create a blank Image 256x256
     *
     *      // set the red channel of the Image using a lambda function
     *      // The lambda function needs to return a float ranging between 0 and 1 and input a const vec2&
     *      T.r = [] (const vec2 & r) { return( cos(glm::length( r-vec2(0.5f))*3.14159); };
     *
     *
     *      // Set teh pixel values of the Image using a lambda function
     *      // The lambda function needs to return a vec4 with values ranging between 0 and 1
     *      T   = [] (const vec2 & r)
     *          {
     *                 float red   =  cos(glm::length( r-vec2(0.5f))*3.14159 );
     *                 float green =  sin(glm::length( r-vec2(0.5f))*3.14159 );
     *                 return( vec4(red,green,1.0,1.0)) ;
     *          };
     *
     *      GPUImage TexGpu = T.toGPU();    // send the Image to the GPU so that it can be used in openGL calls.
     *    \endcode
     */
    class ImageBase
    {


        protected:

            ImageBase() : mData(0)
            {

            }

            ImageBase( const uvec2 & size, unsigned int components=4) : mData(0)
            {
                //mDim.x = size.x;
                //mDim.y = size.y;
                //mComponents = components;
                //mData  = new unsigned char[ mDim[0] * mDim[1] * components];
                Create(size, components);
                memset(mData,0, mDim[0] * mDim[1] * components);
            }

			ImageBase(std::uint32_t w, std::uint32_t h, std::uint32_t components = 4) : mData(0)
            {
                //mDim.x = w;
                //mDim.y = h;
                //mComponents = components;
                //mData  = new unsigned char[ mDim[0] * mDim[1] * components];
                Create( uvec2(w,h), components);
                memset(mData,0, mDim[0] * mDim[1] * components);
            }

            explicit ImageBase(const ImageBase & T) :  mData(0)
            {
                //mDim  = T.mDim;
                //mComponents = T.mComponents;
                //mData = new unsigned char[mDim[0]*mDim[1]*T.mComponents];
                Create( T.mDim, T.mComponents);
                memcpy( mData, (void*)T.mData, mDim[0] * mDim[1] * T.mComponents);

            }

            ImageBase(ImageBase && T) : mData(0)
            {
                *this = std::move(T);
                //if( mData ) clear();
                //
                //mData   = T.mData;
                //mDim    = T.mDim;
                //mComponents = T.mComponents;
                //
                //T.mDim  = { 0, 0 };
                //T.mData = 0;
                //T.mComponents = 0;
                //GLA_DOUT  << " ImageBase::Move constructor\n";

            }



    public:


            void Create( const uvec2 & size, unsigned int components=4)
            {
                if(mData)
                    delete [] mData;

                mDim        = size;
                mComponents = components;
                mData       = new unsigned char[mDim[0]*mDim[1]*mComponents];
                memset(mData,0,mDim[0]*mDim[1]*mComponents);
            }

            ImageBase & operator=(ImageBase && T)
            {
                if(&T == this) return *this;

                if(mData) clear();

                mData       = T.mData;
                mDim        = T.mDim;
                mComponents = T.mComponents;

                T.mDim        = {0,0};
                T.mData       = 0;
                T.mComponents = 0;

                return *this;
            }

            /**
             * Frees all memory associated with this Image including GPU
             * data. The OpenGL ImageBase id will be freed.
             */
            ~ImageBase()
            {
                clear();
            }


            /**
             * @brief get_MAX_Image_SIZE
             * @return the maximum dimension of the Images.
             */
            static GLuint GetMaxSamplerSize()
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
                    throw std::runtime_error("Error loading image from memory");
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
                // Load the ImageBase from an image file.
                //==========================================================
                unsigned char * img = ImageLoader::GLA_load(path.c_str(), &x, &y, &comp, ForceNumberChannels );

                if( img )
                {
                    _handleRawPixels(img, static_cast<unsigned int>( x ), static_cast<unsigned int>( y ) );
                    mComponents = ForceNumberChannels!=0 ? ForceNumberChannels : comp;
                    GLA_DOUT  << "ImageBase loaded with #components = " << mComponents << std::endl;

                } else {
                    //GLA_DOUT  << "Error loading Image: " << path << std::endl;
                    throw std::runtime_error( std::string("Error loading Image: ") + path);
                }

            }


            //===========================================================================

            /**
             * Sends the Image to the GPU and returns a GPUImage object. This does
             * not clear the Image from CPU memory. Once the Image is sent to the
             * gpu, you can clear the memory from the cpu if you do not need it again.
             *
             * @return A GPUImage
             */



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
                mDim = uvec2(0,0);
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
             * @brief size Gets the dimensions of the Image
             * @return
             */
            inline uvec2  size() const { return mDim; }

            /**
             * @brief getRawData gets a pointer to the raw pixel data so that it can be sent to OpenGL
             * @return a pointer to the starting byte;
             */
            inline void* getRawData() const { return (void*)mData; }



            inline void resize( const uvec2 & newSize)
            {
                //GLA_DOUT  << "Resizing from " << mDim.x << ", " << mDim.y << " to " << newSize.x << "," << newSize.y << std::endl;
                ImageBase T( newSize.x, newSize.y , mComponents);
                auto d = T.size();

                for(auto j=0u; j < d.y; j++)
                for(auto i=0u; i < d.x; i++)
                for(auto z=0u; z < mComponents; z++)
                    {
                        // scale the x and y dimeninos to be between 0 and 1
                        float x = (float)i / (float)d.x;
                        float y = (float)j / (float)d.y;


                        // Find the index on the main Image
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

                        T(i,j,z) = static_cast<unsigned char>(f00 * (1-s)*(1-t) + f10*s*(1-t) + f01*(1-s)*t + f11*s*t);

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

            /**
             * Access pixel data on the CPU. This is unpredictable if you
             * have cleared the CPU data.
             */
            inline unsigned char & operator ()(int x, int y, int comp)
            {
                return mData[ y*(mDim[0] * mComponents) + x*mComponents + comp ];
            }
            //=====================================
            // Operators
            //=====================================
            ImageBase& operator=(  std::function<ucol4(float,float) > F)
            {
                uvec2 S = size();
                float W = 1.0f / (float)S.x;
                float H = 1.0f / (float)S.y;
                for(auto y =0u; y < S.y; y++)
                    for(auto x=0u; x < S.x; x++)
                    {
                        auto c = (F( (float)x * W, (float)y*H ));
                        (*this)(x,y,0) = c[0];
                        (*this)(x,y,1) = c[1];
                        (*this)(x,y,2) = c[2];
                        (*this)(x,y,3) = c[3];
                    }

                return *this;
            }

            ImageBase& operator=(  std::function<vec4(float,float) > F)
            {
                uvec2 S = size();
                float W = 1.0f / (float)S.x;
                float H = 1.0f / (float)S.y;
                for(auto y =0u; y < S.y; y++)
                    for(auto x=0u; x < S.x; x++)
                    {
                        auto c = (F( (float)x * W, (float)y*H ) );
                        (*this)(x,y,0) = static_cast<unsigned char>(c[0]*255.f);
                        (*this)(x,y,1) = static_cast<unsigned char>(c[1]*255.f);
                        (*this)(x,y,2) = static_cast<unsigned char>(c[2]*255.f);
                        (*this)(x,y,3) = static_cast<unsigned char>(c[3]*255.f);
                    }

                return *this;
            }


            inline ImageBase operator  + ( const ImageBase & c) const
            {
                uvec2 siz      = glm::min( size(), c.size() );
                unsigned int C = mComponents;
                ImageBase T(    siz.x,siz.y  ,C   );

                for(auto y=0u;y<siz.y;y++)
                for(auto x=0u;x<siz.x;x++)
                for(auto z=0u;z<C;z++)
                {
                    T(x,y,z) = (*this)(x,y,z) + c(x,y,z);
                }

               return T;
            }

            inline ImageBase operator  - ( const ImageBase & c) const
            {
                uvec2 siz      = glm::min( size(), c.size() );
                unsigned int C = mComponents;
                ImageBase T(    siz.x,siz.y  ,C   );

                for(auto y=0u;y<siz.y;y++)
                for(auto x=0u;x<siz.x;x++)
                for(auto z=0u;z<C;z++)
                {
                    T(x,y,z) = (*this)(x,y,z) - c(x,y,z);
                }

               return T;
            }




            inline ImageBase        operator  - ( const float & c) const
            {
               uvec2 siz      = size();
               unsigned int C = mComponents;
               ImageBase T(    siz.x,siz.y ,C    );

                //DOUBLE_LOOP(siz)  {  T(x,y) = LeftOp OPP RightOp ;  }
                for(auto y=0u;y<siz.y;y++)
                for(auto x=0u;x<siz.x;x++)
                for(auto z=0u;z<C;z++)
                {
                    T(x,y,z) = (*this)(x,y,z) - static_cast<unsigned char>(c*255.f);
                }

               return T;
            }

            inline ImageBase        operator  + ( const float & c) const
            {
               uvec2 siz      = size();
               unsigned int C = mComponents;
               ImageBase T(    siz.x,siz.y ,C    );

                //DOUBLE_LOOP(siz)  {  T(x,y) = LeftOp OPP RightOp ;  }
                for(auto y=0u;y<siz.y;y++)
                for(auto x=0u;x<siz.x;x++)
                for(auto z=0u;z<C;z++)
                {
                    T(x,y,z) = (*this)(x,y,z) + static_cast<unsigned char>(c*255.f);
                }

               return T;
            }

            inline ImageBase        operator  * ( const float & c) const
            {
               uvec2 siz      = size();
               unsigned int C = mComponents;
               ImageBase T(    siz.x,siz.y ,C    );

                //DOUBLE_LOOP(siz)  {  T(x,y) = LeftOp OPP RightOp ;  }
                for(auto y=0u;y<siz.y;y++)
                for(auto x=0u;x<siz.x;x++)
                for(auto z=0u;z<C;z++)
                {
                    T(x,y,z) = static_cast<unsigned char>((*this)(x,y,z) * c);
                }

               return T;
            }

            inline ImageBase        operator  / ( const float & c) const
            {
               uvec2 siz      = size();
               unsigned int C = mComponents;
               ImageBase T(    siz.x,siz.y ,C    );

                //DOUBLE_LOOP(siz)  {  T(x,y) = LeftOp OPP RightOp ;  }
                for(auto y=0u;y<siz.y;y++)
                for(auto x=0u;x<siz.x;x++)
                for(auto z=0u;z<C;z++)
                {
                    T(x,y,z) = static_cast<unsigned char>((*this)(x,y,z) / c);
                }

               return T;
            }

            #define OPERATOR(OP)                                                 \
            inline ImageBase        operator  OP ( const unsigned char & c) const   \
            {                                                                    \
               uvec2 siz      = size();                                          \
               unsigned int C = mComponents;                                     \
               ImageBase T(    siz.x,siz.y ,C    );                                \
                                                                                 \
                for(auto y=0u; y<siz.y;y++)                                         \
                for(auto x=0u; x<siz.x;x++)                                         \
                for(auto z=0u; z<C    ;z++)                                         \
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

             #undef OPERATOR

        public:
			void _handleRawPixels(unsigned char * buffer, std::uint32_t width, std::uint32_t height)
            {
                clear();

                mDim = {width, height};

                mData = buffer;;

            }

            unsigned char   *mData;
            unsigned int     mComponents;
            uvec2            mDim;


    };


    class Image;

    //=====================

    class ChannelRef
    {
    public:
        ChannelRef(ImageBase * parent, unsigned int comp) : mParent(parent), mComp(comp)
        {
        }

        inline unsigned char & operator()(int x, int y)
        {
            return (*mParent)(x,y, mComp);
        }

        inline unsigned char & operator()(int x, int y) const
        {
            return (*mParent)(x,y, mComp);
        }

        ChannelRef& operator=(ChannelRef & other)
        {
            int c   = other.mComp;
            uvec2 S = glm::min( mParent->size(), other.mParent->size() );

            for(auto y=0u; y < S.y; y++)
            for(auto x=0u; x < S.x; x++)
            {
                (*mParent)(x,y,(int)mComp) = (*other.mParent)(x, y, c);
            }

            return *this;
        }

        void PasteImage(const ChannelRef & other, unsigned int x_offset, unsigned int y_offset)
        {
            auto S = glm::min( GetSize() , uvec2(x_offset,y_offset)+other.GetSize() );

            int c  = other.mComp;

            for(auto y = y_offset; y < S.y; y++)
                for(auto x = x_offset; x < S.x; x++)
                {
                    (*mParent)(x,y,(int)mComp) = (*other.mParent)( x-x_offset, y-y_offset, c);
                }
        }


        ChannelRef& operator=(const Image & other);


        Image operator+(ChannelRef & other);
        Image operator-(ChannelRef & other);
        Image operator*(ChannelRef & other);
        Image operator/(ChannelRef & other);


        ChannelRef& operator=(  std::function<float(float,float) > F)
        {
            uvec2 S = mParent->size();
            float W = 1.0f / (float)S.x;
            float H = 1.0f / (float)S.y;
            for(auto y =0u; y < S.y; y++)
                for(auto x=0u; x < S.x; x++)
                {
                    (*mParent)(x,y,(int)mComp) = (unsigned char)(F( (float)x * W, (float)y*H ) * 255.f);
                }

            return *this;
        }

        void Print() const
        {
            char data[] = " .:ioVM@";
            auto s = GetSize();
            for (auto j=0u; j < s.y; ++j) std::cout << '#';
             std::cout << std::endl;

            for (auto j=0u; j < s.y; ++j)
            {
               printf("#");
               for (auto i=0u; i < s.x; ++i)
               {
                  printf("%c",  data[ (*this)(i,j)>>5] );
               }
               printf("#\n");
            }

            for (auto j=0u; j < s.y; ++j) std::cout << '#';
             std::cout << std::endl;
        }

        const uvec2 GetSize() const
        {
            return mParent->size();
        }

        ImageBase      *mParent;
        unsigned int    mComp;
    };


    #define IMAGE_EXPRESSION( A ) [] (float x, float y) { return A; };
    #define IMG_EXPR( A ) [] (float x, float y) { return A; };

    class Image : public ImageBase
    {
        public:
            ChannelRef r;
            ChannelRef g;
            ChannelRef b;
            ChannelRef a;


            Image() : r(this,0),g(this,1),b(this,2),a(this,3)
            {

            }

            Image( const uvec2 & size, unsigned int components=4) : ImageBase(size,components),
                                                                    r(this, std::min(0u,components-1) ),
                                                                    g(this, std::min(1u,components-1) ),
                                                                    b(this, std::min(2u,components-1) ),
                                                                    a(this, std::min(3u,components-1) )
            {
            }

			Image(std::uint32_t w, std::uint32_t h, unsigned int components = 4) : ImageBase(w, h, components), r(this, 0), g(this, 1), b(this, 2), a(this, 3)
            {
            }

            Image(const std::string & path, unsigned int ForceNumberChannels=0) : ImageBase(), r(this,0),g(this,1),b(this,2),a(this,3)
            {
                loadFromPath(path, ForceNumberChannels);
            }

            explicit Image(Image & T) : ImageBase(T), r(this,0),g(this,1),b(this,2),a(this,3)
            {
            }

            Image(Image && T) : ImageBase(T), r(this,0),g(this,1),b(this,2),a(this,3)
            {
                *this = std::move(T);
            }

            Image & operator=(Image && T)
            {
                //*this = std::move(T);

                if(mData) clear();

                mData       = T.mData;
                mDim        = T.mDim;
                mComponents = T.mComponents;

                T.mDim        = {0,0};
                T.mData       = 0;
                T.mComponents = 0;

                //GLA_DOUT  << " Image::Move operator\n";
                //return *this;

                return *this;

            }

            void PasteImage(const ChannelRef & other, unsigned int x_offset, unsigned int y_offset)
            {
                auto S = glm::min( size() , uvec2(x_offset,y_offset)+other.GetSize() );

                for(auto y = y_offset; y < S.y; y++)
                    for(auto x = x_offset; x < S.x; x++)
                    {
                        for(auto i=0u;i<mComponents;i++)
                            (*this)(x,y,i) = other( x-x_offset, y-y_offset);
                    }
            }

            void PasteImage(const Image & other, unsigned int x_offset, unsigned int y_offset)
            {
                auto S = glm::min( size() , uvec2(x_offset,y_offset)+other.size() );

                auto comp = std::min( mComponents, other.mComponents);

                for(auto y = y_offset; y < S.y; y++)
                    for(auto x = x_offset; x < S.x; x++)
                    {
                        for(auto i=0u; i < comp;i++)
                            (*this)(x,y,i) = other( x-x_offset , y-y_offset, i);
                    }
            }


    };


    inline ChannelRef& ChannelRef::operator=(const Image & other)
    {
        uvec2 S = glm::min( mParent->size(), other.size() );

        for(auto y = 0u ; y < S.y; y++)
            for(auto x = 0u; x < S.x; x++)
            {
                (*mParent)(x,y,(int)mComp) = other(x, y, 0);
            }
        return *this;
    }


    inline Image ChannelRef::operator+(ChannelRef & other)
    {
        int c   = other.mComp;
        uvec2 S = glm::min( mParent->size(), other.mParent->size() );
        Image T( S, 1);



        for(auto y =0u; y < S.y; y++)
            for(auto x=0u; x < S.x; x++)
            {
                T(x,y,0) = (*mParent)(x, y, mComp) + (*other.mParent)(x, y, c);
            }

        return std::move(T);
    }

    inline Image ChannelRef::operator-(ChannelRef & other)
    {
        int c   = other.mComp;
        uvec2 S = glm::min( mParent->size(), other.mParent->size() );
        Image T( S, 1);



        for(auto y =0u; y < S.y; y++)
            for(auto x=0u; x < S.x; x++)
            {
                T(x,y,0) = (*mParent)(x, y, mComp) - (*other.mParent)(x, y, c);
            }

        return std::move(T);
    }

    inline Image ChannelRef::operator*(ChannelRef & other)
    {
        int c   = other.mComp;
        uvec2 S = glm::min( mParent->size(), other.mParent->size() );
        Image T( S, 1);


        for(auto y =0u; y < S.y; y++)
            for(auto x=0u; x < S.x; x++)
            {
                float t = (float)(*mParent)(x, y, mComp) * (float)(*other.mParent)(x, y, c);
                T(x,y,0) = (unsigned char)(t / (255.f));
            }

        return std::move(T);
    }

    inline Image ChannelRef::operator/(ChannelRef & other)
    {
        int c   = other.mComp;
        uvec2 S = glm::min( mParent->size(), other.mParent->size() );
        Image T( S, 1);


        for(auto y =0u; y < S.y; y++)
            for(auto x=0u; x < S.x; x++)
            {
                float t = (float)(*mParent)(x, y, mComp) / (float)(*other.mParent)(x, y, c);
                T(x,y,0) = (unsigned char)(t * (255.f));
            }

        return std::move(T);
    }



    inline Image operator / (const ChannelRef & left, const Image & right)
    {

        uvec2 S = glm::min( left.mParent->size(), right.size() );
        Image T( S, 1);


        for(auto y =0u; y < S.y; y++)
            for(auto x=0u; x < S.x; x++)
            {
                T(x,y,0) = (unsigned char) (  (float)(*left.mParent)(x, y, left.mComp) / (float) right(x, y, 0) * (255.f)  );
            }

        return std::move(T);
    }

    inline Image operator * (const  ChannelRef & left,  const Image & right)
    {

        uvec2 S = glm::min( left.mParent->size(), right.size() );
        Image T( S, 1);


        for(auto y =0u; y < S.y; y++)
            for(auto x=0u; x < S.x; x++)
            {
                T(x,y,0) = (unsigned char) (  (float)(*left.mParent)(x, y, left.mComp) * (float) right(x, y, 0) * (1.0f/255.f)  );
            }

        return T;
    }

    inline Image operator * ( const Image & left,  const ChannelRef & right)
    {
       return right * left;
    }

    inline Image operator + (const  ChannelRef & left,  const Image & right)
    {

        uvec2 S = glm::min( left.mParent->size(), right.size() );
        Image T( S, 1);


        for(auto y =0u; y < S.y; y++)
            for(auto x=0u; x < S.x; x++)
            {
                T(x,y,0) = (*left.mParent)(x, y, left.mComp) + right(x, y, 0);
            }

        return T;
    }

    inline Image operator - ( const ChannelRef & left,  const Image & right)
    {
        uvec2 S = glm::min( left.mParent->size(), right.size() );
        Image T( S, 1);


        for(auto y =0u; y < S.y; y++)
            for(auto x=0u; x < S.x; x++)
            {
                T(x,y,0) = (*left.mParent)(x, y, left.mComp) - right(x, y, 0);
            }

        return T;
    }


    inline Image operator - ( const Image & left,  const ChannelRef & right)
    {
        uvec2 S = glm::min( left.size(), right.mParent->size() );
        Image T( S, 1);


        for(auto y = 0u; y < S.y; y++)
            for(auto x=0u; x < S.x; x++)
            {
                T(x,y,0) = - left(x, y, 0) - (*right.mParent)(x, y, right.mComp);
            }

        return T;
    }





}
}


#endif // Image_H

