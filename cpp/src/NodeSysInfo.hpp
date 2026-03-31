
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

/**
 *   Holds, struct and access information about SysInfo of a Node.
 *
 *   contains:
 *      SoftwareVersion / NodeVersion
 *      unique identifier for SW: Projectname_subprojectname_mainversion.subversion.revision
 *
 *   optional:
 *
 *
 */

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

    std::string getAddInfosAsString(const char *equals = "=", const char *separator = ",") const {
        std::string result;
        for (auto it = m_addInfos.begin(); it != m_addInfos.end(); ++it) {
            if (!result.empty()) {
                result += separator;
            }
            result += it->first + equals + it->second;
        }
        return result;
    }

    bool isOverallVersionValid() const {
        return get<0>().isValid();
    }

    void invalidateCPU4IfOtherCPUsChanged(const NodeSysInfo &Old_SysInfo) {
        if ((Old_SysInfo.get<1>() != get<1>() //has anything changed in the CPU infos 1-3?
             || Old_SysInfo.get<2>() != get<2>()
             || Old_SysInfo.get<3>() != get<3>()
            )
            && get<4>().isValid()
        ) {
            /* something has changed in cpu1-3 -> and CPU4 is valid ...
                         * invalidate cpu4 -> so that it is read again, since we don't know if it has changed ...
                         *  - that is the crux of the 3+2 method established by HEK ...
                         */
            get<4>().Valid(false);
            get<5>().Valid(false);
            get<6>().Valid(false);
            //MULTI_CPU_MAX_ID
        }
    }

    void invalidateOverallVersionIfAnyCPUChanged(const NodeSysInfo &Old_SysInfo) {
        if (Old_SysInfo.get<1>() != get<1>() //has anything changed in the CPU infos 1-x?
            || Old_SysInfo.get<2>() != get<2>()
            || Old_SysInfo.get<3>() != get<3>()
            || Old_SysInfo.get<4>() != get<4>()
            || Old_SysInfo.get<5>() != get<5>()
            || Old_SysInfo.get<6>() != get<6>()
            //MULTI_CPU_MAX_ID
            || Old_SysInfo.get<0>().ProjectName() != get<0>().ProjectName()
            || Old_SysInfo.get<0>().Subsystem() != get<0>().Subsystem()
        ) {
            //invalidate VersionNumber CPU0 - OverallVersion -> this might trigger container scanner again
            get<0>() = {get<0>().ProjectName(), get<0>().Subsystem(), {}};
        }
    }

    bool isSingleCPUSystemCandidate() const {
        return get<0>().isValid()
               && get<1>().isValid()
               && get<0>() == get<1>()
               && !get<2>().isValid()
               && !get<3>().isValid()
               && !get<4>().isValid()
               && !get<5>().isValid();
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
    // see swversion[0] : CNodeSystemVersion version;
    /**
     * 0 = Node System Version
     * 1..6 = CPUs //MULTI_CPU_MAX_ID
     */
    std::array<CNodeSystemVersion, index_cpu_max + 1> swversion;

    /*
     * additional Infos as string:string map.
     * if value unknown or invalid -> key is missing!!
     */
    std::map<std::string, std::string> m_addInfos;

    /* do we need the keys extra ?
     * serial     - serial number of the node. 6 bytes. [A-Z][0-9]{5} or ??????
     * ProjectNumber                                   {5}
     *
     * DataVersion   [11];  deprecated
     * ArticleNr     [11];   //TODO: used ? format ? in newer systems only 7 characters
     * HWId           [7];   //TODO: format ? meaning ?
     * SystemInfo     0-255;  //TODO: format  :  Brownout, Secure
     *
     * Revision       0-65535;  //
     * buildtime      //TODO: format ?  date(10) ' ' time(8) -> 19
     *
     *
     */
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
