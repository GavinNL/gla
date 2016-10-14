#ifndef EXP_ARRAY_BUFFER
#define EXP_ARRAY_BUFFER


#include "buffer_base.h"
#include "draw.h"
#include <type_traits>
#include <cstdint>

namespace gla { namespace experimental
{

template<typename T, typename ...Tail>
struct type_size
{

};

template<typename T>
struct type_size<T>
{
    static const int max   = sizeof(T);
    static const int min   = sizeof(T);
    static const int sum   = sizeof(T);
};

template<typename T, typename Head>
struct type_size<T,Head>
{
    static const int max   = sizeof(T)>sizeof(Head) ? sizeof(T) : sizeof(Head);
    static const int min   = sizeof(T)<sizeof(Head) ? sizeof(T) : sizeof(Head);
    static const int sum   = sizeof(T)+sizeof(Head);
};

template<typename T, typename Head, typename ...Tail>
struct type_size<T,Head,Tail...>
{
    static const int max = sizeof(T) > type_size<Head,Tail...>::max ? sizeof(T) : type_size<Head,Tail...>::max;
    static const int min = sizeof(T) < type_size<Head,Tail...>::min ? sizeof(T) : type_size<Head,Tail...>::min;
    static const int sum = sizeof(T) + type_size<Head,Tail...>::sum;

};

template<typename ...VecTypes>
struct EnableAttributes
{
    static void Enable(GLuint i, std::int32_t offset, std::uint32_t NormalizeFlags, int Stride_Leave_as_Default=-1)
    {
    }
};

template<>
struct EnableAttributes<>
{
    static void Enable(GLuint i, std::int32_t offset, std::uint32_t NormalizeFlags, int Stride_Leave_as_Default=-1)
    {
        i;offset;NormalizeFlags;Stride_Leave_as_Default;
    }
};

template<typename FirstType, typename ...VecTypes>
struct EnableAttributes<FirstType, VecTypes...>
{
    static void Enable(GLuint i, std::int32_t offset, std::uint32_t NormalizeFlags, int Stride_Leave_as_Default=-1)
    {
        #define BUFFER_OFFSET(idx) (static_cast<char*>(0) + (idx))

        const int total_size     = type_size<FirstType, VecTypes...>::sum;

        static_assert(
            ( !std::is_same< FirstType, double       >::value )   ||
            ( !std::is_same< FirstType, glm::dvec2    >::value )   ||
            ( !std::is_same< FirstType, glm::dvec3    >::value )   ||
            ( !std::is_same< FirstType, glm::dvec4    >::value )   ||
            ( !std::is_same< FirstType, float        >::value )   ||
            ( !std::is_same< FirstType, glm::vec2    >::value )   ||
            ( !std::is_same< FirstType, glm::vec3    >::value )   ||
            ( !std::is_same< FirstType, glm::vec4    >::value )   ||
            ( !std::is_same< FirstType, std::int32_t >::value )   ||
            ( !std::is_same< FirstType, glm::ivec2   >::value )   ||
            ( !std::is_same< FirstType, glm::ivec3   >::value )   ||
            ( !std::is_same< FirstType, glm::ivec4   >::value )   ||
            ( !std::is_same< FirstType, std::uint32_t>::value )   ||
            ( !std::is_same< FirstType, glm::uvec2   >::value )   ||
            ( !std::is_same< FirstType, glm::uvec3   >::value )   ||
            ( !std::is_same< FirstType, glm::uvec4   >::value )   ||
            ( !std::is_same< FirstType, std::int16_t >::value )   ||
            ( !std::is_same< FirstType, glm::i16vec2 >::value )   ||
            ( !std::is_same< FirstType, glm::i16vec3 >::value )   ||
            ( !std::is_same< FirstType, glm::i16vec4 >::value )   ||
            ( !std::is_same< FirstType, std::uint16_t>::value )   ||
            ( !std::is_same< FirstType, glm::u16vec2 >::value )   ||
            ( !std::is_same< FirstType, glm::u16vec3 >::value )   ||
            ( !std::is_same< FirstType, glm::u16vec4 >::value )   ||
            ( !std::is_same< FirstType, std::int8_t  >::value )   ||
            ( !std::is_same< FirstType, glm::i8vec2  >::value )   ||
            ( !std::is_same< FirstType, glm::i8vec3  >::value )   ||
            ( !std::is_same< FirstType, glm::i8vec4  >::value )   ||
            ( !std::is_same< FirstType, std::uint8_t >::value )   ||
            ( !std::is_same< FirstType, glm::u8vec2  >::value )   ||
            ( !std::is_same< FirstType, glm::u8vec3  >::value )   ||
            ( !std::is_same< FirstType, glm::u8vec4  >::value )  ,
            "Unknown data type. Valid types are:  float ,glm::vec2 ,glm::vec3 ,glm::vec4 ,std::int32_t ,glm::ivec2 ,glm::ivec3 ,glm::ivec4 ,std::uint32_t ,glm::uvec2 ,glm::uvec3 ,glm::uvec4 ,std::int16_t ,glm::i16vec2 ,glm::i16vec3 ,glm::i16vec4 ,std::uint16_t ,glm::u16vec2 ,glm::u16vec3 ,glm::u16vec4 ,std::int8_t ,glm::i8vec2 ,glm::i8vec3 ,glm::i8vec4 ,std::uint8_t ,glm::u8vec2 ,glm::u8vec3 ,glm::u8vec4" );


        Stride_Leave_as_Default = Stride_Leave_as_Default==-1 ? total_size : Stride_Leave_as_Default;

     //   std::cout << "Enabling attribute        : " << i << std::endl;
     //   std::cout << "   totalsize              : " << Stride_Leave_as_Default << std::endl;
     //   std::cout << "        size              : " << sizeof(FirstType) << std::endl;
     //   std::cout << "        offset            : " << offset << std::endl;
     //   std::cout << "        normal            : " << ((NormalizeFlags>>i)&01)<< std::endl;
     //   std::cout << "        num_components    : " << num_components<< std::endl;
     //   std::cout << "        base type         : " << GlBaseType << std::endl;

        // compile-time if statements, will be optimized
        if( std::is_same< FirstType, double>::value )        glVertexAttribPointer( i,  1 , GL_DOUBLE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::dvec2>::value )     glVertexAttribPointer( i, 2 , GL_DOUBLE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::dvec3>::value )     glVertexAttribPointer( i, 3 , GL_DOUBLE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::dvec4>::value )     glVertexAttribPointer( i, 4 , GL_DOUBLE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));

