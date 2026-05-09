#include "services/ConsoleInput.h"

#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <istream>
#endif

namespace
{
#ifdef _WIN32
bool tryReadConsoleLine(std::string& value)
{
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    if (input == nullptr || input == INVALID_HANDLE_VALUE || !GetConsoleMode(input, &mode))
    {
        return false;
    }

    std::wstring line;
    wchar_t buffer[256];

    while (true)
    {
        DWORD charsRead = 0;
        if (!ReadConsoleW(input, buffer, 256, &charsRead, nullptr))
        {
            return false;
        }

        if (charsRead == 0)
        {
            value = consoleInput::utf16ToUtf8(line);
            return true;
        }

        for (DWORD i = 0; i < charsRead; ++i)
        {
            if (buffer[i] == L'\r')
            {
                continue;
            }
            if (buffer[i] == L'\n')
            {
                value = consoleInput::utf16ToUtf8(line);
                return true;
            }
            line.push_back(buffer[i]);
        }
    }
}
#endif

#ifndef _WIN32
void appendUtf8(std::string& output, unsigned int codePoint)
{
    if (codePoint <= 0x7F)
    {
        output.push_back(static_cast<char>(codePoint));
    }
    else if (codePoint <= 0x7FF)
    {
        output.push_back(static_cast<char>(0xC0 | (codePoint >> 6)));
        output.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    }
    else if (codePoint <= 0xFFFF)
    {
        output.push_back(static_cast<char>(0xE0 | (codePoint >> 12)));
        output.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
        output.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    }
    else
    {
        output.push_back(static_cast<char>(0xF0 | (codePoint >> 18)));
        output.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
        output.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
        output.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    }
}
#endif
}

namespace consoleInput
{
std::string utf16ToUtf8(std::wstring_view value)
{
    if (value.empty())
    {
        return {};
    }

#ifdef _WIN32
    const int size = WideCharToMultiByte(
        CP_UTF8,
        0,
        value.data(),
        static_cast<int>(value.size()),
        nullptr,
        0,
        nullptr,
        nullptr);

    if (size <= 0)
    {
        throw std::runtime_error("Не удалось преобразовать ввод консоли в UTF-8.");
    }

    std::string result(static_cast<std::size_t>(size), '\0');
    const int written = WideCharToMultiByte(
        CP_UTF8,
        0,
        value.data(),
        static_cast<int>(value.size()),
        result.data(),
        size,
        nullptr,
        nullptr);

    if (written != size)
    {
        throw std::runtime_error("Не удалось преобразовать ввод консоли в UTF-8.");
    }

    return result;
#else
    std::string result;
    for (wchar_t ch : value)
    {
        appendUtf8(result, static_cast<unsigned int>(ch));
    }
    return result;
#endif
}

std::string readUtf8Line(std::istream& fallbackInput)
{
    std::string value;

#ifdef _WIN32
    if (&fallbackInput == &std::cin && tryReadConsoleLine(value))
    {
        return value;
    }
#endif

    std::getline(fallbackInput, value);
    return value;
}
}
