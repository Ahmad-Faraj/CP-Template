#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#define ull unsigned long long int
#define ld long double
#define ll long long
#define PI acos(-1)
#define endl "\n"
#define all(x) x.begin(), x.end()
#define dpp(arr, val) memset(arr, val, siezof(arr))
#define fixed(n) cout << fixed << setprecision(n)
#define loop(i, n) for (int i = 0; i < (int)n; i++)

const int N = 1e7;
const int MOD = 1000000007;
const int OO = 0x3f3f3f3f;
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

void Ahmed_Faraj() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}

void fast() { ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0); }

bool isplain(string x) {
    string s1 = x;
    string s2 = s1;
    reverse(s2.begin(), s2.end());
    if (s1 == s2) return 1;
    return 0;
}

bool issubstring(string s1, string s2) {
    if (s2.find(s1) != string::npos) {
        return 1;
    }
    return 0;
}

bool isprime(ll x) {
    if (x <= 1) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (int i = 5; i * i <= x; i += 6)
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

ll fact(ll x) {
    if (x == 0) {
        return 0;
    }
    if (x == 1) {
        return 1;
    }
    return x * fact(x - 1);
}

ll lcm(ll x, ll y) { return (x / __gcd(x, y)) * y; }

ll sumall(ll x) {
    ll res = (x * (x + 1)) / 2;
    return res;
}

ll sumodd(ll x) {
    ll res = (x + 1) / 2;
    return res * res;
}

ll sumeven(ll x) { return x * (x + 1); }

ll nPr(ll x, ll y) { return fact(x) / (fact(x - y)); }

ll nCr(ll num1, ll num2) {
    ll res = 1;
    ll temp = 2;
    for (int i = num2 + 1; i <= num1; i++) {
        res *= i;
        if (temp <= num1 - num2 && res % temp == 0) {
            res /= temp;
            temp++;
        }
    }
    return res;
}

ll fib(ll n) {
    ll a = 0, b = 1, c, i;
    if (n == 0) {
        return a;
    }
    for (i = 2; i < n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

bool customcompare(const pair<int, int> &a, const pair<int, int> &b) {
    if (a.first != b.first) {
        return a.first < b.first;
    }
    return a.second > b.second;
}

vector<pair<int, int>> factors;
void getFactors(int n) {
    factors.clear();
    int d = 1;
    for (int i = 2; i * i <= n; i += d, d = 2)
        if (n % i == 0) {
            factors.push_back(make_pair(i, 0));
            while (n % i == 0) {
                n /= i;
                factors.back().second++;
            }
        }
    if (n != 1) factors.push_back(make_pair(n, 1));
}

int max(vector<int> &v) {
    int n = v.size();
    vector<bool> vis(n + 1, 0);
    for (auto it : v) {
        if (it > n) continue;
        vis[it] = 1;
    }
    for (int i = 0; i <= n; i++) {
        if (!vis[i]) return i;
    }
    return n + 1;
}

vector<int> divisors;
void getDivisors(int ind = 0, int res = 1) {
    if (ind == (int)factors.size()) {
        divisors.push_back(res);
        return;
    }
    for (int i = 0; i <= factors[ind].second; i++) {
        getDivisors(ind + 1, res);
        res *= factors[ind].first;
    }
}

ll get_sum_div(ll x) {
    ll ans = 0;
    for (ll left = 1, right; left <= x; left = right + 1) {
        right = x / (x / left);
        ans += (x / left) * (left + right) * (right - left + 1) / 2;
        cout << (x / left) * (left + right) * (right - left + 1) / 2 << endl;
        cout << ans << endl;
    }
    return ans;
}

ll countDivisors(ll n) {
    ll cnt = 0;
    for (ll i = 1; i <= sqrt(n); i++) {
        if (n % i == 0) {
            if (n / i == i)
                cnt++;
            else
                cnt = cnt + 2;
        }
    }
    return cnt;
}

set<pair<ll, ll>> divs;
void generateDivisors(ll n) {
    for (ll i = 1; i <= n; ++i) {
        for (ll j = i; j <= n; ++j)
            if (lcm(i, j) == n) divs.insert(make_pair(i, j));
    }
}

ll fpow(ll num, ll power, ll mod = LLONG_MAX) {
    if (power == 0) return 1;
    if (power == 1) return num % mod;
    ll ans = fpow(num, power / 2, mod);
    ans = ((ans % mod) * (ans % mod)) % mod;
    if (power & 1) ans = ((ans % mod) * (num % mod)) % mod;
    return ans % mod;
}

void solve() {
    int n;
    cin >> n;
    vector<pair<int, int>> v(n);
    for (auto &it : v) {
        cin >> it.first >> it.second;
    }
    sort(all(v), customcompare);
    for (int i = 0; i < n; i++) {
        cout << v[i].first << " " << v[i].second << endl;
    }
}

signed main() {
    Ahmed_Faraj();
    // fast();
    ll test = 1;
    // cin >> test;
    while (test--) {
        solve();
    }
    return 0;
}