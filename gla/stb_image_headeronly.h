/*
 ===================================================================================
  NOTE: this file was modified by performing a find and replace of the following
      STB->GLA
      stbi->gla

      This was done to prevent redefinition errors that may occur if two libraries
      both use stb_image. I may change this later.

 ===================================================================================

   stb_image - v1.46 - public domain JPEG/PNG reader - http://nothings.org/stb_image.c
   when you control the images you're loading
                                     no warranty implied; use at your own risk

   Do this:
      #define GLA_IMAGE_IMPLEMENTATION
   before you include this file in *one* C or C++ file to create the implementation.

   #define GLAI_ASSERT(x) to avoid using assert.h.

   QUICK NOTES:
      Primarily of interest to game developers and other people who can
          avoid problematic images and only need the trivial interface

      JPEG baseline (no JPEG progressive)
      PNG 8-bit-per-channel only

      TGA (not sure what subset, if a subset)
      BMP non-1bpp, non-RLE
      PSD (composited view only, no extra channels)

      GIF (*comp always reports as 4-channel)
      HDR (radiance rgbE format)
      PIC (Softimage PIC)

      - decode from memory or through FILE (define GLAI_NO_STDIO to remove code)
      - decode from arbitrary I/O callbacks
      - overridable dequantizing-IDCT, YCbCr-to-RGB conversion (define GLAI_SIMD)

   Latest revisions:
      1.46 (2014-08-26) fix broken tRNS chunk in non-paletted PNG
      1.45 (2014-08-16) workaround MSVC-ARM internal compiler error by wrapping malloc
      1.44 (2014-08-07) warnings
      1.43 (2014-07-15) fix MSVC-only bug in 1.42
      1.42 (2014-07-09) no _CRT_SECURE_NO_WARNINGS; error-path fixes; GLAI_ASSERT
      1.41 (2014-06-25) fix search&replace that messed up comments/error messages
      1.40 (2014-06-22) gcc warning
      1.39 (2014-06-15) TGA optimization bugfix, multiple BMP fixes
      1.38 (2014-06-06) suppress MSVC run-time warnings, fix accidental rename of 'skip'
      1.37 (2014-06-04) remove duplicate typedef
      1.36 (2014-06-03) converted to header file, allow reading incorrect iphoned-images without iphone flag
      1.35 (2014-05-27) warnings, bugfixes, TGA optimization, etc

   See end of file for full revision history.

   TODO:
      GLA_info support for BMP,PSD,HDR,PIC


 ============================    Contributors    =========================
              
 Image formats                                Bug fixes & warning fixes
    Sean Barrett (jpeg, png, bmp)                Marc LeBlanc
    Nicolas Schulz (hdr, psd)                    Christpher Lloyd
    Jonathan Dummer (tga)                        Dave Moore
    Jean-Marc Lienher (gif)                      Won Chun
    Tom Seddon (pic)                             the Horde3D community
    Thatcher Ulrich (psd)                        Janez Zemva
                                                 Jonathan Blow
                                                 Laurent Gomila
 Extensions, features                            Aruelien Pocheville
    Jetro Lauha (GLA_info)                      Ryamond Barbiero
    James "moose2000" Brown (iPhone PNG)         David Woo
    Ben "Disch" Wenger (io callbacks)            Roy Eltham
    Martin "SpartanJ" Golini                     Luke Graham
                                                 Thomas Ruf
                                                 John Bartholomew
 Optimizations & bugfixes                        Ken Hamada
    Fabian "ryg" Giesen                          Cort Stratton
    Arseny Kapoulkine                            Blazej Dariusz Roszkowski
                                                 Thibault Reuille
                                                 Paul Du Bois
                                                 Guillaume George
                                                 Jerry Jansson
  If your name should be here but                Hayaki Saito
  isn't, let Sean know.                          Johan Duparc
                                                 Ronny Chevalier
                                                 Michal Cichon
*/

#ifndef GLAI_GLA_INCLUDE_GLA_IMAGE_H
#define GLAI_GLA_INCLUDE_GLA_IMAGE_H

// Limitations:
//    - no jpeg progressive support
//    - non-HDR formats support 8-bit samples only (jpeg, png)
//    - no delayed line count (jpeg) -- IJG doesn't support either
//    - no 1-bit BMP
//    - GIF always returns *comp=4
//
// Basic usage (see HDR discussion below):
//    int x,y,n;
//    unsigned char *data = GLA_load(filename, &x, &y, &n, 0);
//    // ... process data if not NULL ... 
//    // ... x = width, y = height, n = # 8-bit components per pixel ...
//    // ... replace '0' with '1'..'4' to force that many components per pixel
//    // ... but 'n' will always be the number that it would have been if you said 0
//    GLA_image_free(data)
//
// Standard parameters:
//    int *x       -- outputs image width in pixels
//    int *y       -- outputs image height in pixels
//    int *comp    -- outputs # of image components in image file
//    int req_comp -- if non-zero, # of image components requested in result
//
// The return value from an image loader is an 'unsigned char *' which points
// to the pixel data. The pixel data consists of *y scanlines of *x pixels,
// with each pixel consisting of N interleaved 8-bit components; the first
// pixel pointed to is top-left-most in the image. There is no padding between
// image scanlines or between pixels, regardless of format. The number of
// components N is 'req_comp' if req_comp is non-zero, or *comp otherwise.
// If req_comp is non-zero, *comp has the number of components that _would_
// have been output otherwise. E.g. if you set req_comp to 4, you will always
// get RGBA output, but you can check *comp to easily see if it's opaque.
//
// An output image with N components has the following components interleaved
// in this order in each pixel:
//
//     N=#comp     components
//       1           grey
//       2           grey, alpha
//       3           red, green, blue
//       4           red, green, blue, alpha
//
// If image loading fails for any reason, the return value will be NULL,
// and *x, *y, *comp will be unchanged. The function GLA_failure_reason()
// can be queried for an extremely brief, end-user unfriendly explanation
// of why the load failed. Define GLAI_NO_FAILURE_STRINGS to avoid
// compiling these strings at all, and GLAI_FAILURE_USERMSG to get slightly
// more user-friendly ones.
//
// Paletted PNG, BMP, GIF, and PIC images are automatically depalettized.
//
// ===========================================================================
//
// iPhone PNG support:
//
// By default we convert iphone-formatted PNGs back to RGB; nominally they
// would silently load as BGR, except the existing code should have just
// failed on such iPhone PNGs. But you can disable this conversion by
// by calling GLA_convert_iphone_png_to_rgb(0), in which case
// you will always just get the native iphone "format" through.
//
// Call GLA_set_unpremultiply_on_load(1) as well to force a divide per
// pixel to remove any premultiplied alpha *only* if the image file explicitly
// says there's premultiplied data (currently only happens in iPhone images,
// and only if iPhone convert-to-rgb processing is on).
//
// ===========================================================================
//
// HDR image support   (disable by defining GLAI_NO_HDR)
//
// stb_image now supports loading HDR images in general, and currently
// the Radiance .HDR file format, although the support is provided
// generically. You can still load any file through the existing interface;
// if you attempt to load an HDR file, it will be automatically remapped to
// LDR, assuming gamma 2.2 and an arbitrary scale factor defaulting to 1;
// both of these constants can be reconfigured through this interface:
//
//     GLA_hdr_to_ldr_gamma(2.2f);
//     GLA_hdr_to_ldr_scale(1.0f);
//
// (note, do not use _inverse_ constants; GLA_image will invert them
// appropriately).
//
// Additionally, there is a new, parallel interface for loading files as
// (linear) floats to preserve the full dynamic range:
//
//    float *data = GLA_loadf(filename, &x, &y, &n, 0);
// 
// If you load LDR images through this interface, those images will
// be promoted to floating point values, run through the inverse of
// constants corresponding to the above:
//
//     GLA_ldr_to_hdr_scale(1.0f);
//     GLA_ldr_to_hdr_gamma(2.2f);
//
// Finally, given a filename (or an open file or memory block--see header
// file for details) containing image data, you can query for the "most
// appropriate" interface to use (that is, whether the image is HDR or
// not), using:
//
//     GLA_is_hdr(char *filename);
//
// ===========================================================================
//
// I/O callbacks
//
// I/O callbacks allow you to read from arbitrary sources, like packaged
// files or some other source. Data read from callbacks are processed
// through a small internal buffer (currently 128 bytes) to try to reduce
// overhead. 
//
// The three functions you must define are "read" (reads some bytes of data),
// "skip" (skips some bytes of data), "eof" (reports if the stream is at the end).


#ifndef GLAI_NO_STDIO
    #include <stdio.h>
#endif // GLAI_NO_STDIO

#define GLAI_VERSION 1

enum
{
   GLAI_default = 0, // only used for req_comp

   GLAI_grey       = 1,
   GLAI_grey_alpha = 2,
   GLAI_rgb        = 3,
   GLAI_rgb_alpha  = 4
};

typedef unsigned char GLA_uc;

#ifdef __cplusplus
extern "C" {
#endif

#define GLA_IMAGE_STATIC
#ifdef GLA_IMAGE_STATIC
    #define GLAIDEF static
#else
    #define GLAIDEF extern
#endif


//////////////////////////////////////////////////////////////////////////////
//
// PRIMARY API - works on images of any type
//

//
// load image by filename, open file, or memory buffer
//

//   GLAIDEF GLA_uc *GLA_load_from_memory(GLA_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);
//
//   #ifndef GLAI_NO_STDIO
//   GLAIDEF GLA_uc *GLA_load            (char const *filename,     int *x, int *y, int *comp, int req_comp);
//   GLAIDEF GLA_uc *GLA_load_from_file  (FILE *f,                  int *x, int *y, int *comp, int req_comp);
//   // for GLA_load_from_file, file pointer is left pointing immediately after image
//   #endif
//
   typedef struct
   {
      int      (*read)  (void *user,char *data,int size);   // fill 'data' with 'size' bytes.  return number of bytes actually read
      void     (*skip)  (void *user,int n);                 // skip the next 'n' bytes, or 'unget' the last -n bytes if negative
      int      (*eof)   (void *user);                       // returns nonzero if we are at end of file/data
   } GLA_io_callbacks;
//
//   GLAIDEF GLA_uc *GLA_load_from_callbacks  (GLA_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp);
//
//   #ifndef GLAI_NO_HDR
//      GLAIDEF float *GLA_loadf_from_memory(GLA_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);
//
//      #ifndef GLAI_NO_STDIO
//          GLAIDEF float *GLA_loadf            (char const *filename,   int *x, int *y, int *comp, int req_comp);
//          GLAIDEF float *GLA_loadf_from_file  (FILE *f,                int *x, int *y, int *comp, int req_comp);
//      #endif
//
//      GLAIDEF float *GLA_loadf_from_callbacks  (GLA_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp);
//
//      GLAIDEF void   GLA_hdr_to_ldr_gamma(float gamma);
//      GLAIDEF void   GLA_hdr_to_ldr_scale(float scale);
//
//      GLAIDEF void   GLA_ldr_to_hdr_gamma(float gamma);
//      GLAIDEF void   GLA_ldr_to_hdr_scale(float scale);
//   #endif // GLAI_NO_HDR
//
//   // GLA_is_hdr is always defined
//   GLAIDEF int    GLA_is_hdr_from_callbacks(GLA_io_callbacks const *clbk, void *user);
//   GLAIDEF int    GLA_is_hdr_from_memory(GLA_uc const *buffer, int len);
//   #ifndef GLAI_NO_STDIO
//   GLAIDEF int      GLA_is_hdr          (char const *filename);
//   GLAIDEF int      GLA_is_hdr_from_file(FILE *f);
//   #endif // GLAI_NO_STDIO
//
//
//   // get a VERY brief reason for failure
//   // NOT THREADSAFE
//   GLAIDEF const char *GLA_failure_reason  (void);
//
//   // free the loaded image -- this is just free()
//   GLAIDEF void     GLA_image_free      (void *retval_from_GLA_load);
//
//   // get image dimensions & components without fully decoding
//   GLAIDEF int      GLA_info_from_memory(GLA_uc const *buffer, int len, int *x, int *y, int *comp);
//   GLAIDEF int      GLA_info_from_callbacks(GLA_io_callbacks const *clbk, void *user, int *x, int *y, int *comp);
//
//   #ifndef GLAI_NO_STDIO
//   GLAIDEF int      GLA_info            (char const *filename,     int *x, int *y, int *comp);
//   GLAIDEF int      GLA_info_from_file  (FILE *f,                  int *x, int *y, int *comp);
//
//   #endif
//
//
//
//   // for image formats that explicitly notate that they have premultiplied alpha,
//   // we just return the colors as stored in the file. set this flag to force
//   // unpremultiplication. results are undefined if the unpremultiply overflow.
//   GLAIDEF void GLA_set_unpremultiply_on_load(int flag_true_if_should_unpremultiply);
//
//   // indicate whether we should process iphone images back to canonical format,
//   // or just pass them through "as-is"
//   GLAIDEF void GLA_convert_iphone_png_to_rgb(int flag_true_if_should_convert);
//
//
//   // ZLIB client - used by PNG, available for other purposes
//
//   //GLAIDEF char *GLA_zlib_decode_malloc_guesssize(const char *buffer, int len, int initial_size, int *outlen);
//   //GLAIDEF char *GLA_zlib_decode_malloc_guesssize_headerflag(const char *buffer, int len, int initial_size, int *outlen, int parse_header);
//   GLAIDEF char *GLA_zlib_decode_malloc(const char *buffer, int len, int *outlen);
//   GLAIDEF int   GLA_zlib_decode_buffer(char *obuffer, int olen, const char *ibuffer, int ilen);
//
//   GLAIDEF char *GLA_zlib_decode_noheader_malloc(const char *buffer, int len, int *outlen);
//   GLAIDEF int   GLA_zlib_decode_noheader_buffer(char *obuffer, int olen, const char *ibuffer, int ilen);
//
//
// define faster low-level operations (typically SIMD support)
#ifdef GLAI_SIMD
typedef void (*GLA_idct_8x8)(GLA_uc *out, int out_stride, short data[64], unsigned short *dequantize);
// compute an integer IDCT on "input"
//     input[x] = data[x] * dequantize[x]
//     write results to 'out': 64 samples, each run of 8 spaced by 'out_stride'
//                             CLAMP results to 0..255
typedef void (*GLA_YCbCr_to_RGB_run)(GLA_uc *output, GLA_uc const  *y, GLA_uc const *cb, GLA_uc const *cr, int count, int step);
// compute a conversion from YCbCr to RGB
//     'count' pixels
//     write pixels to 'output'; each pixel is 'step' bytes (either 3 or 4; if 4, write '255' as 4th), order R,G,B
//     y: Y input channel
//     cb: Cb input channel; scale/biased to be 0..255
//     cr: Cr input channel; scale/biased to be 0..255

GLAIDEF void GLA_install_idct(GLA_idct_8x8 func);
GLAIDEF void GLA_install_YCbCr_to_RGB(GLA_YCbCr_to_RGB_run func);
#endif // GLAI_SIMD


#ifdef __cplusplus
}
#endif


//
//
////   end header file   /////////////////////////////////////////////////////
#endif // GLAI_INCLUDE_GLA_IMAGE_H












































#define GLA_IMAGE_IMPLEMENTATION
#ifdef GLA_IMAGE_IMPLEMENTATION

#ifndef GLAI_NO_HDR
    #include <math.h>  // ldexp
    #include <string.h> // strcmp, strtok
#endif

#ifndef GLAI_NO_STDIO
    #include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#ifndef GLAI_ASSERT
    #include <assert.h>
    #define GLAI_ASSERT(x) assert(x)
#endif

#include <stdarg.h>
#include <stddef.h> // ptrdiff_t on osx

#ifndef _MSC_VER
   #ifdef __cplusplus
       #define GLA_inline inline
   #else
       #define GLA_inline
   #endif
#else
       #define GLA_inline __forceinline
#endif


#ifdef _MSC_VER
    typedef unsigned short GLA__uint16;
    typedef   signed short GLA__int16;
    typedef unsigned int   GLA__uint32;
    typedef   signed int   GLA__int32;
#else
#include <stdint.h>
    typedef uint16_t GLA__uint16;
    typedef int16_t  GLA__int16;
    typedef uint32_t GLA__uint32;
    typedef int32_t  GLA__int32;
#endif

// should produce compiler error if size is wrong
typedef unsigned char validate_uint32[sizeof(GLA__uint32)==4 ? 1 : -1];

#ifdef _MSC_VER
    #define GLAI_NOTUSED(v)  (void)(v)
#else
    #define GLAI_NOTUSED(v)  (void)sizeof(v)
#endif

#ifdef _MSC_VER
#define GLAI_HAS_LROTL
#endif

#ifdef GLAI_HAS_LROTL
   #define GLA_lrot(x,y)  _lrotl(x,y)
#else
   #define GLA_lrot(x,y)  (((x) << (y)) | ((x) >> (32 - (y))))
#endif

///////////////////////////////////////////////
//
//  GLA__context struct and start_xxx functions

// GLA__context structure is our basic context used by all images, so it
// contains all the IO context, plus some basic image information
typedef struct
{
   GLA__uint32 img_x, img_y;
   int img_n, img_out_n;
   
   GLA_io_callbacks io;
   void *io_user_data;

   int read_from_callbacks;
   int buflen;
   GLA_uc buffer_start[128];

   GLA_uc *img_buffer, *img_buffer_end;
   GLA_uc *img_buffer_original;
} GLA__context;


