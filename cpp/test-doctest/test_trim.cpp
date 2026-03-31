#include "doctest/doctest.h"
#include <string>
#include <vector>
#include <algorithm>

// We'll implement them here for now to test, then move to a header
namespace alternative {
    auto is_any_of(const std::string& characters) {
        return [characters](char c) {
            return characters.find(c) != std::string::npos;
        };
    }

    template<typename Predicate>
    void trim_if(std::string& s, Predicate p) {
        // Trim from the beginning
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&p](char c) { return !p(c); }));
        // Trim from the end
        s.erase(std::find_if(s.rbegin(), s.rend(), [&p](char c) { return !p(c); }).base(), s.end());
    }
}

TEST_CASE("trim_if and is_any_of alternative implementation") {
    std::string test_chars = " ,.-_\r\t\n" + std::string(1, '\0');
    auto predicate = alternative::is_any_of(test_chars);

    SUBCASE("trim spaces") {
        std::string s = "  hello  ";
        alternative::trim_if(s, predicate);
        CHECK(s == "hello");
    }

    SUBCASE("trim mix of characters") {
        std::string s = " ,.-_\r\t\n";
        s.push_back('\0');
        s += "hello \n\t";
        s.push_back('\0');
        alternative::trim_if(s, predicate);
        CHECK(s == "hello");
    }

    SUBCASE("all characters are trimmed") {
        std::string s = " ,.-_";
        alternative::trim_if(s, predicate);
        CHECK(s == "");
    }

    SUBCASE("no characters are trimmed") {
        std::string s = "hello";
        alternative::trim_if(s, predicate);
        CHECK(s == "hello");
    }
    
    SUBCASE("null character at start") {
        std::string s = std::string(1, '\0') + "hello";
        alternative::trim_if(s, predicate);
        CHECK(s == "hello");
    }
}
