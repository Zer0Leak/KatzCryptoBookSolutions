#include "uniformdist.h"

#include <random>
#include <iostream>
#include <format>
#include <locale>

UniformDist::UniformDist() {}

std::vector<int> UniformDist::genarate01(uint32_t count, float p1)
{
    std::vector<int> v;
    v.reserve(count);

    // Create a random device and use it to seed the generator
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister 19937 generator

    // Create a uniform real distribution between 0 and 1
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // Generate a random number
    for (auto i=0; i < count; ++i) {
        float random_number = dist(gen);
        int b = random_number < p1 ? 1 : 0;
        v.push_back(b);
    }

    return v;
}

std::pair<uint32_t, uint32_t> UniformDist::count01(std::vector<int> v)
{
    uint32_t count0 = 0;
    uint32_t count1 = 0;
    for (auto i=0; i < v.size(); ++i) {
        if (v[i] == 1) {
            count1++;
        } else {
            count0++;
        }
    }
    return {count0, count1};
}

void UniformDist::print01Probabilities(std::vector<int> v)
{
    auto counts = UniformDist::count01(v);
    auto p0 = static_cast<float>(counts.first) / static_cast<float>(v.size());
    auto p1 = static_cast<float>(counts.second) / static_cast<float>(v.size());

    // 1) Pick a locale that defines a thousands separator,
    //    e.g. "en_US.UTF-8" or just use the user’s environment:
    std::locale loc("en_US.UTF-8");
    std::locale::global(loc);

    // 2) Imbue std::cout so that i/o of formatted locale-aware fields uses it
    std::cout.imbue(loc);

    // 3) Use {:L} in the format specifier to insert locale-aware separators:
    std::cout << std::format("v size {:>10L}", v.size()) << std::endl;

    std::cout << std::format("[{:.4f}%, {:.4f}%]", p0, p1) << std::endl;
}

void UniformDist::checkNeumannExtractor()
{
    auto v = UniformDist::genarate01(10'000'000, 0.8f);
    UniformDist::print01Probabilities(v);

    auto uv = UniformDist::createUniform(v);
    UniformDist::print01Probabilities(uv);
}

std::vector<int> UniformDist::createUniform(std::vector<int> v)
{
    // von Neumann extractor

    std::vector<int> uv;
    for (std::size_t i = 0; i + 1 < v.size(); i += 2) {
        const int b0 = v[i];
        const int b1 = v[i + 1];
        if (b0 == 0 && b1 == 1) {
            // pair 01 → output 0
            uv.push_back(0);
        }
        else if (b0 == 1 && b1 == 0) {
            // pair 10 → output 1
            uv.push_back(1);
        }
        // else (00 or 11) → discard, no push_back
    }
    return uv;
}
