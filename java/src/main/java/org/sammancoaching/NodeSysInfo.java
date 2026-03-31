package org.sammancoaching;

import java.util.Map;
import java.util.TreeMap;
import java.util.Objects;

public class NodeSysInfo {
    public static final int INDEX_CPU0 = 0;
    public static final int INDEX_CPU1 = 1;
    public static final int INDEX_CPU_MAX = 6;

    private CNodeSystemVersion[] swversion = new CNodeSystemVersion[INDEX_CPU_MAX + 1];
    private Map<String, String> m_addInfos = new TreeMap<>();

    public NodeSysInfo() {
        for (int i = 0; i <= INDEX_CPU_MAX; i++) {
            swversion[i] = new CNodeSystemVersion();
        }
    }

    public CNodeSystemVersion get(int i) {
        return swversion[i];
    }

    public void set(int i, CNodeSystemVersion version) {
        swversion[i] = version;
    }

    public void setAdditionalInfo(AdditionalInfoKeys key, String value) {
        m_addInfos.put(key.getKeyName(), ltrim(value));
    }

    public void setAdditionalInfo(AdditionalInfoKeys key, int value) {
        m_addInfos.put(key.getKeyName(), String.valueOf(value));
    }

    public String getAdditionalInfo(AdditionalInfoKeys key) {
        return m_addInfos.getOrDefault(key.getKeyName(), "");
    }

    public Map<String, String> getAdditionalInfos() {
        return m_addInfos;
    }

    public boolean isOverallVersionValid() {
        return get(0).isValid();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        NodeSysInfo that = (NodeSysInfo) o;
        if (swversion.length != that.swversion.length) return false;
        for (int i = 0; i < swversion.length; i++) {
            if (!Objects.equals(swversion[i], that.swversion[i])) return false;
        }
        return Objects.equals(m_addInfos, that.m_addInfos);
    }

    private String ltrim(String s) {
        if (s == null) return null;
        int i = 0;
        while (i < s.length() && Character.isWhitespace(s.charAt(i))) {
            i++;
        }
        return s.substring(i);
    }

    @Override
    public String toString() {
        return NodeSysInfoPrinter.print(this);
    }
}
