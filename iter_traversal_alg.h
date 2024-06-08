/**
 * What I would love is a C++ stl algorithm (transform/for_each/generate) that has 
 *  the ability to traverse based on iterators (understanding location), based on idx.
 * 
 * For example, if we have an array we wish to generate based on the index of a separate vector, and do 
 *  logic based on the adjacent indices of said vector.
 * 
 * Also, the C++ algorithms *should* be able to take a container without calling begin/end on it. Which this does.
 * 
 * C++17 and above Compatible
 * 
 * Anish G. Rao, 05/31/2024
 */
#include <experimental/type_traits>
#include <iterator>
namespace detail
{
    template<typename T>
    using iterator_t = typename T::iterator;
    
    template <class Container, typename Func, typename = typename std::enable_if_t<std::experimental::is_detected_v<iterator_t, Container>>>
    void for_each(Container const & container, Func func)
    {
        for (auto iterator = container.cbegin(); iterator!=container.cend(); iterator=std::next(iterator))
        {
            func(iterator);
        }
    }
    
    template <class Container, typename Func, typename = typename std::enable_if_t<std::experimental::is_detected_v<iterator_t, Container>>>
    void for_each(Container & container, Func func)
    {
        for (auto iterator = container.begin(); iterator!=container.end(); iterator=std::next(iterator))
        {
            func(iterator);
        }
    }
}

