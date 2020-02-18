#include <iostream>
#include <list>
#include <vector>
#include <map>
#include "unique_int_uniform.hpp"
#include "algorithms.hpp"
#include "benchmark_container.hpp"

namespace vlt {

template <class Iterable>
void print(const Iterable& iterable)
{
    for (auto x : iterable)
    {
        std::cout << x << ' ';
    }
    std::cout << std::endl;
}

} // namespace vlt

int main()
{
    using namespace vlt;
    std::array<int, 3> seeds = {10, 310, 212};
    std::vector<int> sizes = {
        static_cast<int>(1e1), 
        static_cast<int>(5e1), 
        static_cast<int>(1e2), 
        static_cast<int>(5e2), 
        static_cast<int>(1e3), 
        static_cast<int>(5e3), 
        static_cast<int>(1e4), 
        static_cast<int>(5e4),
        static_cast<int>(1e5),
        static_cast<int>(2e5),
        static_cast<int>(3e5)
    };
    std::vector<std::vector<double>> result(3);
    benchmark_containers<
        std::map<int, int>,
        std::vector<int>,
        std::list<int>
            >(seeds, sizes, result);
    
    std::cout << "map: ";
    print(result[0]);
    std::cout << "vector: ";
    print(result[1]);
    std::cout << "list: ";
    print(result[2]);
    
    return 0;
}
