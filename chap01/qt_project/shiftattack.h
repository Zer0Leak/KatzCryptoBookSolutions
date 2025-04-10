/****************************************************************************
 * File:        shiftattack.h
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

#ifndef SHIFTATTACK_H
#define SHIFTATTACK_H

#include <string>
#include <istream>
#include <limits>
#include "alphabet.h"

class ShiftAttack
{
public:
    enum class Strategy {
        DiffSquareSum,  // works with only first 5 chars of book.txt
        SumSquares  // proposed by the book (works with 7 chars of book.txt)
    };

    static char randomKey();
    static std::string encryptText(char key, const std::string &text);

    ShiftAttack(const Alphabet &alphabet);
    void readPlainTextFile(const std::string &filePath,
                           u_int64_t maxChars = std::numeric_limits<u_int64_t>::max());
    void loadPlainText(const std::string &text);
    void loadCipherText(const std::string &cipherText);
    void encrypt(char key=ShiftAttack::randomKey());
    void attack(Strategy strategy=Strategy::DiffSquareSum);

    const std::string &text() { return _text; }
    const char key() { return _key; }
private:
    void attackProbDiffSquareSum();
    void attackSumSquares();

    const Alphabet &_alphabet;
    std::string _cipherText;
    char _key;
    std::string _text;
};

#endif // SHIFTATTACK_H