        if( std::is_same< FirstType, float>::value )        glVertexAttribPointer( i,  1 , GL_FLOAT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::vec2>::value )     glVertexAttribPointer( i, 2 , GL_FLOAT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::vec3>::value )     glVertexAttribPointer( i, 3 , GL_FLOAT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::vec4>::value )     glVertexAttribPointer( i, 4 , GL_FLOAT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));

        if( std::is_same< FirstType, std::int32_t>::value )  glVertexAttribPointer( i, 1 ,          GL_INT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::ivec2>::value )    glVertexAttribPointer( i, 2 ,          GL_INT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::ivec3>::value )    glVertexAttribPointer( i, 3 ,          GL_INT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::ivec4>::value )    glVertexAttribPointer( i, 4 ,          GL_INT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, std::uint32_t>::value ) glVertexAttribPointer( i, 1 , GL_UNSIGNED_INT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::uvec2>::value )    glVertexAttribPointer( i, 2 , GL_UNSIGNED_INT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::uvec3>::value )    glVertexAttribPointer( i, 3 , GL_UNSIGNED_INT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::uvec4>::value )    glVertexAttribPointer( i, 4 , GL_UNSIGNED_INT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));

        if( std::is_same< FirstType, std::int16_t>::value )    glVertexAttribPointer( i, 1 ,          GL_SHORT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i16vec2>::value )    glVertexAttribPointer( i, 2 ,          GL_SHORT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i16vec3>::value )    glVertexAttribPointer( i, 3 ,          GL_SHORT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i16vec4>::value )    glVertexAttribPointer( i, 4 ,          GL_SHORT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, std::uint16_t>::value )   glVertexAttribPointer( i, 1 , GL_UNSIGNED_SHORT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u16vec2>::value )    glVertexAttribPointer( i, 2 , GL_UNSIGNED_SHORT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u16vec3>::value )    glVertexAttribPointer( i, 3 , GL_UNSIGNED_SHORT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u16vec4>::value )    glVertexAttribPointer( i, 4 , GL_UNSIGNED_SHORT, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));

        if( std::is_same< FirstType, std::int8_t>::value )    glVertexAttribPointer( i, 1 ,          GL_BYTE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i8vec2>::value )    glVertexAttribPointer( i, 2 ,          GL_BYTE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i8vec3>::value )    glVertexAttribPointer( i, 3 ,          GL_BYTE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::i8vec4>::value )    glVertexAttribPointer( i, 4 ,          GL_BYTE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, std::uint8_t>::value )   glVertexAttribPointer( i, 1 , GL_UNSIGNED_BYTE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u8vec2>::value )    glVertexAttribPointer( i, 2 , GL_UNSIGNED_BYTE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u8vec3>::value )    glVertexAttribPointer( i, 3 , GL_UNSIGNED_BYTE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));
        if( std::is_same< FirstType, glm::u8vec4>::value )    glVertexAttribPointer( i, 4 , GL_UNSIGNED_BYTE, ((NormalizeFlags>>i)&01) , Stride_Leave_as_Default, BUFFER_OFFSET(offset));


     //   glVertexAttribPointer( i,
     //                          num_components ,
     //                          GlBaseType,
     //                          ((NormalizeFlags>>i)&01) ,
     //                          Stride_Leave_as_Default,
     //                          BUFFER_OFFSET(offset));

        glEnableVertexAttribArray(i);

        EnableAttributes<VecTypes...>::Enable(i+1, offset + sizeof(FirstType) , NormalizeFlags , Stride_Leave_as_Default);
    }
};

