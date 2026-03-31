package org.sammancoaching;

public class SysInfoParsers {
    public static void parse_infoblock_v2(NodeSysInfo _SysInfo, int cpuindex, StringBuilder sBuffer) {
        String sProjectName, sSubsystem, sProjectNumber, sSerialNumber, sSWVersion, sDataVersion, sArtNo;
        String sRevision, sDate, sTime;
        int bSystemInfo;

        sProjectName = extractFixedLength(sBuffer, 8);
        sProjectName = trimIf(sProjectName, " ,.-_\r\t\n\0");

        sSubsystem = extractFixedLength(sBuffer, 4);
        sSubsystem = trimIf(sSubsystem, " ,.-_\r\t\n\0");

        sRevision = extractFixedLength(sBuffer, 6);
        sRevision = trimIf(sRevision, " ,.-_\r\t\n\0");

        sDate = extractFixedLength(sBuffer, 10);
        sTime = extractFixedLength(sBuffer, 8);

        sProjectNumber = extractFixedLength(sBuffer, 5);
        sProjectNumber = trimIf(sProjectNumber, " ,.-_\r\t\n\0");

        sSerialNumber = extractFixedLength(sBuffer, 6);
        sSerialNumber = trimIf(sSerialNumber, " ,.-_\r\t\n\0");

        sSWVersion = extractFixedLength(sBuffer, 11);
        sDataVersion = extractFixedLength(sBuffer, 11);
        sDataVersion = trimIf(sDataVersion, " ,.-_\r\t\n\0");

        sArtNo = extractFixedLength(sBuffer, 11);
        sArtNo = trimIf(sArtNo, " ,.-_\r\t\n\0");

        bSystemInfo = !sBuffer.isEmpty() ? (sBuffer.charAt(0) & 0xFF) : 0;
        if (!sBuffer.isEmpty()) {
            sBuffer.deleteCharAt(0);
        }

        if (cpuindex == 0) {
            _SysInfo.set(0, new CNodeSystemVersion(sProjectName, sSubsystem, new CVersionNumber(sSWVersion)));
            _SysInfo.set(1, new CNodeSystemVersion(sProjectName, sSubsystem, new CVersionNumber(sSWVersion)));
            _SysInfo.set(2, new CNodeSystemVersion());
            _SysInfo.set(3, new CNodeSystemVersion());
            _SysInfo.set(4, new CNodeSystemVersion());
            _SysInfo.set(5, new CNodeSystemVersion());
        } else {
            _SysInfo.set(cpuindex, new CNodeSystemVersion(sProjectName, sSubsystem, new CVersionNumber(sSWVersion)));
        }

        if (cpuindex == 0 || cpuindex == 1) {
            check_utf8_MakeHexIfBinary(sProjectNumber);
            _SysInfo.setAdditionalInfo(AdditionalInfoKeys.SerialNumber, sProjectNumber);
            check_utf8_MakeHexIfBinary(sSerialNumber);
            _SysInfo.setAdditionalInfo(AdditionalInfoKeys.ProjectNumber, sSerialNumber);
            _SysInfo.setAdditionalInfo(AdditionalInfoKeys.DataVersion,
                    _SysInfo.get(cpuindex).getProjectNameSubsystem() + "_" + sDataVersion);
            check_utf8_MakeHexIfBinary(sArtNo);
            _SysInfo.setAdditionalInfo(AdditionalInfoKeys.ArticleNumber, sArtNo);
            _SysInfo.setAdditionalInfo(AdditionalInfoKeys.SystemInfo, bSystemInfo);
        }
    }

    private static String extractFixedLength(StringBuilder buffer, int length) {
        int actualLength = Math.min(length, buffer.length());
        String result = buffer.substring(0, actualLength);
        buffer.delete(0, actualLength);
        return result;
    }

    private static String trimIf(String s, String set) {
        int start = 0;
        while (start < s.length() && set.indexOf(s.charAt(start)) != -1) {
            start++;
        }
        int end = s.length();
        while (end > start && set.indexOf(s.charAt(end - 1)) != -1) {
            end--;
        }
        return s.substring(start, end);
    }

    private static void check_utf8_MakeHexIfBinary(String s) {
        // Mock implementation
    }
}
