/****************************************************************************
 * File:        helper.cpp
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

#include "helper.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <algorithm>

Helper::Helper() {}

std::string Helper::readFile(const std::string &filePath, u_int64_t maxChars) {
    std::ifstream file(filePath, std::ios::binary);  // Open in binary mode for efficiency
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filePath +
                                 "\nWorking dir: " + std::filesystem::current_path().string());
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    auto filteredRange = content |
                         std::views::filter(isalpha) |
                         std::views::transform([](char c) {
                             return static_cast<char>(tolower(static_cast<int>(c)));
                         });
    std::string fileContent = std::ranges::to<std::string>(filteredRange);

    auto truncate = std::min(maxChars, static_cast<u_int64_t>(fileContent.size()));
    fileContent.resize(truncate);

    return fileContent;
}

std::string Helper::loadPlainText(const std::string &text)
{
    auto filteredRange = text |
                         std::views::filter(isalpha) |
                         std::views::transform([](char c) {
                             return static_cast<char>(tolower(static_cast<int>(c)));
                         });

    std::string content = std::ranges::to<std::string>(filteredRange);
    return content;
}
