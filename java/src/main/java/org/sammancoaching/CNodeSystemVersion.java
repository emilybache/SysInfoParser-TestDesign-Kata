package org.sammancoaching;

import java.util.Objects;

public class CNodeSystemVersion {
    private String m_projectName = "";
    private String m_subsystem = "";
    private CVersionNumber m_version = new CVersionNumber();

    public CNodeSystemVersion() {
    }

    public CNodeSystemVersion(String projectName, String subsystem, CVersionNumber version) {
        this.m_projectName = projectName;
        this.m_subsystem = subsystem;
        this.m_version = version;
    }

    public boolean isValid() {
        return m_version.valid;
    }

    public String getProjectName() {
        return m_projectName;
    }

    public String getSubsystem() {
        return m_subsystem;
    }

    public String getProjectNameSubsystem() {
        return m_projectName + "_" + m_subsystem;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CNodeSystemVersion that = (CNodeSystemVersion) o;
        return Objects.equals(m_projectName, that.m_projectName) &&
                Objects.equals(m_subsystem, that.m_subsystem) &&
                Objects.equals(m_version, that.m_version);
    }

    @Override
    public String toString() {
        if (!isValid()) return "invalid";
        return m_projectName + " (" + m_subsystem + ") v" + m_version.toString();
    }

    public void setValid(boolean cond) {
        m_version.valid = cond;
    }
}
