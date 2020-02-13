#pragma once
#include <unordered_set>
#include <random>
#include <cassert>

namespace vlt {

// Generates unique integers sampled from uniform distribution.
struct UniqueIntUniform
{

    UniqueIntUniform(int size, int lower, int upper, int seed)
        : lower_(lower), upper_(upper), size_(size), seed_(seed), sample_()
    {
        // unif dist range has at least size number of elements
        // this must be true if we want unique entries
        assert((upper - lower + 1) >= size);
    }

    UniqueIntUniform(int size, int seed)
        : UniqueIntUniform(size, 1, 2 * size, seed)
    {}

    UniqueIntUniform(int size)
        : UniqueIntUniform(size, 1, 2 * size, -1)
    {}

    void generate()
    {
        std::default_random_engine gen;
        if (seed_ != -1) {
            gen.seed(seed_);
        }
        std::uniform_int_distribution<int> dist(lower_, upper_);
        
        while (sample_.size() < static_cast<size_t>(size_)) {
            sample_.insert(dist(gen));
        } 
    }

    auto begin() 
    {
        return sample_.begin();
    }

    auto end() 
    {
        return sample_.end();
    }

    auto begin() const
    {
        return sample_.begin();
    }

    auto end() const
    {
        return sample_.end();
    }

private:
    int lower_;
    int upper_;
    int size_;
    int seed_;
    std::unordered_set<int> sample_;
};

} // namespace vlt
