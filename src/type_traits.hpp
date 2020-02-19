#pragma once
#include <type_traits>
#include <map>

namespace vlt {
namespace details {

template <class T>
struct is_map : std::false_type
{};

template <class T, class U>
struct is_map<std::map<T, U>> : std::true_type
{};

} // namespace details

template <class T>
constexpr inline bool is_map_v = details::is_map<T>::value;

} // namespace vlt
