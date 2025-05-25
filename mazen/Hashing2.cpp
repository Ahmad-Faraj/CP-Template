#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
//#define int long long
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
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}
const int N = 1e5 + 5;
int pw1[N], inv_pw1[N], pw2[N], inv_pw2[N], hash1[N], hash2[N], base1 = 31, base2 = 37, mod1 = 1e9 + 9, mod2 = 1e9 + 7;
ll powrmod(ll b, ll n, ll m)
{
    ll pow = 1;
    while (n > 0)
    {
        if (n % 2)
            pow = 1LL * pow * b % m;
        b = 1LL * b * b % m;
        n /= 2;
    }
    return pow;
}
void init()
{
    pw1[0] = pw2[0] = 1;
    inv_pw1[0] = inv_pw2[0] = 1;
    int inverse1 = powrmod(base1, mod1 - 2, mod1);
    int inverse2 = powrmod(base2, mod2 - 2, mod2);
    for (int i = 1; i < N; i++)
    {
        pw1[i] = 1LL * pw1[i - 1] * base1 % mod1;
        pw2[i] = 1LL * pw2[i - 1] * base2 % mod2;
        inv_pw1[i] = 1LL * inv_pw1[i - 1] * inverse1 % mod1;
        inv_pw2[i] = 1LL * inv_pw2[i - 1] * inverse2 % mod2;
    }
}
struct Hashing
{
    deque<char> hash;
    int h1, h2, len;
    Hashing()
    {
        h1 = h2 = 0;
        len = 0;
    }
    void push_back(char c)
    {
        hash.push_back(c);
        h1 = add_mod(1LL * h1 * base1 % mod1, c - 'a' + 1, mod1);
        h2 = add_mod(1LL * h2 * base2 % mod2, c - 'a' + 1, mod2);
        len++;
    }
    void pop_back()
    {
        if (len == 0)
            return;
        h1 = sub_mod(h1, hash.back() - 'a' + 1, mod1);
        h1 = 1LL * h1 * inv_pw1[1] % mod1;
        h2 = sub_mod(h2, hash.back() - 'a' + 1, mod2);
        h2 = 1LL * h2 * inv_pw2[1] % mod2;
        hash.pop_back();
        len--;
    }
    void push_front(char c)
    {
        hash.push_front(c);
        h1 = add_mod(h1, 1LL * (c - 'a' + 1) * pw1[len] % mod1, mod1);
        h2 = add_mod(h2, 1LL * (c - 'a' + 1) * pw2[len] % mod2, mod2);
        len++;
    }
    void pop_front()
    {
        if (len == 0)
            return;
        h1 = sub_mod(h1, 1LL * (hash.front() - 'a' + 1) * pw1[len - 1] % mod1, mod1);
        h2 = sub_mod(h2, 1LL * (hash.front() - 'a' + 1) * pw2[len - 1] % mod2, mod2);
        hash.pop_front();
        len--;
    }

    // combine prefix and suffix hash
    // prefix + substr + suffix
    pair<int,int> combine_prefix_suffix(const Hashing &prefix,const Hashing &suffix){
        int h1 = add_mod(suffix.h1, 1LL * prefix.h1 * pw1[suffix.len] % mod1, mod1);
        int h2 = add_mod(suffix.h2, 1LL * prefix.h2 * pw2[sz(suffix.hash)] % mod2, mod2);
        return {h1, h2};
    }
    pair<int, int> get_hash()
    {
        return {h1, h2};
    }
};
void solve(int tc){
    
}
signed main(void)
{
    fastio();
    init();
    int tc = 1;
    //cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i);
    }
    return 0;
} 