#ifndef NODESYSINFOADDITIONALKEYS_HPP
#define NODESYSINFOADDITIONALKEYS_HPP

#include <string>

namespace AdditionalInfoKeys {
    template<typename T>
    struct TextKey {
        static std::string value() { return "unknown_key"; }
    };

    // Concrete keys as needed
    struct ArticleNr {};
    struct SerialNumber {};
    struct ProjectName {};
    struct Revision {};
    struct BuildTime {};
    struct HWId {};
    struct SystemInfo {};
    struct DeviceConnectionProxyAddress {};
    struct ArticleNumber {};
    struct ProjectNumber {};
    struct DataVersion {};

    template<> struct TextKey<ArticleNr> { static std::string value() { return "ArticleNr"; } };
    template<> struct TextKey<SerialNumber> { static std::string value() { return "SerialNumber"; } };
    template<> struct TextKey<ProjectName> { static std::string value() { return "ProjectName"; } };
    template<> struct TextKey<Revision> { static std::string value() { return "Revision"; } };
    template<> struct TextKey<BuildTime> { static std::string value() { return "BuildTime"; } };
    template<> struct TextKey<HWId> { static std::string value() { return "HWId"; } };
    template<> struct TextKey<SystemInfo> { static std::string value() { return "SystemInfo"; } };
    template<> struct TextKey<DeviceConnectionProxyAddress> { static std::string value() { return "Address"; } };
    template<> struct TextKey<ArticleNumber> { static std::string value() { return "ArticleNumber"; } };
    template<> struct TextKey<ProjectNumber> { static std::string value() { return "ProjectNumber"; } };
    template<> struct TextKey<DataVersion> { static std::string value() { return "DataVersion"; } };
}

#endif // NODESYSINFOADDITIONALKEYS_HPP
