#pragma once
#include <chrono>

namespace vlt {

// Times function when called with args...
// Returns a double representing the time in seconds
template <class Function, class... Args>
inline double benchmark(Function function, Args&&... args)
{
    constexpr double nano = 1e-9;
    auto begin = std::chrono::system_clock::now();
    function(std::forward<Args>(args)...);
    auto end = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() * nano;
}

} // namespace vlt
