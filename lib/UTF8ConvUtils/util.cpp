#include "util.hpp"

std::wstring Utf8ConvUtil::utf8ToWstring(const std::string &utf8)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.from_bytes(utf8);
}
std::string Utf8ConvUtil::wstringToUtf8(const std::wstring &wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wstr);
}