/*
template<std::size_t I = 0, typename TupleType, std::size_t tuplesize>
static inline typename std::enable_if<I == std::tuple_size<TupleType>::value, int>::type
EnableVertexAttribArrayFromTuple(std::int32_t offset, const std::array<bool, tuplesize > & normalized)
{
    return 0;
}




template<std::size_t I = 0, typename TupleType, std::size_t tuplesize>
static inline typename std::enable_if< I < std::tuple_size<TupleType>::value, int>::type
EnableVertexAttribArrayFromTuple( std::int32_t offset, const std::array<bool, tuplesize > & normalized)
{
    using NextElement = typename std::tuple_element<I, TupleType>::type;
    using BaseType    = typename NextElement::value_type;

    int num_components = sizeof( NextElement ) / sizeof( typename NextElement::value_type);

    const int SizeOfBaseType = sizeof(BaseType);

    GLenum GlBaseType;

    if( std::is_floating_point<BaseType>::value )
    {

        if( SizeOfBaseType == sizeof(std::int32_t) )
        {
            GlBaseType = GL_FLOAT;
        }
        else if( SizeOfBaseType == sizeof(std::int64_t) )
        {
            GlBaseType = GL_DOUBLE;
        } else {
            throw std::runtime_error("Unknown format! Make sure to use only the glm:vec_types");
        }
    }
    else
    {
        if( std::is_signed<BaseType>::value )
        {
            if( SizeOfBaseType == sizeof(std::int8_t) )       GlBaseType = GL_BYTE;
            else if( SizeOfBaseType == sizeof(std::int16_t) ) GlBaseType = GL_SHORT;
            else if( SizeOfBaseType == sizeof(std::int32_t) ) GlBaseType = GL_INT;
            else {
                throw std::runtime_error("Unknown format! Make sure to use only the glm:vec_types");
            }
        } else {
            if( SizeOfBaseType == sizeof(std::int8_t) )  GlBaseType = GL_UNSIGNED_BYTE;
            if( SizeOfBaseType == sizeof(std::int16_t) ) GlBaseType = GL_UNSIGNED_SHORT;
            if( SizeOfBaseType == sizeof(std::int32_t) ) GlBaseType = GL_UNSIGNED_INT;
            else {
                throw std::runtime_error("Unknown format! Make sure to use only the glm:vec_types");
            }
        }

    }

    #define BUFFER_OFFSET(idx) (static_cast<char*>(0) + (idx))


     glVertexAttribPointer( GLuint(I),
                            num_components ,
                            GlBaseType,
                            normalized[I] ,
                            sizeof(TupleType),
                            BUFFER_OFFSET(offset));
     glEnableVertexAttribArray(I);

    return EnableVertexAttribArrayFromTuple<I+1, TupleType>( offset + sizeof(NextElement) , normalized );

}
*/
//============================


