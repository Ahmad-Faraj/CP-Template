#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;
struct KMP
{
    string pattern;
    vector<int> lp;

    KMP(const string &str = "") : pattern(str)
    {
        build();
    }

    void build()
    {
        int n = sz(pattern);
        lp.assing(n, 0);
        for (int i = 1; i < n; i++)
            lp[i] = failure(lp[i - 1], pattern[i]);
    }

    // build non overlapping longest suffix equal to proper prefix
    // for example, for ababab, the longest non overlapping suffixes respectively '', '', 'a', 'ab', 'a', 'ab'
    void build_non_overlapping()
    {
        int n = sz(pattern);
        lp.assign(n, 0);
        for (int i = 1; i < n; i++){
            int j = lp[i - 1];
            while (j > 0 && pattern[j] != pattern[i])
                j = lp[j - 1];
            lp[i] = j + (pattern[j] == pattern[i]);
            if(lp[i] * 2 == i + 1) // non overlapping condition
                lp[i] = lp[lp[i] - 1]; // remove the last one
        }
    }

    int failure(int idx, char nxt)
    {
        while (idx > 0 && pattern[idx] != nxt)
            idx = lp[idx - 1];
        return idx + (pattern[idx] == nxt);
    }

    vector<int> match(const string &str)
    {
        int n = sz(str), m = sz(pattern);
        vector<int> ret;
        for (int i = 0, k = 0; i < n; i++)
        {
            k = failure(k, str[i]);
            if (k == m)
                ret.push_back(i - m + 1); // 0-based indexing
        }

        for (auto &idx : ret)
            idx++; // 1-based indexing

        return ret;
    }

    vector<int> count(const string &str)
    { // count number of occurrences of each prefix
        int n = sz(str), m = sz(pattern);
        vector<int> cnt(m + 1);
        for (int i = 1, k = 0; i < n; i++)
        {
            k = failure(k, str[i]);
            cnt[k]++;
        }

        for (int i = m - 1; i > 0; i--)
            cnt[lp[i - 1]] += cnt[i]; // number of occurrences of prefix of length lp[i - 1] is at least cnt[i]

        for (int i = 0; i <= m; i++)
            cnt[i]++; // add the whole substring itself

        return cnt;
    }
    // count the number of occurrences of each prefix of pattern in another string t
    vector<int> countInAnother(const string &t)
    {
        string s = pattern + "#" + t;
        int n = sz(s);
        KMP kmp(s);
        vector<int> cnt(n);
        for (int i = sz(pattern) + 1; i < n; i++)
            cnt[kmp.lp[i]]++;
        for (int i = sz(pattern); i > 0; i--)
            cnt[kmp.lp[i - 1]] += cnt[i];
        vector<int> ans;
        for (int i = 1; i <= sz(pattern); i++)
            ans.push_back(cnt[i]);
        return ans;
    }
    /*
        A border of a string is a prefix that is also a suffix of the string but not the whole string. For example, the borders of abcababcab are ab and abcab.
    */
    vector<int> get_prefixes()
    {
        vector<int> indexes = {sz(pattern)};
        for (int i = lp[sz(pattern) - 1]; i > 0; i = lp[i - 1])
            indexes.push_back(i);
        sort(indexes.begin(), indexes.end()); // indexes of prefixes increasing
        return indexes;
    }
};
void solve(int tc)
{
    string s;
    cin >> s;
    KMP kmp(s);
    vector<int> ans = kmp.get_prefixes();
    for (auto x : ans)
        cout << x << sp;
    cout << nl;
}
signed main(void)
{
    fastio();
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}