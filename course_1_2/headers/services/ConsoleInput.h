#pragma once

#include <iosfwd>
#include <string>
#include <string_view>

namespace consoleInput
{
std::string utf16ToUtf8(std::wstring_view value);
std::string readUtf8Line(std::istream& fallbackInput);
}
