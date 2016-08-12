#ifndef EXP_ARRAY_BUFFER
#define EXP_ARRAY_BUFFER


#include "buffer_base.h"


namespace gla { namespace experimental
{


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
            if( SizeOfBaseType == sizeof(std::int8_t) )  GlBaseType = GL_BYTE;
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
    //std::cout << "Current Offset      : " << (long long)offset << std::endl;
    //std::cout << "Size of next element: " << sizeof(NextElement) << std::endl;
    //std::cout << "Size of int : " << sizeof(int) << std::endl;

    #define BUFFER_OFFSET(idx) (static_cast<char*>(0) + (idx))


     glVertexAttribPointer( GLuint(I),
                            num_components ,
                            GlBaseType,
                            normalized[I] ,
                            sizeof(TupleType),
                            BUFFER_OFFSET(offset));
    glEnableVertexAttribArray(I);

 //   std::cout << "glVertexAttribPointer(" << I << ", ";
 //   std::cout << num_components << ", ";
 //   std::cout << "GL_FLOAT" << ", ";
 //   std::cout << (normalized[I] ? "GL_TRUE" : "GL_FALSE") << ", ";
 //   std::cout << sizeof(TupleType) << ", ";
 //   std::cout << (long long)offset << ");" << std::endl;

    EnableVertexAttribArrayFromTuple<I+1, TupleType>( offset + sizeof(NextElement) , normalized );
}


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
        void Draw( Primitave p, std::size_t first, std::size_t NumberOfPrimitaves)
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
        void EnableAttributes(std::size_t offset = 0) const
        {
            Bind();
            std::array<bool, std::tuple_size< std::tuple< GLM_Vec_Types...> >::value > FalseArray;
            FalseArray.fill(false);
            gla::experimental::EnableVertexAttribArrayFromTuple<0, std::tuple<GLM_Vec_Types...> > (offset, FalseArray);
        }

};




}}

#endif
