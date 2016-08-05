#ifndef ARRAYBUFFER_H
#define ARRAYBUFFER_H

#include <vector>
#include <gla/glad.h>
//#include <GL/glew.h>
//#include <GL/glext.h>
//#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <gla/exceptions.h>
#include <gla/types.h>
#include <gla/handle.h>
#include <memory>

//===================================================================================

namespace gla {

struct ArrayBufferInfo
{
    BUFFER_ELEMENT_TYPE    mBufferType;
    unsigned int           mNumberOfItems;
    unsigned int           mByteSize;
};

struct ArrayBufferInfo_new
{
    unsigned int UseCount       = 0;
    unsigned int ByteSize       = 0;
    unsigned int NumVertices    = 0;
    unsigned int VertexByteSize = 0;
    ArrayBufferUsage Usage      = ArrayBufferUsage::STATIC_DRAW;

};

template<ArrayType AType>
struct ArrayBufferHandler
{
    inline static void Create  (GLuint & h) { glGenBuffers(1, &h); }
    inline static void Release (GLuint & h) { glDeleteBuffers(1, &h); }
    inline static void Bind    (GLuint & h) { glBindBuffer((GLenum)AType, h); }
    inline static void Unbind  (GLuint & h) { glBindBuffer((GLenum)AType, 0);  }
};

template<ArrayType AType>
class GPUArrayBuffer
{
public:
    using HandleType = Handle<GLuint, ArrayBufferHandler<AType>, ArrayBufferInfo_new >;

private:


public:
    HandleType m_Handle;

    void Bind()     { m_Handle.Bind();    }
    void Unbind()   { m_Handle.Unbind();  }
    void Release()  { m_Handle.Release(); }

    void Create(void *data, unsigned int ByteSize, unsigned int VertexSize, ArrayBufferUsage usage = ArrayBufferUsage::STATIC_DRAW)
    {
        m_Handle.Create();
        m_Handle.Bind();

        glBufferData( (GLenum)AType,
                     ByteSize,
                     data,
                     (GLenum)usage);

        auto & In = m_Handle.__GetInfo(   );
        In.ByteSize        = ByteSize;
        In.Usage           = usage;
        In.NumVertices     = ByteSize / VertexSize;
        In.VertexByteSize  = VertexSize;
    }


    inline void Render(Primitave p, int start, int numberofitems)
    {
       m_Handle.Bind();
       glDrawArrays( (GLenum)p, start,  numberofitems );
    }

    const ArrayBufferInfo_new & GetInfo() const
    {
        return m_Handle.GetInfo();
    }




};



using GPUVertexBuffer  = GPUArrayBuffer<ArrayType::ARRAY_BUFFER>;
using GPUElementBuffer = GPUArrayBuffer<ArrayType::ELEMENT_ARRAY_BUFFER>;

template<typename __Type>
class ArrayBuffer
{
public:
    using VertexType = __Type;

    template<ArrayType AType=ArrayType::ARRAY_BUFFER>
    GPUArrayBuffer<AType> ToGPU()
    {
        GPUArrayBuffer<AType> B;
        B.Create( (void*)Data.data(), sizeof(VertexType) * Data.size(), sizeof(VertexType) );
        return B;
    }

    void Insert(const VertexType & Vertex)
    {
        Data.push_back(Vertex);
    }

