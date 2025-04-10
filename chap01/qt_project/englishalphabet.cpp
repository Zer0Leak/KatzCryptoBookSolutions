/****************************************************************************
 * File:        englishalphabet.cpp
 * Author:      Edgard Lima <edgard.lima@gmail.com>
 * Created:     2025-04-05
 * Description: Models English Alphabet.
 *
 * This file is released under CC0 1.0 Universal (Public Domain Dedication).
 * To the extent possible under law, the author has waived all copyright
 * and related or neighboring rights to this file.
 *
 * You can copy, modify, distribute and perform the work, even for commercial
 * purposes, all without asking permission. See:
 * https://creativecommons.org/publicdomain/zero/1.0/
 ****************************************************************************/

#include "englishalphabet.h"
#include "alphabettools.h"

#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <stdexcept>
#include <mutex>
#include <filesystem>
#include <functional>
#include <regex>

const std::vector<float> _probabilities = {
    0.082f,  // A
    0.015f,  // B
    0.028f,  // C
    0.043f,  // D
    0.127f,  // E
    0.022f,  // F
    0.020f,  // G
    0.061f,  // H
    0.070f,  // I
    0.002f,  // J
    0.008f,  // K
    0.040f,  // L
    0.024f,  // M
    0.067f,  // N
    0.075f,  // O
    0.019f,  // P
    0.001f,  // Q
    0.060f,  // R
    0.063f,  // S
    0.091f,  // T
    0.028f,  // U
    0.010f,  // V
    0.024f,  // W
    0.002f,  // X
    0.020f,  // Y
    0.001f   // Z
};

const std::vector<float> &EnglishAlphabet::probabilities() const {
    return _probabilities;
}

float EnglishAlphabet::probabilitiesSquareSum() const {
    static float s = 0.0f;
    static std::once_flag flag;
    std::call_once(flag, [&]() {
        s = AlphabetTools::squareSum(this->probabilities());
    });
    return s;
}

const std::vector<std::pair<std::string, int64_t>> &EnglishAlphabet::wordsCount() const {
    // https://norvig.com/ngrams/
    static const std::filesystem::path filePath = "/tmp/count_1w.txt";
    static std::vector<std::pair<std::string, int64_t>> data;
    static std::once_flag flag;

    std::call_once(flag, [&]() {
        if (!std::filesystem::exists(filePath)) {
            throw std::runtime_error("File not found: " + filePath.string() +
                                     "\nWorking dir: " + std::filesystem::current_path().string());
        }

        std::ifstream file(filePath);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filePath.string());
        }

        std::string word;
        int64_t number;

        while (file >> word >> number) {
            data.emplace_back(std::move(word), number);
        }
    });

    return data;
}

std::vector<std::tuple<int, int64_t, std::string>>
EnglishAlphabet::findWords(const std::string &pattern,
                           std::function<bool(const std::string &)> func,
                           long maxMatches) const
{
    std::regex regex_pattern(pattern);
    long matches = 0;
    const auto &wordsCount = EnglishAlphabet::wordsCount();
    std::vector<std::tuple<int, int64_t, std::string>> wordVector;
    for (auto i = 0; i < wordsCount.size(); i++)
    {
        const auto &p = wordsCount[i];
        if (std::regex_search(p.first, regex_pattern))
        {
            if (func(p.first))
            {
                wordVector.emplace_back(i, p.second, p.first);
                if (maxMatches > 0 && ++matches == maxMatches)
                {
                    break;
                }
            }
        }
    }
    return wordVector;
}

const std::vector<std::pair<float, char>> EnglishAlphabet::sortedProbabilitiesLabeledVector() const {
    static std::vector<std::pair<float, char>> v;
    static std::once_flag flag;

    std::call_once(flag, [&]() {
        v = AlphabetTools::probabilitiesToSortedLabeledVector(this->probabilities(), true);
    });

    return v;
}

const std::unordered_map<char, float> &EnglishAlphabet::letterProbDict() const {
    static std::unordered_map<char, float> m;
    static std::once_flag flag;

    std::call_once(flag, [&]() {
        m = AlphabetTools::probabilityPairsToDict(EnglishAlphabet::sortedProbabilitiesLabeledVector());
    });

    return m;
}

EnglishAlphabet::EnglishAlphabet() {}
