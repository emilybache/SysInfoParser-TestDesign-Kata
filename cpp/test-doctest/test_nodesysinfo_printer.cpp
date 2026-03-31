#include "doctest/doctest.h"
#include "../src/NodeSysInfo.hpp"
#include "../src/NodeSysInfoPrinter.hpp"
#include <string>

#include "Approvals.h"

TEST_CASE("NodeSysInfoPrinter::print") {
    NodeSysInfo info;

    SUBCASE("empty NodeSysInfo") {
        ApprovalTests::Approvals::verify(NodeSysInfoPrinter::print(info));
    }

    SUBCASE("NodeSysInfo with one CPU version") {
        CVersionNumber v(2, 4, 0, true);
        CNodeSystemVersion sv("MainProj", "CPU0", v);
        info.get(0) = sv;
        
        ApprovalTests::Approvals::verify(NodeSysInfoPrinter::print(info));
    }

    SUBCASE("NodeSysInfo with multiple CPU versions and additional info") {
        info.get(0) = CNodeSystemVersion("MainProj", "CPU0", CVersionNumber(1, 0, 0));
        info.get(1) = CNodeSystemVersion("MainProj", "CPU1", CVersionNumber(1, 1, 0));
        
        info.set_AddInfos("SerialNumber", "SN12345");
        info.set_AddInfos("ArticleNumber", "ART678");

        ApprovalTests::Approvals::verify(NodeSysInfoPrinter::print(info));
    }
}
