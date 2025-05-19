#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define PI acos(-1)
#define all(x) x.begin(),x.end()
#define clr(v, d) memset(v, d, sizeof(v))
#define endl "\n"
#define sz(x) (int)x.size()
#define vl vector<ll>
#define pb push_back
#define modo  1000000007

void M7MDAN_FARAG() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}

struct SuffixArray {
    string s;
    vector<int> p, c, lcp;

    SuffixArray(string &s) : s(s + "$"), p(sz(s) + 1), c(sz(s) + 1), lcp(sz(s) + 1) {
        build();
        build_lcp();
    }

    void build() {
        int n = sz(s);
        vector<pair<char, int>> a(n);
        for (int i = 0; i < n; i++)
            a[i] = {s[i], i};
        sort(a.begin(), a.end());
        for (int i = 0; i < n; i++)
            p[i] = a[i].second;
        c[p[0]] = 0;
        for (int i = 1; i < n; i++)
            if (a[i].first == a[i - 1].first)
                c[p[i]] = c[p[i - 1]];
            else
                c[p[i]] = c[p[i - 1]] + 1;
        int k = 0;
        while ((1 << k) < n) {
            vector<pair<pair<int, int>, int>> a(n);
            for (int i = 0; i < n; i++)
                a[i] = {{c[i], c[(i + (1 << k)) % n]}, i};
            radix_sort(a);
            for (int i = 0; i < n; i++)
                p[i] = a[i].second;
            c[p[0]] = 0;
            for (int i = 1; i < n; i++)
                if (a[i].first == a[i - 1].first)
                    c[p[i]] = c[p[i - 1]];
                else
                    c[p[i]] = c[p[i - 1]] + 1;
            k++;
        }
    }

    // Build the LCP array
    void build_lcp() {
        int k = 0;
        int n = sz(s);
        lcp.resize(n);
        for (int i = 0; i < n - 1; i++) {
            int pi = c[i];
            int j = p[pi - 1]; // previous suffix in the suffix array
            while (s[i + k] == s[j + k])
                k++;
            lcp[pi] = k;
            k = max(k - 1, 0);
        }
    }

    // Radix sort for the suffix array
    void radix_sort(vector<pair<pair<int, int>, int>> &a) {
        int n = sz(a);
        vector<int> cnt(n);
        for (auto x : a)
            cnt[x.first.second]++;
        vector<pair<pair<int, int>, int>> a_new(n);
        vector<int> pos(n);
        pos[0] = 0;
        for (int i = 1; i < n; i++)
            pos[i] = pos[i - 1] + cnt[i - 1];
        for (auto x : a) {
            int i = x.first.second;
            a_new[pos[i]] = x;
            pos[i]++;
        }
        a = a_new;

        cnt.assign(n, 0);
        for (auto x : a)
            cnt[x.first.first]++;
        a_new.assign(n, {{0, 0}, 0});
        pos[0] = 0;
        for (int i = 1; i < n; i++)
            pos[i] = pos[i - 1] + cnt[i - 1];
        for (auto x : a) {
            int i = x.first.first;
            a_new[pos[i]] = x;
            pos[i]++;
        }
        a = a_new;
    }

    // Count distinct substrings in the string
    vector<ll> distinct_substrings_per_letter() {
        ll n = sz(s);
        vector<ll> result(26, 0);  // To store result for each letter
        for (int i = 1; i < n; i++) {
            int start = p[i];
            char start_char = s[start];  // Get the starting character of the suffix
            result[start_char - 'a'] += n - 1 - start - lcp[i];  // count distinct substrings for this suffix
        }
        return result;
    }

};

void solve() {
   
}

signed main() {
    M7MDAN_FARAG();
    ll ts = 1;
    // cin >> ts;
    while (ts--) {
        solve();
    }
}
