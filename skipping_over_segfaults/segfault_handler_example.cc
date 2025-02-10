#include "segfault_handler.h"
#include <string>
#include <cassert>
#include <iostream>

// Bad cast.
struct local_struct {
    std::string local_string = "initial_value";
    void operator() (std::string s = "hello") const {
        const_cast<local_struct*>(this)->local_string = s; 
    } 
};

int main()
{
    // Throws a segfault, recovers.
    assert(detail::segfault(
        [](){int *ptr; *ptr+=5;}
    ));

    local_struct s;

    assert(detail::segfault(s));
    assert(s.local_string == "initial_value");

    s.local_string = "new_value";
    assert(detail::segfault(s));
    assert(s.local_string == "new_value");
}