// Manacher: longest palindromic substring, and whether any given range is a palindrome, in O(n).
// Use when: "longest palindrome", "is s[l..r] a palindrome" asked many times, counting palindromic substrings.
// Handles: odd and even palindromes alike, O(1) range palindrome checks, any alphabet.
// Time: build O(n) | getLongest O(1) | checkPalindrome O(1)
// Indexing: 0-based over the original string
// Note: p[] is indexed over the padded string t, never over s. Go through getLongest() / checkPalindrome().

#include <bits/stdc++.h>
using namespace std;
#define sz(x) ((int)(x).size())
using ll = long long;

struct manacher {
    vector<ll> p;
    string s, t; // s stays the original, t is the '#'-padded form that p indexes

    manacher(const string &str) : s(str) { build(); }

    void build() {
        t = "#";
        for (char c : s) t += c, t += '#';
        p.assign(sz(t), 1);
        run_manacher();
    }

    void run_manacher() {
        int n = sz(t);
        ll l = 0, r = 0;
        for (int i = 1; i < n; i++) {
            if (i <= r)
                p[i] = min(p[l + r - i], r - i + 1);
            else
                p[i] = 1;
            while (i - p[i] >= 0 && i + p[i] < n && t[i - p[i]] == t[i + p[i]]) p[i]++;
            if (i + p[i] - 1 > r) l = i - p[i] + 1, r = i + p[i] - 1;
        }
    }

    ll getLongest(int cen, bool odd) { // length of the longest palindrome centred at cen
        ll pos = 2 * cen + 1 + (!odd);
        return p[pos] - 1;
    }

    bool checkPalindrome(int l, int r) { // is s[l .. r] a palindrome
        int len = r - l + 1, cen = (l + r) / 2;
        return getLongest(cen, len & 1) >= len;
    }

    string longest() { // the longest palindromic substring of s
        ll mx = 0;
        int idx = 0;
        for (int i = 0; i < sz(t); i++)
            if (p[i] > mx) mx = p[i], idx = i;
        return s.substr((idx - mx + 1) / 2, mx - 1);
    }
};

// Standard problem: print the longest palindromic substring of s, then answer q range-palindrome queries
void solve() {
    string s;
    int q;
    cin >> s >> q;
    manacher m(s);
    cout << m.longest() << '\n';
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << (m.checkPalindrome(l - 1, r - 1) ? "YES" : "NO") << '\n';
    }
}