class ImageLoader
{
public:

//static void GLA__refill_buffer(GLA__context *s);

// initialize a memory-decode context
static void GLA__start_mem(GLA__context *s, GLA_uc const *buffer, int len)
{
   s->io.read = NULL;
   s->read_from_callbacks = 0;
   s->img_buffer = s->img_buffer_original = (GLA_uc *) buffer;
   s->img_buffer_end = (GLA_uc *) buffer+len;
}

// initialize a callback-based context
static void GLA__start_callbacks(GLA__context *s, GLA_io_callbacks *c, void *user)
{
   s->io = *c;
   s->io_user_data = user;
   s->buflen = sizeof(s->buffer_start);
   s->read_from_callbacks = 1;
   s->img_buffer_original = s->buffer_start;
   GLA__refill_buffer(s);
}

#ifndef GLAI_NO_STDIO

static int GLA__stdio_read(void *user, char *data, int size)
{
   return (int) fread(data,1,size,(FILE*) user);
}

static void GLA__stdio_skip(void *user, int n)
{
   fseek((FILE*) user, n, SEEK_CUR);
}

static int GLA__stdio_eof(void *user)
{
   return feof((FILE*) user);
}



static void GLA__start_file(GLA__context *s, FILE *f)
{
    static GLA_io_callbacks GLA__stdio_callbacks =
    {
       GLA__stdio_read,
       GLA__stdio_skip,
       GLA__stdio_eof,
    };

   GLA__start_callbacks(s, &GLA__stdio_callbacks, (void *) f);
}

//static void stop_file(GLA__context *s) { }

#endif // !GLAI_NO_STDIO

static void GLA__rewind(GLA__context *s)
{
   // conceptually rewind SHOULD rewind to the beginning of the stream,
   // but we just rewind to the beginning of the initial buffer, because
   // we only use it after doing 'test', which only ever looks at at most 92 bytes
   s->img_buffer = s->img_buffer_original;
}

// this is not threadsafe
/*
const char *GLA__g_failure_reason;

GLAIDEF const char *GLA_failure_reason(void)
{
   return GLA__g_failure_reason;
}

static int GLA__err(const char *str)
{
   GLA__g_failure_reason = str;
   return 0;
}
*/

static void *GLA__malloc(size_t size)
{
    return malloc(size);
}

// GLA__err - error
// GLA__errpf - error returning pointer to float
// GLA__errpuc - error returning pointer to unsigned char

#define GLAI_NO_FAILURE_STRINGS
#ifdef GLAI_NO_FAILURE_STRINGS
   #define GLA__err(x,y)  0
#elif defined(GLAI_FAILURE_USERMSG)
   #define GLA__err(x,y)  GLA__err(y)
#else
   #define GLA__err(x,y)  GLA__err(x)
#endif

#define GLA__errpf(x,y)   ((float *) (GLA__err(x,y)?NULL:NULL))
#define GLA__errpuc(x,y)  ((unsigned char *) (GLA__err(x,y)?NULL:NULL))

GLAIDEF void GLA_image_free(void *retval_from_GLA_load)
{
   free(retval_from_GLA_load);
}

#ifndef GLAI_NO_HDR
//static float   *GLA__ldr_to_hdr(GLA_uc *data, int x, int y, int comp);
//static GLA_uc *GLA__hdr_to_ldr(float   *data, int x, int y, int comp);
#endif

static unsigned char *GLA_load_main(GLA__context *s, int *x, int *y, int *comp, int req_comp)
{
   if (GLA__jpeg_test(s)) return GLA__jpeg_load(s,x,y,comp,req_comp);
   if (GLA__png_test(s))  return GLA__png_load(s,x,y,comp,req_comp);
   if (GLA__bmp_test(s))  return GLA__bmp_load(s,x,y,comp,req_comp);
   if (GLA__gif_test(s))  return GLA__gif_load(s,x,y,comp,req_comp);
   if (GLA__psd_test(s))  return GLA__psd_load(s,x,y,comp,req_comp);
   if (GLA__pic_test(s))  return GLA__pic_load(s,x,y,comp,req_comp);

   #ifndef GLAI_NO_HDR
   if (GLA__hdr_test(s))
   {
      float *hdr = GLA__hdr_load(s, x,y,comp,req_comp);
      return GLA__hdr_to_ldr(hdr, *x, *y, req_comp ? req_comp : *comp);
   }
   #endif

   // test tga last because it's a crappy test!
   if (GLA__tga_test(s))
      return GLA__tga_load(s,x,y,comp,req_comp);

   return GLA__errpuc("unknown image type", "Image not of any known type, or corrupt");
}

#ifndef GLAI_NO_STDIO

GLAIDEF FILE *GLA__fopen(char const *filename, char const *mode)
{
   FILE *f;
#if defined(_MSC_VER) && _MSC_VER >= 1400
   if (0 != fopen_s(&f, filename, mode))
      f=0;
#else
   f = fopen(filename, mode);
#endif
   return f;
}


GLAIDEF unsigned char *GLA_load(char const *filename, int *x, int *y, int *comp, int req_comp)
{
   FILE *f = GLA__fopen(filename, "rb");
   unsigned char *result;
   if (!f) return GLA__errpuc("can't fopen", "Unable to open file");
   result = GLA_load_from_file(f,x,y,comp,req_comp);
   fclose(f);
   return result;
}

GLAIDEF unsigned char *GLA_load_from_file(FILE *f, int *x, int *y, int *comp, int req_comp)
{
   unsigned char *result;
   GLA__context s;
   GLA__start_file(&s,f);
   result = GLA_load_main(&s,x,y,comp,req_comp);
   if (result)
   {
      // need to 'unget' all the characters in the IO buffer
      fseek(f, - (int) (s.img_buffer_end - s.img_buffer), SEEK_CUR);
   }
   return result;
}
#endif //!GLAI_NO_STDIO

GLAIDEF unsigned char *GLA_load_from_memory(GLA_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
   GLA__context s;
   GLA__start_mem(&s,buffer,len);
   return GLA_load_main(&s,x,y,comp,req_comp);
}

unsigned char *GLA_load_from_callbacks(GLA_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp)
{
   GLA__context s;
   GLA__start_callbacks(&s, (GLA_io_callbacks *) clbk, user);
   return GLA_load_main(&s,x,y,comp,req_comp);
}

#ifndef GLAI_NO_HDR

float *GLA_loadf_main(GLA__context *s, int *x, int *y, int *comp, int req_comp)
{
   unsigned char *data;
   #ifndef GLAI_NO_HDR
   if (GLA__hdr_test(s))
      return GLA__hdr_load(s,x,y,comp,req_comp);
   #endif
   data = GLA_load_main(s, x, y, comp, req_comp);
   if (data)
      return GLA__ldr_to_hdr(data, *x, *y, req_comp ? req_comp : *comp);
   return GLA__errpf("unknown image type", "Image not of any known type, or corrupt");
}

float *GLA_loadf_from_memory(GLA_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
   GLA__context s;
   GLA__start_mem(&s,buffer,len);
   return GLA_loadf_main(&s,x,y,comp,req_comp);
}

float *GLA_loadf_from_callbacks(GLA_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp)
{
   GLA__context s;
   GLA__start_callbacks(&s, (GLA_io_callbacks *) clbk, user);
   return GLA_loadf_main(&s,x,y,comp,req_comp);
}

#ifndef GLAI_NO_STDIO
float *GLA_loadf(char const *filename, int *x, int *y, int *comp, int req_comp)
{
   float *result;
   FILE *f = GLA__fopen(filename, "rb");
   if (!f) return GLA__errpf("can't fopen", "Unable to open file");
   result = GLA_loadf_from_file(f,x,y,comp,req_comp);
   fclose(f);
   return result;
}

float *GLA_loadf_from_file(FILE *f, int *x, int *y, int *comp, int req_comp)
{
   GLA__context s;
   GLA__start_file(&s,f);
   return GLA_loadf_main(&s,x,y,comp,req_comp);
}
#endif // !GLAI_NO_STDIO

#endif // !GLAI_NO_HDR

// these is-hdr-or-not is defined independent of whether GLAI_NO_HDR is
// defined, for API simplicity; if GLAI_NO_HDR is defined, it always
// reports false!

int GLA_is_hdr_from_memory(GLA_uc const *buffer, int len)
{
   #ifndef GLAI_NO_HDR
   GLA__context s;
   GLA__start_mem(&s,buffer,len);
   return GLA__hdr_test(&s);
   #else
   GLAI_NOTUSED(buffer);
   GLAI_NOTUSED(len);
   return 0;
   #endif
}

#ifndef GLAI_NO_STDIO
GLAIDEF int      GLA_is_hdr          (char const *filename)
{
   FILE *f = GLA__fopen(filename, "rb");
   int result=0;
   if (f) {
      result = GLA_is_hdr_from_file(f);
      fclose(f);
   }
   return result;
}

GLAIDEF int      GLA_is_hdr_from_file(FILE *f)
{
   #ifndef GLAI_NO_HDR
   GLA__context s;
   GLA__start_file(&s,f);
   return GLA__hdr_test(&s);
   #else
   return 0;
   #endif
}
#endif // !GLAI_NO_STDIO

GLAIDEF int      GLA_is_hdr_from_callbacks(GLA_io_callbacks const *clbk, void *user)
{
   #ifndef GLAI_NO_HDR
   GLA__context s;
   GLA__start_callbacks(&s, (GLA_io_callbacks *) clbk, user);
   return GLA__hdr_test(&s);
   #else
   return 0;
   #endif
}

#ifndef GLAI_NO_HDR
    //static float GLA__h2l_gamma_i=1.0f/2.2f, GLA__h2l_scale_i=1.0f;
    //static float GLA__l2h_gamma=2.2f, GLA__l2h_scale=1.0f;


    static float GLA__h2l_gamma_i(bool SET=false, float value=0)
    {
        static float v = 1.0f/2.2f;

        if(SET)
            v = value;

        return v;
    }

    static float GLA__h2l_scale_i(bool SET=false, float value=0)
    {
        static float v = 1.0f/2.2f;

        if(SET)
            v = value;

        return v;
    }

    static float GLA__l2h_gamma(bool SET=false, float value=0)
    {
        static float v = 1.0f/2.2f;

        if(SET)
            v = value;

        return v;
    }

    static float GLA__l2h_scale(bool SET=false, float value=0)
    {
        static float v = 1.0f/2.2f;

        if(SET)
            v = value;

        return v;
    }


    static void   GLA_hdr_to_ldr_gamma(float gamma) { GLA__h2l_gamma_i(true, 1/gamma); }
    static void   GLA_hdr_to_ldr_scale(float scale) { GLA__h2l_scale_i(true, 1/scale); }

    static void   GLA_ldr_to_hdr_gamma(float gamma) { GLA__l2h_gamma(true, gamma); }
    static void   GLA_ldr_to_hdr_scale(float scale) { GLA__l2h_scale(true, scale); }
#endif


//////////////////////////////////////////////////////////////////////////////
//
// Common code used by all image loaders
//

enum
{
   SCAN_load=0,
   SCAN_type,
   SCAN_header
};

static void GLA__refill_buffer(GLA__context *s)
{
   int n = (s->io.read)(s->io_user_data,(char*)s->buffer_start,s->buflen);
   if (n == 0) {
      // at end of file, treat same as if from memory, but need to handle case
      // where s->img_buffer isn't pointing to safe memory, e.g. 0-byte file
      s->read_from_callbacks = 0;
      s->img_buffer = s->buffer_start;
      s->img_buffer_end = s->buffer_start+1;
      *s->img_buffer = 0;
   } else {
      s->img_buffer = s->buffer_start;
      s->img_buffer_end = s->buffer_start + n;
   }
}

GLA_inline static GLA_uc GLA__get8(GLA__context *s)
{
   if (s->img_buffer < s->img_buffer_end)
      return *s->img_buffer++;
   if (s->read_from_callbacks) {
      GLA__refill_buffer(s);
      return *s->img_buffer++;
   }
   return 0;
}

GLA_inline static int GLA__at_eof(GLA__context *s)
{
   if (s->io.read) {
      if (!(s->io.eof)(s->io_user_data)) return 0;
      // if feof() is true, check if buffer = end
      // special case: we've only got the special 0 character at the end
      if (s->read_from_callbacks == 0) return 1;
   }

   return s->img_buffer >= s->img_buffer_end;   
}

static void GLA__skip(GLA__context *s, int n)
{
   if (s->io.read) {
      int blen = (int) (s->img_buffer_end - s->img_buffer);
      if (blen < n) {
         s->img_buffer = s->img_buffer_end;
         (s->io.skip)(s->io_user_data, n - blen);
         return;
      }
   }
   s->img_buffer += n;
}

static int GLA__getn(GLA__context *s, GLA_uc *buffer, int n)
{
   if (s->io.read) {
      int blen = (int) (s->img_buffer_end - s->img_buffer);
      if (blen < n) {
         int res, count;

         memcpy(buffer, s->img_buffer, blen);
         
         count = (s->io.read)(s->io_user_data, (char*) buffer + blen, n - blen);
         res = (count == (n-blen));
         s->img_buffer = s->img_buffer_end;
         return res;
      }
   }

   if (s->img_buffer+n <= s->img_buffer_end) {
      memcpy(buffer, s->img_buffer, n);
      s->img_buffer += n;
      return 1;
   } else
      return 0;
}

static int GLA__get16be(GLA__context *s)
{
   int z = GLA__get8(s);
   return (z << 8) + GLA__get8(s);
}

static GLA__uint32 GLA__get32be(GLA__context *s)
{
   GLA__uint32 z = GLA__get16be(s);
   return (z << 16) + GLA__get16be(s);
}

static int GLA__get16le(GLA__context *s)
{
   int z = GLA__get8(s);
   return z + (GLA__get8(s) << 8);
}

static GLA__uint32 GLA__get32le(GLA__context *s)
{
   GLA__uint32 z = GLA__get16le(s);
   return z + (GLA__get16le(s) << 16);
}

//////////////////////////////////////////////////////////////////////////////
//
//  generic converter from built-in img_n to req_comp
//    individual types do this automatically as much as possible (e.g. jpeg
//    does all cases internally since it needs to colorspace convert anyway,
//    and it never has alpha, so very few cases ). png can automatically
//    interleave an alpha=255 channel, but falls back to this for other cases
//
//  assume data buffer is malloced, so malloc a new one and free that one
//  only failure mode is malloc failing

static GLA_uc GLA__compute_y(int r, int g, int b)
{
   return (GLA_uc) (((r*77) + (g*150) +  (29*b)) >> 8);
}

static unsigned char *GLA__convert_format(unsigned char *data, int img_n, int req_comp, unsigned int x, unsigned int y)
{
   int i,j;
   unsigned char *good;

   if (req_comp == img_n) return data;
   GLAI_ASSERT(req_comp >= 1 && req_comp <= 4);

   good = (unsigned char *) GLA__malloc(req_comp * x * y);
   if (good == NULL) {
      free(data);
      return GLA__errpuc("outofmem", "Out of memory");
   }

   for (j=0; j < (int) y; ++j) {
      unsigned char *src  = data + j * x * img_n   ;
      unsigned char *dest = good + j * x * req_comp;

      #define COMBO(a,b)  ((a)*8+(b))
      #define CASE(a,b)   case COMBO(a,b): for(i=x-1; i >= 0; --i, src += a, dest += b)
      // convert source image with img_n components to one with req_comp components;
      // avoid switch per pixel, so use switch per scanline and massive macros
      switch (COMBO(img_n, req_comp)) {
         CASE(1,2) dest[0]=src[0], dest[1]=255; break;
         CASE(1,3) dest[0]=dest[1]=dest[2]=src[0]; break;
         CASE(1,4) dest[0]=dest[1]=dest[2]=src[0], dest[3]=255; break;
         CASE(2,1) dest[0]=src[0]; break;
         CASE(2,3) dest[0]=dest[1]=dest[2]=src[0]; break;
         CASE(2,4) dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1]; break;
         CASE(3,4) dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=255; break;
         CASE(3,1) dest[0]=GLA__compute_y(src[0],src[1],src[2]); break;
         CASE(3,2) dest[0]=GLA__compute_y(src[0],src[1],src[2]), dest[1] = 255; break;
         CASE(4,1) dest[0]=GLA__compute_y(src[0],src[1],src[2]); break;
         CASE(4,2) dest[0]=GLA__compute_y(src[0],src[1],src[2]), dest[1] = src[3]; break;
         CASE(4,3) dest[0]=src[0],dest[1]=src[1],dest[2]=src[2]; break;
         default: GLAI_ASSERT(0);
      }
      #undef CASE
   }

   free(data);
   return good;
}

