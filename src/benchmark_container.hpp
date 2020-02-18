#pragma once
#include <array>
#include <type_traits>
#include <map>
#include <iostream>
#include "unique_int_uniform.hpp"
#include "algorithms.hpp"
#include "benchmark.hpp"

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

            std::cout << "Benchmark Seed: " << seed << std::endl;

            Container container;

            // compute average time to ordered_insert over all seeds
            
            // generate uniform int
            UniqueIntUniform generator(sizes[i], seed);
            generator.generate();

            // generate indices
            std::default_random_engine gen(seed);
            std::vector<int> permutation;
            permutation.reserve(sizes[i]);
            for (size_t j = 0; j < sizes[i]; ++j) {
                std::uniform_int_distribution<int> dist(0, sizes[i] - 1 - j);
                permutation.push_back(dist(gen));
            }

            auto time = benchmark(
                    [](Container& c, 
                       const UniqueIntUniform& generator,
                       const std::vector<int>& permutation) 
                    {
                        // ordered insert
                        for (auto x : generator) {
                            // if map<int, int>, then create pair
                            if constexpr (std::is_same_v<
                                    std::decay_t<Container>, std::map<int, int>
                                    >) {
                                ordered_insert(c, std::make_pair(x, x));
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
                    generator,
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
