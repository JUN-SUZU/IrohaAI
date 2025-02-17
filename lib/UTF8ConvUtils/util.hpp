#ifndef UTF8ConvUtilsUtil_HPP
#define UTF8ConvUtilsUtil_HPP

#include <string>
#include <vector>
#include <locale>
#include <codecvt>

class Utf8ConvUtil
{
public:
    static std::wstring utf8ToWstring(const std::string &utf8);
    static std::string wstringToUtf8(const std::wstring &wstr);
};

#endif
