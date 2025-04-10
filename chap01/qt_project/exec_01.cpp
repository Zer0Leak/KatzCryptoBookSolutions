/****************************************************************************
 * File:        exec_01.cpp
 * Author:      Edgard Lima <edgard.lima@gmail.com>
 * Created:     2025-04-05
 * Description: Chapter 01, exercise 01 on Mono-Alphabetic Attack.
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

#include <iostream>
#include <string>

#include <iostream>
#include <ranges>
#include <format>

#include "exec_01.h"

static auto & _englishAlphabet = EnglishAlphabet::getInstance();

static void findWords(const std::string &pattern,
                      std::function<bool(const std::string &)> func = [](const std::string &s) -> bool { return true; },
                      long maxMatches = 5) {
    const auto words = _englishAlphabet.findWords(pattern, func, maxMatches);
    for (auto &&[index, count, word]: words) {
        std::cout << index << ": (" << count << ") - " << word << std::endl;
    }
}

static void bruteForceShift(const std::string &cipher_text)
{
    for (int i : std::ranges::views::iota(0, 26)) {
        std::cout << std::format("{:2}: {}", i, AlphabetTools::shiftString(cipher_text, i)) << std::endl;
    }
}

static void printWrapped(const std::string& text, std::size_t width=62) {
    for (std::size_t i = 0; i < text.size(); i += width) {
        std::cout << text.substr(i, width) << '\n';
    }
    std::cout << std::endl;
}

static void attackMonoAlpha(const std::string & cipherText, const std::unordered_map<char, char> &knownDict) {
    auto plainText = AlphabetTools::attackMonoAlphaCipher(_englishAlphabet, cipherText, knownDict, true);

    printWrapped(cipherText);
    printWrapped(plainText);
}

void step01_FindingTheWord(const std::string & cipherText) {}


void tutorial() {
    const auto cipherText = "JGRMQOYGHMVBJWRWQFPWHGFFDQGFPFZRKBEEBJIZQQOCIBZKLFAFGQVFZFWWEOGWOPFGFHWOLPHLRLOLFDMFGQWBLWBWQOLKFWBYLBLYLFSFLJGRMQBOLWJVFPFWQVHQWFFPQOQVFPQOCFPOGFWFJIGFQVHLHLROQVFGWJVFPFOLFHGQVQVFILEOGQILHQFQGIQVVOSFAFGBWQVHQWIJVWJVFPFWHGFIWIHZZRQGBABHZQOCGFHX";
    std::unordered_map<char, char> knownDict;

    enum Steps {
        kStep01_1,
        kStep01_2,
        kStep01_3,
        kStep02_1,
        kStep02_2,
        kStep03_1,
        kStep03_2,
        kStep04_1,
        kStep04_2,
        kStep05_1,
        kStep05_2,
        kStep06_1,
        kStep07_1,
        kStep08_1,
        kStep09_1,
        kStep10_1,
        kStep11_1,
        kStep11_2,
        kStepFull,
    };
    auto step = kStepFull;

    if (step == kStep01_1) {
        const auto sequencies = AlphabetTools::mostFrequentSequencies(cipherText, 3, 4, -1);
        for (const auto& [word, quantity] : sequencies) {
            std::cout << word << " - " << quantity << std::endl;
        }
        return;
    }
    if (step == kStep01_2) {
        AlphabetTools::attackMonoAlphaCipher(_englishAlphabet, cipherText, knownDict, true);
        auto qvf_dist = AlphabetTools::sumSquareDistance("QVF",
                                                         _englishAlphabet,
                                                         cipherText,
                                                         {
                                                             {'Q', 't'},
                                                             {'V', 'h'},
                                                             {'F', 'e'}
                                                         });
        auto vfp_dist = AlphabetTools::sumSquareDistance("VFP",
                                                         _englishAlphabet,
                                                         cipherText,
                                                         {
                                                             {'V', 't'},
                                                             {'F', 'h'},
                                                             {'P', 'e'}
                                                         });
        std::cout << "Sum square distance probabilites mapping to 'the':" << std::endl;
        std::cout << std::format("{} -> ({:.6f})", "QVF", qvf_dist) << std::endl;
        std::cout << std::format("{} -> ({:.6f})", "VFP", vfp_dist) << std::endl;
        return;
    }

    // From step01: QVF -> the
    knownDict['Q'] = 't';
    knownDict['V'] = 'h';
    knownDict['F'] = 'e';

    if (step == kStep01_3) {
        attackMonoAlpha(cipherText, knownDict);
    }

    if (step == kStep02_1) {
        auto plainText = AlphabetTools::attackMonoAlphaCipher(_englishAlphabet, cipherText, knownDict, false);
        std::string cipherTextWithKnownChars = cipherText;
        for (auto i=0; i < plainText.size(); ++i) {
            if (plainText[i] != '_') {
                cipherTextWithKnownChars[i] = plainText[i];
            }
        }
        const auto sequencies = AlphabetTools::mostFrequentSequencies(cipherTextWithKnownChars, 4, 2, -1);
        for (const auto& [word, quantity] : sequencies) {
            std::cout << word << " - " << quantity << std::endl;
        }
        return;
    }

    // From kStep02_1: H -> a
    knownDict['H'] = 'a';

    if (step == kStep02_2) {
        attackMonoAlpha(cipherText, knownDict);
        return;
    }

    if (step == kStep03_1) {
        auto words = AlphabetTools::findWordsByPattern(_englishAlphabet.wordsCount(), "(^a.th$|.*ea.th$)", [](const std::string &s){
            const std::string blist = "thea";
            if (s.size() >= std::string("_ea_th").size()) {
                auto ch = *(s.end()-6);
                if (blist.find(ch) != std::string::npos) {
                    return false;
                }
            }
            if (s.size() >= std::string("__ea_th").size()) {
                auto ch = *(s.end()-7);
                if (blist.find(ch) != std::string::npos) {
                    return false;
                }
            }
            if (s.size() >= std::string("e__ea_th").size()) {
                auto ch = *(s.end()-6);
                if (ch != 'e') {
                    return false;
                }
            }
            return true;
        }, 15);
        for (const auto& [index, count, word] : words) {
            std::cout << std::format("{:>6}: {:>8} - {:>7}", index, count, word) << std::endl;
        }
        return;
    }

    // From kStep03_1: H -> a
    // FHGQV -> earth
    knownDict['G'] = 'r';

    if (step == kStep03_2) {
        attackMonoAlpha(cipherText, knownDict);
        return;
    }

    if (step == kStep04_1) {
        // tr___a_t__rea_ at the end of string brought our attention
        auto words = AlphabetTools::findWordsByPattern(_englishAlphabet.wordsCount(), ".*rea.$",
                                                       [](const std::string &s)
                                                       {
                                                           static const std::string mainstr = "rea";
                                                           static const auto blist = "thear";
                                                           static const int b4count = 2;
                                                           auto it = s.begin() + s.find(mainstr);
                                                           for (auto i = it-1; i >= s.begin() && (it - i < b4count); --i) {
                                                               if (std::string(blist).find(*i) != std::string::npos) {
                                                                   return false;
                                                               }
                                                           }
                                                           static const int afcount = 1;
                                                           it = s.begin() + s.find(mainstr) + mainstr.size();
                                                           for (auto i = it; i < s.end() && (i - it < afcount); ++i) {
                                                               if (std::string(blist).find(*i) != std::string::npos) {
                                                                   return false;
                                                               }
                                                           }
                                                           return true; }, 15);
        for (const auto& [index, count, word] : words) {
            std::cout << std::format("{:>6}: {:>9} - {:>10}", index, count, word) << std::endl;
        }
        return;
    }

    // From kStep04_1
    // QO CGFHX -> to break
    knownDict['O'] = 'o';
    knownDict['C'] = 'b';
    knownDict['X'] = 'k';

    if (step == kStep04_2) {
        attackMonoAlpha(cipherText, knownDict);
        return;
    }

    if (step == kStep05_1) {
        // tr___a_
        auto words = AlphabetTools::findWordsByPattern(_englishAlphabet.wordsCount(), ".*tr...a.$",
                                                       [](const std::string &s)
                                                       {
                                                           static const std::string mainstr = "tr";
                                                           static const auto blist = "thearobk";
                                                           static const int afcount = 3;
                                                           if(s.size() >= std::string("a___tr___a_").size()) {
                                                               auto ch = *(s.end()-11);
                                                               if (ch != 'a') {
                                                                   return false;
                                                               }
                                                           }
                                                           auto it = s.begin() + s.find(mainstr) + mainstr.size();
                                                           for (auto i = it; i < s.end() && (i - it < afcount); ++i) {
                                                               if (std::string(blist).find(*i) != std::string::npos) {
                                                                   return false;
                                                               }
                                                           }
                                                           return true; }, 15);
        for (const auto& [index, count, word] : words) {
            std::cout << std::format("{:>6}: {:>9} - {:>13}", index, count, word) << std::endl;
        }
        return;
    }

    // From kStep05_1
    // QGBABHZ -> trivial
    knownDict['B'] = 'i';
    knownDict['A'] = 'v';
    knownDict['Z'] = 'l';

    if (step == kStep05_2) {
        attackMonoAlpha(cipherText, knownDict);
        return;
    }

    // From kStep05_2
    // VOSFAFG -> however
    knownDict['S'] = 'w';

    if (step == kStep06_1) {
        attackMonoAlpha(cipherText, knownDict);
        return;
    }

    // From kStep06_1
    // BW -> is | ZFWW -> less
    knownDict['W'] = 's';

    if (step == kStep07_1) {
        attackMonoAlpha(cipherText, knownDict);
        return;
    }

    // From kStep07_1
    // LFAFGQVFZFWW -> nevertheless
    knownDict['L'] = 'n';

    if (step == kStep08_1) {
        attackMonoAlpha(cipherText, knownDict);
        return;
    }

    // From kStep08_1
    // EOG WOPF GFHWOL -> for some reason
    knownDict['E'] = 'f';
    knownDict['P'] = 'm';

    if (step == kStep09_1) {
        attackMonoAlpha(cipherText, knownDict);
        return;
    }

    // From kStep09_1
    // PHLR -> many
    knownDict['R'] = 'y';

    if (step == kStep10_1) {
        attackMonoAlpha(cipherText, knownDict);
        return;
    }

    // From kStep10_1
    // FDQGFPFZR -> extremely
    knownDict['D'] = 'x';

    if (step == kStep11_1) {
        // _s_ally
        auto words = AlphabetTools::findWordsByPattern(_englishAlphabet.wordsCount(), "^.s.ally$", [](const std::string &s)
                                                       { return true; }, 15);
        for (const auto& [index, count, word] : words) {
            std::cout << std::format("{:>6}: {:>9} - {:>13}", index, count, word) << std::endl;
        }
        return;
    }

    // From kStep11_1
    // IWIHZZR -> usually
    knownDict['I'] = 'u';

    if (step == kStep11_2) {
        attackMonoAlpha(cipherText, knownDict);
        return;
    }

    // From kStep11_2
    // KBEEBJIZQ -> difficult | CIBZK -> build | FDMFGQW -> experts | KFWBYLBLY -> designing
    knownDict['K'] = 'd';
    knownDict['J'] = 'c';
    knownDict['M'] = 'p';
    knownDict['Y'] = 'g';

    attackMonoAlpha(cipherText, knownDict);
}

int exec_01(int argc, char *argv[])
{
    tutorial();
    return 0;
}
