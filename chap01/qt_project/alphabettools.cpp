/****************************************************************************
 * File:        alphabettools.cpp
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

#include "alphabettools.h"

// #include <numeric>
#include <algorithm>
#include <ranges>
#include <vector>
#include <stdexcept>
#include <string>
#include <cctype>
#include <numeric>
#include <unordered_set>
#include <iostream>
#include <format>
#include <regex>

AlphabetTools::AlphabetTools() {}


void AlphabetTools::printFoundPattern(const std::string &s, const std::string &patternStr)
{
    std::regex pattern(patternStr);
    auto words = std::ranges::subrange(
        std::sregex_iterator(s.begin(), s.end(), pattern),
        std::sregex_iterator()
        );

    for (const auto &match : words)
    {
        std::cout << "Found: " << match.str() << '\n';
    }
}

std::vector<std::pair<std::string, int>> AlphabetTools::mostFrequentSequencies(const std::string &s,
                                                                               int sequenceSize,
                                                                               int minFreqCutOff,
                                                                               int topMostCount)
{
    std::unordered_map<std::string, int> freq_map;

    for (auto i : std::views::iota(0, static_cast<int>(s.size() - sequenceSize + 1)))
    {
        ++freq_map[s.substr(i, sequenceSize)];
    }

    std::vector<std::pair<std::string, int>> v;
    for (const auto &it : freq_map)
    {
        v.push_back({it.first, it.second});
    }
    std::sort(v.begin(), v.end(), [](const auto &a, const auto &b)
              { return a.second > b.second; });

    std::vector<std::pair<std::string, int>> vs;
    for (auto i = 0; i < v.size() && (topMostCount <= 0 || i < topMostCount); i++)
    {
        if (minFreqCutOff <= 0 || v[i].second >= minFreqCutOff)
        {
            vs.push_back(v[i]);
        }
        else
        {
            break;
        }
    }
    return vs;
}




float AlphabetTools::squareSum(const std::vector<float> &probabilities) {
    // sinxe C++23 https://en.cppreference.com/w/cpp/algorithm/ranges/fold_left
    return std::ranges::fold_left(probabilities | std::views::transform([](float p) { return p * p; }),
                                  0.0f, std::plus<>());
    /* C++20 https://en.cppreference.com/w/cpp/algorithm/accumulate
    return std::accumulate(probabilities.begin(), probabilities.end(), 0.0f,
                           [](float sum, float p) { return sum + p * p; });
    */
}

std::unordered_map<char, float> AlphabetTools::probabilityPairsToDict(const std::vector<std::pair<float, char>> &probPairs) {
    std::unordered_map<char, float> m;
    m.reserve(probPairs.size());

    for (auto&& [prob, letter]: probPairs) {
        m.insert_or_assign(letter, prob);
    }
    return m;
}

std::vector<std::pair<float, char>> AlphabetTools::probabilitiesToSortedLabeledVector(const std::vector<float> &probabilities, bool lowChars)
{
    if (probabilities.size() != 26)
    {
        throw std::invalid_argument("pv shall be probabilites from a to z.");
    }
    const auto baseChar = lowChars ? 'a' : 'A';
    auto probLetterVector = probabilities | std::views::transform([i = 0, baseChar](float prob) mutable {
                 return std::pair(prob, baseChar + i++);
             });
    auto sortedProbLetterVector = std::ranges::to<std::vector<std::pair<float, char>>>(probLetterVector);
    std::ranges::sort(sortedProbLetterVector, std::greater<>());

    return sortedProbLetterVector;
}

std::string AlphabetTools::shiftString(const std::string &cipherText, int shift) {
    if (std::ranges::any_of(cipherText, [](char ch) { return !std::isupper(static_cast<unsigned char>(ch)); })) {
        throw std::invalid_argument("cipherText is expected to be upper case.");
    }

    std::string shiftedText;
    shiftedText.reserve(cipherText.size()); // Reserve space to avoid reallocations

    std::ranges::transform(cipherText, std::back_inserter(shiftedText), [shift](char ch) -> char {
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            return std::toupper(static_cast<unsigned char>((std::tolower(ch) - 'a' + shift) % 26 + 'a'));
        }
        return ch; // Non-alphabetic characters remain unchanged
    });

    for (const char &ch : cipherText)
    {
        if (isalpha(ch))
        {
            if (islower(ch))
            {
                throw std::invalid_argument("ciphet_text is expected to be upper case.");
            }
            const char lower_ch = tolower(ch);
            const char shifted_char = (lower_ch - 'a' + shift) % 26 + 'a';
            shiftedText += shifted_char;
        }
        else
        {
            shiftedText += ch;
        }
    }

    return shiftedText;
}

