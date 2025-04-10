/****************************************************************************
 * File:        shiftattack.cpp
 * Author:      Edgard Lima <edgard.lima@gmail.com>
 * Created:     2025-04-05
 * Description: Shift Cipher. Enc, Dec and Attack.
 *
 * This file is released under CC0 1.0 Universal (Public Domain Dedication).
 * To the extent possible under law, the author has waived all copyright
 * and related or neighboring rights to this file.
 *
 * You can copy, modify, distribute and perform the work, even for commercial
 * purposes, all without asking permission. See:
 * https://creativecommons.org/publicdomain/zero/1.0/
 ****************************************************************************/

#include "shiftattack.h"
#include "helper.h"

#include <vector>
#include <string>
#include <cctype>
#include <random>
#include <limits>
#include <format>

// MARK: Static Methods

char ShiftAttack::randomKey() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist('a', 'z');

    return static_cast<char>(dist(gen));
}

std::string ShiftAttack::encryptText(char key, const std::string &text) {
    if (key < 'a' || key > 'z') {
        throw std::invalid_argument(std::format("Invalid key '{}'.", key));
    }

    std::string cipherText;
    cipherText.reserve(text.size());

    const int intKey = key - 'a';
    for (auto plainChar: text) {
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

// MARK: Constructors

ShiftAttack::ShiftAttack(const Alphabet &alphabet)
    : _alphabet(alphabet)
{
}

// MARK: Public Methods
void ShiftAttack::readPlainTextFile(const std::string &filePath, u_int64_t maxChars) {
    this->_text = Helper::readFile(filePath, maxChars);
    this->_cipherText.clear();
}

void ShiftAttack::loadPlainText(const std::string &text)
{
    this->_text = Helper::loadPlainText(text);
    this->_cipherText.clear();
}

void ShiftAttack::loadCipherText(const std::string &cipherText) {
    this->_cipherText = cipherText;
    this->_key = '\0';
}

void ShiftAttack::encrypt(char key) {
    this->_cipherText = ShiftAttack::encryptText(key, this->_text);
    this->_key = key;
}

void ShiftAttack::attack(Strategy strategy) {
    this->_key = '\0';

    switch (strategy) {
    case Strategy::DiffSquareSum:
        return attackProbDiffSquareSum();
    case Strategy::SumSquares:
        return attackSumSquares();
    default:
        throw std::invalid_argument(std::format("Unknown strategy."));
    }
}


void ShiftAttack::attackProbDiffSquareSum() {
    const auto &probabilities = this->_alphabet.probabilities();

    char closestJ;
    float minDiff = std::numeric_limits<float>::max();
    const u_int64_t count = this->_cipherText.size();
    for (char j = 'a'; j <= 'z'; ++j) {
        const auto intKey = j - 'a';
        std::vector<u_int64_t> charCount(26);

        for (auto cipherChar: this->_cipherText) {
            if (!std::isalpha(cipherChar)) {
                throw std::runtime_error("Fix _cipherText to only have alpha chars.");
            }
            if (!std::isupper(cipherChar)) {
                throw std::runtime_error("Fix _cipherText to only have upper chars.");
            }
            const int intCipherChar = cipherChar - 'A';
            int plainCharIndex = (intCipherChar - intKey + 26) % 26;
            charCount[plainCharIndex] += 1;
        }
        float diffSquareSum = 0.0f;
        for (auto k=0; k < 26; ++k) {
            float charProb = static_cast<float>(charCount[k]) / static_cast<float>(count);
            auto diff = probabilities[k] - charProb;
            diffSquareSum += diff * diff;
        }
        if (diffSquareSum < minDiff) {
            closestJ = j;
            minDiff = diffSquareSum;
        }
    }

    this->_key = closestJ;
}

void ShiftAttack::attackSumSquares() {
    const auto &probabilitiesSquareSum = this->_alphabet.probabilitiesSquareSum();
    const auto &probabilities = this->_alphabet.probabilities();

    char closestJ;
    float minDiff = std::numeric_limits<float>::max();
    const u_int64_t count = this->_cipherText.size();
    for (char j = 'a'; j <= 'z'; ++j) {
        const auto intKey = j - 'a';
        std::vector<u_int64_t> charCount(26);

        for (auto cipherChar: this->_cipherText) {
            if (!std::isalpha(cipherChar)) {
                throw std::runtime_error("Fix _cipherText to only have alpha chars.");
            }
            if (!std::isupper(cipherChar)) {
                throw std::runtime_error("Fix _cipherText to only have upper chars.");
            }
            const int intCipherChar = cipherChar - 'A';
            int plainCharIndex = (intCipherChar - intKey + 26) % 26;
            charCount[plainCharIndex] += 1;
        }
        float squareSum = 0.0f;
        for (auto k=0; k < 26; ++k) {
            float charProb = static_cast<float>(charCount[k]) / static_cast<float>(count);
            squareSum += probabilities[k] * charProb;
        }
        float diff = std::abs(probabilitiesSquareSum - squareSum);
        if (diff < minDiff) {
            closestJ = j;
            minDiff = diff;
        }
    }

    this->_key = closestJ;
}

