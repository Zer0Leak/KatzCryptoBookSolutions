/****************************************************************************
 * File:        exec_05.cpp
 * Author:      Edgard Lima <edgard.lima@gmail.com>
 * Created:     2025-04-05
 * Description: Chapter 01, exercise 05 on Shift Cipher Attack.
 *
 * This file is released under CC0 1.0 Universal (Public Domain Dedication).
 * To the extent possible under law, the author has waived all copyright
 * and related or neighboring rights to this file.
 *
 * You can copy, modify, distribute and perform the work, even for commercial
 * purposes, all without asking permission. See:
 * https://creativecommons.org/publicdomain/zero/1.0/
 ****************************************************************************/

#include "exec_05.h"

#include <iostream>
#include <format>
#include <ranges>

#include "englishalphabet.h"
#include "shiftattack.h"
#include "vigenereattack.h"

static void shiftAttack() {
    for (char testKey = 'a'; testKey <= 'z'; ++testKey) {
        auto shiftAttacker = ShiftAttack(EnglishAlphabet::getInstance());
        shiftAttacker.readPlainTextFile("book.txt");
        shiftAttacker.encrypt(testKey);
        auto key = shiftAttacker.key();
        shiftAttacker.attack();
        auto newKey = shiftAttacker.key();
        if (key == newKey) {
            std::cout << std::format("Key '{}' discovered!", key) << std::endl;
        } else {
            std::cout << std::format("Original key '{}' differs from computed one '{}'!", key, newKey) << std::endl;
        }
    }
}

std::string fixRepeatedKey(const std::string &key) {
    // FIX keys like  'tt' 'abab' 'ababab' 'abcabc' etc..
    for (auto t=1; t <= key.size()/2; ++t) {
        if (key.size() % t != 0) {
            continue;
        }
        auto t_ok = true;
        for (auto i=0; t_ok && i <= key.size()-t; i += t) {
            for (auto j=0; j < t; ++j) {
                if (key[i+j] != key[j]) {
                    t_ok = false;
                    break;
                }
            }
        }
        if (t_ok) {
            return key.substr(0, t);
        }
    }

    return key;
}

static void _vigenereAttackTestFindKeyLength() {
    auto vigenereAttack = VigenereAttack(EnglishAlphabet::getInstance());
    vigenereAttack.readPlainTextFile("book.txt");

    for ([[maybe_unused]] int i : std::views::iota(0, 100)) {
        std::string key = VigenereAttack::randomKey();
        key = fixRepeatedKey(key);
        // auto minChars = key.size() * VigenereAttack::charsPerStreamToAttack();
        // vigenereAttack.readPlainTextFile("book.txt", minChars);
        vigenereAttack.encrypt(key);
        // std::cout << "Encrypted with key '" << key << "' (length: " << key.size() << ")." << std::endl;
        try {
            auto [computedKeySize, diff] = vigenereAttack.findKeyLength();
            if (key.size() == computedKeySize) {
                // std::cout << std::format("Key length '{}' discovered!", computedKeySize) << std::endl;
            } else {
                std::cout << "Encrypted with key '" << key << "' (length: " << key.size() << ")." << std::endl;
                std::cout << std::format("Original key length '{}' differs from computed one '{}'!", key.size(), computedKeySize) << std::endl;
                std::cout << "Distance from 0.065: " << diff << std::endl;
                if (computedKeySize * 2 == key.size()) {
                    std::string left = key.substr(0, key.size() / 2);
                    std::string right = key.substr(key.size() / 2);
                    std::cout << "Are left and right parts of key similar?" << std::endl;
                    std::cout << left << std::endl;
                    std::cout << right << std::endl;
                }
            }
        }
        catch (const std::exception& e) {
            std::cout << "Encrypted with key '" << key << "' (length: " << key.size() << ")." << std::endl;
            std::cerr << "Error: " << e.what() << '\n';
        }
        catch (...) {
            std::cout << "Encrypted with key '" << key << "' (length: " << key.size() << ")." << std::endl;
            std::cerr << "Unknown exception occurred\n";
        }
    }

}

static void _vigenereAttackTestAttack()
{
    auto vigenereAttack = VigenereAttack(EnglishAlphabet::getInstance());
    vigenereAttack.readPlainTextFile("book.txt");

    for ([[maybe_unused]] int i : std::views::iota(0, 10)) {
        std::string key = VigenereAttack::randomKey();
        key = fixRepeatedKey(key);
        // auto minChars = key.size() * VigenereAttack::charsPerStreamToAttack();
        // vigenereAttack.readPlainTextFile("book.txt", minChars);
        vigenereAttack.encrypt(key);
        // std::cout << "Encrypted with key '" << key << "' (length: " << key.size() << ")." << std::endl;
        try {
            vigenereAttack.attack();
            auto computedKey = vigenereAttack.key();
            if (key == computedKey) {
                std::cout << std::format("Key '{}' discovered!", computedKey) << std::endl;
            } else {
                std::cout << "Encrypted with key '" << key << "' (length: " << key.size() << ")." << std::endl;
                std::cout << "Differs from key '" << computedKey << "' (length: " << computedKey.size() << ")." << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "Encrypted with key '" << key << "' (length: " << key.size() << ")." << std::endl;
            std::cerr << "Error: " << e.what() << '\n';
        }
        catch (...) {
            std::cout << "Encrypted with key '" << key << "' (length: " << key.size() << ")." << std::endl;
            std::cerr << "Unknown exception occurred\n";
        }
    }

}

void vigenereAttack() {
    // _vigenereAttackTestFindKeyLength();
    _vigenereAttackTestAttack();
}

int exec_05(int argc, char *argv[]) {
    // shiftAttack();
    vigenereAttack();
    return 0;
}
