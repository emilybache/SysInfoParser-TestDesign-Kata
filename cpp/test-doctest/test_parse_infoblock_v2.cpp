#include "doctest/doctest.h"
#include "ApprovalTests.hpp"
#include "../src/SysInfoParsers.h"
#include "../src/NodeSysInfo.hpp"
#include "../src/NodeSysInfoPrinter.hpp"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace ApprovalTests;

namespace {
    std::string to_hex_printable(const std::string& s) {
        std::stringstream ss;
        for (unsigned char c : s) {
            if (std::isprint(c)) {
                ss << c;
            } else {
                ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)c << std::dec;
            }
        }
        return ss.str();
    }

    struct BufferArgs {
        std::string projectName;
        std::string subsystem;
        std::string revision;
        std::string date;
        std::string time;
        std::string projectNumber;
        std::string serialNumber;
        std::string swVersion;
        std::string dataVersion;
        std::string artNo;
        uint8_t systemInfo;
    };

    std::string create_buffer(const BufferArgs& args) {
        auto pad = [](const std::string& s, size_t len) {
            std::string res = s;
            if (res.length() > len) return res.substr(0, len);
            res.append(len - res.length(), ' ');
            return res;
        };

        std::string buffer;
        buffer += pad(args.projectName, 8);
        buffer += pad(args.subsystem, 4);
        buffer += pad(args.revision, 6);
        buffer += pad(args.date, 10);
        buffer += pad(args.time, 8);
        buffer += pad(args.projectNumber, 5);
        buffer += pad(args.serialNumber, 6);
        buffer += pad(args.swVersion, 11);
        buffer += pad(args.dataVersion, 11);
        buffer += pad(args.artNo, 11);
        buffer.push_back(static_cast<char>(args.systemInfo));
        return buffer;
    }

    void verify_parse_v2(size_t cpuindex, const std::string& buffer, const BufferArgs* args = nullptr) {
        NodeSysInfo info;
        std::string buffer_copy = buffer;
        
        std::stringstream ss;
        ss << "Input:\n";
        ss << "  cpuindex: " << cpuindex << "\n";
        if (args) {
            ss << "  Arguments to create_buffer:\n";
            ss << "    projectName: " << to_hex_printable(args->projectName) << "\n";
            ss << "    subsystem: " << to_hex_printable(args->subsystem) << "\n";
            ss << "    revision: " << to_hex_printable(args->revision) << "\n";
            ss << "    date: " << to_hex_printable(args->date) << "\n";
            ss << "    time: " << to_hex_printable(args->time) << "\n";
            ss << "    projectNumber: " << to_hex_printable(args->projectNumber) << "\n";
            ss << "    serialNumber: " << to_hex_printable(args->serialNumber) << "\n";
            ss << "    swVersion: " << to_hex_printable(args->swVersion) << "\n";
            ss << "    dataVersion: " << to_hex_printable(args->dataVersion) << "\n";
            ss << "    artNo: " << to_hex_printable(args->artNo) << "\n";
            ss << "    systemInfo: " << (int)args->systemInfo << "\n";
        }
        ss << "  buffer (hex): " << to_hex_printable(buffer) << "\n";
        ss << "  buffer length: " << buffer.length() << "\n";
        ss << "\n";

        try {
            SysInfoParsers::parse_infoblock_v2(info, cpuindex, buffer_copy);
            ss << "Resulting " << NodeSysInfoPrinter::print(info);
            ss << "\nRemaining buffer length: " << buffer_copy.length();
            if (!buffer_copy.empty()) {
                ss << " (remaining: " << to_hex_printable(buffer_copy) << ")";
            }
        } catch (const std::exception& e) {
            ss << "Exception thrown during parsing: " << e.what() << "\n";
        }

        Approvals::verify(ss.str());
    }
}

TEST_CASE("parse_infoblock_v2 characterization") {
    SUBCASE("Standard buffer cpu0") {
        BufferArgs args = {"PRJ1", "SUB1", "123456", "2023-10-27", "12:34:56",
                           "P1234", "S12345", "1.2.3", "D1.2.3", "A1234567890", 42};
        std::string buffer = create_buffer(args);
        verify_parse_v2(0, buffer, &args);
    }

    SUBCASE("Standard buffer cpu1") {
        BufferArgs args = {"PRJ1", "SUB1", "123456", "2023-10-27", "12:34:56",
                           "P1234", "S12345", "1.2.3", "D1.2.3", "A1234567890", 42};
        std::string buffer = create_buffer(args);
        verify_parse_v2(1, buffer, &args);
    }

    SUBCASE("Standard buffer cpu2") {
        BufferArgs args = {"PRJ1", "SUB1", "123456", "2023-10-27", "12:34:56",
                           "P1234", "S12345", "1.2.3", "D1.2.3", "A1234567890", 42};
        std::string buffer = create_buffer(args);
        verify_parse_v2(2, buffer, &args);
    }

    SUBCASE("Buffer with various trimmable characters") {
        // is_any_of(" ,.-_\r\t\n" + std::string(1, '\0'))
        BufferArgs args = {"PRJ ", "S_B", " 123 ", "2023-10-27", "12:34:56",
                           "P-12", "S.34", "1.2.3", "D_1.2", "A\t\r\n", 0};
        std::string buffer = create_buffer(args);
        // Add some null bytes manually
        buffer[0] = 'P';
        buffer[7] = '\0';
        verify_parse_v2(0, buffer, &args);
    }
}

TEST_CASE("parse_infoblock_v2 with assertions") {
    SUBCASE("Standard buffer cpu0 translated to assertions") {
        BufferArgs args = {"PRJ1", "SUB1", "123456", "2023-10-27", "12:34:56",
                           "P1234", "S12345", "1.2.3", "D1.2.3", "A1234567890", 42};
        std::string buffer = create_buffer(args);
        
        NodeSysInfo info;
        std::string buffer_copy = buffer;
        
        SysInfoParsers::parse_infoblock_v2(info, 0, buffer_copy);
        

        CHECK_EQ(info.get(0).ProjectName(), "PRJ1");
        CHECK_EQ(info.get(0).Subsystem(), "SUB1");
        CHECK_EQ(info.get(1).ProjectName(), "PRJ1");
        CHECK_EQ(info.get(1).Subsystem(), "SUB1");
        CHECK_EQ(info.get<AdditionalInfoKeys::ArticleNumber>(), "A1234567890");
        CHECK_EQ(info.get<AdditionalInfoKeys::DataVersion>(), "PRJ1_SUB1_D1.2.3");
        CHECK_EQ(info.get<AdditionalInfoKeys::ProjectNumber>(), "S1234");
        CHECK_EQ(info.get<AdditionalInfoKeys::SerialNumber>(), "P12345");
        CHECK_EQ(info.get<AdditionalInfoKeys::SystemInfo>(), "42");
        CHECK_EQ(buffer_copy.length(), 0u);
    }
}
