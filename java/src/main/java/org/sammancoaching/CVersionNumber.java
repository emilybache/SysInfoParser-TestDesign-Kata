package org.sammancoaching;

public class CVersionNumber {
    public int v1 = 0;
    public int v2 = 0;
    public int v3 = 0;
    public boolean valid = false;

    public CVersionNumber() {
    }

    public CVersionNumber(int v1, int v2, int v3, boolean valid) {
        this.v1 = v1;
        this.v2 = v2;
        this.v3 = v3;
        this.valid = valid;
    }

    public CVersionNumber(int v1, int v2, int v3) {
        this(v1, v2, v3, true);
    }

    public CVersionNumber(String s) {
        // Simple initialization from string if not empty
        this.valid = s != null && !s.isEmpty();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CVersionNumber that = (CVersionNumber) o;
        return v1 == that.v1 && v2 == that.v2 && v3 == that.v3 && valid == that.valid;
    }

    @Override
    public String toString() {
        if (!valid) return "invalid";
        return v1 + "." + v2 + "." + v3;
    }
}
