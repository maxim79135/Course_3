package max.tools;

public class MyString {
    /**
    * Returns true, and only if, second string is prefix of first string
    * @param str First string
    * @param sub Second string
    * @return true if second string is prefix of first string, otherwise false
     */
    public static boolean isPrefix(String str, String sub) {
        int n = str.length(), m = sub.length();
        if (m > n) return false;
        for (int i = 0; i < m; i++) {
            if (sub.charAt(i) != str.charAt(i)) return false;
        }
        return true;
    }

    /**
     * Returns true, and only if, second string is suffix of first string
     * @param str First string
     * @param sub Second string
     * @return true if second string is suffix of first string, otherwise false
     */
    public static boolean isSuffix(String str, String sub) {
        int n = str.length(), m = sub.length();
        if (m > n) return false;

        for (int i = m - 1; i >= 0; i--) {
            if (sub.charAt(i) != str.charAt(i + n - m)) return false;
        }
        return true;
    }

    /**
     * Returns true, and only if, second string is substring of first string
     * @param str First string
     * @param sub Second string
     * @return true if second string is substring of first string, otherwise false
     */
    public static boolean isSubstring(String str, String sub) {
        int m = sub.length(), n = str.length();
        if (m > n) return false;
        for (int i = 0; i <= n - m; i++) {
            if (str.substring(i, i + m).equals(sub)) return true;
        }
        return false;
    }

    /**a
     * Returns true, and only if, second string is subsequence of first string
     * @param str First string
     * @param sub Second string
     * @return true if second string is subsequence of first string, otherwise false
     */
    public static boolean isSubsequence(String str, String sub) {
        int n = str.length(), m = sub.length();
        int p = 0, t = 0, cnt = 0;
        while (p < m && m - p <= n - t) {
            cnt++;
            if (sub.charAt(p) == str.charAt(t))
                p++;
            t++;
        }
        return p == sub.length();
    }
}
