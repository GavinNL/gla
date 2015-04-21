#ifndef GLA_TEXTURE_H
#define GLA_TEXTURE_H

#include <gla/core/types.h>
#include <gla/core/exceptions.h>
#include <iostream>
#include <string.h>
#include <functional>
#include <gla/stb/stb_image.h>


/**
 * \page page_Testing
 *  this is a test poage
 *
 */
namespace gla {

    //===========================================================================
    // GPUTexture
    //   - A 2D texture stored on the GPU
    //===========================================================================
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


            inline bool create(const uvec2 & size, TEXTURECOLOURFORMAT InternalFormat=RGBA, int MipMaps=-1)
            {
                return( create(size, InternalFormat, InternalFormat, UNSIGNED_BYTE, 0, MipMaps) );
            }

            /**
             * @brief create Creates a blank texture
             * @param size The size of the texture to create on the GPU.
             */
            inline bool create(const uvec2                         &size,
                               TEXTURECOLOURFORMAT InternalFormat = RGBA,
                               TEXTURECOLOURFORMAT Format         = RGBA,
                               FUNDAMENTAL_TYPE    Type           = UNSIGNED_BYTE,
                               void*               Data           = 0,
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

                return true;
            }

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


            /**
             *  Sets this texture to be the active texture for
             *  the.
             *
             *  @param unit The texture unit to set it as, usually between 0-32, but this is HW dependant. Default is 0.
             *
             */
            inline void setActiveTexture(unsigned int unit=0) const
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
            //Texture toCPU();

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
                GLint  max;
                glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max);
                return max;
            }

       private:
            uvec2         mDim;
            GLuint        mTextureID;
            unsigned char mColourFormat;
            unsigned char mFundamentalType;

       friend class FrameBufferObject;
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
    template< class PixelType >
    class Texture
    {

        public:

            Texture() : mData(0)
            {

            }

            Texture( const uvec2 & size) : mData(0)
            {
                mDim.x = size.x;
                mDim.y = size.y;
                mData  = new PixelType[ mDim[0] * mDim[1] ];
            }

            Texture(uint w, uint h) : mData(0)
            {
                mDim.x = w;
                mDim.y = h;
                mData  = new PixelType[ mDim[0] * mDim[1] ];
            }

            Texture(const std::string & path) :  mData(0)
            {
                loadFromPath(path);
            }

            explicit Texture(Texture<PixelType> & T) :  mData(0)
            {

                mDim  = T.mDim;

                mData = new PixelType[mDim[0]*mDim[1]];
                memcpy( mData, (void*)T.mData, mDim[0]*mDim[1] * 4);

            }

            Texture(Texture<PixelType> && T) : mData(0)
            {
                if(mData) clear();

                mData   = T.mData;
                mDim    = T.mDim;
                T.mDim  = { 0, 0 };
                T.mData = 0;

                std::cout << " Texture::Move constructor\n";

            }

            Texture<PixelType> & operator=(Texture<PixelType> && T)
            {
                if(mData) clear();

                mData   = T.mData;
                mDim    = T.mDim;
                T.mDim  = {0,0};
                T.mData = 0;

                std::cout << " Texture::Move operator\n";
                return *this;
            }

            /**
             * Frees all memory associated with this texture including GPU
             * data. The OpenGL Texture id will be freed.
             */
            ~Texture<PixelType>()
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
            void loadFromMemory( unsigned char * Buffer, int buffersize)
            {
                int x, y, comp;
                GLA_uc           * img = GLA_load_from_memory( (GLA_uc*) Buffer, buffersize, &x, &y, &comp, sizeof(PixelType) );

                if( img )
                {
                    _handleRawPixels(img, static_cast<unsigned int>( x ), static_cast<unsigned int>( y ) );

                  //  free(img);
                } else {
                    std::cout << "from Memory error: " << x << "," << y << "\n";
                }

            }

            /**
             *  Load an image from a file.
             *
             * @param Path a path to the file
             */
            void loadFromPath( const std::string & path)
            {

                int x, y, comp;

                //==========================================================
                // Load the Texture from an image file.
                //==========================================================
                unsigned char * img = GLA_load(path.c_str(), &x, &y, &comp, sizeof(PixelType) );

                if( img )
                {
                    _handleRawPixels(img, static_cast<unsigned int>( x ), static_cast<unsigned int>( y ) );

                    //free(img);
                } else {
                    std::cout << "Error loading texture: " << path << std::endl;
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
            GPUTexture toGPU(int MipMaps=-1)
            {
                GPUTexture GPU;

                TEXTURECOLOURFORMAT format;

                switch( sizeof(PixelType) )
                {
                    case 1: format = RED;  break;
                    case 2: format = RG;   break;
                    case 3: format = RGB;  break;
                    case 4: format = RGBA; break;
                }

                if( GPU.create( mDim, format, format, UNSIGNED_BYTE, (void*)mData), MipMaps )
                {
                    GPU.setFilter(     LINEAR, LINEAR, false );
                    GPU.setTextureWrap(REPEAT, REPEAT, false);

                    return GPU;
                }

                return GPU;
            }


            /**
             * Clears the image data from the CPU.
             */
            void clear()
            {
                if( mData ) delete [] mData;
                mData = 0;
            }

            void resize(const uvec2 & newSize);




            /**
             * @brief get Returns the value of the pixel (x,y)
             * @param x
             * @param y
             * @return a copy of the pixel at location (x,y)
             */
            inline PixelType  get(int x, int y) const { return mData[ y*mDim[0] + x ]; }

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

            /**
             * Access pixel data on the CPU. This is unpredictable if you
             * have cleared the CPU data.
             */
            inline PixelType & operator ()(int x, int y) const { return mData[ y*mDim[0] + x ]; }

            //=====================================
            // Operators
            //=====================================

#define DOUBLE_LOOP(size)  for(int y=0; y<siz.y; y++) for(int x=0;x<siz.x;x++)


#define OPERATOR( OP , RIGHT_PAR, LeftOp, OPP,  RightOp )  inline Texture<PixelType>        operator  OP ( const  RIGHT_PAR & c) const \
                                {                                                                                    \
                                   uvec2 siz = glm::min( size(), c.size() );                                         \
                                   Texture T(    siz.x,siz.y     );                                                  \
                                                                                                                     \
                                    DOUBLE_LOOP(siz)  {  T(x,y) = LeftOp OPP RightOp ;  }                             \
                                                                                                                     \
                                   return T;                                                                         \
                                };


              OPERATOR( +,  Texture<PixelType> , (*this)(x,y) , + , c(x,y) )
              OPERATOR( -,  Texture<PixelType> , (*this)(x,y) , - , c(x,y) )



#undef OPERATOR
#define OPERATOR( OP , RIGHT_PAR, LeftOp, OPP,  RightOp )  inline Texture<PixelType>        operator  OP ( const  RIGHT_PAR & c) const \
                                {                                                                                                      \
                                   uvec2 siz = size();                                                                                 \
                                   Texture T(    siz.x,siz.y     );                                                                    \
                                                                                                                                       \
                                    DOUBLE_LOOP(siz)  {  T(x,y) = LeftOp OPP RightOp ;  }                                              \
                                                                                                                                       \
                                   return T;                                                                                           \
                                };


              OPERATOR( -,  float         ,  (*this)(x,y) ,   - ,    static_cast<unsigned char>(c * 255.f) )
              OPERATOR( +,  float         ,  (*this)(x,y) ,   + ,    static_cast<unsigned char>(c * 255.f) )
              OPERATOR( *,  float         ,  (*this)(x,y) ,   * ,    c  )
              OPERATOR( /,  float         ,  (*this)(x,y) ,   / ,    c  )

              OPERATOR( -,  unsigned char ,  (*this)(x,y) ,   - ,    c  )
              OPERATOR( +,  unsigned char ,  (*this)(x,y) ,   + ,    c  )
              OPERATOR( *,  unsigned char ,  (*this)(x,y) ,   * ,    c  )
              OPERATOR( /,  unsigned char ,  (*this)(x,y) ,   / ,    c  )


        protected:
            void _handleRawPixels(unsigned char * buffer, uint width, uint height)
            {
                clear();

                mDim = {width, height};

                mData = (PixelType*)buffer;;

            }

            PixelType   *mData;

            uvec2        mDim;


    };




template<class PixelType, int index>
class ChannelReference
{
    public:
        explicit ChannelReference() : mData(0), mSize(0){};
        explicit ChannelReference(PixelType * data, uvec2 size) : mData(data), mSize(size) {}

        void setup(PixelType * data, uvec2 size) {

            mData = data; mSize = size;
        }


        unsigned char & operator()(int x, int y) const
        {
            return mData[y*mSize.x + x][index];
        }


    public:



        #undef OPERATOR
        #define ASSIGNMENT_OPERATOR( RightOp,  EXPRESSION )                                   \
        ChannelReference<PixelType,index> & operator = ( const RightOp & c)                   \
        {                                                                                     \
            for(int y=0;y<mSize.y;y++) for(int x=0; x<mSize.x;x++)                            \
            {                                                                                 \
                  EXPRESSION ;                                                                \
            }                                                                                 \
            return *this;                                                                     \
        };

        ASSIGNMENT_OPERATOR( float        ,  (*this)(x,y) = static_cast<unsigned char>(c*255.f) )
        ASSIGNMENT_OPERATOR( unsigned char,  (*this)(x,y) = c )
        ASSIGNMENT_OPERATOR( int          ,  (*this)(x,y) = static_cast<unsigned char>(c) )

        ChannelReference<PixelType,index> & operator=( const Texture<glm::u8vec1> & other)
        {
            std::cout << "ChannelRef = Texture1\n";
            std::cout <<    mSize.x << ", " << mSize.y << std::endl;
                std::cout <<    other.size().x << ", " << other.size().y << std::endl;
            uvec2 size = glm::min( mSize, other.size() );

            for(int y=0;y<size.y;y++) for(int x=0;x<size.x;x++)
            {
                (*this)(x,y) = other(x,y)[0];
            }
            return *this;
        }


        template<class P, int in>
        ChannelReference<PixelType,index> & operator=( const ChannelReference<P,in> & other)
        {


            uvec2 size = glm::min( mSize, other.mSize );

            for(int y=0;y<size.y;y++) for(int x=0;x<size.x;x++)
            {
                (*this)(x,y) = other(x,y);
            }
            return *this;
        }


        //============================================================================
        // Opeartor           Texture1d
        //============================================================================

        #define OPERATOR_TEXTURE( OP , LeftOp, OPP,  RightOp )                             \
        Texture<glm::u8vec1>  operator OP ( const Texture<glm::u8vec1> & c)                \
        {                                                                                  \
            uvec2 size = glm::min( mSize, c.size());                                       \
            Texture<glm::u8vec1> T(size);                                                  \
            for(int y=0;y<size.y;y++) for(int x=0; x<size.x;x++)                           \
            {                                                                              \
                       T(x,y)= LeftOp OPP RightOp  ;                                       \
            }                                                                              \
            return T;                                                                      \
        }

        OPERATOR_TEXTURE( +,   (*this)(x,y),  +,  c(x,y).r )
        OPERATOR_TEXTURE( -,   (*this)(x,y),  -,  c(x,y).r )
        OPERATOR_TEXTURE( *,   (*this)(x,y),  *,  static_cast<float>(c(x,y).r)/255.0f )
        OPERATOR_TEXTURE( /,   (*this)(x,y),  *,  static_cast<float>(c(x,y).r)/255.0f )

        #undef OPERATOR_TEXTURE


        //============================================================================
        // Opeartor           ChannelReference
        //============================================================================

        #define OPERATOR_CHANNELREFERENCE( OP , LeftOp, OPP,  RightOp )                       \
        template<class P, int in>                                                             \
        Texture<glm::u8vec1>  operator OP ( const ChannelReference<P,in> & c)                 \
        {                                                                                     \
            uvec2 size = glm::min( mSize, c.mSize);                                           \
            Texture<glm::u8vec1> T(size);                                                     \
            for(int y=0;y<size.y;y++) for(int x=0; x<size.x;x++)                              \
            {                                                                                 \
                       T(x,y)= LeftOp OPP RightOp;                                            \
            }                                                                                 \
            return T;                                                                         \
        };

        OPERATOR_CHANNELREFERENCE( +,      (*this)(x,y),  +,  c(x,y) )
        OPERATOR_CHANNELREFERENCE( -,      (*this)(x,y),  -,  c(x,y) )
        OPERATOR_CHANNELREFERENCE( *,      (*this)(x,y),  *,  static_cast<float>( c(x,y) )/255.0f )

        #undef OPERATOR_CHANNELREFERENCE


        //============================================================================
        // Opeartor           Integral
        //============================================================================
        #define OPERATOR_CHANNELREF_INTEGRAL( OP , INTEGRAL, LeftOp, OPP,  RightOp )       \
        Texture<glm::u8vec1>  operator OP ( const INTEGRAL & c)                            \
        {                                                                                  \
            uvec2 size = glm::min( mSize, mSize   );                                       \
            Texture<glm::u8vec1> T(size);                                                  \
            for(int y=0;y<size.y;y++) for(int x=0; x<size.x;x++)                           \
            {                                                                              \
                       T(x,y)= LeftOp OPP RightOp  ;                                       \
            }                                                                              \
            return T;                                                                      \
        }

        OPERATOR_CHANNELREF_INTEGRAL( +,   float, (*this)(x,y),  +,  static_cast<unsigned char>(c*255.f) )
        OPERATOR_CHANNELREF_INTEGRAL( -,   float, (*this)(x,y),  -,  static_cast<unsigned char>(c*255.f) )
        OPERATOR_CHANNELREF_INTEGRAL( *,   float, (*this)(x,y),  +,  c )


        OPERATOR_CHANNELREF_INTEGRAL( +,   int, (*this)(x,y),  +,  static_cast<unsigned char>(c) )
        OPERATOR_CHANNELREF_INTEGRAL( -,   int, (*this)(x,y),  -,  static_cast<unsigned char>(c) )
        OPERATOR_CHANNELREF_INTEGRAL( +,   unsigned char, (*this)(x,y),  +,  c )
        OPERATOR_CHANNELREF_INTEGRAL( -,   unsigned char, (*this)(x,y),  -,  c )


        ChannelReference<PixelType,index> & operator=( std::function<float(vec2)> f)
        {
                std::cout << "Function: Size: " << mSize.x << "," << mSize.y << std::endl;
                for(int y=0;y<mSize.y;y++)
                for(int x=0;x<mSize.x;x++)
                {
                    (*this)(x,y) = static_cast<unsigned char>( 255.0f * f( vec2( (float)x / (float)mSize.x , (float) y / (float)mSize.y ) ) );
                }
        };


        PixelType * mData;
        uvec2       mSize;

};


template<class T>
class ChannelMembers
{
    ChannelMembers(){};
    void setup(T * data, uvec2 size);
};

template<>
class ChannelMembers<glm::u8vec3>
{
    public:
        ChannelMembers<glm::u8vec3>(glm::u8vec3 * data, uvec2 size) : r(data, size), g(data, size), b(data, size)
        {};


        void setup(glm::u8vec3 * data, uvec2 size)
        {
            r.setup(data, size);
            g.setup(data, size);
            b.setup(data, size);
        };

        ChannelReference<glm::u8vec3,0> r;
        ChannelReference<glm::u8vec3,1> g;
        ChannelReference<glm::u8vec3,2> b;
};

template<>
class ChannelMembers<glm::u8vec4>
{
public:
        ChannelMembers<glm::u8vec4>(glm::u8vec4 * data, uvec2 size) : r(data, size),g(data, size), b(data, size), a(data, size)
        {};

       // template<class P>
        void setup(glm::u8vec4 * data, uvec2 size)
        {
            r.setup(data, size);
            g.setup(data, size);
            b.setup(data, size);
            a.setup(data, size);
            std::cout << "setup all channel references\n";
        };

        ChannelReference<glm::u8vec4,0> r;
        ChannelReference<glm::u8vec4,1> g;
        ChannelReference<glm::u8vec4,2> b;
        ChannelReference<glm::u8vec4,3> a;
};


template<class PixelType>
class Texture_T : public Texture<PixelType>, public ChannelMembers<PixelType>
{
    public:

        Texture_T(unsigned int w, unsigned int h) : Texture<PixelType>(w,h) , ChannelMembers<PixelType>(this->mData,uvec2(w,h)) {};

        Texture_T(const std::string & path) : Texture<PixelType>(path) , ChannelMembers<PixelType>(this->mData,uvec2(0,0))
        {
            this->ChannelMembers<PixelType>::setup( this->mData, this->size() );
        };
};

//====================================================================================================


// #define SCALAR_MULTIPLICATION( TYP )
// template<class PixelType>                                                                 \
// inline Texture<PixelType> operator * (const TYP & left, const Texture<PixelType> & right) \
// {                                                                                         \
//     return( std::move( right * left ) );                                                  \
// }

template<class PixelType>
inline Texture<PixelType> operator * (const float & left, const Texture<PixelType> & right)
{
    return(  right * left  );
}

template<class PixelType>
inline Texture<PixelType> operator - (const float & left, const Texture<PixelType> & right)
{
    Texture<PixelType> T( right.size() );
    T = left;
    return(  T * right );
}




//SCALAR_MULTIPLICATION(float)

//SCALAR_MULTIPLICATION(double)

//SCALAR_MULTIPLICATION(int)

//SCALAR_MULTIPLICATION(unsigned int)


//template<class PixelType, class Typ>
//inline Texture<PixelType> operator + (const Typ & left, const Texture<PixelType> & right)
//{
//    return( std::move( right + left ) );
//}
//
//template<class PixelType, class Typ>
//inline Texture<PixelType> operator - (const Typ & left, const Texture<PixelType> & right)
//{
//    return( std::move( right - left ) );
//}



// template<class PixelType, int in, class Typ>
// inline Texture<glm::u8vec1> operator * (const Typ & left, const ChannelReference<PixelType,in> & right)
// {
//     return( std::move( right * left ) );
// }
//
// template<class PixelType, int in, class Typ>
// inline Texture<glm::u8vec1> operator - (const Typ & left, const ChannelReference<PixelType,in> & right)
// {
//     return( std::move( left - right ) );
// }
//
// template<class PixelType, int in, class Typ>
// inline Texture<glm::u8vec1> operator + (const Typ & left, const ChannelReference<PixelType,in> & right)
// {
//     return( std::move( right + left ) );
// }




template<>
inline void Texture<glm::u8vec1>::resize( const uvec2 & newSize)
{
    Texture<glm::u8vec1> T( newSize.x, newSize.y);
    auto d = T.size();

    for(int i=0; i < d.x; i++)
        for(int j=0; j < d.y; j++)
        {
            // scale the x and y dimeninos to be between 0 and 1
            float x = (float)i / (float)d.x;
            float y = (float)j / (float)d.y;

            // Find the index on the main texture
            int X = static_cast<int>(x * mDim.x);
            int Y = static_cast<int>(y * mDim.y);


            // sample the 4 locations
            float f00 = float( (*this)(X,Y)    [0]);
            float f01 = float( (*this)(X,Y+1)  [0]);
            float f10 = float( (*this)(X+1,Y)  [0]);
            float f11 = float( (*this)(X+1,Y+1)[0]);

            float s = x * static_cast<float>(mDim.x);
            float t = y * static_cast<float>(mDim.y);

            // get the fractional part
            s = s-floor(s);
            t = t-floor(t);

            T(i,j) = glm::u8vec1( f00 * (1-s)*(1-t) + f10*s*(1-t) + f01*(1-s)*t + f11*s*t);

        }

    *this = std::move(T);
}


template<>
inline void Texture<glm::u8vec2>::resize( const uvec2 & newSize)
{
    Texture<glm::u8vec2> T( newSize.x, newSize.y);
    auto d = T.size();

    for(int i=0; i < d.x; i++)
        for(int j=0; j < d.y; j++)
        {
            // scale the x and y dimeninos to be between 0 and 1
            float x = (float)i / (float)d.x;
            float y = (float)j / (float)d.y;

            // Find the index on the main texture
            int X = static_cast<int>(x * mDim.x);
            int Y = static_cast<int>(y * mDim.y);


            // sample the 4 locations
            vec2 f00 = vec2( (*this)(X,Y)    );
            vec2 f01 = vec2( (*this)(X,Y+1)  );
            vec2 f10 = vec2( (*this)(X+1,Y)  );
            vec2 f11 = vec2( (*this)(X+1,Y+1));

            float s = x * static_cast<float>(mDim.x);
            float t = y * static_cast<float>(mDim.y);

            // get the fractional part
            s = s-floor(s);
            t = t-floor(t);

            T(i,j) = glm::u8vec2( f00 * (1-s)*(1-t) + f10*s*(1-t) + f01*(1-s)*t + f11*s*t);

        }

    *this = std::move(T);
}

template<>
inline void Texture<glm::u8vec3>::resize( const uvec2 & newSize)
{
    Texture<glm::u8vec3> T( newSize.x, newSize.y);
    auto d = T.size();

    for(int i=0; i < d.x; i++)
        for(int j=0; j < d.y; j++)
        {
            // scale the x and y dimeninos to be between 0 and 1
            float x = (float)i / (float)d.x;
            float y = (float)j / (float)d.y;

            // Find the index on the main texture
            int X = static_cast<int>(x * mDim.x);
            int Y = static_cast<int>(y * mDim.y);


            // sample the 4 locations
            vec3 f00 = vec3( (*this)(X,Y)    );
            vec3 f01 = vec3( (*this)(X,Y+1)  );
            vec3 f10 = vec3( (*this)(X+1,Y)  );
            vec3 f11 = vec3( (*this)(X+1,Y+1));

            float s = x * static_cast<float>(mDim.x);
            float t = y * static_cast<float>(mDim.y);

            // get the fractional part
            s = s-floor(s);
            t = t-floor(t);

            T(i,j) = glm::u8vec3( f00 * (1-s)*(1-t) + f10*s*(1-t) + f01*(1-s)*t + f11*s*t);

        }

    *this = std::move(T);
}

template<>
inline void Texture<glm::u8vec4>::resize( const uvec2 & newSize)
{
    Texture<glm::u8vec4> T( newSize.x, newSize.y);
    auto d = T.size();

    for(int i=0; i < d.x; i++)
        for(int j=0; j < d.y; j++)
        {
            // scale the x and y dimeninos to be between 0 and 1
            float x = (float)i / (float)d.x;
            float y = (float)j / (float)d.y;

            // Find the index on the main texture
            int X = static_cast<int>(x * mDim.x);
            int Y = static_cast<int>(y * mDim.y);


            // sample the 4 locations
            vec4 f00 = vec4( (*this)(X,Y)    );
            vec4 f01 = vec4( (*this)(X,Y+1)  );
            vec4 f10 = vec4( (*this)(X+1,Y)  );
            vec4 f11 = vec4( (*this)(X+1,Y+1));

            float s = x * static_cast<float>(mDim.x);
            float t = y * static_cast<float>(mDim.y);

            // get the fractional part
            s = s-floor(s);
            t = t-floor(t);

            T(i,j) = glm::u8vec4( f00 * (1-s)*(1-t) + f10*s*(1-t) + f01*(1-s)*t + f11*s*t);

        }

    *this = std::move(T);
}


}
#endif // TEXTURE_H

