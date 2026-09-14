#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'
#define all(x) x.begin(), x.end()

vector<int> prefix_function(const string &s) {
    int n = sz(s);
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    return pi;
}

int kmp(const string &text, const string &pattern) {
    int n = sz(text);
    int m = sz(pattern);

    if (m > n) return 0;

    vector<int> pi = prefix_function(pattern);
    int cnt = 0;

    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && text[i] != pattern[j]) j = pi[j - 1];

        if (text[i] == pattern[j]) j++;

        if (j == m) {
            cnt++;
            j = pi[j - 1]; // j = 0 when you don't need overlapping
        }
    }

    return cnt;
}