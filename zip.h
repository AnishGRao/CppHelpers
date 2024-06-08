/**
 * This is an algorithm to zip multiple containers, 
 *  and iterate over them simultaneously. I also want this to be type-agnostic, 
 *  so different-typed containers can be iterated over, up to the length of the first container.
 * 
 * Usage -- use this in a regular range-based for loop, with structured bindings.
 *  using namespace detail;
 *  for (auto [a, b, c] : detail::zip(d,e,f)) { ... }
 * 
 * You must use the detail namespace, or move everything (other than the std:: namespaced functions) to non-namespace.
 * All reference are mutable by default, even with const& -- so be wary of that.
 * Don't try to cv-qualify (other than volatile) -- it wont matter.
 * 
 * C++17 and above Compatible
 * 
 * Anish G. Rao, 05/31/2024
 */

#include <experimental/type_traits>
#include <iterator>
#include <tuple>
namespace detail
{
    template <typename ... T>
    auto operator++(std::tuple<T...> & iterator_tuple)
    {
        std::apply([](auto & ... args){((++args), ...); }, iterator_tuple);
        return iterator_tuple;
    }
    
    template <typename ... T>
    auto operator*(std::tuple<T...> & iterator_tuple)
    {
        auto _ = [] (T & ... args){ return std::tie(*args...); };
        return std::apply(_, iterator_tuple);
    }

    template <typename ... T>
    auto operator*(std::tuple<T...> const & iterator_tuple)
    {
        auto _ = [] (T & ... args){ return std::tie(*args...); };
        return std::apply(_, iterator_tuple);
    }

    template <typename ... T>
    auto && operator*(std::tuple<T...> && iterator_tuple)
    {           
        auto _ = [] (T && ... args)  { return std::tie(*args...); };
        return std::apply(_, std::forward(iterator_tuple));         
    }

    template <typename ... T>
    struct tuple_override
    {
        std::tuple<T...> tuple_container;
        std::tuple<typename T::iterator...> tuple_begin_container;
        std::tuple<typename T::iterator...> tuple_end_container;

        // Get accessors.
        template <size_t Idx>
        std::tuple_element_t<Idx, tuple_override<T...>> get()
        {
            return std::get<Idx>(tuple_container);
        }
        template <size_t Idx>
        std::tuple_element_t<Idx, tuple_override<T...>> const get() const
        {
            return std::get<Idx>(tuple_container);
        }

        template <size_t Idx>
        std::tuple_element_t<Idx, tuple_override<T...>> & get() &
        {
            return std::get<Idx>(tuple_container);
        }
        template <size_t Idx>
        std::tuple_element_t<Idx, tuple_override<T...>> const & get() const &
        {
            return std::get<Idx>(tuple_container);
        }

        template <size_t Idx>
        std::tuple_element_t<Idx, tuple_override<T...>> & get() &&
        {
            return std::move(std::get<Idx>(tuple_container));
        }
        template <size_t Idx>
        std::tuple_element_t<Idx, tuple_override<T...>> const& get() const&&
        {
            return std::move(std::get<Idx>(tuple_container));
        }
    };

    template<typename... Args>
    auto tie_to_tup( Args&... args )
    {
        // We are going to limit the iteration to the length of the first container
        //  (Also might be working with a structure w/o randomaccessiterator specified, so std::next required)
        auto _ = [first_container_size = std::get<0>(std::forward_as_tuple(args...)).size()] (Args & ... args) 
            { return std::make_tuple(std::next(args.begin(), first_container_size)...); };
        
        tuple_override<Args...> ret = { .tuple_container = std::tie(args...), 
                                        .tuple_begin_container = std::tuple<typename Args::iterator...>(args.begin()...), 
                                        .tuple_end_container = std::apply(_, std::forward_as_tuple(args...))};
        return ret;
    }

    template <typename ... T>
    auto begin(tuple_override<T...> & input_tuple)
    {
        return input_tuple.tuple_begin_container;
    }

    template <typename ... T>
    auto end(tuple_override<T...> & input_tuple)
    {
        return input_tuple.tuple_end_container;
    }
    
    template <typename...T>
    auto zip(T & ...containers)
    {
        return tie_to_tup(containers...);
    }
}

// We want structured bindings for tuple_override.
// To learn more, read Raymond Chen's blog post: https://devblogs.microsoft.com/oldnewthing/20201015-00/?p=104369
namespace std
{
    template <typename ... T>
    struct tuple_size<detail::tuple_override<T...>>
        : integral_constant<size_t, sizeof...(T)>
    {};

    template <size_t Idx, typename ... T>
    struct tuple_element<Idx, detail::tuple_override<T...>>
        : tuple_element<Idx, tuple<T...>>
    {};
}