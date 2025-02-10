#pragma once

namespace detail
{
    template <typename T> struct type_abstraction { using type=T; };
    template <typename T> using explicit_type = typename type_abstraction<T>::type;
}