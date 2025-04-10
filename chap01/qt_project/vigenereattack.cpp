/****************************************************************************
 * File:        vigenereattack.cpp
 * Author:      Edgard Lima <edgard.lima@gmail.com>
 * Created:     2025-04-05
 * Description: Vigenère Cipher. Enc, Dec and Attack.
 *
 * This file is released under CC0 1.0 Universal (Public Domain Dedication).
 * To the extent possible under law, the author has waived all copyright
 * and related or neighboring rights to this file.
 *
 * You can copy, modify, distribute and perform the work, even for commercial
 * purposes, all without asking permission. See:
 * https://creativecommons.org/publicdomain/zero/1.0/
 ****************************************************************************/

#include "vigenereattack.h"
#include "helper.h"

#include <random>
#include <string>
#include <algorithm>
#include <format>
#include <set>

std::string VigenereAttack::randomKey(int maxSize) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> lenDist(1, maxSize);
    std::uniform_int_distribution<int> charDist('a', 'z');

    int len = lenDist(gen);
    std::string result;
    result.reserve(len);

    for (int i = 0; i < len; ++i) {
        result += charDist(gen);
    }

    return result;
}

std::string VigenereAttack::encryptText(const std::string &key, const std::string &text) {
    if (!std::ranges::all_of(key, [](char c) {
            return c >= 'a' && c <= 'z';
        })) {
        throw std::invalid_argument(std::format("Invalid key '{}'.", key));
    }

    std::string cipherText;
    cipherText.reserve(text.size());

    for (auto i=0; i < text.size(); ++i) {
        const int intKey = key[i % key.size()] - 'a';
        auto plainChar = text[i];
        if (!std::isalpha(plainChar)) {
            throw std::runtime_error("Fix _text to only have alpha chars.");
        }
        if (!std::islower(plainChar)) {
            throw std::runtime_error("Fix _text to only have lower chars.");
        }
        const int intPlainChar = plainChar - 'a';
        char cipherChar = (intPlainChar + intKey) % 26 + 'a';
        cipherChar = toupper(static_cast<char>(cipherChar));
        cipherText += cipherChar;
    }

    return cipherText;
}

float VigenereAttack::_calculateDiff(int t, const std::string &cipherText, float probabilitiesSquareSum, float threshold)
{
    std::vector<int> offsets = {0};
    float worseDiff = 0.0f;

    if (cipherText.size() > 1) {
        offsets.push_back(1);
    }

    for (auto offset: offsets) {
        std::vector<u_int64_t> charCount(26);

        // u_int64_t testStreamCount = 0;
        for (auto i=offset; i < cipherText.size(); i += t) {
            auto intCipherChar = cipherText[i] - 'A';
            charCount[intCipherChar] += 1;
            // testStreamCount++;
        }
        auto streamCount = (cipherText.size() - offset) / t;
        if (((cipherText.size() - offset) % t) > 0) streamCount++;
        //if (testStreamCount != streamCount) {
        //    throw std::runtime_error(std::format("I'm a bad programmer. testStreamCount != streamCount. {} != {}. (offset={}, t={}, size={})",
        //                                         testStreamCount, streamCount, offset, t, cipherText.size()));
        //}

        float squareSum = 0.0f;
        for (auto k=0; k < 26; ++k) {
            float charProb = static_cast<float>(charCount[k]) / static_cast<float>(streamCount);
            squareSum += charProb * charProb;
        }
        float diff = std::abs(probabilitiesSquareSum - squareSum);
        if (diff > worseDiff) {
            worseDiff = diff;
        }
        if (diff > threshold) {
            break;
        }
    }
    return worseDiff;
}

std::pair<int, float> VigenereAttack::_findKeyLength(const std::string &cipherText, const Alphabet &alphabet)
{
    static const auto minStreamSize = VigenereAttack::charsPerStreamToAttack();
    auto maxKeyLength = cipherText.size() / minStreamSize;  // we assume we need at least 30 chars per stream to get something relevant
    const auto probabilitiesSquareSum = alphabet.probabilitiesSquareSum();
    static const auto kThreshold = 0.007f;

    if (maxKeyLength < 1) {
        throw std::runtime_error(std::format("Can't find key lenght with text smaller than {}.", minStreamSize));
    }

    for (auto cipherChar: cipherText) {
        if (!std::isalpha(cipherChar)) {
            throw std::runtime_error("Fix _cipherText to only have alpha chars.");
        }
        if (!std::isupper(cipherChar)) {
            throw std::runtime_error("Fix _cipherText to only have upper chars.");
        }
    }

    for (auto t=1; t <= maxKeyLength; ++t) {
        float diff = _calculateDiff(t, cipherText, probabilitiesSquareSum, kThreshold);
        if (diff <= kThreshold) {
            return {t, diff};
        }
    }

    throw std::runtime_error("Cannot detect key length.");
}

VigenereAttack::VigenereAttack(const Alphabet &alphabet) : _alphabet(alphabet) {}

void VigenereAttack::readPlainTextFile(const std::string &filePath,
                       u_int64_t maxChars)
{
    this->_text = Helper::readFile(filePath, maxChars);
    this->_cipherText.clear();
}

void VigenereAttack::loadPlainText(const std::string &text)
{
    this->_text = Helper::loadPlainText(text);
    this->_cipherText.clear();
}

void VigenereAttack::encrypt(std::string key)
{
    this->_cipherText = VigenereAttack::encryptText(key, this->_text);
    this->_key = key;
}

std::vector<std::string> VigenereAttack::_splitIntoT(const std::string& s, int t)
{
    std::vector<std::string> result(t);
    for (size_t i = 0; i < s.size(); ++i) {
        int offset = i % t;
        result[offset] += s[i];
    }
    return result;
}

void VigenereAttack::attack()
{
    this->_key.clear();
    auto [keyLength, _] = findKeyLength();
    auto streams = _splitIntoT(this->cipherText(), keyLength);
    std::string key;
    for (const auto &stream: streams) {
        auto shiftAttack = ShiftAttack(this->_alphabet);
        shiftAttack.loadCipherText(stream);
        shiftAttack.attack();
        key.push_back(shiftAttack.key());
    }
    this->_key = key;
}

std::pair<int, float> VigenereAttack::findKeyLength()
{
    return _findKeyLength(this->_cipherText, this->_alphabet);
}
