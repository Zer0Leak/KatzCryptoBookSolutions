/****************************************************************************
 * File:        englishalphabet.h
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

#ifndef ENGLISHALPHABET_H
#define ENGLISHALPHABET_H

#include "alphabet.h"

#include <vector>
#include <tuple>
#include <functional>

class EnglishAlphabet: public Alphabet
{
public:
    virtual const std::vector<float> &probabilities() const;
    virtual float probabilitiesSquareSum() const;
    virtual const std::vector<std::pair<std::string, int64_t>> &wordsCount() const;
    virtual std::vector<std::tuple<int, int64_t, std::string>>
    findWords(const std::string &pattern,
              std::function<bool(const std::string &)> func = [](const std::string &s) -> bool { return true; },
              long maxMatches = 5) const;
    virtual const std::vector<std::pair<float, char>> sortedProbabilitiesLabeledVector() const;
    virtual const std::unordered_map<char, float> &letterProbDict() const;
    static const Alphabet &getInstance() {
        static EnglishAlphabet instance;
        return instance;
    }
private:
    EnglishAlphabet();
};

#endif // ENGLISHALPHABET_H
