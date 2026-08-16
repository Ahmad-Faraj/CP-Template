#include "../../core.h"

// Time Complexity: O(N)
// Space Complexity: O(N)
struct Manacher {
    vector<int> p;
    string s; // Original string
    string t; // Transformed string

    Manacher(string str) : s(str) { build(); }

    void build() {
        t = "#";
        for (auto c : s) {
            t += c;
            t += "#";
        }
        int n = t.size();
        p.assign(n, 1);
        
        int l = 0, r = 0;
        for (int i = 1; i < n; i++) {
            if (i <= r) {
                p[i] = min(p[l + r - i], r - i + 1);
            }
            while (i - p[i] >= 0 && i + p[i] < n && t[i - p[i]] == t[i + p[i]]) {
                p[i]++;
            }
            if (i + p[i] - 1 > r) {
                l = i - p[i] + 1;
                r = i + p[i] - 1;
            }
        }
    }
};