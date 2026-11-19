// KMP: prefix function (lp) of a pattern, its occurrences in a text, borders and prefix counts.
// Use when: "find all occurrences of p in s", borders, periods, "how often does each prefix occur".
// Usage: KMP k(p); k.match(s) (1-based starts); k.get_prefixes(); k.count(); k.countInAnother(t);
//        k.build_non_overlapping(); then k.nol[i].
// Handles: overlapping matches, any alphabet except '\0' (and '#' in countInAnother).
// Time: build, match, count, countInAnother, build_non_overlapping all O(|p| + |text|)
// Indexing: lp[i] / nol[i] are 0-based over the pattern: lp[i] = longest proper border of p[0..i],
//           nol[i] = longest border of p[0..i] with 2 * len <= i + 1 (non-overlapping, may touch).
//           match() returns 1-based start positions in the text.
// Note: pattern must be non-empty. count()[len] = occurrences of the prefix of length len inside
//       the pattern itself (len 1..|p|; ignore [0]). countInAnother(t)[len - 1] = occurrences of
//       the prefix of length len inside t. get_prefixes() = borders, increasing, then |p|.

#include <bits/stdc++.h>
using namespace std;

struct KMP {
    string pattern;
    vector<int> lp, nol;

    KMP(const string &str = "") : pattern(str) { build(); }

    void build() {
        int n = pattern.size();
        lp.assign(n, 0);
        for (int i = 1; i < n; i++) lp[i] = failure(lp[i - 1], pattern[i]);
    }

    void build_non_overlapping() {
        int n = pattern.size();
        nol.assign(n, 0);
        for (int i = 1, k = 0; i < n; i++) {
            while (k > 0 && pattern[k] != pattern[i]) k = lp[k - 1];
            k += pattern[k] == pattern[i];
            while (2 * k > i + 1) k = lp[k - 1];
            nol[i] = k;
        }
    }

    int failure(int idx, char nxt) {
        while (idx > 0 && pattern[idx] != nxt) idx = lp[idx - 1];
        return idx + (pattern[idx] == nxt);
    }

    vector<int> match(const string &str) {
        int n = str.size(), m = pattern.size();
        vector<int> ret;
        for (int i = 0, k = 0; i < n; i++) {
            k = failure(k, str[i]);
            if (k == m) ret.push_back(i - m + 2);
        }
        return ret;
    }

    vector<int> count() {
        int m = pattern.size();
        vector<int> cnt(m + 1);
        for (int i = 1; i < m; i++) cnt[lp[i]]++;
        for (int i = m - 1; i > 0; i--) cnt[lp[i - 1]] += cnt[i];
        for (int i = 0; i <= m; i++) cnt[i]++;
        return cnt;
    }

    vector<int> countInAnother(const string &t) {
        string s = pattern + "#" + t;
        int n = s.size();
        KMP kmp(s);
        vector<int> cnt(n);
        for (int i = pattern.size() + 1; i < n; i++) cnt[kmp.lp[i]]++;
        for (int i = pattern.size(); i > 0; i--) cnt[kmp.lp[i - 1]] += cnt[i];
        vector<int> ans;
        for (int i = 1; i <= (int)pattern.size(); i++) ans.push_back(cnt[i]);
        return ans;
    }

    // a border is a prefix that is also a suffix, not the whole string ("abcababcab": ab, abcab)
    vector<int> get_prefixes() {
        vector<int> indexes = {(int)pattern.size()};
        for (int i = lp[pattern.size() - 1]; i > 0; i = lp[i - 1]) indexes.push_back(i);
        sort(indexes.begin(), indexes.end());
        return indexes;
    }
};
