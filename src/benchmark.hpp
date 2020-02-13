#pragma once
#include <chrono>

namespace vlt {

// Times function when called with args...
// Returns a double representing the time in seconds
template <class Function, class... Args>
inline double time_routine(Function function, Args&&... args)
{
    constexpr double nano = 1e-9;
    auto begin = std::chrono::system_clock::now();
    function(std::forward<Args>(args)...);
    auto end = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() * nano;
}

// Returns the time it took to benchmark ordered_insert
// generator must have already generated the sample.
template <class Container, class GeneratorType, class Algorithm>
inline double benchmark(Container& container, const GeneratorType& generator, Algorithm f)
{
    auto to_benchmark = [&]()
    {
        for (auto x : generator) {
            f(container, x);
        } 
    };
    return time_routine(to_benchmark);
}

} // namespace vlt
