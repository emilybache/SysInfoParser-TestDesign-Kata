
#include "StringUtilities.h"

#include "SysInfoParsers.h"

#include <string>

#include "NodeSysInfo.hpp" //defines NodeSysInfo


void SysInfoParsers::parse_infoblock_v2(NodeSysInfo &_SysInfo, size_t cpuindex, std::string &sBuffer) {
    std::string sProjectName, sSubsystem, sProjectNumber, sSerialNumber, sSWVersion, sDataVersion, sArtNo;
    std::string sRevision, sDate, sTime;
    uint8_t bSystemInfo;
    sStringExtractFixedLengthBinaryString(sBuffer, 8, sProjectName);
    trim_if(sProjectName, is_any_of(" ,.-_\r\t\n" + std::string(1, '\0')));
    sStringExtractFixedLengthBinaryString(sBuffer, 4, sSubsystem);
    trim_if(sSubsystem, is_any_of(" ,.-_\r\t\n" + std::string(1, '\0')));
    sStringExtractFixedLengthBinaryString(sBuffer, 6, sRevision); //"%6u"
    trim_if(sRevision, is_any_of(" ,.-_\r\t\n" + std::string(1, '\0')));
    sStringExtractFixedLengthBinaryString(sBuffer, 10, sDate); //"%4hu-%2hu-%2hu"
    sStringExtractFixedLengthBinaryString(sBuffer, 8, sTime); //"%2hhu:%2hhu:%2hhu"
    sStringExtractFixedLengthBinaryString(sBuffer, 5, sProjectNumber);
    trim_if(sProjectNumber, is_any_of(" ,.-_\r\t\n" + std::string(1, '\0')));
    sStringExtractFixedLengthBinaryString(sBuffer, 6, sSerialNumber);
    trim_if(sSerialNumber, is_any_of(" ,.-_\r\t\n" + std::string(1, '\0')));
    sStringExtractFixedLengthBinaryString(sBuffer, 11, sSWVersion); //"%3hu.%3hu.%3hu"
    sStringExtractFixedLengthBinaryString(sBuffer, 11, sDataVersion); //"%3hu.%3hu.%3hu"
    trim_if(sDataVersion, is_any_of(" ,.-_\r\t\n" + std::string(1, '\0')));
    sStringExtractFixedLengthBinaryString(sBuffer, 11, sArtNo);
    trim_if(sArtNo, is_any_of(" ,.-_\r\t\n" + std::string(1, '\0')));
    sBuffer >> bSystemInfo;

    if (cpuindex == 0) 
    {
        _SysInfo.get<0>() = {sProjectName, sSubsystem, {sSWVersion}};
        _SysInfo.get<1>() = {sProjectName, sSubsystem, {sSWVersion}};
        _SysInfo.get<2>() = {};
        _SysInfo.get<3>() = {};
        _SysInfo.get<4>() = {};
        _SysInfo.get<5>() = {};
    } else {
        //these are partial information .... only store in the corresponding CPU
        _SysInfo.get(cpuindex) = {sProjectName, sSubsystem, {sSWVersion}};
    }

    if (cpuindex == 0 || cpuindex == 1) {
        check_utf8_MakeHexIfBinary(sProjectNumber); //MUST BE VALID UTF8!
        _SysInfo.set<AdditionalInfoKeys::SerialNumber>(sProjectNumber);
        check_utf8_MakeHexIfBinary(sSerialNumber); //MUST BE VALID UTF8!
        _SysInfo.set<AdditionalInfoKeys::ProjectNumber>(sSerialNumber);
        _SysInfo.set<AdditionalInfoKeys::DataVersion>(
            _SysInfo.get(cpuindex).ProjectName_Subsystem() + "_" + sDataVersion);
        check_utf8_MakeHexIfBinary(sArtNo); //MUST BE VALID UTF8!
        _SysInfo.set<AdditionalInfoKeys::ArticleNumber>(sArtNo);
        _SysInfo.set<AdditionalInfoKeys::SystemInfo>((int) bSystemInfo);
    }
}
