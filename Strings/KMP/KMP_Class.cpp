#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'
#define all(x) x.begin(), x.end()

struct KMP {
    string pattern;
    vector<int> pi;

    KMP(const string &s = "") : pattern(s) {
        build_overlapping();
    }

    // build overlapping "ababab"  0 0 1 2 3 5
    void build_overlapping() {
        int n = sz(pattern);
        pi.assign(n, 0);

        for (int i = 1; i < n; i++) {
            int j = pi[i - 1];

            while (j > 0 && pattern[i] != pattern[j]) j = pi[j - 1];

            if (pattern[i] == pattern[j]) j++;

            pi[i] = j;
        }
    }

    // build non-overlapping "ababab" 0 0 1 2 1 2
    void build_non_overlapping() {
        int n = sz(pattern);
        pi.assign(n, 0);

        for (int i = 1; i < n; i++) {
            int j = pi[i - 1];

            while (j > 0 && pattern[i] != pattern[j]) j = pi[j - 1];

            if (pattern[i] == pattern[j]) j++;

            pi[i] = j;

            if (pi[i] * 2 > i + 1) pi[i] = pi[pi[i] - 1];
        }
    }

    vector<int> match(const string &text) {
        int n = sz(text);
        int m = sz(pattern);

        vector<int> ans;
        if (m > n) return ans;

        for (int i = 0, j = 0; i < n; i++) {
            while (j > 0 && text[i] != pattern[j]) j = pi[j - 1];

            if (text[i] == pattern[j]) j++;

            if (j == m) {
                ans.push_back(i - m + 2);
                j = pi[j - 1]; // j = 0 when you don't need overlapping
            }
        }

        return ans;
    }

    // count all prefixes : "abab" count all [ a, ab, aba, abab ]
    vector<int> count_in(const string &text) {
        string s = pattern + "#" + text;

        KMP kmp(s);

        int n = sz(s);
        int m = sz(pattern);

        vector<int> cnt(n, 0);
        if (m > n) return cnt;

        for (int i = m + 1; i < n; i++) cnt[kmp.pi[i]]++;

        for (int i = m; i > 0; i--) cnt[kmp.pi[i - 1]] += cnt[i];

        vector<int> ans;

        for (int len = 1; len <= m; len++) ans.push_back(cnt[len]);

        return ans;
    }

    // return all borders
    vector<int> get_borders() {
        vector<int> borders;

        borders.push_back(sz(pattern));

        for (int len = pi.back(); len > 0; len = pi[len - 1]) borders.push_back(len);

        reverse(all(borders));

        return borders;
    }
};