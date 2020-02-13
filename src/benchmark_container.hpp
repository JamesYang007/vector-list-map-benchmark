#pragma once
#include <array>
#include <type_traits>
#include <iostream>
#include "unique_int_uniform.hpp"
#include "algorithms.hpp"
#include "benchmark.hpp"

namespace vlt {

template <class Container, size_t N>
inline void benchmark_container(const std::array<int, N>& seeds,
                         const std::vector<int>& sizes,
                         std::vector<double>& ins_result,
                         std::vector<double>& rem_result)
{
    assert(sizes.size() == ins_result.size());
    assert(sizes.size() == rem_result.size());

    auto compute_avg = [](size_t counter, double old_avg, double new_val) 
    {
        return (counter/(counter + 1.)) * old_avg +  (1/(counter + 1.)) * new_val;
    };

    for (size_t i = 0; i < sizes.size(); ++i) {

        std::cout << "Benchmark Size: " << sizes[i] << std::endl;

        double ins_avg = 0.0;   // average benchmark time (s) over all seeds
        double rem_avg = 0.0;   // average benchmark time (s) over all seeds
        size_t counter = 0;

        for (auto seed : seeds) {

            std::cout << "Benchmark Seed: " << seed << std::endl;

            Container container;

            // compute average time to ordered_insert over all seeds
            UniqueIntUniform generator(sizes[i], seed);
            generator.generate();
            auto time = benchmark(container, generator, 
                    [](Container& c, typename std::decay<Container>::type::value_type x) {
                        ordered_insert(c, x);
                    }
                    );
            // sanity-check: container should be of size sizes[i]
            assert(container.size() == sizes[i]);
            ins_avg = compute_avg(counter, ins_avg, time);

            // compute average time to remove 
            std::default_random_engine gen(seed);
            std::vector<int> permutation;
            permutation.reserve(container.size());
            for (size_t i = 0; i < container.size(); ++i) {
                std::uniform_int_distribution<int> dist(0, container.size() - 1 - i);
                permutation.push_back(dist(gen));
            }
            time = benchmark(container, permutation,
                    [](Container& c, size_t x) {
                        ordered_remove(c, x);
                    }
                    );
            // sanity-check: every element must have been removed
            assert(container.size() == 0);
            rem_avg = compute_avg(counter, rem_avg, time);

            ++counter;
        }

        // update avg result
        ins_result[i] = ins_avg;
        rem_result[i] = rem_avg;
    } 
}

// No containers
template <class... Containers, size_t N, size_t... I>
inline void benchmark_containers(const std::array<int, N>& seeds,
                         const std::vector<int>& sizes,
                         std::vector<std::vector<double>>& ins_result,
                         std::vector<std::vector<double>>& rem_result,
                         std::index_sequence<I...>)
{
    static_assert(sizeof...(Containers) == sizeof...(I));
    (ins_result[I].resize(sizes.size()), ...);
    (rem_result[I].resize(sizes.size()), ...);
    (benchmark_container<Containers>(seeds, sizes, ins_result[I], rem_result[I]), ...);
}

// At least one container
template <class... Containers, size_t N>
inline void benchmark_containers(const std::array<int, N>& seeds,
                         const std::vector<int>& sizes,
                         std::vector<std::vector<double>>& ins_result,
                         std::vector<std::vector<double>>& rem_result)
{
    benchmark_containers<Containers...>(seeds, sizes, ins_result, rem_result,
            std::make_index_sequence<sizeof...(Containers)>());
}

} // namespace vlt
