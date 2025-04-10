/****************************************************************************
 * File:        alphabettools.h
 * Author:      Edgard Lima <edgard.lima@gmail.com>
 * Created:     2025-04-05
 * Description: Alphabet helper class.
 *
 * This file is released under CC0 1.0 Universal (Public Domain Dedication).
 * To the extent possible under law, the author has waived all copyright
 * and related or neighboring rights to this file.
 *
 * You can copy, modify, distribute and perform the work, even for commercial
 * purposes, all without asking permission. See:
 * https://creativecommons.org/publicdomain/zero/1.0/
 ****************************************************************************/

#ifndef ALPHABETTOOLS_H
#define ALPHABETTOOLS_H

#include <vector>
#include <unordered_map>

#include "alphabet.h"

class AlphabetTools
{
public:
    static void printFoundPattern(const std::string &s, const std::string &patternStr = R"(t.e)");  // e.g.: try to find T.E for THE

    static float squareSum(const std::vector<float> &probabilities);
    static std::unordered_map<char, float> probabilityPairsToDict(const std::vector<std::pair<float, char>> &probPairs);
    static std::vector<std::pair<float, char>> probabilitiesToSortedLabeledVector(const std::vector<float> &probabilities, bool lowChars);
    static std::string shiftString(const std::string &cipherText, int shift);
    static std::vector<float> calculateLetterProbabilities(const std::string &text);
    static std::string attackMonoAlphaCipher(const Alphabet &alphabet,
                                             const std::string &cipherText,
                                             std::unordered_map<char, char> knowDict = {},
                                             bool debug=false);
    static std::vector<std::pair<std::string, int>> mostFrequentSequencies(const std::string &s,
                                                                           int sequenceSize,
                                                                           int minFreqCutOff,
                                                                           int topMostCount);
    static float sumSquareDistance(const std::string &cipherText,
                                   const Alphabet &alphabet,
                                   const std::string &fullCipherText,
                                   const std::unordered_map<char, char> &dict);
    static std::vector<std::tuple<int, int64_t, std::string>>
        findWordsByPattern(const std::vector<std::pair<std::string, int64_t>> &words_count,
                           const std::string &pattern, std::function<bool(const std::string &)> func = [](const std::string &s) -> bool
                           { return true; }, long max_matches = 5);

    AlphabetTools();
};

#endif // ALPHABETTOOLS_H
