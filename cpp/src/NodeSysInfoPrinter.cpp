#include "NodeSysInfoPrinter.hpp"
#include "NodeSysInfo.hpp"

std::string NodeSysInfoPrinter::print(const NodeSysInfo& info) {
    std::string result = "NodeSysInfo:\n";
    result += "  Software Versions:\n";
    for (size_t i = 0; i <= NodeSysInfo::index_cpu_max; ++i) {
        const auto& swversion = info.get(i);
        if (swversion.isValid()) {
            result += "    CPU " + std::to_string(i) + ": " + swversion.to_string() + "\n";
        }
    }
    if (!info.get_AddInfos().empty()) {
        result += "  Additional Infos:\n";
        for (auto const& [key, value] : info.get_AddInfos()) {
            result += "    " + key + ": " + value + "\n";
        }
    }
    return result;
}
