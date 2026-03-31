#include "doctest/doctest.h"
#include "../src/StringUtilities.h"
#include <string>

TEST_CASE("sStringExtractFixedLengthBinaryString extracts fixed length") {
    std::string source = "ABCDEFGHIJ";
    std::string result;

    SUBCASE("Extract first 4 characters") {
        sStringExtractFixedLengthBinaryString(source, 4, result);
        CHECK(result == "ABCD");
        CHECK(source == "EFGHIJ");
    }

    SUBCASE("Extract from remaining") {
        sStringExtractFixedLengthBinaryString(source, 4, result);
        sStringExtractFixedLengthBinaryString(source, 3, result);
        CHECK(result == "EFG");
        CHECK(source == "HIJ");
    }

    SUBCASE("Extract more than available") {
        sStringExtractFixedLengthBinaryString(source, 20, result);
        CHECK(result == "ABCDEFGHIJ");
        CHECK(source == "");
    }

    SUBCASE("Extract from empty string") {
        std::string emptySource = "";
        sStringExtractFixedLengthBinaryString(emptySource, 5, result);
        CHECK(result == "");
        CHECK(emptySource == "");
    }
}