std::vector<float> AlphabetTools::calculateLetterProbabilities(const std::string &text) {
    std::vector<int> counts(26, 0);

    if (text.size() > 0) {
        auto baseChar = std::isupper(text[0]) ? 'A' : 'a';
        std::ranges::for_each(text, [&counts, baseChar](char ch) {
            if (std::isalpha(static_cast<unsigned char>(ch))) {
                counts[ch - baseChar]++;
            }
        });
    }

    float sum = std::reduce(counts.begin(), counts.end(), 0.0f);

    std::vector<float> probabilities(26);
    std::ranges::transform(counts, probabilities.begin(), [sum](int count) {
        return count / sum;
    });

    return probabilities;
}

float AlphabetTools::sumSquareDistance(const std::string &cipherText,
                                       const Alphabet &alphabet,
                                       const std::string &fullCipherText,
                                       const std::unordered_map<char, char> &dict)
{
    auto fullCipherTextProbabilities = AlphabetTools::calculateLetterProbabilities(fullCipherText);
    auto fullCipherTextProbabilitiesLabeled = AlphabetTools::probabilitiesToSortedLabeledVector(fullCipherTextProbabilities, false);
    auto fullCipherTextProbabilitiesDict = AlphabetTools::probabilityPairsToDict(fullCipherTextProbabilitiesLabeled);

    auto alphabetProbsDict = alphabet.letterProbDict();

    auto sum = 0.0f;
    for (const auto &letter: cipherText) {
        const auto t = dict.at(letter);
        auto d = fullCipherTextProbabilitiesDict[static_cast<char>(tolower(letter))] - alphabetProbsDict[t];
        sum += d * d;
    }

    return sum;
}

std::string AlphabetTools::attackMonoAlphaCipher(const Alphabet &alphabet,
                                                 const std::string &cipherText,
                                                 std::unordered_map<char, char> knowDict,
                                                 bool debug)
{
    auto cipher_text_probs = AlphabetTools::calculateLetterProbabilities(cipherText);
    auto cipher_text_probs_sorted = AlphabetTools::probabilitiesToSortedLabeledVector(cipher_text_probs, false);

    std::unordered_set<char> english_set;
    auto alphabetProbsDict = alphabet.letterProbDict();
    auto alphabetSortedProbabilitiesLabeledVector = alphabet.sortedProbabilitiesLabeledVector();
    auto dict = knowDict;

    auto cipher_probs_dict = AlphabetTools::probabilityPairsToDict(cipher_text_probs_sorted);

    // for (const auto &[key, value] : dict)
    for (auto it = dict.begin(); it != dict.end(); ++it)
    {
        const auto key = it->first;
        const auto &value = it->second;
        english_set.insert(value);

        auto cp = cipher_probs_dict[key];
        auto ep = alphabetProbsDict.at(value);

        std::cout << std::format("{} ({:.3f}) <--> {} ({:.3f})", key, cp, value, ep) << std::endl;
    }

    std::cout << "- - - -" << std::endl;

    auto itc = cipher_text_probs_sorted.begin();
    auto ite = alphabetSortedProbabilitiesLabeledVector.begin();

    while (itc != cipher_text_probs_sorted.end() && ite != alphabetSortedProbabilitiesLabeledVector.end())
    {
        if (dict.find(itc->second) != dict.end())
        {
            itc++;
            continue;
        }
        if (english_set.find(ite->second) != english_set.end())
        {
            ite++;
            continue;
        }

        auto cp = itc->first;
        auto cc = itc->second;
        auto ep = ite->first;
        auto ec = ite->second;

        dict[cc] = ec;
        english_set.insert(ec);


        std::cout << std::format("{} ({:.3f}) <--> {} ({:.3f})", cc, cp, ec, ep) << std::endl;

        itc++;
        ite++;
    }

    if (itc != cipher_text_probs_sorted.end() || ite != alphabetSortedProbabilitiesLabeledVector.end())
    {
        std::logic_error("both iterators must reach the end.");
    }

    std::string plain_text;
    for (auto ch : cipherText)
    {
        if (isalpha(ch))
        {
            if (cipher_probs_dict[ch] >= 0.0f)
            {
                if (knowDict.find(ch) != knowDict.end())
                {
                    plain_text += dict[ch];
                }
                else
                {
                    plain_text += '_'; // toupper(dict[lch]);
                }
            }
            else
            {
                plain_text += '_';
            }
        }
        else
        {
            plain_text += ch;
        }
    }
    return plain_text;
}

std::vector<std::tuple<int, int64_t, std::string>>
AlphabetTools::findWordsByPattern(const std::vector<std::pair<std::string, int64_t>> &words_count,
                                  const std::string &pattern, std::function<bool(const std::string &)> func,
                                  long max_matches)
{
    std::regex regex_pattern(pattern);
    std::vector<std::tuple<int, int64_t, std::string>> result;
    long matches = 0;
    for (auto i = 0; i < words_count.size(); i++)
    {
        const auto &p = words_count[i];
        if (std::regex_search(p.first, regex_pattern))
        {
            if (func(p.first))
            {
                result.push_back(std::make_tuple(i, p.second, p.first));
                if (max_matches > 0 && ++matches == max_matches)
                {
                    break;
                }
            }
        }
    }
    return result;
}
