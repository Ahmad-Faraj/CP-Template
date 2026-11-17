// NTT Large: polynomial product mod 998244353 for ANY length, even past NTT's 2^23 limit.
// Use when: product length > 2^23 (~8.4e6) under 998244353, where plain NTT has no root that big.
// Usage: vector<int> c = NTT_Large::fast_ntt_poly_mul(a, b);  // a, b are restored on return
// Handles: any lengths >= 1, lengths that are not powers of two (pads to y * 2^x with y <= 64).
// Time: O(n log n + n * lim), lim = 64 (naive base case on blocks of <= 64)
// Indexing: 0-based; c[k] = sum a[i] * b[j] over i + j == k (mod 998244353), |c| = |a| + |b| - 1
// Note: inputs in [0, 998244353), non-empty, |a| + |b| <= ~5e8. Computes a * b mod (X^n - g^x) by
//       X^n - c = (X^(n/2) - sqrt c)(X^(n/2) + sqrt c); recursion depth ~log2(n / 64) only.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct NTT_Large {
    static constexpr int special_mod = 998244353; // 119 * 2^23 + 1
    static constexpr int primitive_root = 3;
    static constexpr int lim = 64; // naive multiplication at or below this size

    static ll powrmod(ll b, ll n, ll m) {
        ll result = 1;
        while (n > 0) {
            if (n & 1) result = result * b % m;
            b = b * b % m;
            n >>= 1;
        }
        return result;
    }

    static int mod_div(int x) { // x / 2 mod special_mod, x in [0, 2 * special_mod)
        if (x >= special_mod) x -= special_mod;
        if (x & 1) x += special_mod;
        return x / 2;
    }

    // a mod (X^(n/2) - g^x) and a mod (X^(n/2) + g^x)
    static pair<vector<int>, vector<int>> mod_poly(vector<int> &a, int x) {
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

    // a * b mod (X^n - g^x), with n = a.size() = b.size()
    static vector<int> fast_ntt_poly_mul(vector<int> &a, vector<int> &b, int x) {
        int n = a.size();
        if (n <= lim || (n & 1)) { // naive O(n^2), reduced every 16 rows so u64 never overflows
            vector<unsigned long long> temp(2 * n);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) temp[i + j] += (ll)a[i] * b[j];
                if ((i & 15) == 15)
                    for (int j = i + 1; j < i + n; j++) temp[j] %= special_mod;
            }
            vector<int> result(n);
            ll c = powrmod(primitive_root, x, special_mod);
            for (int i = 0; i < n; i++)
                result[i] = (temp[i] % special_mod + c * (temp[i + n] % special_mod)) % special_mod;
            return result;
        }
        int x1 = x / 2;
        int x2 = (x1 + special_mod / 2) % (special_mod - 1); // g^x2 = -g^x1
        vector<int> ans_minus, ans_plus;
        {
            auto [a_minus, a_plus] = mod_poly(a, x1);
            auto [b_minus, b_plus] = mod_poly(b, x1);
            ans_minus = fast_ntt_poly_mul(a_minus, b_minus, x1);
            ans_plus = fast_ntt_poly_mul(a_plus, b_plus, x2);
        }
        int sqrtc_inv = powrmod(primitive_root, special_mod - 1 - x1, special_mod);
        int two_sqrtc_inv = mod_div(sqrtc_inv);
        vector<int> result(n);
        for (int i = 0; i < n / 2; i++) result[i] = mod_div(ans_minus[i] + ans_plus[i]);
        for (int i = 0; i < n / 2; i++)
            result[i + n / 2] =
                (ll)(ans_minus[i] - ans_plus[i] + special_mod) * two_sqrtc_inv % special_mod;
        return result;
    }

    static vector<int> fast_ntt_poly_mul(vector<int> &a, vector<int> &b) {
        int n1 = a.size(), n2 = b.size();
        int len = n1 + n2 - 1;
        int x = 1; // pad to n = y * 2^x with y <= lim
        while ((lim << x) < len) ++x;
        int y = ((len - 1) >> x) + 1;
        int n = (y << x);
        a.resize(n), b.resize(n);
        vector<int> ans = fast_ntt_poly_mul(a, b, 0);
        a.resize(n1), b.resize(n2);
        ans.resize(len);
        return ans;
    }
};