    std::vector<VertexType> Data;
};





//==============================================================
template<size_t Idx,class T>
constexpr size_t tuple_element_offset()
{
        return static_cast<size_t>(
                    reinterpret_cast<char*>(&std::get<Idx>(*reinterpret_cast<T*>(0))) - reinterpret_cast<char*>(0));
}



template<std::size_t I = 0, typename TupleType>
static inline typename std::enable_if<I == std::tuple_size<TupleType>::value, int>::type
EnableVertexAttribArrayFromTuple()
{
    return 0;//-sizeof(first);
}


template<std::size_t I = 0, typename TupleType>
inline typename std::enable_if< I < std::tuple_size<TupleType>::value, int>::type
EnableVertexAttribArrayFromTuple()
{

    uint ElementType;
    uint ElementsPerAttribute = 1;
    GLboolean IsNormalized    = GL_FALSE;

    using First = typename std::tuple_element<I, TupleType>::type;

    if( IS_SAME(First, ivec4) || IS_SAME(First, uvec4) || IS_SAME(First,  vec4) || IS_SAME(First, ucol4) )
        ElementsPerAttribute = 4;
    if( IS_SAME(First, ivec3) || IS_SAME(First, uvec3) || IS_SAME(First,  vec3)  || IS_SAME(First, ucol3) )
        ElementsPerAttribute = 3;
    if( IS_SAME(First, ivec2) || IS_SAME(First, ucol2) || IS_SAME(First,  vec2) || IS_SAME(First, ucol2) )
        ElementsPerAttribute = 2;


    std::string elemtype;
    if( IS_SAME(First, ivec4) || IS_SAME(First, ivec3) || IS_SAME(First, ivec2) || IS_SAME(First, int  )    )
    {
        ElementType = GL_INT;
        elemtype = "GL_INT";
    }
    if( IS_SAME(First, ucol4) || IS_SAME(First, ucol3) || IS_SAME(First, ucol2) || IS_SAME(First, unsigned char) || IS_SAME(First, ucol1)    )
    {
        ElementType = GL_UNSIGNED_BYTE;
        elemtype = "GL_UNSIGNED_BYTE";
    }
    if( IS_SAME(First, uvec4) || IS_SAME(First, uvec3) || IS_SAME(First, uvec2) || IS_SAME(First, unsigned int) )
    {
        ElementType = GL_UNSIGNED_INT;
        elemtype = "GL_UNSIGNED_INT";
    }
    if( IS_SAME(First, vec4) || IS_SAME(First, vec3) || IS_SAME(First, vec2) || IS_SAME(First, float) )
    {
        ElementType = GL_FLOAT;
        elemtype = "GL_FLOAT";
    }



    //GLA_DOUT  << "EnableVertexAttributeArray(" << (I) << ");" << std::endl;
    glEnableVertexAttribArray( I );
    glVertexAttribPointer(I, ElementsPerAttribute, ElementType, IsNormalized, sizeof(TupleType), (void*)tuple_element_offset<I, TupleType>());
    //GLA_DOUT  <<  "   size  : " << ElementsPerAttribute << std::endl;
    //GLA_DOUT  <<  "   Stride: " << sizeof(TupleType) << std::endl;
    //GLA_DOUT  <<  "   Type  : " << elemtype << std::endl;
    //GLA_DOUT  <<  "   offset: " << tuple_element_offset<I, TupleType>() << std::endl;

    EnableVertexAttribArrayFromTuple<I+1, TupleType>();
}




template <typename... AllTheRest>
inline static void EnableVertexAttribArray()
{
    EnableVertexAttribArrayFromTuple< 0, std::tuple<AllTheRest...> >();
}

//template <typename... AllTheRest>
//inline static void EnableVertexAttribArray(gla::GPUArrayBuffer & B)
//{
//    B.bind(ARRAY_BUFFER);
//    EnableVertexAttribArrayFromTuple< 0, std::tuple<AllTheRest...> >();
//    //glEnableVertexAttribArrayFromTuple< sizeof(MemoryAlignedTuple<AllTheRest...> ), AllTheRest...>( (int)0, (long)0 );
//}
//

template <typename... AllTheRest>
inline static void EnableVertexAttribute(gla::GPUArrayBuffer<ArrayType::ARRAY_BUFFER> & B)
{
    B.Bind();
    EnableVertexAttribArrayFromTuple< 0, std::tuple<AllTheRest...> >();
}


//==============================================================

typedef gla::ArrayBuffer<vec2>  v2ArrayBuffer;
typedef gla::ArrayBuffer<vec3>  v3ArrayBuffer;
typedef gla::ArrayBuffer<vec4>  v4ArrayBuffer;

typedef gla::ArrayBuffer<uvec2> u2ArrayBuffer;
typedef gla::ArrayBuffer<uvec3> u3ArrayBuffer;
typedef gla::ArrayBuffer<uvec4> u4ArrayBuffer;


}

#endif // ARRAYBUFFER_H

