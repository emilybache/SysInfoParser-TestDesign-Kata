package org.sammancoaching;

import java.util.Map;

public class NodeSysInfoPrinter {
    public static String print(NodeSysInfo info) {
        StringBuilder result = new StringBuilder("NodeSysInfo:\n");
        result.append("  Software Versions:\n");
        for (int i = 0; i <= NodeSysInfo.INDEX_CPU_MAX; i++) {
            CNodeSystemVersion swversion = info.get(i);
            if (swversion.isValid()) {
                result.append("    CPU ")
                      .append(i)
                      .append(": ")
                      .append(swversion.toString())
                      .append("\n");
            }
        }
        Map<String, String> addInfos = info.getAdditionalInfos();
        if (!addInfos.isEmpty()) {
            result.append("  Additional Infos:\n");
            for (Map.Entry<String, String> entry : addInfos.entrySet()) {
                result.append("    ")
                      .append(entry.getKey())
                      .append(": ")
                      .append(entry.getValue())
                      .append("\n");
            }
        }
        return result.toString();
    }
}
