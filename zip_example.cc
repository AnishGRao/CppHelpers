#include "zip.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>

int main()
{
    std::vector<int> a = {1,2,3,4,5};
    std::vector<double> b = {6.0,7.0,8.0,9.0,10.0,0.0};
    std::map<int, int> c = {
        {1,2},
        {3,4},
        {5,6},
        {7,8},
        {20,20}
    };

    // remember -- unoredered map is just that, unordered. 
    //  do not depend on order for iteration!
    std::unordered_map<int, std::string> d = {
        {1,"1"},
        {3,"3"},
        {5,"5"},
        {9,"9"},
        {7,"7"}
    };

    std::set<std::string> e = {"1", "3", "5", "9", "7"};

    using namespace detail;
    for (auto const & [ivec, dvec, iimap, istrumap, strset] : detail::zip(a, b, c, d, e))
    {
        ivec = 3;
        auto [iimap_k, iimap_v] = iimap;
        auto [istrumap_k, istrumap_v] = istrumap; 
        std::cout << ivec << "\t" 
                    << dvec << "\t" 
                    << iimap_k << "\t" << iimap_v << "\t"
                    << istrumap_k << "\t" << istrumap_v << "\t"
                    << strset << "\t"
                    <<  std::endl;
    }

    for (auto i : a) std::cout << i << std::endl;
}