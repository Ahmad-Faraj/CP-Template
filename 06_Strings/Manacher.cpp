// Manacher: longest palindrome around every center, so any range is checked for palindrome in O(1).
// Use when: "longest palindromic substring", "is s[l..r] a palindrome" many times.
// Usage: manacher m(s); m.getLongest(c, odd); m.checkPalindrome(l, r);
// Handles: odd and even palindromes, any alphabet except '#'.
// Time: build O(n) | getLongest O(1) | checkPalindrome O(1)
// Indexing: 0-based over the original string. getLongest(c, true) = longest odd palindrome
//           centred at c; getLongest(c, false) = longest even one between c and c + 1 (0 if none).
// Note: after construction s is the '#'-padded string t ("#a#b#"), and p[i] - 1 is the length in
//       the original string of the longest palindrome centred at t[i].
//       Longest overall: max p[i] - 1, starting at (i - p[i] + 1) / 2 in the original string.

#include <bits/stdc++.h>
using namespace std;

struct manacher {
    vector<long long> p;
    string s;
    manacher(string str) {
        s = str;
        build();
    }
    void build() {
        string t = "#";
        for (auto c : s) t += c, t += "#";
        int n = t.size();
        s = t;
        p.assign(n, 1);
        run_manacher(t);
    }
    void run_manacher(const string &t) {
        int n = t.size();
        long long l = 0, r = 0;
        for (int i = 1; i < n; i++) {
            if (i <= r)
                p[i] = min(p[l + r - i], r - i + 1);
            else
                p[i] = 1;

            while (i - p[i] >= 0 && i + p[i] < n && t[i - p[i]] == t[i + p[i]]) p[i]++;
            if (i + p[i] - 1 > r) {
                l = i - p[i] + 1;
                r = i + p[i] - 1;
            }
        }
    }
    long long getLongest(int cen, bool odd) {
        long long pos = 2 * cen + 1 + (!odd);
        return p[pos] - 1;
    }
    bool checkPalindrome(int l, int r) {
        int len = r - l + 1;
        int cen = (l + r) / 2;
        if (len & 1) return getLongest(cen, true) >= len;
        return getLongest(cen, false) >= len;
    }
};
