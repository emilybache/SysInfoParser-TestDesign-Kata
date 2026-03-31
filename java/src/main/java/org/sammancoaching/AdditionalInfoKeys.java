package org.sammancoaching;

public enum AdditionalInfoKeys {
    SystemInfo,
    ArticleNumber,
    SerialNumber,
    ProjectNumber,
    DataVersion,
    ArticleNr,
    ProjectName,
    Revision,
    BuildTime,
    HWId,
    Address;

    private final String keyName;

    AdditionalInfoKeys() {
        if (this.name().equals("Address")) {
            this.keyName = "Address";
        } else {
            this.keyName = this.name();
        }
    }

    public String getKeyName() {
        return keyName;
    }
}
