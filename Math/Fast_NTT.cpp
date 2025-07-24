#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define ld long double

// Used when have a good mod but in put size very large exceeding 2^k
// Modulus used for NTT: 998244353 = 119 * 2^23 + 1, which is a prime.
const int special_mod = 998244353;
const int primitive_root = 3; // Primitive root modulo special_mod
const int lim = 64; // Threshold under which naive multiplication is used

// Fast modular exponentiation: computes (b^n) % m
ll powrmod(ll b, ll n, ll m) {
    ll result = 1;
    while (n > 0) {
        if (n & 1)
            result = result * b % m;
        b = b * b % m;
        n >>= 1;
    }
    return result;
}

// Modular division by 2: ensures the result is valid modulo special_mod
int mod_div(int x) {
    if (x >= special_mod) x -= special_mod;
    if (x & 1) x += special_mod;
    return x / 2;
}

// Splits a polynomial into two parts for recursive processing
// a_minus[i] = a[i] + c * a[i + n/2]
// a_plus[i]  = a[i] - c * a[i + n/2]
pair<vector<int>, vector<int>> mod_poly(vector<int> &a, int x) {
    int n = a.size();
    int c = powrmod(primitive_root, x, special_mod);

    vector<int> a_minus(n / 2), a_plus(n / 2);
    for (int i = 0; i < n / 2; i++) {
        int tmp = (ll)c * a[i + n / 2] % special_mod;

        int m = a[i] + tmp;
        a_minus[i] = (m >= special_mod ? m - special_mod : m);

        int p = a[i] - tmp;
        a_plus[i] = (p < 0 ? p + special_mod : p);
    }

    return {a_minus, a_plus};
}

// Recursive Fast Half-NTT Polynomial Multiplication
vector<int> fast_ntt_poly_mul(vector<int> &a, vector<int> &b, int x) {
    int n = a.size();

    // Base case: use naive O(n^2) multiplication for small n or odd size
    if ((n <= lim) || (n & 1)) {
        vector<unsigned ll> temp(2 * n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                temp[i + j] += (ll)a[i] * b[j];

            // Periodically reduce to avoid overflow
            if ((i & 15) == 15)
                for (int j = i + 1; j < i + n; j++)
                    temp[j] %= special_mod;
        }

        // Combine result using root of unity
        vector<int> result(n);
        int c = powrmod(primitive_root, x, special_mod);
        for (int i = 0; i < n; i++)
            result[i] = (temp[i] + c * (temp[i + n] % special_mod)) % special_mod;

        return result;
    }

    // Recursive case: divide-and-conquer
    int x1 = x / 2;
    int x2 = (x1 + special_mod / 2) % (special_mod - 1);

    vector<int> ans_minus, ans_plus;
    {
        // Evaluate both a and b at two different roots
        auto [a_minus, a_plus] = mod_poly(a, x1);
        auto [b_minus, b_plus] = mod_poly(b, x1);

        // Recurse on both parts
        ans_minus = fast_ntt_poly_mul(a_minus, b_minus, x1);
        ans_plus = fast_ntt_poly_mul(a_plus, b_plus, x2);
    }

    // Compute final result by combining answers
    int sqrtc_inv = powrmod(primitive_root, special_mod - 1 - x1, special_mod);
    int two_sqrtc_inv = mod_div(sqrtc_inv);

    vector<int> result(n);
    for (int i = 0; i < n / 2; i++)
        result[i] = mod_div(ans_minus[i] + ans_plus[i]);
    for (int i = 0; i < n / 2; i++)
        result[i + n / 2] = (ll)(ans_minus[i] - ans_plus[i] + special_mod) * two_sqrtc_inv % special_mod;

    return result;
}

// Wrapper: pads vectors to the next suitable power-of-two length
vector<int> fast_ntt_poly_mul(vector<int> &a, vector<int> &b) {
    int n1 = a.size(), n2 = b.size();
    int len = n1 + n2 - 1;

    // Find smallest multiple of (lim * 2^x) >= len
    int x = 1;
    while ((lim << x) < len) ++x;
    int y = ((len - 1) >> x) + 1;
    int n = (y << x);

    // Resize both vectors to padded length
    a.resize(n);
    b.resize(n);

    // Multiply
    vector<int> ans = fast_ntt_poly_mul(a, b, 0);

    // Resize back to actual result length
    a.resize(n1);
    b.resize(n2);
    ans.resize(len);

    return ans;
}

void solve() {
    ll n, m;
    cin >> n >> m;
    vector<int> X(n), Y(m);
    for (int i = 0; i < n; i++) cin >> X[i];
    for (int i = 0; i < m; i++) cin >> Y[i];

    vector<int> res = fast_ntt_poly_mul(X, Y);

    for (auto val : res)
        cout << val << ' ';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int tc = 1;
    // cin >> tc;
    while (tc--) solve();

    return 0;
}