#ifndef GLAI_NO_HDR
static float   *GLA__ldr_to_hdr(GLA_uc *data, int x, int y, int comp)
{
   float l2h_scale =  GLA__l2h_scale();
   float l2h_gamma =  GLA__l2h_gamma();

   int i,k,n;
   float *output = (float *) GLA__malloc(x * y * comp * sizeof(float));
   if (output == NULL) { free(data); return GLA__errpf("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   if (comp & 1) n = comp; else n = comp-1;
   for (i=0; i < x*y; ++i) {
      for (k=0; k < n; ++k) {
         output[i*comp + k] = (float) (pow(data[i*comp+k]/255.0f, l2h_gamma) * l2h_scale);
      }
      if (k < comp) output[i*comp + k] = data[i*comp+k]/255.0f;
   }
   free(data);
   return output;
}

#define GLA__float2int(x)   ((int) (x))
static GLA_uc *GLA__hdr_to_ldr(float   *data, int x, int y, int comp)
{
   float h2l_scale_i =  GLA__h2l_scale_i();
   float h2l_gamma_i =  GLA__h2l_gamma_i();

   int i,k,n;
   GLA_uc *output = (GLA_uc *) GLA__malloc(x * y * comp);
   if (output == NULL) { free(data); return GLA__errpuc("outofmem", "Out of memory"); }
   // compute number of non-alpha components
   if (comp & 1) n = comp; else n = comp-1;
   for (i=0; i < x*y; ++i) {
      for (k=0; k < n; ++k) {
         float z = (float) pow(data[i*comp+k]*h2l_scale_i, h2l_gamma_i) * 255 + 0.5f;
         if (z < 0) z = 0;
         if (z > 255) z = 255;
         output[i*comp + k] = (GLA_uc) GLA__float2int(z);
      }
      if (k < comp) {
         float z = data[i*comp+k] * 255 + 0.5f;
         if (z < 0) z = 0;
         if (z > 255) z = 255;
         output[i*comp + k] = (GLA_uc) GLA__float2int(z);
      }
   }
   free(data);
   return output;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//  "baseline" JPEG/JFIF decoder (not actually fully baseline implementation)
//
//    simple implementation
//      - channel subsampling of at most 2 in each dimension
//      - doesn't support delayed output of y-dimension
//      - simple interface (only one output format: 8-bit interleaved RGB)
//      - doesn't try to recover corrupt jpegs
//      - doesn't allow partial loading, loading multiple at once
//      - still fast on x86 (copying globals into locals doesn't help x86)
//      - allocates lots of intermediate memory (full size of all components)
//        - non-interleaved case requires this anyway
//        - allows good upsampling (see next)
//    high-quality
//      - upsampled channels are bilinearly interpolated, even across blocks
//      - quality integer IDCT derived from IJG's 'slow'
//    performance
//      - fast huffman; reasonable integer IDCT
//      - uses a lot of intermediate memory, could cache poorly
//      - load http://nothings.org/remote/anemones.jpg 3 times on 2.8Ghz P4
//          stb_jpeg:   1.34 seconds (MSVC6, default release build)
//          stb_jpeg:   1.06 seconds (MSVC6, processor = Pentium Pro)
//          IJL11.dll:  1.08 seconds (compiled by intel)
//          IJG 1998:   0.98 seconds (MSVC6, makefile provided by IJG)
//          IJG 1998:   0.95 seconds (MSVC6, makefile + proc=PPro)

// huffman decoding acceleration
#define FAST_BITS   9  // larger handles more cases; smaller stomps less cache

typedef struct
{
   GLA_uc  fast[1 << FAST_BITS];
   // weirdly, repacking this into AoS is a 10% speed loss, instead of a win
   GLA__uint16 code[256];
   GLA_uc  values[256];
   GLA_uc  size[257];
   unsigned int maxcode[18];
   int    delta[17];   // old 'firstsymbol' - old 'firstcode'
} GLA__huffman;

typedef struct
{
   #ifdef GLAI_SIMD
   unsigned short dequant2[4][64];
   #endif
   GLA__context *s;
   GLA__huffman huff_dc[4];
   GLA__huffman huff_ac[4];
   GLA_uc dequant[4][64];

// sizes for components, interleaved MCUs
   int img_h_max, img_v_max;
   int img_mcu_x, img_mcu_y;
   int img_mcu_w, img_mcu_h;

// definition of jpeg image component
   struct
   {
      int id;
      int h,v;
      int tq;
      int hd,ha;
      int dc_pred;

      int x,y,w2,h2;
      GLA_uc *data;
      void *raw_data;
      GLA_uc *linebuf;
   } img_comp[4];

   GLA__uint32         code_buffer; // jpeg entropy-coded buffer
   int            code_bits;   // number of valid bits
   unsigned char  marker;      // marker seen while filling entropy buffer
   int            nomore;      // flag if we saw a marker so must stop

   int scan_n, order[4];
   int restart_interval, todo;
} GLA__jpeg;

static int GLA__build_huffman(GLA__huffman *h, int *count)
{
   int i,j,k=0,code;
   // build size list for each symbol (from JPEG spec)
   for (i=0; i < 16; ++i)
      for (j=0; j < count[i]; ++j)
         h->size[k++] = (GLA_uc) (i+1);
   h->size[k] = 0;

   // compute actual symbols (from jpeg spec)
   code = 0;
   k = 0;
   for(j=1; j <= 16; ++j) {
      // compute delta to add to code to compute symbol id
      h->delta[j] = k - code;
      if (h->size[k] == j) {
         while (h->size[k] == j)
            h->code[k++] = (GLA__uint16) (code++);
         if (code-1 >= (1 << j)) return GLA__err("bad code lengths","Corrupt JPEG");
      }
      // compute largest code + 1 for this size, preshifted as needed later
      h->maxcode[j] = code << (16-j);
      code <<= 1;
   }
   h->maxcode[j] = 0xffffffff;

   // build non-spec acceleration table; 255 is flag for not-accelerated
   memset(h->fast, 255, 1 << FAST_BITS);
   for (i=0; i < k; ++i) {
      int s = h->size[i];
      if (s <= FAST_BITS) {
         int c = h->code[i] << (FAST_BITS-s);
         int m = 1 << (FAST_BITS-s);
         for (j=0; j < m; ++j) {
            h->fast[c+j] = (GLA_uc) i;
         }
      }
   }
   return 1;
}

static void GLA__grow_buffer_unsafe(GLA__jpeg *j)
{
   do {
      int b = j->nomore ? 0 : GLA__get8(j->s);
      if (b == 0xff) {
         int c = GLA__get8(j->s);
         if (c != 0) {
            j->marker = (unsigned char) c;
            j->nomore = 1;
            return;
         }
      }
      j->code_buffer |= b << (24 - j->code_bits);
      j->code_bits += 8;
   } while (j->code_bits <= 24);
}

// (1 << n) - 1


// decode a jpeg huffman value from the bitstream
GLA_inline static int GLA__jpeg_huff_decode(GLA__jpeg *j, GLA__huffman *h)
{
    static GLA__uint32 GLA__bmask[17]={0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};
   unsigned int temp;
   int c,k;

   if (j->code_bits < 16) GLA__grow_buffer_unsafe(j);

   // look at the top FAST_BITS and determine what symbol ID it is,
   // if the code is <= FAST_BITS
   c = (j->code_buffer >> (32 - FAST_BITS)) & ((1 << FAST_BITS)-1);
   k = h->fast[c];
   if (k < 255) {
      int s = h->size[k];
      if (s > j->code_bits)
         return -1;
      j->code_buffer <<= s;
      j->code_bits -= s;
      return h->values[k];
   }

   // naive test is to shift the code_buffer down so k bits are
   // valid, then test against maxcode. To speed this up, we've
   // preshifted maxcode left so that it has (16-k) 0s at the
   // end; in other words, regardless of the number of bits, it
   // wants to be compared against something shifted to have 16;
   // that way we don't need to shift inside the loop.
   temp = j->code_buffer >> 16;
   for (k=FAST_BITS+1 ; ; ++k)
      if (temp < h->maxcode[k])
         break;
   if (k == 17) {
      // error! code not found
      j->code_bits -= 16;
      return -1;
   }

   if (k > j->code_bits)
      return -1;

   // convert the huffman code to the symbol id
   c = ((j->code_buffer >> (32 - k)) & GLA__bmask[k]) + h->delta[k];
   GLAI_ASSERT((((j->code_buffer) >> (32 - h->size[c])) & GLA__bmask[h->size[c]]) == h->code[c]);

   // convert the id to a symbol
   j->code_bits -= k;
   j->code_buffer <<= k;
   return h->values[c];
}

// combined JPEG 'receive' and JPEG 'extend', since baseline
// always extends everything it receives.
GLA_inline static int GLA__extend_receive(GLA__jpeg *j, int n)
{
    static GLA__uint32 GLA__bmask[17]={0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};
   unsigned int m = 1 << (n-1);
   unsigned int k;
   if (j->code_bits < n) GLA__grow_buffer_unsafe(j);

   #if 1
   k = GLA_lrot(j->code_buffer, n);
   j->code_buffer = k & ~GLA__bmask[n];
   k &= GLA__bmask[n];
   j->code_bits -= n;
   #else
   k = (j->code_buffer >> (32 - n)) & GLA__bmask[n];
   j->code_bits -= n;
   j->code_buffer <<= n;
   #endif
   // the following test is probably a random branch that won't
   // predict well. I tried to table accelerate it but failed.
   // maybe it's compiling as a conditional move?
   if (k < m)
      return (-1 << n) + k + 1;
   else
      return k;
}

// given a value that's at position X in the zigzag stream,
// where does it appear in the 8x8 matrix coded as row-major?
//static constexpr GLA_uc GLA__jpeg_dezigzag[64+15] =
//{
//    0,  1,  8, 16,  9,  2,  3, 10,
//   17, 24, 32, 25, 18, 11,  4,  5,
//   12, 19, 26, 33, 40, 48, 41, 34,
//   27, 20, 13,  6,  7, 14, 21, 28,
//   35, 42, 49, 56, 57, 50, 43, 36,
//   29, 22, 15, 23, 30, 37, 44, 51,
//   58, 59, 52, 45, 38, 31, 39, 46,
//   53, 60, 61, 54, 47, 55, 62, 63,
//   // let corrupt input sample past end
//   63, 63, 63, 63, 63, 63, 63, 63,
//   63, 63, 63, 63, 63, 63, 63
//};


// decode one 64-entry block--
static int GLA__jpeg_decode_block(GLA__jpeg *j, short data[64], GLA__huffman *hdc, GLA__huffman *hac, int b)
{
   int diff,dc,k;
   int t = GLA__jpeg_huff_decode(j, hdc);

   static GLA_uc GLA__jpeg_dezigzag[64+15] =
   {
       0,  1,  8, 16,  9,  2,  3, 10,
      17, 24, 32, 25, 18, 11,  4,  5,
      12, 19, 26, 33, 40, 48, 41, 34,
      27, 20, 13,  6,  7, 14, 21, 28,
      35, 42, 49, 56, 57, 50, 43, 36,
      29, 22, 15, 23, 30, 37, 44, 51,
      58, 59, 52, 45, 38, 31, 39, 46,
      53, 60, 61, 54, 47, 55, 62, 63,
      // let corrupt input sample past end
      63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63
   };

   if (t < 0) return GLA__err("bad huffman code","Corrupt JPEG");

   // 0 all the ac values now so we can do it 32-bits at a time
   memset(data,0,64*sizeof(data[0]));

   diff = t ? GLA__extend_receive(j, t) : 0;
   dc = j->img_comp[b].dc_pred + diff;
   j->img_comp[b].dc_pred = dc;
   data[0] = (short) dc;

   // decode AC components, see JPEG spec
   k = 1;
   do {
      int r,s;
      int rs = GLA__jpeg_huff_decode(j, hac);
      if (rs < 0) return GLA__err("bad huffman code","Corrupt JPEG");
      s = rs & 15;
      r = rs >> 4;
      if (s == 0) {
         if (rs != 0xf0) break; // end block
         k += 16;
      } else {
         k += r;
         // decode into unzigzag'd location
         data[GLA__jpeg_dezigzag[k++]] = (short) GLA__extend_receive(j,s);
      }
   } while (k < 64);
   return 1;
}

// take a -128..127 value and GLA__clamp it and convert to 0..255
GLA_inline static GLA_uc GLA__clamp(int x)
{
   // trick to use a single test to catch both cases
   if ((unsigned int) x > 255) {
      if (x < 0) return 0;
      if (x > 255) return 255;
   }
   return (GLA_uc) x;
}

#define GLA__f2f(x)  (int) (((x) * 4096 + 0.5))
#define GLA__fsh(x)  ((x) << 12)

// derived from jidctint -- DCT_ISLOW
#define GLAI__IDCT_1D(s0,s1,s2,s3,s4,s5,s6,s7)       \
   int t0,t1,t2,t3,p1,p2,p3,p4,p5,x0,x1,x2,x3; \
   p2 = s2;                                    \
   p3 = s6;                                    \
   p1 = (p2+p3) * GLA__f2f(0.5411961f);             \
   t2 = p1 + p3*GLA__f2f(-1.847759065f);            \
   t3 = p1 + p2*GLA__f2f( 0.765366865f);            \
   p2 = s0;                                    \
   p3 = s4;                                    \
   t0 = GLA__fsh(p2+p3);                            \
   t1 = GLA__fsh(p2-p3);                            \
   x0 = t0+t3;                                 \
   x3 = t0-t3;                                 \
   x1 = t1+t2;                                 \
   x2 = t1-t2;                                 \
   t0 = s7;                                    \
   t1 = s5;                                    \
   t2 = s3;                                    \
   t3 = s1;                                    \
   p3 = t0+t2;                                 \
   p4 = t1+t3;                                 \
   p1 = t0+t3;                                 \
   p2 = t1+t2;                                 \
   p5 = (p3+p4)*GLA__f2f( 1.175875602f);            \
   t0 = t0*GLA__f2f( 0.298631336f);                 \
   t1 = t1*GLA__f2f( 2.053119869f);                 \
   t2 = t2*GLA__f2f( 3.072711026f);                 \
   t3 = t3*GLA__f2f( 1.501321110f);                 \
   p1 = p5 + p1*GLA__f2f(-0.899976223f);            \
   p2 = p5 + p2*GLA__f2f(-2.562915447f);            \
   p3 = p3*GLA__f2f(-1.961570560f);                 \
   p4 = p4*GLA__f2f(-0.390180644f);                 \
   t3 += p1+p4;                                \
   t2 += p2+p3;                                \
   t1 += p2+p4;                                \
   t0 += p1+p3;

#ifdef GLAI_SIMD
typedef unsigned short GLA_dequantize_t;
#else
typedef GLA_uc GLA_dequantize_t;
#endif

// .344 seconds on 3*anemones.jpg
static void GLA__idct_block(GLA_uc *out, int out_stride, short data[64], GLA_dequantize_t *dequantize)
{
   int i,val[64],*v=val;
   GLA_dequantize_t *dq = dequantize;
   GLA_uc *o;
   short *d = data;

   // columns
   for (i=0; i < 8; ++i,++d,++dq, ++v) {
      // if all zeroes, shortcut -- this avoids dequantizing 0s and IDCTing
      if (d[ 8]==0 && d[16]==0 && d[24]==0 && d[32]==0
           && d[40]==0 && d[48]==0 && d[56]==0) {
         //    no shortcut                 0     seconds
         //    (1|2|3|4|5|6|7)==0          0     seconds
         //    all separate               -0.047 seconds
         //    1 && 2|3 && 4|5 && 6|7:    -0.047 seconds
         int dcterm = d[0] * dq[0] << 2;
         v[0] = v[8] = v[16] = v[24] = v[32] = v[40] = v[48] = v[56] = dcterm;
      } else {
         GLAI__IDCT_1D(d[ 0]*dq[ 0],d[ 8]*dq[ 8],d[16]*dq[16],d[24]*dq[24],
                 d[32]*dq[32],d[40]*dq[40],d[48]*dq[48],d[56]*dq[56])
         // constants scaled things up by 1<<12; let's bring them back
         // down, but keep 2 extra bits of precision
         x0 += 512; x1 += 512; x2 += 512; x3 += 512;
         v[ 0] = (x0+t3) >> 10;
         v[56] = (x0-t3) >> 10;
         v[ 8] = (x1+t2) >> 10;
         v[48] = (x1-t2) >> 10;
         v[16] = (x2+t1) >> 10;
         v[40] = (x2-t1) >> 10;
         v[24] = (x3+t0) >> 10;
         v[32] = (x3-t0) >> 10;
      }
   }

   for (i=0, v=val, o=out; i < 8; ++i,v+=8,o+=out_stride) {
      // no fast case since the first 1D IDCT spread components out
      GLAI__IDCT_1D(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7])
      // constants scaled things up by 1<<12, plus we had 1<<2 from first
      // loop, plus horizontal and vertical each scale by sqrt(8) so together
      // we've got an extra 1<<3, so 1<<17 total we need to remove.
      // so we want to round that, which means adding 0.5 * 1<<17,
      // aka 65536. Also, we'll end up with -128 to 127 that we want
      // to encode as 0..255 by adding 128, so we'll add that before the shift
      x0 += 65536 + (128<<17);
      x1 += 65536 + (128<<17);
      x2 += 65536 + (128<<17);
      x3 += 65536 + (128<<17);
      // tried computing the shifts into temps, or'ing the temps to see
      // if any were out of range, but that was slower
      o[0] = GLA__clamp((x0+t3) >> 17);
      o[7] = GLA__clamp((x0-t3) >> 17);
      o[1] = GLA__clamp((x1+t2) >> 17);
      o[6] = GLA__clamp((x1-t2) >> 17);
      o[2] = GLA__clamp((x2+t1) >> 17);
      o[5] = GLA__clamp((x2-t1) >> 17);
      o[3] = GLA__clamp((x3+t0) >> 17);
      o[4] = GLA__clamp((x3-t0) >> 17);
   }
}

#ifdef GLAI_SIMD
static GLA_idct_8x8 GLA__idct_installed = GLA__idct_block;

GLAIDEF void GLA_install_idct(GLA_idct_8x8 func)
{
   GLA__idct_installed = func;
}
#endif

#define GLAI__MARKER_none  0xff
// if there's a pending marker from the entropy stream, return that
// otherwise, fetch from the stream and get a marker. if there's no
// marker, return 0xff, which is never a valid marker value
static GLA_uc GLA__get_marker(GLA__jpeg *j)
{
   GLA_uc x;
   if (j->marker != GLAI__MARKER_none) { x = j->marker; j->marker = GLAI__MARKER_none; return x; }
   x = GLA__get8(j->s);
   if (x != 0xff) return GLAI__MARKER_none;
   while (x == 0xff)
      x = GLA__get8(j->s);
   return x;
}

// in each scan, we'll have scan_n components, and the order
// of the components is specified by order[]
#define GLAI__RESTART(x)     ((x) >= 0xd0 && (x) <= 0xd7)

// after a restart interval, GLA__jpeg_reset the entropy decoder and
// the dc prediction
static void GLA__jpeg_reset(GLA__jpeg *j)
{
   j->code_bits = 0;
   j->code_buffer = 0;
   j->nomore = 0;
   j->img_comp[0].dc_pred = j->img_comp[1].dc_pred = j->img_comp[2].dc_pred = 0;
   j->marker = GLAI__MARKER_none;
   j->todo = j->restart_interval ? j->restart_interval : 0x7fffffff;
   // no more than 1<<31 MCUs if no restart_interal? that's plenty safe,
   // since we don't even allow 1<<30 pixels
}

static int GLA__parse_entropy_coded_data(GLA__jpeg *z)
{
   GLA__jpeg_reset(z);
   if (z->scan_n == 1) {
      int i,j;
      #ifdef GLAI_SIMD
      __declspec(align(16))
      #endif
      short data[64];
      int n = z->order[0];
      // non-interleaved data, we just need to process one block at a time,
      // in trivial scanline order
      // number of blocks to do just depends on how many actual "pixels" this
      // component has, independent of interleaved MCU blocking and such
      int w = (z->img_comp[n].x+7) >> 3;
      int h = (z->img_comp[n].y+7) >> 3;
      for (j=0; j < h; ++j) {
         for (i=0; i < w; ++i) {
            if (!GLA__jpeg_decode_block(z, data, z->huff_dc+z->img_comp[n].hd, z->huff_ac+z->img_comp[n].ha, n)) return 0;
            #ifdef GLAI_SIMD
            GLA__idct_installed(z->img_comp[n].data+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, data, z->dequant2[z->img_comp[n].tq]);
            #else
            GLA__idct_block(z->img_comp[n].data+z->img_comp[n].w2*j*8+i*8, z->img_comp[n].w2, data, z->dequant[z->img_comp[n].tq]);
            #endif
            // every data block is an MCU, so countdown the restart interval
            if (--z->todo <= 0) {
               if (z->code_bits < 24) GLA__grow_buffer_unsafe(z);
               // if it's NOT a restart, then just bail, so we get corrupt data
               // rather than no data
               if (!GLAI__RESTART(z->marker)) return 1;
               GLA__jpeg_reset(z);
            }
         }
      }
   } else { // interleaved!
      int i,j,k,x,y;
      short data[64];
      for (j=0; j < z->img_mcu_y; ++j) {
         for (i=0; i < z->img_mcu_x; ++i) {
            // scan an interleaved mcu... process scan_n components in order
            for (k=0; k < z->scan_n; ++k) {
               int n = z->order[k];
               // scan out an mcu's worth of this component; that's just determined
               // by the basic H and V specified for the component
               for (y=0; y < z->img_comp[n].v; ++y) {
                  for (x=0; x < z->img_comp[n].h; ++x) {
                     int x2 = (i*z->img_comp[n].h + x)*8;
                     int y2 = (j*z->img_comp[n].v + y)*8;
                     if (!GLA__jpeg_decode_block(z, data, z->huff_dc+z->img_comp[n].hd, z->huff_ac+z->img_comp[n].ha, n)) return 0;
                     #ifdef GLAI_SIMD
                     GLA__idct_installed(z->img_comp[n].data+z->img_comp[n].w2*y2+x2, z->img_comp[n].w2, data, z->dequant2[z->img_comp[n].tq]);
                     #else
                     GLA__idct_block(z->img_comp[n].data+z->img_comp[n].w2*y2+x2, z->img_comp[n].w2, data, z->dequant[z->img_comp[n].tq]);
                     #endif
                  }
               }
            }
            // after all interleaved components, that's an interleaved MCU,
            // so now count down the restart interval
            if (--z->todo <= 0) {
               if (z->code_bits < 24) GLA__grow_buffer_unsafe(z);
               // if it's NOT a restart, then just bail, so we get corrupt data
               // rather than no data
               if (!GLAI__RESTART(z->marker)) return 1;
               GLA__jpeg_reset(z);
            }
         }
      }
   }
   return 1;
}

static int GLA__process_marker(GLA__jpeg *z, int m)
{
   int L;
   static GLA_uc GLA__jpeg_dezigzag[64+15] =
   {
       0,  1,  8, 16,  9,  2,  3, 10,
      17, 24, 32, 25, 18, 11,  4,  5,
      12, 19, 26, 33, 40, 48, 41, 34,
      27, 20, 13,  6,  7, 14, 21, 28,
      35, 42, 49, 56, 57, 50, 43, 36,
      29, 22, 15, 23, 30, 37, 44, 51,
      58, 59, 52, 45, 38, 31, 39, 46,
      53, 60, 61, 54, 47, 55, 62, 63,
      // let corrupt input sample past end
      63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63
   };


   switch (m) {
      case GLAI__MARKER_none: // no marker found
         return GLA__err("expected marker","Corrupt JPEG");

      case 0xC2: // GLA__SOF - progressive
         return GLA__err("progressive jpeg","JPEG format not supported (progressive)");

      case 0xDD: // DRI - specify restart interval
         if (GLA__get16be(z->s) != 4) return GLA__err("bad DRI len","Corrupt JPEG");
         z->restart_interval = GLA__get16be(z->s);
         return 1;

      case 0xDB: // DQT - define quantization table
         L = GLA__get16be(z->s)-2;
         while (L > 0) {
            int q = GLA__get8(z->s);
            int p = q >> 4;
            int t = q & 15,i;
            if (p != 0) return GLA__err("bad DQT type","Corrupt JPEG");
            if (t > 3) return GLA__err("bad DQT table","Corrupt JPEG");
            for (i=0; i < 64; ++i)
               z->dequant[t][GLA__jpeg_dezigzag[i]] = GLA__get8(z->s);
            #ifdef GLAI_SIMD
            for (i=0; i < 64; ++i)
               z->dequant2[t][i] = z->dequant[t][i];
            #endif
            L -= 65;
         }
         return L==0;

      case 0xC4: // DHT - define huffman table
         L = GLA__get16be(z->s)-2;
         while (L > 0) {
            GLA_uc *v;
            int sizes[16],i,n=0;
            int q = GLA__get8(z->s);
            int tc = q >> 4;
            int th = q & 15;
            if (tc > 1 || th > 3) return GLA__err("bad DHT header","Corrupt JPEG");
            for (i=0; i < 16; ++i) {
               sizes[i] = GLA__get8(z->s);
               n += sizes[i];
            }
            L -= 17;
            if (tc == 0) {
               if (!GLA__build_huffman(z->huff_dc+th, sizes)) return 0;
               v = z->huff_dc[th].values;
            } else {
               if (!GLA__build_huffman(z->huff_ac+th, sizes)) return 0;
               v = z->huff_ac[th].values;
            }
            for (i=0; i < n; ++i)
               v[i] = GLA__get8(z->s);
            L -= n;
         }
         return L==0;
   }
   // check for comment block or APP blocks
   if ((m >= 0xE0 && m <= 0xEF) || m == 0xFE) {
      GLA__skip(z->s, GLA__get16be(z->s)-2);
      return 1;
   }
   return 0;
}

// after we see GLA__SOS
static int GLA__process_scan_header(GLA__jpeg *z)
{
   int i;
   int Ls = GLA__get16be(z->s);
   z->scan_n = GLA__get8(z->s);
   if (z->scan_n < 1 || z->scan_n > 4 || z->scan_n > (int) z->s->img_n) return GLA__err("bad GLA__SOS component count","Corrupt JPEG");
   if (Ls != 6+2*z->scan_n) return GLA__err("bad GLA__SOS len","Corrupt JPEG");
   for (i=0; i < z->scan_n; ++i) {
      int id = GLA__get8(z->s), which;
      int q = GLA__get8(z->s);
      for (which = 0; which < z->s->img_n; ++which)
         if (z->img_comp[which].id == id)
            break;
      if (which == z->s->img_n) return 0;
      z->img_comp[which].hd = q >> 4;   if (z->img_comp[which].hd > 3) return GLA__err("bad DC huff","Corrupt JPEG");
      z->img_comp[which].ha = q & 15;   if (z->img_comp[which].ha > 3) return GLA__err("bad AC huff","Corrupt JPEG");
      z->order[i] = which;
   }
   if (GLA__get8(z->s) != 0) return GLA__err("bad GLA__SOS","Corrupt JPEG");
   GLA__get8(z->s); // should be 63, but might be 0
   if (GLA__get8(z->s) != 0) return GLA__err("bad GLA__SOS","Corrupt JPEG");

   return 1;
}

static int GLA__process_frame_header(GLA__jpeg *z, int scan)
{
   GLA__context *s = z->s;
   int Lf,p,i,q, h_max=1,v_max=1,c;
   Lf = GLA__get16be(s);         if (Lf < 11) return GLA__err("bad GLA__SOF len","Corrupt JPEG"); // JPEG
   p  = GLA__get8(s);          if (p != 8) return GLA__err("only 8-bit","JPEG format not supported: 8-bit only"); // JPEG baseline
   s->img_y = GLA__get16be(s);   if (s->img_y == 0) return GLA__err("no header height", "JPEG format not supported: delayed height"); // Legal, but we don't handle it--but neither does IJG
   s->img_x = GLA__get16be(s);   if (s->img_x == 0) return GLA__err("0 width","Corrupt JPEG"); // JPEG requires
   c = GLA__get8(s);
   if (c != 3 && c != 1) return GLA__err("bad component count","Corrupt JPEG");    // JFIF requires
   s->img_n = c;
   for (i=0; i < c; ++i) {
      z->img_comp[i].data = NULL;
      z->img_comp[i].linebuf = NULL;
   }

   if (Lf != 8+3*s->img_n) return GLA__err("bad GLA__SOF len","Corrupt JPEG");

   for (i=0; i < s->img_n; ++i) {
      z->img_comp[i].id = GLA__get8(s);
      if (z->img_comp[i].id != i+1)   // JFIF requires
         if (z->img_comp[i].id != i)  // some version of jpegtran outputs non-JFIF-compliant files!
            return GLA__err("bad component ID","Corrupt JPEG");
      q = GLA__get8(s);
      z->img_comp[i].h = (q >> 4);  if (!z->img_comp[i].h || z->img_comp[i].h > 4) return GLA__err("bad H","Corrupt JPEG");
      z->img_comp[i].v = q & 15;    if (!z->img_comp[i].v || z->img_comp[i].v > 4) return GLA__err("bad V","Corrupt JPEG");
      z->img_comp[i].tq = GLA__get8(s);  if (z->img_comp[i].tq > 3) return GLA__err("bad TQ","Corrupt JPEG");
   }

   if (scan != SCAN_load) return 1;

   if ((1 << 30) / s->img_x / s->img_n < s->img_y) return GLA__err("too large", "Image too large to decode");

   for (i=0; i < s->img_n; ++i) {
      if (z->img_comp[i].h > h_max) h_max = z->img_comp[i].h;
      if (z->img_comp[i].v > v_max) v_max = z->img_comp[i].v;
   }

   // compute interleaved mcu info
   z->img_h_max = h_max;
   z->img_v_max = v_max;
   z->img_mcu_w = h_max * 8;
   z->img_mcu_h = v_max * 8;
   z->img_mcu_x = (s->img_x + z->img_mcu_w-1) / z->img_mcu_w;
   z->img_mcu_y = (s->img_y + z->img_mcu_h-1) / z->img_mcu_h;

   for (i=0; i < s->img_n; ++i) {
      // number of effective pixels (e.g. for non-interleaved MCU)
      z->img_comp[i].x = (s->img_x * z->img_comp[i].h + h_max-1) / h_max;
      z->img_comp[i].y = (s->img_y * z->img_comp[i].v + v_max-1) / v_max;
      // to simplify generation, we'll allocate enough memory to decode
      // the bogus oversized data from using interleaved MCUs and their
      // big blocks (e.g. a 16x16 iMCU on an image of width 33); we won't
      // discard the extra data until colorspace conversion
      z->img_comp[i].w2 = z->img_mcu_x * z->img_comp[i].h * 8;
      z->img_comp[i].h2 = z->img_mcu_y * z->img_comp[i].v * 8;
      z->img_comp[i].raw_data = GLA__malloc(z->img_comp[i].w2 * z->img_comp[i].h2+15);
      if (z->img_comp[i].raw_data == NULL) {
         for(--i; i >= 0; --i) {
            free(z->img_comp[i].raw_data);
            z->img_comp[i].data = NULL;
         }
         return GLA__err("outofmem", "Out of memory");
      }
      // align blocks for installable-idct using mmx/sse
      z->img_comp[i].data = (GLA_uc*) (((size_t) z->img_comp[i].raw_data + 15) & ~15);
      z->img_comp[i].linebuf = NULL;
   }

   return 1;
}

// use comparisons since in some cases we handle more than one case (e.g. GLA__SOF)
#define GLA__DNL(x)         ((x) == 0xdc)
#define GLA__SOI(x)         ((x) == 0xd8)
#define GLA__EOI(x)         ((x) == 0xd9)
#define GLA__SOF(x)         ((x) == 0xc0 || (x) == 0xc1)
#define GLA__SOS(x)         ((x) == 0xda)

static int decode_jpeg_header(GLA__jpeg *z, int scan)
{
   int m;
   z->marker = GLAI__MARKER_none; // initialize cached marker to empty
   m = GLA__get_marker(z);
   if (!GLA__SOI(m)) return GLA__err("no GLA__SOI","Corrupt JPEG");
   if (scan == SCAN_type) return 1;
   m = GLA__get_marker(z);
   while (!GLA__SOF(m)) {
      if (!GLA__process_marker(z,m)) return 0;
      m = GLA__get_marker(z);
      while (m == GLAI__MARKER_none) {
         // some files have extra padding after their blocks, so ok, we'll scan
         if (GLA__at_eof(z->s)) return GLA__err("no GLA__SOF", "Corrupt JPEG");
         m = GLA__get_marker(z);
      }
   }
   if (!GLA__process_frame_header(z, scan)) return 0;
   return 1;
}

static int decode_jpeg_image(GLA__jpeg *j)
{
   int m;
   j->restart_interval = 0;
   if (!decode_jpeg_header(j, SCAN_load)) return 0;
   m = GLA__get_marker(j);
   while (!GLA__EOI(m)) {
      if (GLA__SOS(m)) {
         if (!GLA__process_scan_header(j)) return 0;
         if (!GLA__parse_entropy_coded_data(j)) return 0;
         if (j->marker == GLAI__MARKER_none ) {
            // handle 0s at the end of image data from IP Kamera 9060
            while (!GLA__at_eof(j->s)) {
               int x = GLA__get8(j->s);
               if (x == 255) {
                  j->marker = GLA__get8(j->s);
                  break;
               } else if (x != 0) {
                  return 0;
               }
            }
            // if we reach eof without hitting a marker, GLA__get_marker() below will fail and we'll eventually return 0
         }
      } else {
         if (!GLA__process_marker(j, m)) return 0;
      }
      m = GLA__get_marker(j);
   }
   return 1;
}

// static jfif-centered resampling (across block boundaries)

typedef GLA_uc *(*resample_row_func)(GLA_uc *out, GLA_uc *in0, GLA_uc *in1,
                                    int w, int hs);

#define GLA__div4(x) ((GLA_uc) ((x) >> 2))

static GLA_uc *resample_row_1(GLA_uc *out, GLA_uc *in_near, GLA_uc *in_far, int w, int hs)
{
   GLAI_NOTUSED(out);
   GLAI_NOTUSED(in_far);
   GLAI_NOTUSED(w);
   GLAI_NOTUSED(hs);
   return in_near;
}

static GLA_uc* GLA__resample_row_v_2(GLA_uc *out, GLA_uc *in_near, GLA_uc *in_far, int w, int hs)
{
   // need to generate two samples vertically for every one in input
   int i;
   GLAI_NOTUSED(hs);
   for (i=0; i < w; ++i)
      out[i] = GLA__div4(3*in_near[i] + in_far[i] + 2);
   return out;
}

static GLA_uc*  GLA__resample_row_h_2(GLA_uc *out, GLA_uc *in_near, GLA_uc *in_far, int w, int hs)
{
   // need to generate two samples horizontally for every one in input
   int i;
   GLA_uc *input = in_near;

   if (w == 1) {
      // if only one sample, can't do any interpolation
      out[0] = out[1] = input[0];
      return out;
   }

   out[0] = input[0];
   out[1] = GLA__div4(input[0]*3 + input[1] + 2);
   for (i=1; i < w-1; ++i) {
      int n = 3*input[i]+2;
      out[i*2+0] = GLA__div4(n+input[i-1]);
      out[i*2+1] = GLA__div4(n+input[i+1]);
   }
   out[i*2+0] = GLA__div4(input[w-2]*3 + input[w-1] + 2);
   out[i*2+1] = input[w-1];

   GLAI_NOTUSED(in_far);
   GLAI_NOTUSED(hs);

   return out;
}

#define GLA__div16(x) ((GLA_uc) ((x) >> 4))

static GLA_uc *GLA__resample_row_hv_2(GLA_uc *out, GLA_uc *in_near, GLA_uc *in_far, int w, int hs)
{
   // need to generate 2x2 samples for every one in input
   int i,t0,t1;
   if (w == 1) {
      out[0] = out[1] = GLA__div4(3*in_near[0] + in_far[0] + 2);
      return out;
   }

   t1 = 3*in_near[0] + in_far[0];
   out[0] = GLA__div4(t1+2);
   for (i=1; i < w; ++i) {
      t0 = t1;
      t1 = 3*in_near[i]+in_far[i];
      out[i*2-1] = GLA__div16(3*t0 + t1 + 8);
      out[i*2  ] = GLA__div16(3*t1 + t0 + 8);
   }
   out[w*2-1] = GLA__div4(t1+2);

   GLAI_NOTUSED(hs);

   return out;
}

static GLA_uc *GLA__resample_row_generic(GLA_uc *out, GLA_uc *in_near, GLA_uc *in_far, int w, int hs)
{
   // resample with nearest-neighbor
   int i,j;
   GLAI_NOTUSED(in_far);
   for (i=0; i < w; ++i)
      for (j=0; j < hs; ++j)
         out[i*hs+j] = in_near[i];
   return out;
}

#define float2fixed(x)  ((int) ((x) * 65536 + 0.5))

// 0.38 seconds on 3*anemones.jpg   (0.25 with processor = Pro)
// VC6 without processor=Pro is generating multiple LEAs per multiply!
static void GLA__YCbCr_to_RGB_row(GLA_uc *out, const GLA_uc *y, const GLA_uc *pcb, const GLA_uc *pcr, int count, int step)
{
   int i;
   for (i=0; i < count; ++i) {
      int y_fixed = (y[i] << 16) + 32768; // rounding
      int r,g,b;
      int cr = pcr[i] - 128;
      int cb = pcb[i] - 128;
      r = y_fixed + cr*float2fixed(1.40200f);
      g = y_fixed - cr*float2fixed(0.71414f) - cb*float2fixed(0.34414f);
      b = y_fixed                            + cb*float2fixed(1.77200f);
      r >>= 16;
      g >>= 16;
      b >>= 16;
      if ((unsigned) r > 255) { if (r < 0) r = 0; else r = 255; }
      if ((unsigned) g > 255) { if (g < 0) g = 0; else g = 255; }
      if ((unsigned) b > 255) { if (b < 0) b = 0; else b = 255; }
      out[0] = (GLA_uc)r;
      out[1] = (GLA_uc)g;
      out[2] = (GLA_uc)b;
      out[3] = 255;
      out += step;
   }
}

#ifdef GLAI_SIMD
static GLA_YCbCr_to_RGB_run GLA__YCbCr_installed = GLA__YCbCr_to_RGB_row;

GLAIDEF void GLA_install_YCbCr_to_RGB(GLA_YCbCr_to_RGB_run func)
{
   GLA__YCbCr_installed = func;
}
#endif


// clean up the temporary component buffers
static void GLA__cleanup_jpeg(GLA__jpeg *j)
{
   int i;
   for (i=0; i < j->s->img_n; ++i) {
      if (j->img_comp[i].raw_data) {
         free(j->img_comp[i].raw_data);
         j->img_comp[i].raw_data = NULL;
         j->img_comp[i].data = NULL;
      }
      if (j->img_comp[i].linebuf) {
         free(j->img_comp[i].linebuf);
         j->img_comp[i].linebuf = NULL;
      }
   }
}

typedef struct
{
   resample_row_func resample;
   GLA_uc *line0,*line1;
   int hs,vs;   // expansion factor in each axis
   int w_lores; // horizontal pixels pre-expansion 
   int ystep;   // how far through vertical expansion we are
   int ypos;    // which pre-expansion row we're on
} GLA__resample;

static GLA_uc *load_jpeg_image(GLA__jpeg *z, int *out_x, int *out_y, int *comp, int req_comp)
{
   int n, decode_n;
   z->s->img_n = 0; // make GLA__cleanup_jpeg safe

   // validate req_comp
   if (req_comp < 0 || req_comp > 4) return GLA__errpuc("bad req_comp", "Internal error");

   // load a jpeg image from whichever source
   if (!decode_jpeg_image(z)) { GLA__cleanup_jpeg(z); return NULL; }

   // determine actual number of components to generate
   n = req_comp ? req_comp : z->s->img_n;

   if (z->s->img_n == 3 && n < 3)
      decode_n = 1;
   else
      decode_n = z->s->img_n;

   // resample and color-convert
   {
      int k;
      unsigned int i,j;
      GLA_uc *output;
      GLA_uc *coutput[4];

      GLA__resample res_comp[4];

      for (k=0; k < decode_n; ++k) {
         GLA__resample *r = &res_comp[k];

         // allocate line buffer big enough for upsampling off the edges
         // with upsample factor of 4
         z->img_comp[k].linebuf = (GLA_uc *) GLA__malloc(z->s->img_x + 3);
         if (!z->img_comp[k].linebuf) { GLA__cleanup_jpeg(z); return GLA__errpuc("outofmem", "Out of memory"); }

         r->hs      = z->img_h_max / z->img_comp[k].h;
         r->vs      = z->img_v_max / z->img_comp[k].v;
         r->ystep   = r->vs >> 1;
         r->w_lores = (z->s->img_x + r->hs-1) / r->hs;
         r->ypos    = 0;
         r->line0   = r->line1 = z->img_comp[k].data;

         if      (r->hs == 1 && r->vs == 1) r->resample = resample_row_1;
         else if (r->hs == 1 && r->vs == 2) r->resample = GLA__resample_row_v_2;
         else if (r->hs == 2 && r->vs == 1) r->resample = GLA__resample_row_h_2;
         else if (r->hs == 2 && r->vs == 2) r->resample = GLA__resample_row_hv_2;
         else                               r->resample = GLA__resample_row_generic;
      }

      // can't error after this so, this is safe
      output = (GLA_uc *) GLA__malloc(n * z->s->img_x * z->s->img_y + 1);
      if (!output) { GLA__cleanup_jpeg(z); return GLA__errpuc("outofmem", "Out of memory"); }

      // now go ahead and resample
      for (j=0; j < z->s->img_y; ++j) {
         GLA_uc *out = output + n * z->s->img_x * j;
         for (k=0; k < decode_n; ++k) {
            GLA__resample *r = &res_comp[k];
            int y_bot = r->ystep >= (r->vs >> 1);
            coutput[k] = r->resample(z->img_comp[k].linebuf,
                                     y_bot ? r->line1 : r->line0,
                                     y_bot ? r->line0 : r->line1,
                                     r->w_lores, r->hs);
            if (++r->ystep >= r->vs) {
               r->ystep = 0;
               r->line0 = r->line1;
               if (++r->ypos < z->img_comp[k].y)
                  r->line1 += z->img_comp[k].w2;
            }
         }
         if (n >= 3) {
            GLA_uc *y = coutput[0];
            if (z->s->img_n == 3) {
               #ifdef GLAI_SIMD
               GLA__YCbCr_installed(out, y, coutput[1], coutput[2], z->s->img_x, n);
               #else
               GLA__YCbCr_to_RGB_row(out, y, coutput[1], coutput[2], z->s->img_x, n);
               #endif
            } else
               for (i=0; i < z->s->img_x; ++i) {
                  out[0] = out[1] = out[2] = y[i];
                  out[3] = 255; // not used if n==3
                  out += n;
               }
         } else {
            GLA_uc *y = coutput[0];
            if (n == 1)
               for (i=0; i < z->s->img_x; ++i) out[i] = y[i];
            else
               for (i=0; i < z->s->img_x; ++i) *out++ = y[i], *out++ = 255;
         }
      }
      GLA__cleanup_jpeg(z);
      *out_x = z->s->img_x;
      *out_y = z->s->img_y;
      if (comp) *comp  = z->s->img_n; // report original components, not output
      return output;
   }
}

static unsigned char *GLA__jpeg_load(GLA__context *s, int *x, int *y, int *comp, int req_comp)
{
   GLA__jpeg j;
   j.s = s;
   return load_jpeg_image(&j, x,y,comp,req_comp);
}

static int GLA__jpeg_test(GLA__context *s)
{
   int r;
   GLA__jpeg j;
   j.s = s;
   r = decode_jpeg_header(&j, SCAN_type);
   GLA__rewind(s);
   return r;
}

static int GLA__jpeg_info_raw(GLA__jpeg *j, int *x, int *y, int *comp)
{
   if (!decode_jpeg_header(j, SCAN_header)) {
      GLA__rewind( j->s );
      return 0;
   }
   if (x) *x = j->s->img_x;
   if (y) *y = j->s->img_y;
   if (comp) *comp = j->s->img_n;
   return 1;
}

static int GLA__jpeg_info(GLA__context *s, int *x, int *y, int *comp)
{
   GLA__jpeg j;
   j.s = s;
   return GLA__jpeg_info_raw(&j, x, y, comp);
}

// public domain zlib decode    v0.2  Sean Barrett 2006-11-18
//    simple implementation
//      - all input must be provided in an upfront buffer
//      - all output is written to a single output buffer (can malloc/realloc)
//    performance
//      - fast huffman

// fast-way is faster to check than jpeg huffman, but slow way is slower
#define GLAI__ZFAST_BITS  9 // accelerate all cases in default tables
#define GLAI__ZFAST_MASK  ((1 << GLAI__ZFAST_BITS) - 1)

// zlib-style huffman encoding
// (jpegs packs from left, zlib from right, so can't share code)
typedef struct
{
   GLA__uint16 fast[1 << GLAI__ZFAST_BITS];
   GLA__uint16 firstcode[16];
   int maxcode[17];
   GLA__uint16 firstsymbol[16];
   GLA_uc  size[288];
   GLA__uint16 value[288];
} GLA__zhuffman;

GLA_inline static int GLA__bitreverse16(int n)
{
  n = ((n & 0xAAAA) >>  1) | ((n & 0x5555) << 1);
  n = ((n & 0xCCCC) >>  2) | ((n & 0x3333) << 2);
  n = ((n & 0xF0F0) >>  4) | ((n & 0x0F0F) << 4);
  n = ((n & 0xFF00) >>  8) | ((n & 0x00FF) << 8);
  return n;
}

GLA_inline static int GLA__bit_reverse(int v, int bits)
{
   GLAI_ASSERT(bits <= 16);
   // to bit reverse n bits, reverse 16 and shift
   // e.g. 11 bits, bit reverse and shift away 5
   return GLA__bitreverse16(v) >> (16-bits);
}

static int GLA__zbuild_huffman(GLA__zhuffman *z, GLA_uc *sizelist, int num)
{
   int i,k=0;
   int code, next_code[16], sizes[17];

   // DEFLATE spec for generating codes
   memset(sizes, 0, sizeof(sizes));
   memset(z->fast, 255, sizeof(z->fast));
   for (i=0; i < num; ++i) 
      ++sizes[sizelist[i]];
   sizes[0] = 0;
   for (i=1; i < 16; ++i)
      GLAI_ASSERT(sizes[i] <= (1 << i));
   code = 0;
   for (i=1; i < 16; ++i) {
      next_code[i] = code;
      z->firstcode[i] = (GLA__uint16) code;
      z->firstsymbol[i] = (GLA__uint16) k;
      code = (code + sizes[i]);
      if (sizes[i])
         if (code-1 >= (1 << i)) return GLA__err("bad codelengths","Corrupt JPEG");
      z->maxcode[i] = code << (16-i); // preshift for inner loop
      code <<= 1;
      k += sizes[i];
   }
   z->maxcode[16] = 0x10000; // sentinel
   for (i=0; i < num; ++i) {
      int s = sizelist[i];
      if (s) {
         int c = next_code[s] - z->firstcode[s] + z->firstsymbol[s];
         z->size [c] = (GLA_uc     ) s;
         z->value[c] = (GLA__uint16) i;
         if (s <= GLAI__ZFAST_BITS) {
            int k = GLA__bit_reverse(next_code[s],s);
            while (k < (1 << GLAI__ZFAST_BITS)) {
               z->fast[k] = (GLA__uint16) c;
               k += (1 << s);
            }
         }
         ++next_code[s];
      }
   }
   return 1;
}

// zlib-from-memory implementation for PNG reading
//    because PNG allows splitting the zlib stream arbitrarily,
//    and it's annoying structurally to have PNG call ZLIB call PNG,
//    we require PNG read all the IDATs and combine them into a single
//    memory buffer

typedef struct
{
   GLA_uc *zbuffer, *zbuffer_end;
   int num_bits;
   GLA__uint32 code_buffer;

   char *zout;
   char *zout_start;
   char *zout_end;
   int   z_expandable;

   GLA__zhuffman z_length, z_distance;
} GLA__zbuf;

GLA_inline static GLA_uc GLA__zget8(GLA__zbuf *z)
{
   if (z->zbuffer >= z->zbuffer_end) return 0;
   return *z->zbuffer++;
}

static void GLA__fill_bits(GLA__zbuf *z)
{
   do {
      GLAI_ASSERT(z->code_buffer < (1U << z->num_bits));
      z->code_buffer |= GLA__zget8(z) << z->num_bits;
      z->num_bits += 8;
   } while (z->num_bits <= 24);
}

GLA_inline static unsigned int GLA__zreceive(GLA__zbuf *z, int n)
{
   unsigned int k;
   if (z->num_bits < n) GLA__fill_bits(z);
   k = z->code_buffer & ((1 << n) - 1);
   z->code_buffer >>= n;
   z->num_bits -= n;
   return k;   
}

GLA_inline static int GLA__zhuffman_decode(GLA__zbuf *a, GLA__zhuffman *z)
{
   int b,s,k;
   if (a->num_bits < 16) GLA__fill_bits(a);
   b = z->fast[a->code_buffer & GLAI__ZFAST_MASK];
   if (b < 0xffff) {
      s = z->size[b];
      a->code_buffer >>= s;
      a->num_bits -= s;
      return z->value[b];
   }

   // not resolved by fast table, so compute it the slow way
   // use jpeg approach, which requires MSbits at top
   k = GLA__bit_reverse(a->code_buffer, 16);
   for (s=GLAI__ZFAST_BITS+1; ; ++s)
      if (k < z->maxcode[s])
         break;
   if (s == 16) return -1; // invalid code!
   // code size is s, so:
   b = (k >> (16-s)) - z->firstcode[s] + z->firstsymbol[s];
   GLAI_ASSERT(z->size[b] == s);
   a->code_buffer >>= s;
   a->num_bits -= s;
   return z->value[b];
}

static int GLA__zexpand(GLA__zbuf *z, int n)  // need to make room for n bytes
{
   char *q;
   int cur, limit;
   if (!z->z_expandable) return GLA__err("output buffer limit","Corrupt PNG");
   cur   = (int) (z->zout     - z->zout_start);
   limit = (int) (z->zout_end - z->zout_start);
   while (cur + n > limit)
      limit *= 2;
   q = (char *) realloc(z->zout_start, limit);
   if (q == NULL) return GLA__err("outofmem", "Out of memory");
   z->zout_start = q;
   z->zout       = q + cur;
   z->zout_end   = q + limit;
   return 1;
}





static int GLA__parse_huffman_block(GLA__zbuf *a)
{
   static  int GLA__zdist_extra[32] = { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};
   static  int GLA__zlength_base[31] = {
      3,4,5,6,7,8,9,10,11,13,
      15,17,19,23,27,31,35,43,51,59,
      67,83,99,115,131,163,195,227,258,0,0 };
   static  int GLA__zlength_extra[31] = { 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0 };
   static int GLA__zdist_base[32] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193, 257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0};


   for(;;) {
      int z = GLA__zhuffman_decode(a, &a->z_length);
      if (z < 256) {
         if (z < 0) return GLA__err("bad huffman code","Corrupt PNG"); // error in huffman codes
         if (a->zout >= a->zout_end) if (!GLA__zexpand(a, 1)) return 0;
         *a->zout++ = (char) z;
      } else {
         GLA_uc *p;
         int len,dist;
         if (z == 256) return 1;
         z -= 257;
         len = GLA__zlength_base[z];
         if (GLA__zlength_extra[z]) len += GLA__zreceive(a, GLA__zlength_extra[z]);
         z = GLA__zhuffman_decode(a, &a->z_distance);
         if (z < 0) return GLA__err("bad huffman code","Corrupt PNG");
         dist = GLA__zdist_base[z];
         if (GLA__zdist_extra[z]) dist += GLA__zreceive(a, GLA__zdist_extra[z]);
         if (a->zout - a->zout_start < dist) return GLA__err("bad dist","Corrupt PNG");
         if (a->zout + len > a->zout_end) if (!GLA__zexpand(a, len)) return 0;
         p = (GLA_uc *) (a->zout - dist);
         while (len--)
            *a->zout++ = *p++;
      }
   }
}

static int GLA__compute_huffman_codes(GLA__zbuf *a)
{
   static GLA_uc length_dezigzag[19] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
   GLA__zhuffman z_codelength;
   GLA_uc lencodes[286+32+137];//padding for maximum single op
   GLA_uc codelength_sizes[19];
   int i,n;

   int hlit  = GLA__zreceive(a,5) + 257;
   int hdist = GLA__zreceive(a,5) + 1;
   int hclen = GLA__zreceive(a,4) + 4;

   memset(codelength_sizes, 0, sizeof(codelength_sizes));
   for (i=0; i < hclen; ++i) {
      int s = GLA__zreceive(a,3);
      codelength_sizes[length_dezigzag[i]] = (GLA_uc) s;
   }
   if (!GLA__zbuild_huffman(&z_codelength, codelength_sizes, 19)) return 0;

   n = 0;
   while (n < hlit + hdist) {
      int c = GLA__zhuffman_decode(a, &z_codelength);
      GLAI_ASSERT(c >= 0 && c < 19);
      if (c < 16)
         lencodes[n++] = (GLA_uc) c;
      else if (c == 16) {
         c = GLA__zreceive(a,2)+3;
         memset(lencodes+n, lencodes[n-1], c);
         n += c;
      } else if (c == 17) {
         c = GLA__zreceive(a,3)+3;
         memset(lencodes+n, 0, c);
         n += c;
      } else {
         GLAI_ASSERT(c == 18);
         c = GLA__zreceive(a,7)+11;
         memset(lencodes+n, 0, c);
         n += c;
      }
   }
   if (n != hlit+hdist) return GLA__err("bad codelengths","Corrupt PNG");
   if (!GLA__zbuild_huffman(&a->z_length, lencodes, hlit)) return 0;
   if (!GLA__zbuild_huffman(&a->z_distance, lencodes+hlit, hdist)) return 0;
   return 1;
}

static int GLA__parse_uncomperssed_block(GLA__zbuf *a)
{
   GLA_uc header[4];
   int len,nlen,k;
   if (a->num_bits & 7)
      GLA__zreceive(a, a->num_bits & 7); // discard
   // drain the bit-packed data into header
   k = 0;
   while (a->num_bits > 0) {
      header[k++] = (GLA_uc) (a->code_buffer & 255); // suppress MSVC run-time check
      a->code_buffer >>= 8;
      a->num_bits -= 8;
   }
   GLAI_ASSERT(a->num_bits == 0);
   // now fill header the normal way
   while (k < 4)
      header[k++] = GLA__zget8(a);
   len  = header[1] * 256 + header[0];
   nlen = header[3] * 256 + header[2];
   if (nlen != (len ^ 0xffff)) return GLA__err("zlib corrupt","Corrupt PNG");
   if (a->zbuffer + len > a->zbuffer_end) return GLA__err("read past buffer","Corrupt PNG");
   if (a->zout + len > a->zout_end)
      if (!GLA__zexpand(a, len)) return 0;
   memcpy(a->zout, a->zbuffer, len);
   a->zbuffer += len;
   a->zout += len;
   return 1;
}

static int GLA__parse_zlib_header(GLA__zbuf *a)
{
   int cmf   = GLA__zget8(a);
   int cm    = cmf & 15;
   /* int cinfo = cmf >> 4; */
   int flg   = GLA__zget8(a);
   if ((cmf*256+flg) % 31 != 0) return GLA__err("bad zlib header","Corrupt PNG"); // zlib spec
   if (flg & 32) return GLA__err("no preset dict","Corrupt PNG"); // preset dictionary not allowed in png
   if (cm != 8) return GLA__err("bad compression","Corrupt PNG"); // DEFLATE required for png
   // window = 1 << (8 + cinfo)... but who cares, we fully buffer output
   return 1;
}

// @TODO: should statically initialize these for optimal thread safety
//static GLA_uc GLA__zdefault_length[288], GLA__zdefault_distance[32];

static GLA_uc* GLA__zdefault_length()
{
    static GLA_uc v[288];
    return v;
}

static GLA_uc* GLA__zdefault_distance()
{
    static GLA_uc v[288];
    return v;
}


static void GLA__init_zdefaults(void)
{
   int i;   // use <= to match clearly with spec
   for (i=0; i <= 143; ++i)     GLA__zdefault_length()[i]   = 8;
   for (   ; i <= 255; ++i)     GLA__zdefault_length()[i]   = 9;
   for (   ; i <= 279; ++i)     GLA__zdefault_length()[i]   = 7;
   for (   ; i <= 287; ++i)     GLA__zdefault_length()[i]   = 8;

   for (i=0; i <=  31; ++i)     GLA__zdefault_distance()[i] = 5;
}

static int GLA__parse_zlib(GLA__zbuf *a, int parse_header)
{
   int final, type;
   if (parse_header)
      if (!GLA__parse_zlib_header(a)) return 0;
   a->num_bits = 0;
   a->code_buffer = 0;
   do {
      final = GLA__zreceive(a,1);
      type = GLA__zreceive(a,2);
      if (type == 0) {
         if (!GLA__parse_uncomperssed_block(a)) return 0;
      } else if (type == 3) {
         return 0;
      } else {
         if (type == 1) {
            // use fixed code lengths
            if (!GLA__zdefault_distance()[31]) GLA__init_zdefaults();
            if (!GLA__zbuild_huffman(&a->z_length  , GLA__zdefault_length()  , 288)) return 0;
            if (!GLA__zbuild_huffman(&a->z_distance, GLA__zdefault_distance(),  32)) return 0;
         } else {
            if (!GLA__compute_huffman_codes(a)) return 0;
         }
         if (!GLA__parse_huffman_block(a)) return 0;
      }
   } while (!final);
   return 1;
}

static int GLA__do_zlib(GLA__zbuf *a, char *obuf, int olen, int exp, int parse_header)
{
   a->zout_start = obuf;
   a->zout       = obuf;
   a->zout_end   = obuf + olen;
   a->z_expandable = exp;

   return GLA__parse_zlib(a, parse_header);
}

GLAIDEF char *GLA_zlib_decode_malloc_guesssize(const char *buffer, int len, int initial_size, int *outlen)
{
   GLA__zbuf a;
   char *p = (char *) GLA__malloc(initial_size);
   if (p == NULL) return NULL;
   a.zbuffer = (GLA_uc *) buffer;
   a.zbuffer_end = (GLA_uc *) buffer + len;
   if (GLA__do_zlib(&a, p, initial_size, 1, 1)) {
      if (outlen) *outlen = (int) (a.zout - a.zout_start);
      return a.zout_start;
   } else {
      free(a.zout_start);
      return NULL;
   }
}

GLAIDEF char *GLA_zlib_decode_malloc(char const *buffer, int len, int *outlen)
{
   return GLA_zlib_decode_malloc_guesssize(buffer, len, 16384, outlen);
}

GLAIDEF char *GLA_zlib_decode_malloc_guesssize_headerflag(const char *buffer, int len, int initial_size, int *outlen, int parse_header)
{
   GLA__zbuf a;
   char *p = (char *) GLA__malloc(initial_size);
   if (p == NULL) return NULL;
   a.zbuffer = (GLA_uc *) buffer;
   a.zbuffer_end = (GLA_uc *) buffer + len;
   if (GLA__do_zlib(&a, p, initial_size, 1, parse_header)) {
      if (outlen) *outlen = (int) (a.zout - a.zout_start);
      return a.zout_start;
   } else {
      free(a.zout_start);
      return NULL;
   }
}

GLAIDEF int GLA_zlib_decode_buffer(char *obuffer, int olen, char const *ibuffer, int ilen)
{
   GLA__zbuf a;
   a.zbuffer = (GLA_uc *) ibuffer;
   a.zbuffer_end = (GLA_uc *) ibuffer + ilen;
   if (GLA__do_zlib(&a, obuffer, olen, 0, 1))
      return (int) (a.zout - a.zout_start);
   else
      return -1;
}

GLAIDEF char *GLA_zlib_decode_noheader_malloc(char const *buffer, int len, int *outlen)
{
   GLA__zbuf a;
   char *p = (char *) GLA__malloc(16384);
   if (p == NULL) return NULL;
   a.zbuffer = (GLA_uc *) buffer;
   a.zbuffer_end = (GLA_uc *) buffer+len;
   if (GLA__do_zlib(&a, p, 16384, 1, 0)) {
      if (outlen) *outlen = (int) (a.zout - a.zout_start);
      return a.zout_start;
   } else {
      free(a.zout_start);
      return NULL;
   }
}

GLAIDEF int GLA_zlib_decode_noheader_buffer(char *obuffer, int olen, const char *ibuffer, int ilen)
{
   GLA__zbuf a;
   a.zbuffer = (GLA_uc *) ibuffer;
   a.zbuffer_end = (GLA_uc *) ibuffer + ilen;
   if (GLA__do_zlib(&a, obuffer, olen, 0, 0))
      return (int) (a.zout - a.zout_start);
   else
      return -1;
}

// public domain "baseline" PNG decoder   v0.10  Sean Barrett 2006-11-18
//    simple implementation
//      - only 8-bit samples
//      - no CRC checking
//      - allocates lots of intermediate memory
//        - avoids problem of streaming data between subsystems
//        - avoids explicit window management
//    performance
//      - uses stb_zlib, a PD zlib implementation with fast huffman decoding


typedef struct
{
   GLA__uint32 length;
   GLA__uint32 type;
} GLA__pngchunk;

#define PNG_TYPE(a,b,c,d)  (((a) << 24) + ((b) << 16) + ((c) << 8) + (d))

static GLA__pngchunk GLA__get_chunk_header(GLA__context *s)
{
   GLA__pngchunk c;
   c.length = GLA__get32be(s);
   c.type   = GLA__get32be(s);
   return c;
}

static int GLA__check_png_header(GLA__context *s)
{
   static GLA_uc png_sig[8] = { 137,80,78,71,13,10,26,10 };
   int i;
   for (i=0; i < 8; ++i)
      if (GLA__get8(s) != png_sig[i]) return GLA__err("bad png sig","Not a PNG");
   return 1;
}

typedef struct
{
   GLA__context *s;
   GLA_uc *idata, *expanded, *out;
} GLA__png;


enum {
   GLAI__F_none=0, GLAI__F_sub=1, GLAI__F_up=2, GLAI__F_avg=3, GLAI__F_paeth=4,
   GLAI__F_avg_first, GLAI__F_paeth_first
};



static int GLA__paeth(int a, int b, int c)
{
   int p = a + b - c;
   int pa = abs(p-a);
   int pb = abs(p-b);
   int pc = abs(p-c);
   if (pa <= pb && pa <= pc) return a;
   if (pb <= pc) return b;
   return c;
}

#define GLAI__BYTECAST(x)  ((GLA_uc) ((x) & 255))  // truncate int to byte without warnings

// create the png data from post-deflated data
static int GLA__create_png_image_raw(GLA__png *a, GLA_uc *raw, GLA__uint32 raw_len, int out_n, GLA__uint32 x, GLA__uint32 y)
{
    static GLA_uc first_row_filter[5] =
    {
       GLAI__F_none, GLAI__F_sub, GLAI__F_none, GLAI__F_avg_first, GLAI__F_paeth_first
    };

   GLA__context *s = a->s;
   GLA__uint32 i,j,stride = x*out_n;
   int k;
   int img_n = s->img_n; // copy it into a local for later
   GLAI_ASSERT(out_n == s->img_n || out_n == s->img_n+1);
   a->out = (GLA_uc *) GLA__malloc(x * y * out_n);
   if (!a->out) return GLA__err("outofmem", "Out of memory");
   if (s->img_x == x && s->img_y == y) {
      if (raw_len != (img_n * x + 1) * y) return GLA__err("not enough pixels","Corrupt PNG");
   } else { // interlaced:
      if (raw_len < (img_n * x + 1) * y) return GLA__err("not enough pixels","Corrupt PNG");
   }
   for (j=0; j < y; ++j) {
      GLA_uc *cur = a->out + stride*j;
      GLA_uc *prior = cur - stride;
      int filter = *raw++;
      if (filter > 4) return GLA__err("invalid filter","Corrupt PNG");
      // if first row, use special filter that doesn't sample previous row
      if (j == 0) filter = first_row_filter[filter];
      // handle first pixel explicitly
      for (k=0; k < img_n; ++k) {
         switch (filter) {
            case GLAI__F_none       : cur[k] = raw[k]; break;
            case GLAI__F_sub        : cur[k] = raw[k]; break;
            case GLAI__F_up         : cur[k] = GLAI__BYTECAST(raw[k] + prior[k]); break;
            case GLAI__F_avg        : cur[k] = GLAI__BYTECAST(raw[k] + (prior[k]>>1)); break;
            case GLAI__F_paeth      : cur[k] = GLAI__BYTECAST(raw[k] + GLA__paeth(0,prior[k],0)); break;
            case GLAI__F_avg_first  : cur[k] = raw[k]; break;
            case GLAI__F_paeth_first: cur[k] = raw[k]; break;
         }
      }
      if (img_n != out_n) cur[img_n] = 255;
      raw += img_n;
      cur += out_n;
      prior += out_n;
      // this is a little gross, so that we don't switch per-pixel or per-component
      if (img_n == out_n) {
         #define CASE(f) \
             case f:     \
                for (i=x-1; i >= 1; --i, raw+=img_n,cur+=img_n,prior+=img_n) \
                   for (k=0; k < img_n; ++k)
         switch (filter) {
            CASE(GLAI__F_none)         cur[k] = raw[k]; break;
            CASE(GLAI__F_sub)          cur[k] = GLAI__BYTECAST(raw[k] + cur[k-img_n]); break;
            CASE(GLAI__F_up)           cur[k] = GLAI__BYTECAST(raw[k] + prior[k]); break;
            CASE(GLAI__F_avg)          cur[k] = GLAI__BYTECAST(raw[k] + ((prior[k] + cur[k-img_n])>>1)); break;
            CASE(GLAI__F_paeth)        cur[k] = GLAI__BYTECAST(raw[k] + GLA__paeth(cur[k-img_n],prior[k],prior[k-img_n])); break;
            CASE(GLAI__F_avg_first)    cur[k] = GLAI__BYTECAST(raw[k] + (cur[k-img_n] >> 1)); break;
            CASE(GLAI__F_paeth_first)  cur[k] = GLAI__BYTECAST(raw[k] + GLA__paeth(cur[k-img_n],0,0)); break;
         }
         #undef CASE
      } else {
         GLAI_ASSERT(img_n+1 == out_n);
         #define CASE(f) \
             case f:     \
                for (i=x-1; i >= 1; --i, cur[img_n]=255,raw+=img_n,cur+=out_n,prior+=out_n) \
                   for (k=0; k < img_n; ++k)
         switch (filter) {
            CASE(GLAI__F_none)         cur[k] = raw[k]; break;
            CASE(GLAI__F_sub)          cur[k] = GLAI__BYTECAST(raw[k] + cur[k-out_n]); break;
            CASE(GLAI__F_up)           cur[k] = GLAI__BYTECAST(raw[k] + prior[k]); break;
            CASE(GLAI__F_avg)          cur[k] = GLAI__BYTECAST(raw[k] + ((prior[k] + cur[k-out_n])>>1)); break;
            CASE(GLAI__F_paeth)        cur[k] = GLAI__BYTECAST(raw[k] + GLA__paeth(cur[k-out_n],prior[k],prior[k-out_n])); break;
            CASE(GLAI__F_avg_first)    cur[k] = GLAI__BYTECAST(raw[k] + (cur[k-out_n] >> 1)); break;
            CASE(GLAI__F_paeth_first)  cur[k] = GLAI__BYTECAST(raw[k] + GLA__paeth(cur[k-out_n],0,0)); break;
         }
         #undef CASE
      }
   }
   return 1;
}

static int GLA__create_png_image(GLA__png *a, GLA_uc *raw, GLA__uint32 raw_len, int out_n, int interlaced)
{
   GLA_uc *final;
   int p;
   if (!interlaced)
      return GLA__create_png_image_raw(a, raw, raw_len, out_n, a->s->img_x, a->s->img_y);

   // de-interlacing
   final = (GLA_uc *) GLA__malloc(a->s->img_x * a->s->img_y * out_n);
   for (p=0; p < 7; ++p) {
      int xorig[] = { 0,4,0,2,0,1,0 };
      int yorig[] = { 0,0,4,0,2,0,1 };
      int xspc[]  = { 8,8,4,4,2,2,1 };
      int yspc[]  = { 8,8,8,4,4,2,2 };
      int i,j,x,y;
      // pass1_x[4] = 0, pass1_x[5] = 1, pass1_x[12] = 1
      x = (a->s->img_x - xorig[p] + xspc[p]-1) / xspc[p];
      y = (a->s->img_y - yorig[p] + yspc[p]-1) / yspc[p];
      if (x && y) {
         if (!GLA__create_png_image_raw(a, raw, raw_len, out_n, x, y)) {
            free(final);
            return 0;
         }
         for (j=0; j < y; ++j)
            for (i=0; i < x; ++i)
               memcpy(final + (j*yspc[p]+yorig[p])*a->s->img_x*out_n + (i*xspc[p]+xorig[p])*out_n,
                      a->out + (j*x+i)*out_n, out_n);
         free(a->out);
         raw += (x*out_n+1)*y;
         raw_len -= (x*out_n+1)*y;
      }
   }
   a->out = final;

   return 1;
}

static int GLA__compute_transparency(GLA__png *z, GLA_uc tc[3], int out_n)
{
   GLA__context *s = z->s;
   GLA__uint32 i, pixel_count = s->img_x * s->img_y;
   GLA_uc *p = z->out;

   // compute color-based transparency, assuming we've
   // already got 255 as the alpha value in the output
   GLAI_ASSERT(out_n == 2 || out_n == 4);

   if (out_n == 2) {
      for (i=0; i < pixel_count; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 255);
         p += 2;
      }
   } else {
      for (i=0; i < pixel_count; ++i) {
         if (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   return 1;
}

static int GLA__expand_png_palette(GLA__png *a, GLA_uc *palette, int len, int pal_img_n)
{
   GLA__uint32 i, pixel_count = a->s->img_x * a->s->img_y;
   GLA_uc *p, *temp_out, *orig = a->out;

   p = (GLA_uc *) GLA__malloc(pixel_count * pal_img_n);
   if (p == NULL) return GLA__err("outofmem", "Out of memory");

   // between here and free(out) below, exitting would leak
   temp_out = p;

   if (pal_img_n == 3) {
      for (i=0; i < pixel_count; ++i) {
         int n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p += 3;
      }
   } else {
      for (i=0; i < pixel_count; ++i) {
         int n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p[3] = palette[n+3];
         p += 4;
      }
   }
   free(a->out);
   a->out = temp_out;

   GLAI_NOTUSED(len);

   return 1;
}

//static int GLA__unpremultiply_on_load = 0;
//static int GLA__de_iphone_flag        = 0;

static int GLA__unpremultiply_on_load(bool SET=false, int value=0)
{
    static int v = 0;
    if(SET) v = value;
    return v;
}

static int GLA__de_iphone_flag(bool SET=false, int value=0)
{
    static int v = 0;
    if(SET) v = value;
    return v;
}

GLAIDEF void GLA_set_unpremultiply_on_load(int flag_true_if_should_unpremultiply)
{
   GLA__unpremultiply_on_load(true, flag_true_if_should_unpremultiply);
}

GLAIDEF void GLA_convert_iphone_png_to_rgb(int flag_true_if_should_convert)
{
   GLA__de_iphone_flag(true, flag_true_if_should_convert);
}

static void GLA__de_iphone(GLA__png *z)
{
   GLA__context *s = z->s;
   GLA__uint32 i, pixel_count = s->img_x * s->img_y;
   GLA_uc *p = z->out;

   if (s->img_out_n == 3) {  // convert bgr to rgb
      for (i=0; i < pixel_count; ++i) {
         GLA_uc t = p[0];
         p[0] = p[2];
         p[2] = t;
         p += 3;
      }
   } else {
      GLAI_ASSERT(s->img_out_n == 4);
      if (GLA__unpremultiply_on_load()) {
         // convert bgr to rgb and unpremultiply
         for (i=0; i < pixel_count; ++i) {
            GLA_uc a = p[3];
            GLA_uc t = p[0];
            if (a) {
               p[0] = p[2] * 255 / a;
               p[1] = p[1] * 255 / a;
               p[2] =  t   * 255 / a;
            } else {
               p[0] = p[2];
               p[2] = t;
            } 
            p += 4;
         }
      } else {
         // convert bgr to rgb
         for (i=0; i < pixel_count; ++i) {
            GLA_uc t = p[0];
            p[0] = p[2];
            p[2] = t;
            p += 4;
         }
      }
   }
}

static int GLA__parse_png_file(GLA__png *z, int scan, int req_comp)
{
   GLA_uc palette[1024], pal_img_n=0;
   GLA_uc has_trans=0, tc[3];
   GLA__uint32 ioff=0, idata_limit=0, i, pal_len=0;
   int first=1,k,interlace=0, is_iphone=0;
   GLA__context *s = z->s;

   z->expanded = NULL;
   z->idata    = NULL;
   z->out      = NULL;

   if (!GLA__check_png_header(s)) return 0;

   if (scan == SCAN_type) return 1;

   for (;;) {
      GLA__pngchunk c = GLA__get_chunk_header(s);
      switch (c.type) {
         case PNG_TYPE('C','g','B','I'):
            is_iphone = 1;
            GLA__skip(s, c.length);
            break;
         case PNG_TYPE('I','H','D','R'): {
            int depth,color,comp,filter;
            if (!first) return GLA__err("multiple IHDR","Corrupt PNG");
            first = 0;
            if (c.length != 13) return GLA__err("bad IHDR len","Corrupt PNG");
            s->img_x = GLA__get32be(s); if (s->img_x > (1 << 24)) return GLA__err("too large","Very large image (corrupt?)");
            s->img_y = GLA__get32be(s); if (s->img_y > (1 << 24)) return GLA__err("too large","Very large image (corrupt?)");
            depth = GLA__get8(s);  if (depth != 8)        return GLA__err("8bit only","PNG not supported: 8-bit only");
            color = GLA__get8(s);  if (color > 6)         return GLA__err("bad ctype","Corrupt PNG");
            if (color == 3) pal_img_n = 3; else if (color & 1) return GLA__err("bad ctype","Corrupt PNG");
            comp  = GLA__get8(s);  if (comp) return GLA__err("bad comp method","Corrupt PNG");
            filter= GLA__get8(s);  if (filter) return GLA__err("bad filter method","Corrupt PNG");
            interlace = GLA__get8(s); if (interlace>1) return GLA__err("bad interlace method","Corrupt PNG");
            if (!s->img_x || !s->img_y) return GLA__err("0-pixel image","Corrupt PNG");
            if (!pal_img_n) {
               s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
               if ((1 << 30) / s->img_x / s->img_n < s->img_y) return GLA__err("too large", "Image too large to decode");
               if (scan == SCAN_header) return 1;
            } else {
               // if paletted, then pal_n is our final components, and
               // img_n is # components to decompress/filter.
               s->img_n = 1;
               if ((1 << 30) / s->img_x / 4 < s->img_y) return GLA__err("too large","Corrupt PNG");
               // if SCAN_header, have to scan to see if we have a tRNS
            }
            break;
         }

         case PNG_TYPE('P','L','T','E'):  {
            if (first) return GLA__err("first not IHDR", "Corrupt PNG");
            if (c.length > 256*3) return GLA__err("invalid PLTE","Corrupt PNG");
            pal_len = c.length / 3;
            if (pal_len * 3 != c.length) return GLA__err("invalid PLTE","Corrupt PNG");
            for (i=0; i < pal_len; ++i) {
               palette[i*4+0] = GLA__get8(s);
               palette[i*4+1] = GLA__get8(s);
               palette[i*4+2] = GLA__get8(s);
               palette[i*4+3] = 255;
            }
            break;
         }

         case PNG_TYPE('t','R','N','S'): {
            if (first) return GLA__err("first not IHDR", "Corrupt PNG");
            if (z->idata) return GLA__err("tRNS after IDAT","Corrupt PNG");
            if (pal_img_n) {
               if (scan == SCAN_header) { s->img_n = 4; return 1; }
               if (pal_len == 0) return GLA__err("tRNS before PLTE","Corrupt PNG");
               if (c.length > pal_len) return GLA__err("bad tRNS len","Corrupt PNG");
               pal_img_n = 4;
               for (i=0; i < c.length; ++i)
                  palette[i*4+3] = GLA__get8(s);
            } else {
               if (!(s->img_n & 1)) return GLA__err("tRNS with alpha","Corrupt PNG");
               if (c.length != (GLA__uint32) s->img_n*2) return GLA__err("bad tRNS len","Corrupt PNG");
               has_trans = 1;
               for (k=0; k < s->img_n; ++k)
                  tc[k] = (GLA_uc) (GLA__get16be(s) & 255); // non 8-bit images will be larger
            }
            break;
         }

         case PNG_TYPE('I','D','A','T'): {
            if (first) return GLA__err("first not IHDR", "Corrupt PNG");
            if (pal_img_n && !pal_len) return GLA__err("no PLTE","Corrupt PNG");
            if (scan == SCAN_header) { s->img_n = pal_img_n; return 1; }
            if (ioff + c.length > idata_limit) {
               GLA_uc *p;
               if (idata_limit == 0) idata_limit = c.length > 4096 ? c.length : 4096;
               while (ioff + c.length > idata_limit)
                  idata_limit *= 2;
               p = (GLA_uc *) realloc(z->idata, idata_limit); if (p == NULL) return GLA__err("outofmem", "Out of memory");
               z->idata = p;
            }
            if (!GLA__getn(s, z->idata+ioff,c.length)) return GLA__err("outofdata","Corrupt PNG");
            ioff += c.length;
            break;
         }

         case PNG_TYPE('I','E','N','D'): {
            GLA__uint32 raw_len;
            if (first) return GLA__err("first not IHDR", "Corrupt PNG");
            if (scan != SCAN_load) return 1;
            if (z->idata == NULL) return GLA__err("no IDAT","Corrupt PNG");
            z->expanded = (GLA_uc *) GLA_zlib_decode_malloc_guesssize_headerflag((char *) z->idata, ioff, 16384, (int *) &raw_len, !is_iphone);
            if (z->expanded == NULL) return 0; // zlib should set error
            free(z->idata); z->idata = NULL;
            if ((req_comp == s->img_n+1 && req_comp != 3 && !pal_img_n) || has_trans)
               s->img_out_n = s->img_n+1;
            else
               s->img_out_n = s->img_n;
            if (!GLA__create_png_image(z, z->expanded, raw_len, s->img_out_n, interlace)) return 0;
            if (has_trans)
               if (!GLA__compute_transparency(z, tc, s->img_out_n)) return 0;
            if (is_iphone && GLA__de_iphone_flag && s->img_out_n > 2)
               GLA__de_iphone(z);
            if (pal_img_n) {
               // pal_img_n == 3 or 4
               s->img_n = pal_img_n; // record the actual colors we had
               s->img_out_n = pal_img_n;
               if (req_comp >= 3) s->img_out_n = req_comp;
               if (!GLA__expand_png_palette(z, palette, pal_len, s->img_out_n))
                  return 0;
            }
            free(z->expanded); z->expanded = NULL;
            return 1;
         }

         default:
            // if critical, fail
            if (first) return GLA__err("first not IHDR", "Corrupt PNG");
            if ((c.type & (1 << 29)) == 0) {
               #ifndef GLAI_NO_FAILURE_STRINGS
               // not threadsafe
               static char invalid_chunk[] = "XXXX PNG chunk not known";
               invalid_chunk[0] = GLAI__BYTECAST(c.type >> 24);
               invalid_chunk[1] = GLAI__BYTECAST(c.type >> 16);
               invalid_chunk[2] = GLAI__BYTECAST(c.type >>  8);
               invalid_chunk[3] = GLAI__BYTECAST(c.type >>  0);
               #endif
               return GLA__err(invalid_chunk, "PNG not supported: unknown PNG chunk type");
            }
            GLA__skip(s, c.length);
            break;
      }
      // end of PNG chunk, read and skip CRC
      GLA__get32be(s);
   }
}

static unsigned char *GLA__do_png(GLA__png *p, int *x, int *y, int *n, int req_comp)
{
   unsigned char *result=NULL;
   if (req_comp < 0 || req_comp > 4) return GLA__errpuc("bad req_comp", "Internal error");
   if (GLA__parse_png_file(p, SCAN_load, req_comp)) {
      result = p->out;
      p->out = NULL;
      if (req_comp && req_comp != p->s->img_out_n) {
         result = GLA__convert_format(result, p->s->img_out_n, req_comp, p->s->img_x, p->s->img_y);
         p->s->img_out_n = req_comp;
         if (result == NULL) return result;
      }
      *x = p->s->img_x;
      *y = p->s->img_y;
      if (n) *n = p->s->img_out_n;
   }
   free(p->out);      p->out      = NULL;
   free(p->expanded); p->expanded = NULL;
   free(p->idata);    p->idata    = NULL;

   return result;
}

static unsigned char *GLA__png_load(GLA__context *s, int *x, int *y, int *comp, int req_comp)
{
   GLA__png p;
   p.s = s;
   return GLA__do_png(&p, x,y,comp,req_comp);
}

static int GLA__png_test(GLA__context *s)
{
   int r;
   r = GLA__check_png_header(s);
   GLA__rewind(s);
   return r;
}

static int GLA__png_info_raw(GLA__png *p, int *x, int *y, int *comp)
{
   if (!GLA__parse_png_file(p, SCAN_header, 0)) {
      GLA__rewind( p->s );
      return 0;
   }
   if (x) *x = p->s->img_x;
   if (y) *y = p->s->img_y;
   if (comp) *comp = p->s->img_n;
   return 1;
}

static int GLA__png_info(GLA__context *s, int *x, int *y, int *comp)
{
   GLA__png p;
   p.s = s;
   return GLA__png_info_raw(&p, x, y, comp);
}

// Microsoft/Windows BMP image
static int GLA__bmp_test_raw(GLA__context *s)
{
   int r;
   int sz;
   if (GLA__get8(s) != 'B') return 0;
   if (GLA__get8(s) != 'M') return 0;
   GLA__get32le(s); // discard filesize
   GLA__get16le(s); // discard reserved
   GLA__get16le(s); // discard reserved
   GLA__get32le(s); // discard data offset
   sz = GLA__get32le(s);
   r = (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);
   return r;
}

static int GLA__bmp_test(GLA__context *s)
{
   int r = GLA__bmp_test_raw(s);
   GLA__rewind(s);
   return r;
}


// returns 0..31 for the highest set bit
static int GLA__high_bit(unsigned int z)
{
   int n=0;
   if (z == 0) return -1;
   if (z >= 0x10000) n += 16, z >>= 16;
   if (z >= 0x00100) n +=  8, z >>=  8;
   if (z >= 0x00010) n +=  4, z >>=  4;
   if (z >= 0x00004) n +=  2, z >>=  2;
   if (z >= 0x00002) n +=  1, z >>=  1;
   return n;
}

static int GLA__bitcount(unsigned int a)
{
   a = (a & 0x55555555) + ((a >>  1) & 0x55555555); // max 2
   a = (a & 0x33333333) + ((a >>  2) & 0x33333333); // max 4
   a = (a + (a >> 4)) & 0x0f0f0f0f; // max 8 per 4, now 8 bits
   a = (a + (a >> 8)); // max 16 per 8 bits
   a = (a + (a >> 16)); // max 32 per 8 bits
   return a & 0xff;
}

static int GLA__shiftsigned(int v, int shift, int bits)
{
   int result;
   int z=0;

   if (shift < 0) v <<= -shift;
   else v >>= shift;
   result = v;

   z = bits;
   while (z < 8) {
      result += v >> z;
      z += bits;
   }
   return result;
}

static GLA_uc *GLA__bmp_load(GLA__context *s, int *x, int *y, int *comp, int req_comp)
{
   GLA_uc *out;
   unsigned int mr=0,mg=0,mb=0,ma=0, fake_a=0;
   GLA_uc pal[256][4];
   int psize=0,i,j,compress=0,width;
   int bpp, flip_vertically, pad, target, offset, hsz;
   if (GLA__get8(s) != 'B' || GLA__get8(s) != 'M') return GLA__errpuc("not BMP", "Corrupt BMP");
   GLA__get32le(s); // discard filesize
   GLA__get16le(s); // discard reserved
   GLA__get16le(s); // discard reserved
   offset = GLA__get32le(s);
   hsz = GLA__get32le(s);
   if (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) return GLA__errpuc("unknown BMP", "BMP type not supported: unknown");
   if (hsz == 12) {
      s->img_x = GLA__get16le(s);
      s->img_y = GLA__get16le(s);
   } else {
      s->img_x = GLA__get32le(s);
      s->img_y = GLA__get32le(s);
   }
   if (GLA__get16le(s) != 1) return GLA__errpuc("bad BMP", "bad BMP");
   bpp = GLA__get16le(s);
   if (bpp == 1) return GLA__errpuc("monochrome", "BMP type not supported: 1-bit");
   flip_vertically = ((int) s->img_y) > 0;
   s->img_y = abs((int) s->img_y);
   if (hsz == 12) {
      if (bpp < 24)
         psize = (offset - 14 - 24) / 3;
   } else {
      compress = GLA__get32le(s);
      if (compress == 1 || compress == 2) return GLA__errpuc("BMP RLE", "BMP type not supported: RLE");
      GLA__get32le(s); // discard sizeof
      GLA__get32le(s); // discard hres
      GLA__get32le(s); // discard vres
      GLA__get32le(s); // discard colorsused
      GLA__get32le(s); // discard max important
      if (hsz == 40 || hsz == 56) {
         if (hsz == 56) {
            GLA__get32le(s);
            GLA__get32le(s);
            GLA__get32le(s);
            GLA__get32le(s);
         }
         if (bpp == 16 || bpp == 32) {
            mr = mg = mb = 0;
            if (compress == 0) {
               if (bpp == 32) {
                  mr = 0xffu << 16;
                  mg = 0xffu <<  8;
                  mb = 0xffu <<  0;
                  ma = 0xffu << 24;
                  fake_a = 1; // @TODO: check for cases like alpha value is all 0 and switch it to 255
                  GLAI_NOTUSED(fake_a);
               } else {
                  mr = 31u << 10;
                  mg = 31u <<  5;
                  mb = 31u <<  0;
               }
            } else if (compress == 3) {
               mr = GLA__get32le(s);
               mg = GLA__get32le(s);
               mb = GLA__get32le(s);
               // not documented, but generated by photoshop and handled by mspaint
               if (mr == mg && mg == mb) {
                  // ?!?!?
                  return GLA__errpuc("bad BMP", "bad BMP");
               }
            } else
               return GLA__errpuc("bad BMP", "bad BMP");
         }
      } else {
         GLAI_ASSERT(hsz == 108 || hsz == 124);
         mr = GLA__get32le(s);
         mg = GLA__get32le(s);
         mb = GLA__get32le(s);
         ma = GLA__get32le(s);
         GLA__get32le(s); // discard color space
         for (i=0; i < 12; ++i)
            GLA__get32le(s); // discard color space parameters
         if (hsz == 124) {
            GLA__get32le(s); // discard rendering intent
            GLA__get32le(s); // discard offset of profile data
            GLA__get32le(s); // discard size of profile data
            GLA__get32le(s); // discard reserved
         }
      }
      if (bpp < 16)
         psize = (offset - 14 - hsz) >> 2;
   }
   s->img_n = ma ? 4 : 3;
   if (req_comp && req_comp >= 3) // we can directly decode 3 or 4
      target = req_comp;
   else
      target = s->img_n; // if they want monochrome, we'll post-convert
   out = (GLA_uc *) GLA__malloc(target * s->img_x * s->img_y);
   if (!out) return GLA__errpuc("outofmem", "Out of memory");
   if (bpp < 16) {
      int z=0;
      if (psize == 0 || psize > 256) { free(out); return GLA__errpuc("invalid", "Corrupt BMP"); }
      for (i=0; i < psize; ++i) {
         pal[i][2] = GLA__get8(s);
         pal[i][1] = GLA__get8(s);
         pal[i][0] = GLA__get8(s);
         if (hsz != 12) GLA__get8(s);
         pal[i][3] = 255;
      }
      GLA__skip(s, offset - 14 - hsz - psize * (hsz == 12 ? 3 : 4));
      if (bpp == 4) width = (s->img_x + 1) >> 1;
      else if (bpp == 8) width = s->img_x;
      else { free(out); return GLA__errpuc("bad bpp", "Corrupt BMP"); }
      pad = (-width)&3;
      for (j=0; j < (int) s->img_y; ++j) {
         for (i=0; i < (int) s->img_x; i += 2) {
            int v=GLA__get8(s),v2=0;
            if (bpp == 4) {
               v2 = v & 15;
               v >>= 4;
            }
            out[z++] = pal[v][0];
            out[z++] = pal[v][1];
            out[z++] = pal[v][2];
            if (target == 4) out[z++] = 255;
            if (i+1 == (int) s->img_x) break;
            v = (bpp == 8) ? GLA__get8(s) : v2;
            out[z++] = pal[v][0];
            out[z++] = pal[v][1];
            out[z++] = pal[v][2];
            if (target == 4) out[z++] = 255;
         }
         GLA__skip(s, pad);
      }
   } else {
      int rshift=0,gshift=0,bshift=0,ashift=0,rcount=0,gcount=0,bcount=0,acount=0;
      int z = 0;
      int easy=0;
      GLA__skip(s, offset - 14 - hsz);
      if (bpp == 24) width = 3 * s->img_x;
      else if (bpp == 16) width = 2*s->img_x;
      else /* bpp = 32 and pad = 0 */ width=0;
      pad = (-width) & 3;
      if (bpp == 24) {
         easy = 1;
      } else if (bpp == 32) {
         if (mb == 0xff && mg == 0xff00 && mr == 0x00ff0000 && ma == 0xff000000)
            easy = 2;
      }
      if (!easy) {
         if (!mr || !mg || !mb) { free(out); return GLA__errpuc("bad masks", "Corrupt BMP"); }
         // right shift amt to put high bit in position #7
         rshift = GLA__high_bit(mr)-7; rcount = GLA__bitcount(mr);
         gshift = GLA__high_bit(mg)-7; gcount = GLA__bitcount(mg);
         bshift = GLA__high_bit(mb)-7; bcount = GLA__bitcount(mb);
         ashift = GLA__high_bit(ma)-7; acount = GLA__bitcount(ma);
      }
      for (j=0; j < (int) s->img_y; ++j) {
         if (easy) {
            for (i=0; i < (int) s->img_x; ++i) {
               unsigned char a;
               out[z+2] = GLA__get8(s);
               out[z+1] = GLA__get8(s);
               out[z+0] = GLA__get8(s);
               z += 3;
               a = (easy == 2 ? GLA__get8(s) : 255);
               if (target == 4) out[z++] = a;
            }
         } else {
            for (i=0; i < (int) s->img_x; ++i) {
               GLA__uint32 v = (GLA__uint32) (bpp == 16 ? GLA__get16le(s) : GLA__get32le(s));
               int a;
               out[z++] = GLAI__BYTECAST(GLA__shiftsigned(v & mr, rshift, rcount));
               out[z++] = GLAI__BYTECAST(GLA__shiftsigned(v & mg, gshift, gcount));
               out[z++] = GLAI__BYTECAST(GLA__shiftsigned(v & mb, bshift, bcount));
               a = (ma ? GLA__shiftsigned(v & ma, ashift, acount) : 255);
               if (target == 4) out[z++] = GLAI__BYTECAST(a);
            }
         }
         GLA__skip(s, pad);
      }
   }
   if (flip_vertically) {
      GLA_uc t;
      for (j=0; j < (int) s->img_y>>1; ++j) {
         GLA_uc *p1 = out +      j     *s->img_x*target;
         GLA_uc *p2 = out + (s->img_y-1-j)*s->img_x*target;
         for (i=0; i < (int) s->img_x*target; ++i) {
            t = p1[i], p1[i] = p2[i], p2[i] = t;
         }
      }
   }

   if (req_comp && req_comp != target) {
      out = GLA__convert_format(out, target, req_comp, s->img_x, s->img_y);
      if (out == NULL) return out; // GLA__convert_format frees input on failure
   }

   *x = s->img_x;
   *y = s->img_y;
   if (comp) *comp = s->img_n;
   return out;
}

// Targa Truevision - TGA
// by Jonathan Dummer

static int GLA__tga_info(GLA__context *s, int *x, int *y, int *comp)
{
    int tga_w, tga_h, tga_comp;
    int sz;
    GLA__get8(s);                   // discard Offset
    sz = GLA__get8(s);              // color type
    if( sz > 1 ) {
        GLA__rewind(s);
        return 0;      // only RGB or indexed allowed
    }
    sz = GLA__get8(s);              // image type
    // only RGB or grey allowed, +/- RLE
    if ((sz != 1) && (sz != 2) && (sz != 3) && (sz != 9) && (sz != 10) && (sz != 11)) return 0;
    GLA__skip(s,9);
    tga_w = GLA__get16le(s);
    if( tga_w < 1 ) {
        GLA__rewind(s);
        return 0;   // test width
    }
    tga_h = GLA__get16le(s);
    if( tga_h < 1 ) {
        GLA__rewind(s);
        return 0;   // test height
    }
    sz = GLA__get8(s);               // bits per pixel
    // only RGB or RGBA or grey allowed
    if ((sz != 8) && (sz != 16) && (sz != 24) && (sz != 32)) {
        GLA__rewind(s);
        return 0;
    }
    tga_comp = sz;
    if (x) *x = tga_w;
    if (y) *y = tga_h;
    if (comp) *comp = tga_comp / 8;
    return 1;                   // seems to have passed everything
}

static int GLA__tga_test(GLA__context *s)
{
   int res;
   int sz;
   GLA__get8(s);      //   discard Offset
   sz = GLA__get8(s);   //   color type
   if ( sz > 1 ) return 0;   //   only RGB or indexed allowed
   sz = GLA__get8(s);   //   image type
   if ( (sz != 1) && (sz != 2) && (sz != 3) && (sz != 9) && (sz != 10) && (sz != 11) ) return 0;   //   only RGB or grey allowed, +/- RLE
   GLA__get16be(s);      //   discard palette start
   GLA__get16be(s);      //   discard palette length
   GLA__get8(s);         //   discard bits per palette color entry
   GLA__get16be(s);      //   discard x origin
   GLA__get16be(s);      //   discard y origin
   if ( GLA__get16be(s) < 1 ) return 0;      //   test width
   if ( GLA__get16be(s) < 1 ) return 0;      //   test height
   sz = GLA__get8(s);   //   bits per pixel
   if ( (sz != 8) && (sz != 16) && (sz != 24) && (sz != 32) )
      res = 0;
   else
      res = 1;
   GLA__rewind(s);
   return res;
}

static GLA_uc *GLA__tga_load(GLA__context *s, int *x, int *y, int *comp, int req_comp)
{
   //   read in the TGA header stuff
   int tga_offset = GLA__get8(s);
   int tga_indexed = GLA__get8(s);
   int tga_image_type = GLA__get8(s);
   int tga_is_RLE = 0;
   int tga_palette_start = GLA__get16le(s);
   int tga_palette_len = GLA__get16le(s);
   int tga_palette_bits = GLA__get8(s);
   int tga_x_origin = GLA__get16le(s);
   int tga_y_origin = GLA__get16le(s);
   int tga_width = GLA__get16le(s);
   int tga_height = GLA__get16le(s);
   int tga_bits_per_pixel = GLA__get8(s);
   int tga_comp = tga_bits_per_pixel / 8;
   int tga_inverted = GLA__get8(s);
   //   image data
   unsigned char *tga_data;
   unsigned char *tga_palette = NULL;
   int i, j;
   unsigned char raw_data[4];
   int RLE_count = 0;
   int RLE_repeating = 0;
   int read_next_pixel = 1;

   //   do a tiny bit of precessing
   if ( tga_image_type >= 8 )
   {
      tga_image_type -= 8;
      tga_is_RLE = 1;
   }
   /* int tga_alpha_bits = tga_inverted & 15; */
   tga_inverted = 1 - ((tga_inverted >> 5) & 1);

   //   error check
   if ( //(tga_indexed) ||
      (tga_width < 1) || (tga_height < 1) ||
      (tga_image_type < 1) || (tga_image_type > 3) ||
      ((tga_bits_per_pixel != 8) && (tga_bits_per_pixel != 16) &&
      (tga_bits_per_pixel != 24) && (tga_bits_per_pixel != 32))
      )
   {
      return NULL; // we don't report this as a bad TGA because we don't even know if it's TGA
   }

   //   If I'm paletted, then I'll use the number of bits from the palette
   if ( tga_indexed )
   {
      tga_comp = tga_palette_bits / 8;
   }

   //   tga info
   *x = tga_width;
   *y = tga_height;
   if (comp) *comp = tga_comp;

   tga_data = (unsigned char*)GLA__malloc( tga_width * tga_height * tga_comp );
   if (!tga_data) return GLA__errpuc("outofmem", "Out of memory");

   // skip to the data's starting position (offset usually = 0)
   GLA__skip(s, tga_offset );

   if ( !tga_indexed && !tga_is_RLE) {
      for (i=0; i < tga_height; ++i) {
         int y = tga_inverted ? tga_height -i - 1 : i;
         GLA_uc *tga_row = tga_data + y*tga_width*tga_comp;
         GLA__getn(s, tga_row, tga_width * tga_comp);
      }
   } else  {
      //   do I need to load a palette?
      if ( tga_indexed)
      {
         //   any data to skip? (offset usually = 0)
         GLA__skip(s, tga_palette_start );
         //   load the palette
         tga_palette = (unsigned char*)GLA__malloc( tga_palette_len * tga_palette_bits / 8 );
         if (!tga_palette) {
            free(tga_data);
            return GLA__errpuc("outofmem", "Out of memory");
         }
         if (!GLA__getn(s, tga_palette, tga_palette_len * tga_palette_bits / 8 )) {
            free(tga_data);
            free(tga_palette);
            return GLA__errpuc("bad palette", "Corrupt TGA");
         }
      }
      //   load the data
      for (i=0; i < tga_width * tga_height; ++i)
      {
         //   if I'm in RLE mode, do I need to get a RLE GLA__pngchunk?
         if ( tga_is_RLE )
         {
            if ( RLE_count == 0 )
            {
               //   yep, get the next byte as a RLE command
               int RLE_cmd = GLA__get8(s);
               RLE_count = 1 + (RLE_cmd & 127);
               RLE_repeating = RLE_cmd >> 7;
               read_next_pixel = 1;
            } else if ( !RLE_repeating )
            {
               read_next_pixel = 1;
            }
         } else
         {
            read_next_pixel = 1;
         }
         //   OK, if I need to read a pixel, do it now
         if ( read_next_pixel )
         {
            //   load however much data we did have
            if ( tga_indexed )
            {
               //   read in 1 byte, then perform the lookup
               int pal_idx = GLA__get8(s);
               if ( pal_idx >= tga_palette_len )
               {
                  //   invalid index
                  pal_idx = 0;
               }
               pal_idx *= tga_bits_per_pixel / 8;
               for (j = 0; j*8 < tga_bits_per_pixel; ++j)
               {
                  raw_data[j] = tga_palette[pal_idx+j];
               }
            } else
            {
               //   read in the data raw
               for (j = 0; j*8 < tga_bits_per_pixel; ++j)
               {
                  raw_data[j] = GLA__get8(s);
               }
            }
            //   clear the reading flag for the next pixel
            read_next_pixel = 0;
         } // end of reading a pixel

         // copy data
         for (j = 0; j < tga_comp; ++j)
           tga_data[i*tga_comp+j] = raw_data[j];

         //   in case we're in RLE mode, keep counting down
         --RLE_count;
      }
      //   do I need to invert the image?
      if ( tga_inverted )
      {
         for (j = 0; j*2 < tga_height; ++j)
         {
            int index1 = j * tga_width * tga_comp;
            int index2 = (tga_height - 1 - j) * tga_width * tga_comp;
            for (i = tga_width * tga_comp; i > 0; --i)
            {
               unsigned char temp = tga_data[index1];
               tga_data[index1] = tga_data[index2];
               tga_data[index2] = temp;
               ++index1;
               ++index2;
            }
         }
      }
      //   clear my palette, if I had one
      if ( tga_palette != NULL )
      {
         free( tga_palette );
      }
   }

   // swap RGB
   if (tga_comp >= 3)
   {
      unsigned char* tga_pixel = tga_data;
      for (i=0; i < tga_width * tga_height; ++i)
      {
         unsigned char temp = tga_pixel[0];
         tga_pixel[0] = tga_pixel[2];
         tga_pixel[2] = temp;
         tga_pixel += tga_comp;
      }
   }

   // convert to target component count
   if (req_comp && req_comp != tga_comp)
      tga_data = GLA__convert_format(tga_data, tga_comp, req_comp, tga_width, tga_height);

   //   the things I do to get rid of an error message, and yet keep
   //   Microsoft's C compilers happy... [8^(
   tga_palette_start = tga_palette_len = tga_palette_bits =
         tga_x_origin = tga_y_origin = 0;
   //   OK, done
   return tga_data;
}

// *************************************************************************************************
// Photoshop PSD loader -- PD by Thatcher Ulrich, integration by Nicolas Schulz, tweaked by GLA

static int GLA__psd_test(GLA__context *s)
{
   int r = (GLA__get32be(s) == 0x38425053);
   GLA__rewind(s);
   return r;
}

static GLA_uc *GLA__psd_load(GLA__context *s, int *x, int *y, int *comp, int req_comp)
{
   int   pixelCount;
   int channelCount, compression;
   int channel, i, count, len;
   int w,h;
   GLA_uc *out;

   // Check identifier
   if (GLA__get32be(s) != 0x38425053)   // "8BPS"
      return GLA__errpuc("not PSD", "Corrupt PSD image");

   // Check file type version.
   if (GLA__get16be(s) != 1)
      return GLA__errpuc("wrong version", "Unsupported version of PSD image");

   // Skip 6 reserved bytes.
   GLA__skip(s, 6 );

   // Read the number of channels (R, G, B, A, etc).
   channelCount = GLA__get16be(s);
   if (channelCount < 0 || channelCount > 16)
      return GLA__errpuc("wrong channel count", "Unsupported number of channels in PSD image");

   // Read the rows and columns of the image.
   h = GLA__get32be(s);
   w = GLA__get32be(s);
   
   // Make sure the depth is 8 bits.
   if (GLA__get16be(s) != 8)
      return GLA__errpuc("unsupported bit depth", "PSD bit depth is not 8 bit");

   // Make sure the color mode is RGB.
   // Valid options are:
   //   0: Bitmap
   //   1: Grayscale
   //   2: Indexed color
   //   3: RGB color
   //   4: CMYK color
   //   7: Multichannel
   //   8: Duotone
   //   9: Lab color
   if (GLA__get16be(s) != 3)
      return GLA__errpuc("wrong color format", "PSD is not in RGB color format");

   // Skip the Mode Data.  (It's the palette for indexed color; other info for other modes.)
   GLA__skip(s,GLA__get32be(s) );

   // Skip the image resources.  (resolution, pen tool paths, etc)
   GLA__skip(s, GLA__get32be(s) );

   // Skip the reserved data.
   GLA__skip(s, GLA__get32be(s) );

   // Find out if the data is compressed.
   // Known values:
   //   0: no compression
   //   1: RLE compressed
   compression = GLA__get16be(s);
   if (compression > 1)
      return GLA__errpuc("bad compression", "PSD has an unknown compression format");

   // Create the destination image.
   out = (GLA_uc *) GLA__malloc(4 * w*h);
   if (!out) return GLA__errpuc("outofmem", "Out of memory");
   pixelCount = w*h;

   // Initialize the data to zero.
   //memset( out, 0, pixelCount * 4 );
   
   // Finally, the image data.
   if (compression) {
      // RLE as used by .PSD and .TIFF
      // Loop until you get the number of unpacked bytes you are expecting:
      //     Read the next source byte into n.
      //     If n is between 0 and 127 inclusive, copy the next n+1 bytes literally.
      //     Else if n is between -127 and -1 inclusive, copy the next byte -n+1 times.
      //     Else if n is 128, noop.
      // Endloop

      // The RLE-compressed data is preceeded by a 2-byte data count for each row in the data,
      // which we're going to just skip.
      GLA__skip(s, h * channelCount * 2 );

      // Read the RLE data by channel.
      for (channel = 0; channel < 4; channel++) {
         GLA_uc *p;
         
         p = out+channel;
         if (channel >= channelCount) {
            // Fill this channel with default data.
            for (i = 0; i < pixelCount; i++) *p = (channel == 3 ? 255 : 0), p += 4;
         } else {
            // Read the RLE data.
            count = 0;
            while (count < pixelCount) {
               len = GLA__get8(s);
               if (len == 128) {
                  // No-op.
               } else if (len < 128) {
                  // Copy next len+1 bytes literally.
                  len++;
                  count += len;
                  while (len) {
                     *p = GLA__get8(s);
                     p += 4;
                     len--;
                  }
               } else if (len > 128) {
                  GLA_uc   val;
                  // Next -len+1 bytes in the dest are replicated from next source byte.
                  // (Interpret len as a negative 8-bit int.)
                  len ^= 0x0FF;
                  len += 2;
                  val = GLA__get8(s);
                  count += len;
                  while (len) {
                     *p = val;
                     p += 4;
                     len--;
                  }
               }
            }
         }
      }
      
   } else {
      // We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
      // where each channel consists of an 8-bit value for each pixel in the image.
      
      // Read the data by channel.
      for (channel = 0; channel < 4; channel++) {
         GLA_uc *p;
         
         p = out + channel;
         if (channel > channelCount) {
            // Fill this channel with default data.
            for (i = 0; i < pixelCount; i++) *p = channel == 3 ? 255 : 0, p += 4;
         } else {
            // Read the data.
            for (i = 0; i < pixelCount; i++)
               *p = GLA__get8(s), p += 4;
         }
      }
   }

   if (req_comp && req_comp != 4) {
      out = GLA__convert_format(out, 4, req_comp, w, h);
      if (out == NULL) return out; // GLA__convert_format frees input on failure
   }

   if (comp) *comp = channelCount;
   *y = h;
   *x = w;
   
   return out;
}

// *************************************************************************************************
// Softimage PIC loader
// by Tom Seddon
//
// See http://softimage.wiki.softimage.com/index.php/INFO:_PIC_file_format
// See http://ozviz.wasp.uwa.edu.au/~pbourke/dataformats/softimagepic/

static int GLA__pic_is4(GLA__context *s,const char *str)
{
   int i;
   for (i=0; i<4; ++i)
      if (GLA__get8(s) != (GLA_uc)str[i])
         return 0;

   return 1;
}

static int GLA__pic_test_core(GLA__context *s)
{
   int i;

   if (!GLA__pic_is4(s,"\x53\x80\xF6\x34"))
      return 0;

   for(i=0;i<84;++i)
      GLA__get8(s);

   if (!GLA__pic_is4(s,"PICT"))
      return 0;

   return 1;
}

typedef struct
{
   GLA_uc size,type,channel;
} GLA__pic_packet;

static GLA_uc *GLA__readval(GLA__context *s, int channel, GLA_uc *dest)
{
   int mask=0x80, i;

   for (i=0; i<4; ++i, mask>>=1) {
      if (channel & mask) {
         if (GLA__at_eof(s)) return GLA__errpuc("bad file","PIC file too short");
         dest[i]=GLA__get8(s);
      }
   }

   return dest;
}

static void GLA__copyval(int channel,GLA_uc *dest,const GLA_uc *src)
{
   int mask=0x80,i;

   for (i=0;i<4; ++i, mask>>=1)
      if (channel&mask)
         dest[i]=src[i];
}

static GLA_uc *GLA__pic_load_core(GLA__context *s,int width,int height,int *comp, GLA_uc *result)
{
   int act_comp=0,num_packets=0,y,chained;
   GLA__pic_packet packets[10];

   // this will (should...) cater for even some bizarre stuff like having data
    // for the same channel in multiple packets.
   do {
      GLA__pic_packet *packet;

      if (num_packets==sizeof(packets)/sizeof(packets[0]))
         return GLA__errpuc("bad format","too many packets");

      packet = &packets[num_packets++];

      chained = GLA__get8(s);
      packet->size    = GLA__get8(s);
      packet->type    = GLA__get8(s);
      packet->channel = GLA__get8(s);

      act_comp |= packet->channel;

      if (GLA__at_eof(s))          return GLA__errpuc("bad file","file too short (reading packets)");
      if (packet->size != 8)  return GLA__errpuc("bad format","packet isn't 8bpp");
   } while (chained);

   *comp = (act_comp & 0x10 ? 4 : 3); // has alpha channel?

   for(y=0; y<height; ++y) {
      int packet_idx;

      for(packet_idx=0; packet_idx < num_packets; ++packet_idx) {
         GLA__pic_packet *packet = &packets[packet_idx];
         GLA_uc *dest = result+y*width*4;

         switch (packet->type) {
            default:
               return GLA__errpuc("bad format","packet has bad compression type");

            case 0: {//uncompressed
               int x;

               for(x=0;x<width;++x, dest+=4)
                  if (!GLA__readval(s,packet->channel,dest))
                     return 0;
               break;
            }

            case 1://Pure RLE
               {
                  int left=width, i;

                  while (left>0) {
                     GLA_uc count,value[4];

                     count=GLA__get8(s);
                     if (GLA__at_eof(s))   return GLA__errpuc("bad file","file too short (pure read count)");

                     if (count > left)
                        count = (GLA_uc) left;

                     if (!GLA__readval(s,packet->channel,value))  return 0;

                     for(i=0; i<count; ++i,dest+=4)
                        GLA__copyval(packet->channel,dest,value);
                     left -= count;
                  }
               }
               break;

            case 2: {//Mixed RLE
               int left=width;
               while (left>0) {
                  int count = GLA__get8(s), i;
                  if (GLA__at_eof(s))  return GLA__errpuc("bad file","file too short (mixed read count)");

                  if (count >= 128) { // Repeated
                     GLA_uc value[4];
                     int i;

                     if (count==128)
                        count = GLA__get16be(s);
                     else
                        count -= 127;
                     if (count > left)
                        return GLA__errpuc("bad file","scanline overrun");

                     if (!GLA__readval(s,packet->channel,value))
                        return 0;

                     for(i=0;i<count;++i, dest += 4)
                        GLA__copyval(packet->channel,dest,value);
                  } else { // Raw
                     ++count;
                     if (count>left) return GLA__errpuc("bad file","scanline overrun");

                     for(i=0;i<count;++i, dest+=4)
                        if (!GLA__readval(s,packet->channel,dest))
                           return 0;
                  }
                  left-=count;
               }
               break;
            }
         }
      }
   }

   return result;
}

static GLA_uc *GLA__pic_load(GLA__context *s,int *px,int *py,int *comp,int req_comp)
{
   GLA_uc *result;
   int i, x,y;

   for (i=0; i<92; ++i)
      GLA__get8(s);

   x = GLA__get16be(s);
   y = GLA__get16be(s);
   if (GLA__at_eof(s))  return GLA__errpuc("bad file","file too short (pic header)");
   if ((1 << 28) / x < y) return GLA__errpuc("too large", "Image too large to decode");

   GLA__get32be(s); //skip `ratio'
   GLA__get16be(s); //skip `fields'
   GLA__get16be(s); //skip `pad'

   // intermediate buffer is RGBA
   result = (GLA_uc *) GLA__malloc(x*y*4);
   memset(result, 0xff, x*y*4);

   if (!GLA__pic_load_core(s,x,y,comp, result)) {
      free(result);
      result=0;
   }
   *px = x;
   *py = y;
   if (req_comp == 0) req_comp = *comp;
   result=GLA__convert_format(result,4,req_comp,x,y);

   return result;
}

static int GLA__pic_test(GLA__context *s)
{
   int r = GLA__pic_test_core(s);
   GLA__rewind(s);
   return r;
}

// *************************************************************************************************
// GIF loader -- public domain by Jean-Marc Lienher -- simplified/shrunk by stb
typedef struct 
{
   GLA__int16 prefix;
   GLA_uc first;
   GLA_uc suffix;
} GLA__gif_lzw;

typedef struct
{
   int w,h;
   GLA_uc *out;                 // output buffer (always 4 components)
   int flags, bgindex, ratio, transparent, eflags;
   GLA_uc  pal[256][4];
   GLA_uc lpal[256][4];
   GLA__gif_lzw codes[4096];
   GLA_uc *color_table;
   int parse, step;
   int lflags;
   int start_x, start_y;
   int max_x, max_y;
   int cur_x, cur_y;
   int line_size;
} GLA__gif;

static int GLA__gif_test_raw(GLA__context *s)
{
   int sz;
   if (GLA__get8(s) != 'G' || GLA__get8(s) != 'I' || GLA__get8(s) != 'F' || GLA__get8(s) != '8') return 0;
   sz = GLA__get8(s);
   if (sz != '9' && sz != '7') return 0;
   if (GLA__get8(s) != 'a') return 0;
   return 1;
}

static int GLA__gif_test(GLA__context *s)
{
   int r = GLA__gif_test_raw(s);
   GLA__rewind(s);
   return r;
}

static void GLA__gif_parse_colortable(GLA__context *s, GLA_uc pal[256][4], int num_entries, int transp)
{
   int i;
   for (i=0; i < num_entries; ++i) {
      pal[i][2] = GLA__get8(s);
      pal[i][1] = GLA__get8(s);
      pal[i][0] = GLA__get8(s);
      pal[i][3] = transp ? 0 : 255;
   }   
}

static int GLA__gif_header(GLA__context *s, GLA__gif *g, int *comp, int is_info)
{
   GLA_uc version;
   if (GLA__get8(s) != 'G' || GLA__get8(s) != 'I' || GLA__get8(s) != 'F' || GLA__get8(s) != '8')
      return GLA__err("not GIF", "Corrupt GIF");

   version = GLA__get8(s);
   if (version != '7' && version != '9')    return GLA__err("not GIF", "Corrupt GIF");
   if (GLA__get8(s) != 'a')                      return GLA__err("not GIF", "Corrupt GIF");
 
   //GLA__g_failure_reason = "";
   g->w = GLA__get16le(s);
   g->h = GLA__get16le(s);
   g->flags = GLA__get8(s);
   g->bgindex = GLA__get8(s);
   g->ratio = GLA__get8(s);
   g->transparent = -1;

   if (comp != 0) *comp = 4;  // can't actually tell whether it's 3 or 4 until we parse the comments

   if (is_info) return 1;

   if (g->flags & 0x80)
      GLA__gif_parse_colortable(s,g->pal, 2 << (g->flags & 7), -1);

   return 1;
}

static int GLA__gif_info_raw(GLA__context *s, int *x, int *y, int *comp)
{
   GLA__gif g;
   if (!GLA__gif_header(s, &g, comp, 1)) {
      GLA__rewind( s );
      return 0;
   }
   if (x) *x = g.w;
   if (y) *y = g.h;
   return 1;
}

static void GLA__out_gif_code(GLA__gif *g, GLA__uint16 code)
{
   GLA_uc *p, *c;

   // recurse to decode the prefixes, since the linked-list is backwards,
   // and working backwards through an interleaved image would be nasty
   if (g->codes[code].prefix >= 0)
      GLA__out_gif_code(g, g->codes[code].prefix);

   if (g->cur_y >= g->max_y) return;
  
   p = &g->out[g->cur_x + g->cur_y];
   c = &g->color_table[g->codes[code].suffix * 4];

   if (c[3] >= 128) {
      p[0] = c[2];
      p[1] = c[1];
      p[2] = c[0];
      p[3] = c[3];
   }
   g->cur_x += 4;

   if (g->cur_x >= g->max_x) {
      g->cur_x = g->start_x;
      g->cur_y += g->step;

      while (g->cur_y >= g->max_y && g->parse > 0) {
         g->step = (1 << g->parse) * g->line_size;
         g->cur_y = g->start_y + (g->step >> 1);
         --g->parse;
      }
   }
}

static GLA_uc *GLA__process_gif_raster(GLA__context *s, GLA__gif *g)
{
   GLA_uc lzw_cs;
   GLA__int32 len, code;
   GLA__uint32 first;
   GLA__int32 codesize, codemask, avail, oldcode, bits, valid_bits, clear;
   GLA__gif_lzw *p;

   lzw_cs = GLA__get8(s);
   clear = 1 << lzw_cs;
   first = 1;
   codesize = lzw_cs + 1;
   codemask = (1 << codesize) - 1;
   bits = 0;
   valid_bits = 0;
   for (code = 0; code < clear; code++) {
      g->codes[code].prefix = -1;
      g->codes[code].first = (GLA_uc) code;
      g->codes[code].suffix = (GLA_uc) code;
   }

   // support no starting clear code
   avail = clear+2;
   oldcode = -1;

   len = 0;
   for(;;) {
      if (valid_bits < codesize) {
         if (len == 0) {
            len = GLA__get8(s); // start new block
            if (len == 0) 
               return g->out;
         }
         --len;
         bits |= (GLA__int32) GLA__get8(s) << valid_bits;
         valid_bits += 8;
      } else {
         GLA__int32 code = bits & codemask;
         bits >>= codesize;
         valid_bits -= codesize;
         // @OPTIMIZE: is there some way we can accelerate the non-clear path?
         if (code == clear) {  // clear code
            codesize = lzw_cs + 1;
            codemask = (1 << codesize) - 1;
            avail = clear + 2;
            oldcode = -1;
            first = 0;
         } else if (code == clear + 1) { // end of stream code
            GLA__skip(s, len);
            while ((len = GLA__get8(s)) > 0)
               GLA__skip(s,len);
            return g->out;
         } else if (code <= avail) {
            if (first) return GLA__errpuc("no clear code", "Corrupt GIF");

            if (oldcode >= 0) {
               p = &g->codes[avail++];
               if (avail > 4096)        return GLA__errpuc("too many codes", "Corrupt GIF");
               p->prefix = (GLA__int16) oldcode;
               p->first = g->codes[oldcode].first;
               p->suffix = (code == avail) ? p->first : g->codes[code].first;
            } else if (code == avail)
               return GLA__errpuc("illegal code in raster", "Corrupt GIF");

            GLA__out_gif_code(g, (GLA__uint16) code);

            if ((avail & codemask) == 0 && avail <= 0x0FFF) {
               codesize++;
               codemask = (1 << codesize) - 1;
            }

            oldcode = code;
         } else {
            return GLA__errpuc("illegal code in raster", "Corrupt GIF");
         }
      } 
   }
}

static void GLA__fill_gif_background(GLA__gif *g)
{
   int i;
   GLA_uc *c = g->pal[g->bgindex];
   // @OPTIMIZE: write a dword at a time
   for (i = 0; i < g->w * g->h * 4; i += 4) {
      GLA_uc *p  = &g->out[i];
      p[0] = c[2];
      p[1] = c[1];
      p[2] = c[0];
      p[3] = c[3];
   }
}

// this function is designed to support animated gifs, although stb_image doesn't support it
static GLA_uc *GLA__gif_load_next(GLA__context *s, GLA__gif *g, int *comp, int req_comp)
{
   int i;
   GLA_uc *old_out = 0;

   if (g->out == 0) {
      if (!GLA__gif_header(s, g, comp,0))     return 0; // GLA__g_failure_reason set by GLA__gif_header
      g->out = (GLA_uc *) GLA__malloc(4 * g->w * g->h);
      if (g->out == 0)                      return GLA__errpuc("outofmem", "Out of memory");
      GLA__fill_gif_background(g);
   } else {
      // animated-gif-only path
      if (((g->eflags & 0x1C) >> 2) == 3) {
         old_out = g->out;
         g->out = (GLA_uc *) GLA__malloc(4 * g->w * g->h);
         if (g->out == 0)                   return GLA__errpuc("outofmem", "Out of memory");
         memcpy(g->out, old_out, g->w*g->h*4);
      }
   }
    
   for (;;) {
      switch (GLA__get8(s)) {
         case 0x2C: /* Image Descriptor */
         {
            GLA__int32 x, y, w, h;
            GLA_uc *o;

            x = GLA__get16le(s);
            y = GLA__get16le(s);
            w = GLA__get16le(s);
            h = GLA__get16le(s);
            if (((x + w) > (g->w)) || ((y + h) > (g->h)))
               return GLA__errpuc("bad Image Descriptor", "Corrupt GIF");

            g->line_size = g->w * 4;
            g->start_x = x * 4;
            g->start_y = y * g->line_size;
            g->max_x   = g->start_x + w * 4;
            g->max_y   = g->start_y + h * g->line_size;
            g->cur_x   = g->start_x;
            g->cur_y   = g->start_y;

            g->lflags = GLA__get8(s);

            if (g->lflags & 0x40) {
               g->step = 8 * g->line_size; // first interlaced spacing
               g->parse = 3;
            } else {
               g->step = g->line_size;
               g->parse = 0;
            }

            if (g->lflags & 0x80) {
               GLA__gif_parse_colortable(s,g->lpal, 2 << (g->lflags & 7), g->eflags & 0x01 ? g->transparent : -1);
               g->color_table = (GLA_uc *) g->lpal;
            } else if (g->flags & 0x80) {
               for (i=0; i < 256; ++i)  // @OPTIMIZE: GLA__jpeg_reset only the previous transparent
                  g->pal[i][3] = 255; 
               if (g->transparent >= 0 && (g->eflags & 0x01))
                  g->pal[g->transparent][3] = 0;
               g->color_table = (GLA_uc *) g->pal;
            } else
               return GLA__errpuc("missing color table", "Corrupt GIF");
   
            o = GLA__process_gif_raster(s, g);
            if (o == NULL) return NULL;

            if (req_comp && req_comp != 4)
               o = GLA__convert_format(o, 4, req_comp, g->w, g->h);
            return o;
         }

         case 0x21: // Comment Extension.
         {
            int len;
            if (GLA__get8(s) == 0xF9) { // Graphic Control Extension.
               len = GLA__get8(s);
               if (len == 4) {
                  g->eflags = GLA__get8(s);
                  GLA__get16le(s); // delay
                  g->transparent = GLA__get8(s);
               } else {
                  GLA__skip(s, len);
                  break;
               }
            }
            while ((len = GLA__get8(s)) != 0)
               GLA__skip(s, len);
            break;
         }

         case 0x3B: // gif stream termination code
            return (GLA_uc *) s; // using '1' causes warning on some compilers

         default:
            return GLA__errpuc("unknown code", "Corrupt GIF");
      }
   }
}

static GLA_uc *GLA__gif_load(GLA__context *s, int *x, int *y, int *comp, int req_comp)
{
   GLA_uc *u = 0;
   GLA__gif g;
   memset(&g, 0, sizeof(g));

   u = GLA__gif_load_next(s, &g, comp, req_comp);
   if (u == (GLA_uc *) s) u = 0;  // end of animated gif marker
   if (u) {
      *x = g.w;
      *y = g.h;
   }

   return u;
}

static int GLA__gif_info(GLA__context *s, int *x, int *y, int *comp)
{
   return GLA__gif_info_raw(s,x,y,comp);
}


// *************************************************************************************************
// Radiance RGBE HDR loader
// originally by Nicolas Schulz
#ifndef GLAI_NO_HDR
static int GLA__hdr_test_core(GLA__context *s)
{
   const char *signature = "#?RADIANCE\n";
   int i;
   for (i=0; signature[i]; ++i)
      if (GLA__get8(s) != signature[i])
         return 0;
   return 1;
}

static int GLA__hdr_test(GLA__context* s)
{
   int r = GLA__hdr_test_core(s);
   GLA__rewind(s);
   return r;
}

#define GLAI__HDR_BUFLEN  1024
static char *GLA__hdr_gettoken(GLA__context *z, char *buffer)
{
   int len=0;
   char c = '\0';

   c = (char) GLA__get8(z);

   while (!GLA__at_eof(z) && c != '\n') {
      buffer[len++] = c;
      if (len == GLAI__HDR_BUFLEN-1) {
         // flush to end of line
         while (!GLA__at_eof(z) && GLA__get8(z) != '\n')
            ;
         break;
      }
      c = (char) GLA__get8(z);
   }

   buffer[len] = 0;
   return buffer;
}

static void GLA__hdr_convert(float *output, GLA_uc *input, int req_comp)
{
   if ( input[3] != 0 ) {
      float f1;
      // Exponent
      f1 = (float) ldexp(1.0f, input[3] - (int)(128 + 8));
      if (req_comp <= 2)
         output[0] = (input[0] + input[1] + input[2]) * f1 / 3;
      else {
         output[0] = input[0] * f1;
         output[1] = input[1] * f1;
         output[2] = input[2] * f1;
      }
      if (req_comp == 2) output[1] = 1;
      if (req_comp == 4) output[3] = 1;
   } else {
      switch (req_comp) {
         case 4: output[3] = 1; /* fallthrough */
         case 3: output[0] = output[1] = output[2] = 0;
                 break;
         case 2: output[1] = 1; /* fallthrough */
         case 1: output[0] = 0;
                 break;
      }
   }
}

static float *GLA__hdr_load(GLA__context *s, int *x, int *y, int *comp, int req_comp)
{
   char buffer[GLAI__HDR_BUFLEN];
   char *token;
   int valid = 0;
   int width, height;
   GLA_uc *scanline;
   float *hdr_data;
   int len;
   unsigned char count, value;
   int i, j, k, c1,c2, z;


   // Check identifier
   if (strcmp(GLA__hdr_gettoken(s,buffer), "#?RADIANCE") != 0)
      return GLA__errpf("not HDR", "Corrupt HDR image");
   
   // Parse header
   for(;;) {
      token = GLA__hdr_gettoken(s,buffer);
      if (token[0] == 0) break;
      if (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   if (!valid)    return GLA__errpf("unsupported format", "Unsupported HDR format");

   // Parse width and height
   // can't use sscanf() if we're not using stdio!
   token = GLA__hdr_gettoken(s,buffer);
   if (strncmp(token, "-Y ", 3))  return GLA__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   height = (int) strtol(token, &token, 10);
   while (*token == ' ') ++token;
   if (strncmp(token, "+X ", 3))  return GLA__errpf("unsupported data layout", "Unsupported HDR format");
   token += 3;
   width = (int) strtol(token, NULL, 10);

   *x = width;
   *y = height;

   if (comp) *comp = 3;
   if (req_comp == 0) req_comp = 3;

   // Read data
   hdr_data = (float *) GLA__malloc(height * width * req_comp * sizeof(float));

   // Load image data
   // image data is stored as some number of sca
   if ( width < 8 || width >= 32768) {
      // Read flat data
      for (j=0; j < height; ++j) {
         for (i=0; i < width; ++i) {
            GLA_uc rgbe[4];
           main_decode_loop:
            GLA__getn(s, rgbe, 4);
            GLA__hdr_convert(hdr_data + j * width * req_comp + i * req_comp, rgbe, req_comp);
         }
      }
   } else {
      // Read RLE-encoded data
      scanline = NULL;

      for (j = 0; j < height; ++j) {
         c1 = GLA__get8(s);
         c2 = GLA__get8(s);
         len = GLA__get8(s);
         if (c1 != 2 || c2 != 2 || (len & 0x80)) {
            // not run-length encoded, so we have to actually use THIS data as a decoded
            // pixel (note this can't be a valid pixel--one of RGB must be >= 128)
            GLA_uc rgbe[4];
            rgbe[0] = (GLA_uc) c1;
            rgbe[1] = (GLA_uc) c2;
            rgbe[2] = (GLA_uc) len;
            rgbe[3] = (GLA_uc) GLA__get8(s);
            GLA__hdr_convert(hdr_data, rgbe, req_comp);
            i = 1;
            j = 0;
            free(scanline);
            goto main_decode_loop; // yes, this makes no sense
         }
         len <<= 8;
         len |= GLA__get8(s);
         if (len != width) { free(hdr_data); free(scanline); return GLA__errpf("invalid decoded scanline length", "corrupt HDR"); }
         if (scanline == NULL) scanline = (GLA_uc *) GLA__malloc(width * 4);
            
         for (k = 0; k < 4; ++k) {
            i = 0;
            while (i < width) {
               count = GLA__get8(s);
               if (count > 128) {
                  // Run
                  value = GLA__get8(s);
                  count -= 128;
                  for (z = 0; z < count; ++z)
                     scanline[i++ * 4 + k] = value;
               } else {
                  // Dump
                  for (z = 0; z < count; ++z)
                     scanline[i++ * 4 + k] = GLA__get8(s);
               }
            }
         }
         for (i=0; i < width; ++i)
            GLA__hdr_convert(hdr_data+(j*width + i)*req_comp, scanline + i*4, req_comp);
      }
      free(scanline);
   }

   return hdr_data;
}

static int GLA__hdr_info(GLA__context *s, int *x, int *y, int *comp)
{
   char buffer[GLAI__HDR_BUFLEN];
   char *token;
   int valid = 0;

   if (strcmp(GLA__hdr_gettoken(s,buffer), "#?RADIANCE") != 0) {
       GLA__rewind( s );
       return 0;
   }

   for(;;) {
      token = GLA__hdr_gettoken(s,buffer);
      if (token[0] == 0) break;
      if (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

   if (!valid) {
       GLA__rewind( s );
       return 0;
   }
   token = GLA__hdr_gettoken(s,buffer);
   if (strncmp(token, "-Y ", 3)) {
       GLA__rewind( s );
       return 0;
   }
   token += 3;
   *y = (int) strtol(token, &token, 10);
   while (*token == ' ') ++token;
   if (strncmp(token, "+X ", 3)) {
       GLA__rewind( s );
       return 0;
   }
   token += 3;
   *x = (int) strtol(token, NULL, 10);
   *comp = 3;
   return 1;
}
#endif // GLAI_NO_HDR

static int GLA__bmp_info(GLA__context *s, int *x, int *y, int *comp)
{
   int hsz;
   if (GLA__get8(s) != 'B' || GLA__get8(s) != 'M') {
       GLA__rewind( s );
       return 0;
   }
   GLA__skip(s,12);
   hsz = GLA__get32le(s);
   if (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) {
       GLA__rewind( s );
       return 0;
   }
   if (hsz == 12) {
      *x = GLA__get16le(s);
      *y = GLA__get16le(s);
   } else {
      *x = GLA__get32le(s);
      *y = GLA__get32le(s);
   }
   if (GLA__get16le(s) != 1) {
       GLA__rewind( s );
       return 0;
   }
   *comp = GLA__get16le(s) / 8;
   return 1;
}

static int GLA__psd_info(GLA__context *s, int *x, int *y, int *comp)
{
   int channelCount;
   if (GLA__get32be(s) != 0x38425053) {
       GLA__rewind( s );
       return 0;
   }
   if (GLA__get16be(s) != 1) {
       GLA__rewind( s );
       return 0;
   }
   GLA__skip(s, 6);
   channelCount = GLA__get16be(s);
   if (channelCount < 0 || channelCount > 16) {
       GLA__rewind( s );
       return 0;
   }
   *y = GLA__get32be(s);
   *x = GLA__get32be(s);
   if (GLA__get16be(s) != 8) {
       GLA__rewind( s );
       return 0;
   }
   if (GLA__get16be(s) != 3) {
       GLA__rewind( s );
       return 0;
   }
   *comp = 4;
   return 1;
}

static int GLA__pic_info(GLA__context *s, int *x, int *y, int *comp)
{
   int act_comp=0,num_packets=0,chained;
   GLA__pic_packet packets[10];

   GLA__skip(s, 92);

   *x = GLA__get16be(s);
   *y = GLA__get16be(s);
   if (GLA__at_eof(s))  return 0;
   if ( (*x) != 0 && (1 << 28) / (*x) < (*y)) {
       GLA__rewind( s );
       return 0;
   }

   GLA__skip(s, 8);

   do {
      GLA__pic_packet *packet;

      if (num_packets==sizeof(packets)/sizeof(packets[0]))
         return 0;

      packet = &packets[num_packets++];
      chained = GLA__get8(s);
      packet->size    = GLA__get8(s);
      packet->type    = GLA__get8(s);
      packet->channel = GLA__get8(s);
      act_comp |= packet->channel;

      if (GLA__at_eof(s)) {
          GLA__rewind( s );
          return 0;
      }
      if (packet->size != 8) {
          GLA__rewind( s );
          return 0;
      }
   } while (chained);

   *comp = (act_comp & 0x10 ? 4 : 3);

   return 1;
}

static int GLA__info_main(GLA__context *s, int *x, int *y, int *comp)
{
   if (GLA__jpeg_info(s, x, y, comp))
       return 1;
   if (GLA__png_info(s, x, y, comp))
       return 1;
   if (GLA__gif_info(s, x, y, comp))
       return 1;
   if (GLA__bmp_info(s, x, y, comp))
       return 1;
   if (GLA__psd_info(s, x, y, comp))
       return 1;
   if (GLA__pic_info(s, x, y, comp))
       return 1;
   #ifndef GLAI_NO_HDR
   if (GLA__hdr_info(s, x, y, comp))
       return 1;
   #endif
   // test tga last because it's a crappy test!
   if (GLA__tga_info(s, x, y, comp))
       return 1;
   return GLA__err("unknown image type", "Image not of any known type, or corrupt");
}

#ifndef GLAI_NO_STDIO
    GLAIDEF int GLA_info(char const *filename, int *x, int *y, int *comp)
    {
        FILE *f = GLA__fopen(filename, "rb");
        int result;
        if (!f) return GLA__err("can't fopen", "Unable to open file");
        result = GLA_info_from_file(f, x, y, comp);
        fclose(f);
        return result;
    }

    GLAIDEF int GLA_info_from_file(FILE *f, int *x, int *y, int *comp)
    {
       int r;
       GLA__context s;
       long pos = ftell(f);
       GLA__start_file(&s, f);
       r = GLA__info_main(&s,x,y,comp);
       fseek(f,pos,SEEK_SET);
       return r;
    }
#endif // !GLAI_NO_STDIO

GLAIDEF int GLA_info_from_memory(GLA_uc const *buffer, int len, int *x, int *y, int *comp)
{
   GLA__context s;
   GLA__start_mem(&s,buffer,len);
   return GLA__info_main(&s,x,y,comp);
}

GLAIDEF int GLA_info_from_callbacks(GLA_io_callbacks const *c, void *user, int *x, int *y, int *comp)
{
   GLA__context s;
   GLA__start_callbacks(&s, (GLA_io_callbacks *) c, user);
   return GLA__info_main(&s,x,y,comp);
}

};

#endif // GLA_IMAGE_IMPLEMENTATION

/*
   revision history:
      1.46 (2014-08-26)
             fix broken tRNS chunk (colorkey-style transparency) in non-paletted PNG
      1.45 (2014-08-16)
             fix MSVC-ARM internal compiler error by wrapping malloc
      1.44 (2014-08-07)
		       various warning fixes from Ronny Chevalier
      1.43 (2014-07-15)
             fix MSVC-only compiler problem in code changed in 1.42
      1.42 (2014-07-09)
             don't define _CRT_SECURE_NO_WARNINGS (affects user code)
             fixes to GLA__cleanup_jpeg path
             added GLAI_ASSERT to avoid requiring assert.h
      1.41 (2014-06-25)
             fix search&replace from 1.36 that messed up comments/error messages
      1.40 (2014-06-22)
             fix gcc struct-initialization warning
      1.39 (2014-06-15)
             fix to TGA optimization when req_comp != number of components in TGA;
             fix to GIF loading because BMP wasn't rewinding (whoops, no GIFs in my test suite)
             add support for BMP version 5 (more ignored fields)
      1.38 (2014-06-06)
             suppress MSVC warnings on integer casts truncating values
             fix accidental rename of 'skip' field of I/O
      1.37 (2014-06-04)
             remove duplicate typedef
      1.36 (2014-06-03)
             convert to header file single-file library
             if de-iphone isn't set, load iphone images color-swapped instead of returning NULL
      1.35 (2014-05-27)
             various warnings
             fix broken GLAI_SIMD path
             fix bug where GLA_load_from_file no longer left file pointer in correct place
             fix broken non-easy path for 32-bit BMP (possibly never used)
             TGA optimization by Arseny Kapoulkine
      1.34 (unknown)
             use GLAI_NOTUSED in GLA__resample_row_generic(), fix one more leak in tga failure case
      1.33 (2011-07-14)
             make GLA_is_hdr work in GLAI_NO_HDR (as specified), minor compiler-friendly improvements
      1.32 (2011-07-13)
             support for "info" function for all supported filetypes (SpartanJ)
      1.31 (2011-06-20)
             a few more leak fixes, bug in PNG handling (SpartanJ)
      1.30 (2011-06-11)
             added ability to load files via callbacks to accomidate custom input streams (Ben Wenger)
             removed deprecated format-specific test/load functions
             removed support for installable file formats (GLA_loader) -- would have been broken for IO callbacks anyway
             error cases in bmp and tga give messages and don't leak (Raymond Barbiero, grisha)
             fix inefficiency in decoding 32-bit BMP (David Woo)
      1.29 (2010-08-16)
             various warning fixes from Aurelien Pocheville 
      1.28 (2010-08-01)
             fix bug in GIF palette transparency (SpartanJ)
      1.27 (2010-08-01)
             cast-to-GLA_uc to fix warnings
      1.26 (2010-07-24)
             fix bug in file buffering for PNG reported by SpartanJ
      1.25 (2010-07-17)
             refix trans_data warning (Won Chun)
      1.24 (2010-07-12)
             perf improvements reading from files on platforms with lock-heavy fgetc()
             minor perf improvements for jpeg
             deprecated type-specific functions so we'll get feedback if they're needed
             attempt to fix trans_data warning (Won Chun)
      1.23   fixed bug in iPhone support
      1.22 (2010-07-10)
             removed image *writing* support
             GLA_info support from Jetro Lauha
             GIF support from Jean-Marc Lienher
             iPhone PNG-extensions from James Brown
             warning-fixes from Nicolas Schulz and Janez Zemva (i.GLA__err. Janez (U+017D)emva)
      1.21   fix use of 'GLA_uc' in header (reported by jon blow)
      1.20   added support for Softimage PIC, by Tom Seddon
      1.19   bug in interlaced PNG corruption check (found by ryg)
      1.18 2008-08-02
             fix a threading bug (local mutable static)
      1.17   support interlaced PNG
      1.16   major bugfix - GLA__convert_format converted one too many pixels
      1.15   initialize some fields for thread safety
      1.14   fix threadsafe conversion bug
             header-file-only version (#define GLAI_HEADER_FILE_ONLY before including)
      1.13   threadsafe
      1.12   const qualifiers in the API
      1.11   Support installable IDCT, colorspace conversion routines
      1.10   Fixes for 64-bit (don't use "unsigned long")
             optimized upsampling by Fabian "ryg" Giesen
      1.09   Fix format-conversion for PSD code (bad global variables!)
      1.08   Thatcher Ulrich's PSD code integrated by Nicolas Schulz
      1.07   attempt to fix C++ warning/errors again
      1.06   attempt to fix C++ warning/errors again
      1.05   fix TGA loading to return correct *comp and use good luminance calc
      1.04   default float alpha is 1, not 255; use 'void *' for GLA_image_free
      1.03   bugfixes to GLAI_NO_STDIO, GLAI_NO_HDR
      1.02   support for (subset of) HDR files, float interface for preferred access to them
      1.01   fix bug: possible bug in handling right-side up bmps... not sure
             fix bug: the GLA__bmp_load() and GLA__tga_load() functions didn't work at all
      1.00   interface to zlib that skips zlib header
      0.99   correct handling of alpha in palette
      0.98   TGA loader by lonesock; dynamically add loaders (untested)
      0.97   jpeg errors on too large a file; also catch another malloc failure
      0.96   fix detection of invalid v value - particleman@mollyrocket forum
      0.95   during header scan, seek to markers in case of padding
      0.94   GLAI_NO_STDIO to disable stdio usage; rename all #defines the same
      0.93   handle jpegtran output; verbose errors
      0.92   read 4,8,16,24,32-bit BMP files of several formats
      0.91   output 24-bit Windows 3.0 BMP files
      0.90   fix a few more warnings; bump version number to approach 1.0
      0.61   bugfixes due to Marc LeBlanc, Christopher Lloyd
      0.60   fix compiling as c++
      0.59   fix warnings: merge Dave Moore's -Wall fixes
      0.58   fix bug: zlib uncompressed mode len/nlen was wrong endian
      0.57   fix bug: jpg last huffman symbol before marker was >9 bits but less than 16 available
      0.56   fix bug: zlib uncompressed mode len vs. nlen
      0.55   fix bug: restart_interval not initialized to 0
      0.54   allow NULL for 'int *comp'
      0.53   fix bug in png 3->4; speedup png decoding
      0.52   png handles req_comp=3,4 directly; minor cleanup; jpeg comments
      0.51   obey req_comp requests, 1-component jpegs return as 1-component,
             on 'test' only check type, not whether we support this variant
      0.50   first released version
*/
