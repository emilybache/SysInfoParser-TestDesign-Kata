#ifndef CNODEVERSION_H
#define CNODEVERSION_H

#include <string>
#include <vector>
#include <type_traits>

#include <sstream>

#define MULTI_CPU_MAX_ID 6

namespace boost {
    template<typename Target, typename Source>
    Target lexical_cast(const Source& arg) {
        std::stringstream ss;
        ss << arg;
        Target res;
        ss >> res;
        return res;
    }
}

#define SBS_ENABLE_IF(condition, value, type) \
    std::enable_if_t<condition, type>

struct CVersionNumber {
    uint8_t v1 = 0;
    uint8_t v2 = 0;
    uint8_t v3 = 0;
    bool valid = false;

    CVersionNumber() = default;
    CVersionNumber(uint8_t _v1, uint8_t _v2, uint8_t _v3, bool _valid = true)
        : v1(_v1), v2(_v2), v3(_v3), valid(_valid) {}
    
    // Support initialization from string if needed (e.g. sSWVersion in SysInfoParsers.cpp)
    CVersionNumber(const std::string& s) {
        // Simple parser or just mark as valid for now if not empty
        valid = !s.empty();
    }

    bool operator==(const CVersionNumber& other) const {
        return v1 == other.v1 && v2 == other.v2 && v3 == other.v3 && valid == other.valid;
    }

    bool operator!=(const CVersionNumber& other) const {
        return !(*this == other);
    }

    std::string to_string() const {
        if (!valid) return "invalid";
        return std::to_string(v1) + "." + std::to_string(v2) + "." + std::to_string(v3);
    }
};

class CNodeSystemVersion {
public:
    CNodeSystemVersion() = default;
    CNodeSystemVersion(const std::string& projectName, const std::string& subsystem, const CVersionNumber& version)
        : m_projectName(projectName), m_subsystem(subsystem), m_version(version) {}

    bool isValid() const { return m_version.valid; }
    const std::string& ProjectName() const { return m_projectName; }
    const std::string& Subsystem() const { return m_subsystem; }
    
    std::string ProjectName_Subsystem() const {
        return m_projectName + "_" + m_subsystem;
    }

    bool operator==(const CNodeSystemVersion& other) const {
        return m_projectName == other.m_projectName &&
               m_subsystem == other.m_subsystem &&
               m_version == other.m_version;
    }

    bool operator!=(const CNodeSystemVersion& other) const {
        return !(*this == other);
    }

    std::string to_string() const {
        if (!isValid()) return "invalid";
        return m_projectName + " (" + m_subsystem + ") v" + m_version.to_string();
    }

    void Valid(bool cond);

private:
    std::string m_projectName;
    std::string m_subsystem;
    CVersionNumber m_version;
};

inline void CNodeSystemVersion::Valid(bool cond) {
    m_version.valid = cond;
}

#endif // CNODEVERSION_H
