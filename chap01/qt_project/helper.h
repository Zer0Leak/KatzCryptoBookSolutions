/****************************************************************************
 * File:        helper.h
 * Author:      Edgard Lima <edgard.lima@gmail.com>
 * Created:     2025-04-05
 * Description: Class with tools and helper methods.
 *
 * This file is released under CC0 1.0 Universal (Public Domain Dedication).
 * To the extent possible under law, the author has waived all copyright
 * and related or neighboring rights to this file.
 *
 * You can copy, modify, distribute and perform the work, even for commercial
 * purposes, all without asking permission. See:
 * https://creativecommons.org/publicdomain/zero/1.0/
 ****************************************************************************/

#ifndef HELPER_H
#define HELPER_H

#include <string>

class Helper
{
public:
    static std::string readFile(const std::string &filePath, u_int64_t maxChars);
    static std::string loadPlainText(const std::string &text);

    Helper();
};

#endif // HELPER_H
