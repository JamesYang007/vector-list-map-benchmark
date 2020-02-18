#pragma once
#include <iterator>
#include <map>

namespace vlt {

// Inserts x into container such that container is ordered.
// Algorithm explicitly uses iterators to loop and find the correct position to insert.
template <class Container>
inline void ordered_insert(Container& container,
                    typename std::decay<Container>::type::value_type x)
{
    auto it = container.begin();
    // find the position before which to insert x 
    for (; it != container.end(); ++it) {
        if (*it > x) {
            break;
        }
    }
    // insert before where it points
    container.insert(it, x);
}

// Specialization: map<int, int>
// Simply do insert - no O(n) search
template <>
inline void ordered_insert<std::map<int, int>>(
        std::map<int, int>& container,
        typename std::map<int, int>::value_type x)
{
    container.insert(x);
}

// Remove x from container.
// Algorithm explicitly uses iterators to loop and find the correct position to remove.
template <class Container>
inline void ordered_remove(Container& container, size_t x)
{
    auto it = container.begin();
    for (; x > 0; --x, ++it);
    container.erase(it);
}

} // namespace vlt
