// Booth: the lexicographically smallest rotation of a string, in O(n).
// Use when: "smallest rotation", canonical form of a cyclic string, comparing necklaces or circular sequences.
// Handles: any alphabet, repeated characters, fully periodic strings. Returns the rotation itself.
// Time: O(n)
// Indexing: 0-based; the input is left untouched
// Note: for the largest rotation, flip the two `<` comparisons inside the loop.

#include <bits/stdc++.h>
using namespace std;

string least_rotation(string s) {
    s += s;
    int n = (int)s.size();
    vector<int> f(n, -1);
    int k = 0;
    for (int j = 1; j < n; j++) {
        char sj = s[j];
        int i = f[j - k - 1];
        while (i != -1 && sj != s[k + i + 1]) {
            if (sj < s[k + i + 1]) k = j - i - 1;
            i = f[i];
        }
        if (sj != s[k + i + 1]) {
            if (sj < s[k]) k = j;
            f[j - k] = -1;
        } else
            f[j - k] = i + 1;
    }
    return s.substr(k, n / 2);
}

// Standard problem: print the lexicographically smallest rotation of s
void solve() {
    string s;
    cin >> s;
    cout << least_rotation(s) << '\n';
}
