
#ifndef NODESYSINFO_HPP_INCLUDED
#define NODESYSINFO_HPP_INCLUDED

#include <map>

#include "STLCommon.h"
#include <vector>
#include <array>
#include <string_view>
#include "CNodeVersion.h" // provides CVersionNumber, CNodeSystemVersion  todo: move ?!?
#include "NodeSysInfoAdditionalKeys.hpp"

#include <algorithm>
#include <cctype>
#include <locale>
#include "StringUtilities.h"

class NodeSysInfo {
public:
    enum {
        index_cpu0 = 0,
        index_cpu1 = 1,
        index_cpu_max = MULTI_CPU_MAX_ID
    };

    template<size_t i>
    typename std::enable_if<(i <= index_cpu_max), CNodeSystemVersion &>::type get() {
        return swversion[i];
    }

    template<size_t i>
    typename std::enable_if<(i <= index_cpu_max), CNodeSystemVersion const &>::type get() const {
        return swversion[i];
    }

    CNodeSystemVersion &get(const size_t i) {
        return swversion[i];
    }

    const CNodeSystemVersion &get(const size_t i) const {
        return swversion[i];
    }

    template<class key, class T>
    SBS_ENABLE_IF(std::is_class<T>::value, true, void) set(T _v) {
        ltrim((m_addInfos[AdditionalInfoKeys::TextKey<key>::value()] = _v));
    }

    template<class key, class T>
    SBS_ENABLE_IF(std::is_integral<T>::value, true, void) set(T _v) {
        m_addInfos[AdditionalInfoKeys::TextKey<key>::value()] = boost::lexical_cast<std::string>(_v);
    }

    template<class TKey>
    SBS_ENABLE_IF(std::is_class<TKey>::value, true, std::string) get() const {
        return map_at(m_addInfos, AdditionalInfoKeys::TextKey<TKey>::value(), std::string());
    }

    std::string get_AddInfos(const std::string &key) const {
        return map_at(m_addInfos, key, std::string()); // default to empty for now
    }

    std::map<std::string, std::string> const &get_AddInfos() const {
        return m_addInfos;
    }

    void set_AddInfos(const std::string &key, const std::string &value) {
        m_addInfos[key] = value;
    }


    bool operator==(const NodeSysInfo &r) const {
        return (swversion == r.swversion) && (m_addInfos == r.m_addInfos);
    }

    bool operator!=(const NodeSysInfo &r) const {
        return (swversion != r.swversion) || (m_addInfos != r.m_addInfos);
    }

    std::string GetSoftwareVersionPrefix() const;

    template<typename Archive>
    struct SerializationHelper;

private:
    std::array<CNodeSystemVersion, index_cpu_max + 1> swversion;

    /*
     * additional Infos as string:string map.
     * if value unknown or invalid -> key is missing!!
     */
    std::map<std::string, std::string> m_addInfos;

    friend std::string &operator<<(std::string &_sBuffer, NodeSysInfo const &_element);

    friend std::string &operator>>(std::string &_sBuffer, NodeSysInfo &_element);

    friend std::string_view &operator>>(std::string_view &_sBuffer, NodeSysInfo &_element);
};

#include "NodeSysInfoPrinter.hpp"

inline std::basic_ostream<char> &operator<<(std::basic_ostream<char> &o, const NodeSysInfo &r) {
    return o << NodeSysInfoPrinter::print(r);
}

inline std::basic_ostream<wchar_t> &operator<<(std::basic_ostream<wchar_t> &o, const NodeSysInfo &r) {
    std::string s = NodeSysInfoPrinter::print(r);
    return o << std::wstring(s.begin(), s.end());
}

std::string &operator>>(std::string &_sBuffer, NodeSysInfo &_element);

inline std::string &operator<<(std::string &_sBuffer, NodeSysInfo const &_element) {
    _sBuffer += NodeSysInfoPrinter::print(_element);
    return _sBuffer;
}

#endif // NODESYSINFO_HPP_INCLUDED
