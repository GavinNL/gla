#ifndef EXP_ARRAY_BUFFER
#define EXP_ARRAY_BUFFER


#include "buffer_base.h"


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
    static void Enable(GLuint i, std::int32_t offset, std::uint32_t NormalizeFlags, int Size_Leave_as_Default=-1)
    {
    }
};

template<>
struct EnableAttributes<>
{
    static void Enable(GLuint i, std::int32_t offset, std::uint32_t NormalizeFlags, int Size_Leave_as_Default=-1)
    {
    }
};

template<typename FirstType, typename ...VecTypes>
struct EnableAttributes<FirstType, VecTypes...>
{
    static void Enable(GLuint i, std::int32_t offset, std::uint32_t NormalizeFlags, int Size_Leave_as_Default=-1)
    {
        #define BUFFER_OFFSET(idx) (static_cast<char*>(0) + (idx))
        using BaseType    = typename FirstType::value_type;

        const int total_size     = type_size<FirstType, VecTypes...>::sum;
        const int num_components = sizeof(FirstType) / sizeof(BaseType);
        GLenum GlBaseType;

        if( std::is_floating_point<BaseType>::value )
        {
            if     ( sizeof(BaseType) == sizeof(std::int32_t) ) GlBaseType = GL_FLOAT;
            else if( sizeof(BaseType) == sizeof(std::int64_t) ) GlBaseType = GL_DOUBLE;
            else throw std::runtime_error("Unknown format! Make sure to use only the glm:vec_types");
        }
        else
        {
            if( std::is_signed<BaseType>::value )
            {
                switch(sizeof(BaseType))
                {
                case sizeof(std::int8_t)  : GlBaseType = GL_BYTE;  break;
                case sizeof(std::int16_t) : GlBaseType = GL_SHORT; break;
                case sizeof(std::int32_t) : GlBaseType = GL_INT;   break;
                default:
                    throw std::runtime_error("Unknown format! Make sure to use only the glm:vec_types");
                }
            }
            else
            {
                switch(sizeof(BaseType))
                {
                     case sizeof(std::uint8_t)  : GlBaseType = GL_UNSIGNED_BYTE;  break;
                     case sizeof(std::uint16_t) : GlBaseType = GL_UNSIGNED_SHORT; break;
                     case sizeof(std::uint32_t) : GlBaseType = GL_UNSIGNED_INT;   break;
                     default:
                         throw std::runtime_error("Unknown format! Make sure to use only the glm:vec_types");
                }
            }
        }

        Size_Leave_as_Default = Size_Leave_as_Default==-1 ? total_size : Size_Leave_as_Default;

        std::cout << "Enabling attribute        : " << i << std::endl;
        std::cout << "   totalsize              : " << Size_Leave_as_Default << std::endl;
        std::cout << "        size              : " << sizeof(FirstType) << std::endl;
        std::cout << "        offset            : " << offset << std::endl;
        std::cout << "        normal            : " << ((NormalizeFlags>>i)&01)<< std::endl;
        std::cout << "        num_components    : " << num_components<< std::endl;
        std::cout << "        base type         : " << GlBaseType << std::endl;


        glVertexAttribPointer( i,
                               num_components ,
                               GlBaseType,
                               ((NormalizeFlags>>i)&01) ,
                               Size_Leave_as_Default,
                               BUFFER_OFFSET(offset));

        glEnableVertexAttribArray(i);

        EnableAttributes<VecTypes...>::Enable(i+1, offset + sizeof(FirstType) , NormalizeFlags , Size_Leave_as_Default);
    }
};

// Terminating case
template<std::size_t I = 0, typename TupleType, std::size_t tuplesize>
static inline typename std::enable_if<I == std::tuple_size<TupleType>::value, int>::type
EnableVertexAttribArrayFromTuple(std::int32_t offset, const std::array<bool, tuplesize > & normalized)
{
    return 0;
}



// A recursive template function
// That will enable the vertex attribute based on the GLM types in a
// tuple.
// Do not call this function directly.
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

//============================


class Array_Buffer : public Buffer<BufferBindTarget::ARRAY_BUFFER>
{
    public:
        Array_Buffer() : Buffer<BufferBindTarget::ARRAY_BUFFER>()
        {

        }

        template<typename VertexData>
        Array_Buffer( const std::vector<VertexData> & data, BufferUsage usage = BufferUsage::STATIC_DRAW) : Buffer(data, usage)
        {
        }

        template<bool BindFirst=true>
        void Draw( Primitave p, std::size_t first, std::size_t NumberOfPrimitaves) const
        {
            if(BindFirst) Bind();

            glDrawArrays( static_cast<GLenum>(p),  static_cast<GLint>(first),  static_cast<GLsizei>(NumberOfPrimitaves) );
        }


        template<typename T>
        void operator << (const T & structure)
        {
            Bind();
            Buffer::CopyData(structure);
        }

        /**
         * @brief EnableAttributes
         * @param normalized
         *
         * Enable vertex attributes using GLM types. For example:
         *
         *
         * EnableAttributes<glm::vec3, glm::vec2, glm::vec3>( {false, false, true}  )
         *
         *
         */
        template <typename... GLM_Vec_Types>
        void EnableAttributes(  const std::array<bool, std::tuple_size< std::tuple< GLM_Vec_Types...> >::value > & normalized ) const
        {
            Bind();
            gla::experimental::EnableVertexAttribArrayFromTuple<0, std::tuple<GLM_Vec_Types...> > (0, normalized);
        }

        /**
         * @brief EnableAttributes
         * Enables attributes  assuming all attributes are unnormalized
         */
        template <typename... GLM_Vec_Types>
        void EnableAttributes( std::size_t offset = 0) const
        {
            Bind();
            std::array<bool, std::tuple_size< std::tuple< GLM_Vec_Types...> >::value > FalseArray;
            FalseArray.fill(false);
            //gla::experimental::EnableAttributes<
            gla::experimental::EnableAttributes<GLM_Vec_Types...>::Enable(0, 0, 0);
            //gla::experimental::EnableVertexAttribArrayFromTuple<0, std::tuple<GLM_Vec_Types...> > (offset, FalseArray);
        }

        std::size_t m_Number_Of_Items;

};




}}

#endif
