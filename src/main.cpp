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
        static_cast<int>(1e4), 
        static_cast<int>(2e4), 
        static_cast<int>(3e4),
        static_cast<int>(4e4), 
        static_cast<int>(5e4)
    };
    std::vector<std::vector<double>> ins_result(2);
    std::vector<std::vector<double>> rem_result(2);
    benchmark_containers<
        std::vector<int>,
        std::list<int>
            >(seeds, sizes, ins_result, rem_result);
    
    print(ins_result[0]);
    print(rem_result[0]);
    print(ins_result[1]);
    print(rem_result[1]);
    
    return 0;
}
