#ifndef UNIFORMDIST_H
#define UNIFORMDIST_H

#include <vector>
#include <utility>

class UniformDist
{
public:
    UniformDist();
    static std::vector<int> genarate01(uint32_t count, float p1);
    static std::pair<uint32_t, uint32_t> count01(std::vector<int>);
    static void print01Probabilities(std::vector<int> v);
    static std::vector<int> createUniform(std::vector<int> v);
    static void checkNeumannExtractor();
};

#endif // UNIFORMDIST_H
