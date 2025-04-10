/****************************************************************************
 * File:        vigenereattack.h
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

#ifndef VIGENEREATTACK_H
#define VIGENEREATTACK_H

#include <string>

#include "alphabet.h"
#include "shiftattack.h"
#include <utility>

class VigenereAttack
{
public:
    static std::string randomKey(int maxSize=32);
    static std::string encryptText(const std::string &key, const std::string &text);
    static const int charsPerStreamToAttack() { return 600; }

    VigenereAttack(const Alphabet &alphabet);
    void readPlainTextFile(const std::string &filePath,
                           u_int64_t maxChars = std::numeric_limits<u_int64_t>::max());
    void loadPlainText(const std::string &text);
    void encrypt(std::string key=VigenereAttack::randomKey());
    void attack();
    std::pair<int, float> findKeyLength();

    const std::string &text() { return _text; }
    const std::string &cipherText() { return _cipherText; }
    const std::string key() { return _key; }
private:
    static std::pair<int, float> _findKeyLength(const std::string &cipherText, const Alphabet &alphabet);
    static float _calculateDiff(int t, const std::string &cipherText, float probabilitiesSquareSum, float threshold);
    static std::vector<std::string> _splitIntoT(const std::string& s, int t);

    const Alphabet &_alphabet;
    std::string _cipherText;
    std::string _key;
    std::string _text;
};

#endif // VIGENEREATTACK_H
