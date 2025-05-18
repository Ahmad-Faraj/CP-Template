#include <bits/stdc++.h>

using namespace std;

#define fixed(n) fixed << setprecision(n)
#define ceil(n, m) (((n) + (m) - 1) / (m))
#define add_mod(a, b, m) (((a % m) + (b % m)) % m)
#define sub_mod(a, b, m) (((a % m) - (b % m) + m) % m)
#define mul_mod(a, b, m) (((a % m) * (b % m)) % m)
#define all(vec) vec.begin(), vec.end()
#define rall(vec) vec.rbegin(), vec.rend()
#define sz(x) int(x.size())
#define debug(x) cout << #x << ": " << (x) << "\n";
#define fi first
#define se second
#define ll long long
#define ull unsigned long long
#define EPS 1e-9
constexpr int INF = 1 << 30, Mod = 1e9 + 7;
constexpr ll LINF = 1LL << 62;
#define PI acos(-1)
template <typename T = int>
using Pair = pair<T, T>;
vector<string> RET = {"NO", "YES"};

template <typename T = int>
istream &operator>>(istream &in, vector<T> &v)
{
    for (auto &x : v)
        in >> x;
    return in;
}

template <typename T = int>
ostream &operator<<(ostream &out, const vector<T> &v)
{
    for (const T &x : v)
        out << x << ' ';
    return out;
}

constexpr int MOD = 998244353;

ll fast_power(ll b, ll e)
{
    ll ret = 1;
    while (e)
    {
        if (e & 1)
            ret = (ret * b) % MOD;
        b = (b * b) % MOD;
        e >>= 1;
    }
    return ret;
}

ll inv(ll x)
{
    return fast_power(x, MOD - 2);
}

ll inv5;
map<ll, ll> dp;

ll prob(ll n)
{
    if (n <= 1)
        return n == 1;
    if (dp.count(n))
        return dp[n];
    ll ret = 0;
    for (int i = 2; i <= 6; i++)
        if (n % i == 0)
            ret = add_mod(ret, mul_mod(prob(n / i), inv5, MOD), MOD);
    return dp[n] = ret;
}

void Solve()
{
    ll n;
    cin >> n;
    inv5 = inv(5);
    cout << prob(n) << '\n';
}

int main()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int test_cases = 1;
    // cin >> test_cases;
    for (int tc = 1; tc <= test_cases; tc++)
    {
        // cout << "Case #" << tc << ": ";
        Solve();
    }
    return 0;
}