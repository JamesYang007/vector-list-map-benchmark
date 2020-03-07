#include <iostream>
#include <list>
#include <vector>
#include <map>
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

struct LargeStruct
{
    LargeStruct(int value)
        : value_(value)
    {
        static_cast<void>(value_);
        static_cast<void>(arr_);
    }

    operator int() const
    {
        return value_;
    }

private:
    int value_;
    std::array<int, 1000> arr_ = {0};
};

bool operator<(const LargeStruct& x, const LargeStruct& y)
{
    return static_cast<int>(x) < static_cast<int>(y);
}

} // namespace vlt

int main()
{
    using namespace vlt;
    std::array<int, 3> seeds = {10, 310, 212};
    std::vector<int> sizes = {
        static_cast<int>(1e1), 
        //static_cast<int>(5e1), 
        //static_cast<int>(1e2), 
        //static_cast<int>(5e2), 
        //static_cast<int>(1e3), 
        //static_cast<int>(5e3), 
        //static_cast<int>(1e4), 
        //static_cast<int>(5e4),
        //static_cast<int>(1e5),
        //static_cast<int>(2e5)
    };

    // Original benchmark
    std::vector<std::vector<double>> result(3);
    benchmark_containers<
        std::map<int, int>,
        std::vector<int>,
        std::list<int>
            >(seeds, sizes, result);

    // Put integers in a large struct
    std::vector<std::vector<double>> result2(3);
    benchmark_containers<
        std::map<LargeStruct, int>,
        std::vector<LargeStruct>,
        std::list<LargeStruct>
            >(seeds, sizes, result2);
    
    std::cout << "---Original benchmark---" << std::endl;
    std::cout << "map: ";
    print(result[0]);
    std::cout << "vector: ";
    print(result[1]);
    std::cout << "list: ";
    print(result[2]);

    std::cout << "---Large Struct benchmark---" << std::endl;
    std::cout << "map: ";
    print(result2[0]);
    std::cout << "vector: ";
    print(result2[1]);
    std::cout << "list: ";
    print(result2[2]);
    
    return 0;
}
