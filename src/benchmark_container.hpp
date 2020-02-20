#pragma once
#include <array>
#include <type_traits>
#include <map>
#include <iostream>
#include <unordered_set>
#include <random>
#include "algorithms.hpp"
#include "benchmark.hpp"
#include "type_traits.hpp"

namespace vlt {

template <class Container, size_t N>
inline void benchmark_container(const std::array<int, N>& seeds,
                         const std::vector<int>& sizes,
                         std::vector<double>& result)
{
    assert(sizes.size() == result.size());

    auto compute_avg = [](size_t counter, double old_avg, double new_val) 
    {
        return (counter/(counter + 1.)) * old_avg +  (1/(counter + 1.)) * new_val;
    };

    for (size_t i = 0; i < sizes.size(); ++i) {

        std::cout << "Benchmark Size: " << sizes[i] << std::endl;

        double avg = 0.0;   // average benchmark time (s) over all seeds
        size_t counter = 0;

        for (auto seed : seeds) {
            using container_t = std::decay_t<Container>;

            std::cout << "Benchmark Seed: " << seed << std::endl;

            container_t container;

            // compute average time to ordered_insert over all seeds
            
            // generate uniform int
            std::unordered_set<int> sample;
            std::default_random_engine gen(seed);
            std::uniform_int_distribution<int> dist(0, sizes[i] * 10);
            while (sample.size() < sizes[i]) {
                sample.insert(dist(gen));
            } 

            // generate indices
            std::vector<int> permutation;
            permutation.reserve(sizes[i]);
            for (size_t j = 0; j < sizes[i]; ++j) {
                std::uniform_int_distribution<int> dist(0, sizes[i] - 1 - j);
                permutation.push_back(dist(gen));
            }

            auto time = benchmark(
                    [](Container& c, 
                       const std::unordered_set<int>& sample,
                       const std::vector<int>& permutation) 
                    {
                        // ordered insert
                        for (auto x : sample) {
                            // if map<int, int>, then create pair
                            if constexpr (is_map_v<container_t>) {
                                using pair_t = typename container_t::value_type;
                                ordered_insert(c, pair_t(x, x));
                            }
                            else {
                                ordered_insert(c, x);
                            }
                        }

                        // ordered remove
                        for (auto i : permutation) {
                            ordered_remove(c, i);
                        }
                    },
                    container, 
                    sample,
                    permutation);

            // sanity-check: every element must have been removed
            assert(container.size() == 0);
            avg = compute_avg(counter, avg, time);

            ++counter;
        }

        // update avg result
        result[i] = avg;
    } 
}

// No containers
template <class... Containers, size_t N, size_t... I>
inline void benchmark_containers(const std::array<int, N>& seeds,
                         const std::vector<int>& sizes,
                         std::vector<std::vector<double>>& result,
                         std::index_sequence<I...>)
{
    static_assert(sizeof...(Containers) == sizeof...(I));
    (result[I].resize(sizes.size()), ...);
    (benchmark_container<Containers>(seeds, sizes, result[I]), ...);
}

// At least one container
template <class... Containers, size_t N>
inline void benchmark_containers(const std::array<int, N>& seeds,
                         const std::vector<int>& sizes,
                         std::vector<std::vector<double>>& result)
{
    benchmark_containers<Containers...>(seeds, sizes, result,
            std::make_index_sequence<sizeof...(Containers)>());
}

} // namespace vlt
