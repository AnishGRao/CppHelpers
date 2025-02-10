#include "force_explicit_template.h"


template <typename T>
void explicit_func(detail::explicit_type<T> t) {}

int main()
{
    // wont compile
    // explicit_func(5.0);
    explicit_func<double>(5.0);
}