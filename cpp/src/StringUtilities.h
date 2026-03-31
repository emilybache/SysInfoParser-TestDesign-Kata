#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

#include <string>
#include <algorithm>
#include <cctype>

inline std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }
    if (start == s.end()) return "";
    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

template<typename T, typename Pred>
inline void trim_if(T& s, Pred p) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&p](auto ch) { return !p(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [&p](auto ch) { return !p(ch); }).base(), s.end());
}

inline auto is_any_of(const std::string& set) {
    return [set](char ch) { return set.find(ch) != std::string::npos; };
}

inline void sStringExtractFixedLengthBinaryString(std::string& buffer, size_t length, std::string& out) {
    if (buffer.length() >= length) {
        out = buffer.substr(0, length);
        buffer.erase(0, length);
    } else {
        out = buffer;
        buffer.clear();
    }
}

inline void check_utf8_MakeHexIfBinary(std::string& s) {}

#endif // STRINGUTILITIES_H
