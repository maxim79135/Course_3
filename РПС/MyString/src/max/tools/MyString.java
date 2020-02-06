package max.tools;

import org.jetbrains.annotations.Contract;

public class MyString {
    public static boolean isPrefix(String str, String sub) {
        int n = str.length(), m = sub.length();
        if (m > n) return false;
        for (int i = 0; i < m; i++) {
            if (sub.charAt(i) != str.charAt(i)) return false;
        }
        return true;
    }

    public static boolean isSuffix(String str, String sub) {
        int n = str.length(), m = sub.length();
        if (m > n) return false;

        for (int i = m - 1; i >= 0; i--) {
            if (sub.charAt(i) != str.charAt(i + n - m)) return false;
        }
        return true;
    }

    public static boolean isSubstring(String str, String sub) {
        int m = sub.length(), n = str.length();
        if (m > n) return false;
        for (int i = 0; i <= n - m; i++) {
            if (str.substring(i, i + m).equals(sub)) return true;
        }
        return false;
    }

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
