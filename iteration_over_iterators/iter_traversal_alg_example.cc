#include "iter_traversal_alg.h"
#include <vector>
#include <stdexcept>


int main()
{
    std::vector<int> x = {1,2,3,4,5};

    auto func = [&](auto iter)
    {
        if (iter == x.begin())
            return;
        
        if (*(iter-1) != std::distance(x.cbegin(), iter))
        {
            throw std::runtime_error("Failed!");
        }
    };

    detail::for_each(x, func);
}