#ifndef GLA_MEMORYALIGNEDTUPLE_H
#define GLA_MEMORYALIGNEDTUPLE_H


namespace gla
{


template<std::size_t I = 0, typename... Tp>
constexpr static inline typename std::enable_if<I == -1, int>::type
  tuplesize()
{
    using first   = typename std::tuple_element<0, std::tuple<Tp...> >::type;
    return 0;//-sizeof(first);
}

template<std::size_t I = 0, typename... Tp>
constexpr inline typename std::enable_if<I < sizeof...(Tp), int>::type
  tuplesize()
  {

    using element = typename std::tuple_element<I, std::tuple<Tp...> >::type;

    return sizeof(element) + tuplesize<I - 1, Tp...>();
  }

template<std::size_t I = 0, typename... Tp>
constexpr inline typename std::enable_if<I < sizeof...(Tp), int>::type
  tupleoffset()
  {

    using element = typename std::tuple_element<I, std::tuple<Tp...> >::type;

    return tuplesize<I, Tp...>() - sizeof(element);
  }


template<typename... Tp>
struct MemoryAlignedTuple
{
    unsigned char data[ tuplesize<sizeof...(Tp)-1,Tp...>()] = {0};

    template< std::size_t I>
    typename std::tuple_element<I, std::tuple<Tp...> >::type&
        attr()
        {
            using element = typename std::tuple_element<I, std::tuple<Tp...> >::type;

            return *reinterpret_cast<element*>( data + tupleoffset<I, Tp...>());
        }


    MemoryAlignedTuple()
    {

    }

    MemoryAlignedTuple(const Tp&&... u)
    {
        set( std::forward<const Tp>(u)... );
    }

    //==================
    template<typename a>
    void set(const a & A)
    {
        attr<0>() = A;
    }

    template<typename a,
             typename b>
    void set(const a & A, const b & B)
    {
        attr<0>() = A;
        attr<1>() = B;
    }

    template<typename a,
             typename b,
             typename c>
    void set(const a & A, const b & B, const c & C)
    {
        attr<0>() = A;
        attr<1>() = B;
        attr<2>() = C;
    }

    template<typename a,
             typename b,
             typename c,
             typename d>
    void set(const a & A, const b & B, const c & C, const d & D)
    {
        attr<0>() = A;
        attr<1>() = B;
        attr<2>() = C;
        attr<3>() = D;
    }

    template<typename a,
             typename b,
             typename c,
             typename d,
             typename e>
    void set(const a & A, const b & B, const c & C, const d & D, const e & E)
    {
        attr<0>() = A;
        attr<1>() = B;
        attr<2>() = C;
        attr<3>() = D;
        attr<4>() = E;
    }

    template<typename a,
             typename b,
             typename c,
             typename d,
             typename e,
             typename f>
    void set(const a & A, const b & B, const c & C, const d & D, const e & E, const f & F)
    {
        attr<0>() = A;
        attr<1>() = B;
        attr<2>() = C;
        attr<3>() = D;
        attr<4>() = E;
        attr<5>() = F;
    }

};



}

#endif // MEMORYALIGNEDTUPLE_H

