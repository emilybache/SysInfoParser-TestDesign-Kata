package org.sammancoaching;

import org.approvaltests.Approvals;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class SysInfoParsersTest {

    static class BufferArgs {
        String projectName;
        String subsystem;
        String revision;
        String date;
        String time;
        String projectNumber;
        String serialNumber;
        String swVersion;
        String dataVersion;
        String artNo;
        int systemInfo;

        public BufferArgs(String projectName, String subsystem, String revision, String date, String time,
                          String projectNumber, String serialNumber, String swVersion, String dataVersion,
                          String artNo, int systemInfo) {
            this.projectName = projectName;
            this.subsystem = subsystem;
            this.revision = revision;
            this.date = date;
            this.time = time;
            this.projectNumber = projectNumber;
            this.serialNumber = serialNumber;
            this.swVersion = swVersion;
            this.dataVersion = dataVersion;
            this.artNo = artNo;
            this.systemInfo = systemInfo;
        }
    }

    private String createBuffer(BufferArgs args) {
        StringBuilder buffer = new StringBuilder();
        buffer.append(pad(args.projectName, 8));
        buffer.append(pad(args.subsystem, 4));
        buffer.append(pad(args.revision, 6));
        buffer.append(pad(args.date, 10));
        buffer.append(pad(args.time, 8));
        buffer.append(pad(args.projectNumber, 5));
        buffer.append(pad(args.serialNumber, 6));
        buffer.append(pad(args.swVersion, 11));
        buffer.append(pad(args.dataVersion, 11));
        buffer.append(pad(args.artNo, 11));
        buffer.append((char) (args.systemInfo & 0xFF));
        return buffer.toString();
    }

    private String pad(String s, int len) {
        if (s.length() >= len) return s.substring(0, len);
        StringBuilder sb = new StringBuilder(s);
        while (sb.length() < len) {
            sb.append(' ');
        }
        return sb.toString();
    }

    private String toHexPrintable(String s) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c >= 32 && c < 127) {
                sb.append(c);
            } else {
                sb.append(String.format("\\x%02x", (int) c));
            }
        }
        return sb.toString();
    }

    private void verifyParseV2(int cpuindex, String buffer, BufferArgs args) {
        NodeSysInfo info = new NodeSysInfo();
        StringBuilder bufferCopy = new StringBuilder(buffer);

        StringBuilder ss = new StringBuilder();
        ss.append("Input:\n");
        ss.append("  cpuindex: ").append(cpuindex).append("\n");
        if (args != null) {
            ss.append("  Arguments to create_buffer:\n");
            ss.append("    projectName: ").append(toHexPrintable(args.projectName)).append("\n");
            ss.append("    subsystem: ").append(toHexPrintable(args.subsystem)).append("\n");
            ss.append("    revision: ").append(toHexPrintable(args.revision)).append("\n");
            ss.append("    date: ").append(toHexPrintable(args.date)).append("\n");
            ss.append("    time: ").append(toHexPrintable(args.time)).append("\n");
            ss.append("    projectNumber: ").append(toHexPrintable(args.projectNumber)).append("\n");
            ss.append("    serialNumber: ").append(toHexPrintable(args.serialNumber)).append("\n");
            ss.append("    swVersion: ").append(toHexPrintable(args.swVersion)).append("\n");
            ss.append("    dataVersion: ").append(toHexPrintable(args.dataVersion)).append("\n");
            ss.append("    artNo: ").append(toHexPrintable(args.artNo)).append("\n");
            ss.append("    systemInfo: ").append(args.systemInfo).append("\n");
        }
        ss.append("  buffer (hex): ").append(toHexPrintable(buffer)).append("\n");
        ss.append("  buffer length: ").append(buffer.length()).append("\n");
        ss.append("\n");

        SysInfoParsers.parse_infoblock_v2(info, cpuindex, bufferCopy);
        ss.append("Resulting ").append(NodeSysInfoPrinter.print(info));
        ss.append("\nRemaining buffer length: ").append(bufferCopy.length());
        if (bufferCopy.length() > 0) {
            ss.append(" (remaining: ").append(toHexPrintable(bufferCopy.toString())).append(")");
        }

        Approvals.verify(ss.toString());
    }

    @Test
    public void testStandardBufferCpu0() {
        BufferArgs args = new BufferArgs("PRJ1", "SUB1", "123456", "2023-10-27", "12:34:56",
                "P1234", "S12345", "1.2.3", "D1.2.3", "A1234567890", 42);
        String buffer = createBuffer(args);
        verifyParseV2(0, buffer, args);
    }

    @Test
    public void testBufferWithVariousTrimmableCharacters() {
        BufferArgs args = new BufferArgs("PRJ ", "S_B", " 123 ", "2023-10-27", "12:34:56",
                "P-12", "S.34", "1.2.3", "D_1.2", "A\t\r\n", 0);
        String bufferStr = createBuffer(args);
        char[] buffer = bufferStr.toCharArray();
        buffer[0] = 'P';
        buffer[7] = '\0';
        verifyParseV2(0, new String(buffer), args);
    }

    @Test
    public void testStandardBufferCpu0WithAssertions() {
        BufferArgs args = new BufferArgs("PRJ1", "SUB1", "123456", "2023-10-27", "12:34:56",
                "P1234", "S12345", "1.2.3", "D1.2.3", "A1234567890", 42);
        String buffer = createBuffer(args);

        NodeSysInfo info = new NodeSysInfo();
        StringBuilder bufferCopy = new StringBuilder(buffer);

        SysInfoParsers.parse_infoblock_v2(info, 0, bufferCopy);

        assertEquals("PRJ1", info.get(0).getProjectName());
        assertEquals("SUB1", info.get(0).getSubsystem());
        assertEquals("PRJ1", info.get(1).getProjectName());
        assertEquals("SUB1", info.get(1).getSubsystem());
        assertEquals("A1234567890", info.getAdditionalInfo(AdditionalInfoKeys.ArticleNumber));
        assertEquals("PRJ1_SUB1_D1.2.3", info.getAdditionalInfo(AdditionalInfoKeys.DataVersion));

        assertEquals("P1234", info.getAdditionalInfo(AdditionalInfoKeys.ProjectNumber));
        assertEquals("S12345", info.getAdditionalInfo(AdditionalInfoKeys.SerialNumber));

        assertEquals("42", info.getAdditionalInfo(AdditionalInfoKeys.SystemInfo));
        assertEquals(0, bufferCopy.length());
    }
}
