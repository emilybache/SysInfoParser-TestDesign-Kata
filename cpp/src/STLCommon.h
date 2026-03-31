#ifndef STLCOMMON_H
#define STLCOMMON_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <chrono>
#include <mutex>
#include <cstdint>
#include <cstddef>
#include <functional>
#include <algorithm>
#include <sstream>

// chrono_clock is likely std::chrono::steady_clock or system_clock
using chrono_clock = std::chrono::steady_clock;

// scoped_lock
using scoped_lock = std::scoped_lock<std::mutex>;

// These are not standard, providing plausible implementations
inline std::string& operator<<(std::string& s, uint8_t b) {
    s.push_back(static_cast<char>(b));
    return s;
}

inline std::string& operator<<(std::string& s, uint16_t w) {
    s.push_back(static_cast<char>(w >> 8));
    s.push_back(static_cast<char>(w & 0xFF));
    return s;
}

inline std::string& operator<<(std::string& s, std::byte b) {
    s.push_back(static_cast<char>(b));
    return s;
}

inline std::byte& operator+=(std::byte& lhs, char rhs) {
    lhs = static_cast<std::byte>(static_cast<unsigned char>(lhs) + static_cast<unsigned char>(rhs));
    return lhs;
}

inline std::string& operator<<(std::string& s, const std::string& other) {
    s.append(other);
    return s;
}

inline std::string& operator>>(std::string& s, uint8_t& b) {
    if (!s.empty()) {
        b = static_cast<uint8_t>(s[0]);
        s.erase(0, 1);
    }
    return s;
}

inline std::string& operator>>(std::string& s, uint16_t& w) {
    if (s.size() >= 2) {
        w = (static_cast<uint16_t>(static_cast<uint8_t>(s[0])) << 8) | static_cast<uint16_t>(static_cast<uint8_t>(s[1]));
        s.erase(0, 2);
    }
    return s;
}

inline std::string_view& operator>>(std::string_view& s, uint8_t& b) {
    if (!s.empty()) {
        b = static_cast<uint8_t>(s[0]);
        s.remove_prefix(1);
    }
    return s;
}

// map_at helper used in NodeSysInfo.hpp
template<typename K, typename V>
V map_at(const std::map<K, V>& m, const K& key, const V& defaultValue) {
    auto it = m.find(key);
    if (it != m.end()) {
        return it->second;
    }
    return defaultValue;
}

// ltrim helper used in NodeSysInfo.hpp
inline std::string& ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

#endif // STLCOMMON_H
