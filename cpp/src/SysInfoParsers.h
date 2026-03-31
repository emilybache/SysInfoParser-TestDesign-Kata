#ifndef SYSINFOPARSERS_H
#define SYSINFOPARSERS_H

#include <string>
#include <map>

class NodeSysInfo;

namespace SysInfoParsers {
    inline std::map<std::string, std::string> parse_idblock_v2(const std::string& s) { return {}; }
    inline std::map<std::string, std::string> parse_idblock_v3(const std::string& s) { return {}; }

    void parse_infoblock_v3(NodeSysInfo &_SysInfo, std::string &sBuffer);
    void parse_infoblock_v2(NodeSysInfo &_SysInfo, size_t cpuindex, std::string &sBuffer);
    void parse_infoblock_app1(NodeSysInfo &_SysInfo, std::string &sBuffer);
}

#endif // SYSINFOPARSERS_H