class ArrayBuffer : public Buffer
{
    public:
        static BufferBindTarget const BindTarget = BufferBindTarget::ARRAY_BUFFER;

        ArrayBuffer() : Buffer() {}
        ArrayBuffer(std::size_t size_in_bytes) : Buffer(size_in_bytes)
        {}

        template<typename VertexData>
        ArrayBuffer( const std::vector<VertexData> & data, BufferUsage usage = BufferUsage::STATIC_DRAW) : Buffer(data, usage)
        {}

        /**
         * @brief Bind
         * Binds this handle as the current GL_ARRAY_BUFFER
         */
        void Bind() const
        {
            Buffer::Bind( *this, BindTarget);
        }

        /**
         * @brief Unbind
         * Unbinds any current GL_ARRAY_BUFFER
         */
        void Unbind() const
        {
            Buffer::Unbind(  BindTarget );
        }

        template<bool BindFirst=true>
        /**
         * @brief Draw
         * @param p - the pimitave type (LINES, TRIANGLES, TRIANGLE_FAN, etc)
         * @param first - the index of the first
         * @param NumberOfVertices - number of vertices to render.
         *
         * Use the BindFirst=false template paramter to not automatically bind before each call.
         * This is useful if are making a more optimized rendering engine.
         */
        void Draw( Primitave p,  std::size_t NumberOfVertices, std::size_t first_vertex) const
        {
            if(BindFirst) Bind();
            gla::core::DrawArrays( p,  NumberOfVertices,  first_vertex );
            //glDrawArrays( static_cast<GLenum>(p),  static_cast<GLint>(first_vertex),  static_cast<GLsizei>(NumberOfVertices) );
        }

        /**
         * @brief EnableAttributes
         * @param NormalizeFlags - flags which indicate which attributes should be normalized.
         *                         eg:  NormalizeFlags::_0 |  NormalizeFlags::_3
         *
         * Tells OpenGL how the attributes in the array_buffer should be read as, eg:
         *
         * Given an array_buffer, where each vertex is composed of position(vec3), uv_coords(vec2), normal(vec3), and colour(vec4)
         * where the normal must be normalized
         *
         * Then we can tell openGL to read the array buffer by calling the following function.
         *
         * EnableAttributes<glm::vec3, glm::vec2, glm::vec3, glm::vec4>(NormalizeFlags::_2);
         */
        template <typename... GLM_Vec_Types>
        void EnableAttributes( NormalizeFlags normalizeFlags = NormalizeFlags::none ) const
        {
            Bind();
            gla::experimental::EnableAttributes<GLM_Vec_Types...>::Enable(0, 0, normalizeFlags._flags);
        }

};




}}

#endif
