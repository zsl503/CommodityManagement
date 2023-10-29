#include <iterator>
#include <regex>
#include "utils.h"
#include <Windows.h>
// std::wstring°æ±¾
std::vector<std::wstring> split(const std::wstring& in, const std::wstring& delim) {
    std::wregex re{ delim };
    return std::vector<std::wstring> {
        std::wsregex_token_iterator(in.begin(), in.end(), re, -1),
            std::wsregex_token_iterator()
    };
}
