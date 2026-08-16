#include <bits/stdc++.h>
using namespace std;
#define sz(x) ((int)(x).size())
#define ll long long

struct manacher {
    vector<ll> p;
    string s;
    manacher(string str) {
        s = str;
        build();
    }
    void build() {
        string t = "#";
        for (auto c : s) t += c, t += "#";
        int n = sz(t);
        s = t;
        p.assign(n, 1);
        run_manacher(t);
    }
    void run_manacher(string t) {
        int n = sz(t);
        ll l = 0, r = 0;
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
    ll getLongest(int cen, bool odd) {
        ll pos = 2 * cen + 1 + (!odd);
        return p[pos] - 1;
    }
    bool checkPalindrome(int l, int r) {
        int len = r - l + 1;
        int cen = (l + r) / 2;
        if (len & 1) return getLongest(cen, true) >= len;
        return getLongest(cen, false) >= len;
    }
};

void solve() {
    string s;
    cin >> s;
    manacher m(s);
    vector<ll> &p = m.p;
    int n = (int)p.size();

    ll mx = 0;
    int idx = 0;
    for (int i = 0; i < n; i++) {
        if (p[i] > mx) {
            mx = p[i];
            idx = i;
        }
    }

    // mx is radius in transformed string; palindrome length = mx - 1
    int start = (idx - mx + 1) / 2; // map back to original string index
    cout << s.substr(start, mx - 1) << endl;
}