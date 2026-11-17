#include <bits/stdc++.h>

#define ll long long
#define int long long
#define ld long double

#define nl "\n"
#define sp " "
#define EPS (1e-12)

#define all(p) p.begin(), p.end()
#define fixed(n) fixed << setprecision(n)

using namespace std;

void Fast_IO() {
// ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}
#include <bits/stdc++.h>
using namespace std;

template <typename T = long long, int B = 62> struct Basis {
    T basis[B];
    int sz;

    Basis() {
        clear();
    }

    void clear() {
        memset(basis, 0, sizeof(basis));
        sz = 0;
    }

    bool insert(T x) {
        for (int i = B - 1; i >= 0; i--) {
            if ((x >> i) & 1) {
                if (!basis[i]) {
                    basis[i] = x;
                    sz++;
                    return true;
                }
                x ^= basis[i];
            }
        }
        return false;
    }

    bool can(T x) {
        for (int i = B - 1; i >= 0; i--) {
            if ((x >> i) & 1) {
                if (!basis[i]) return false;
                x ^= basis[i];
            }
        }
        return x == 0;
    }

    T max_xor(T x = 0) {
        for (int i = B - 1; i >= 0; i--) {
            x = max(x, x ^ basis[i]);
        }
        return x;
    }

    T min_xor(T x) {
        for (int i = B - 1; i >= 0; i--) {
            x = min(x, x ^ basis[i]);
        }
        return x;
    }

    // 1-based index, returns the k-th smallest XOR sum of unique subsets
    T kth(T k) {
        T cnt = ((T)1 << sz);
        if (k < 1 || k > cnt) return -1;

        T x = 0;
        for (int i = B - 1; i >= 0; i--) {
            if (basis[i]) {
                T half = cnt >> 1;
                if (k > half) {
                    if (!(x >> i & 1)) x ^= basis[i];
                    k -= half;
                } else {
                    if (x >> i & 1) x ^= basis[i];
                }
                cnt = half;
            }
        }
        return x;
    }

    // Returns the number of unique subset XORs strictly less than x
    T count_lt(T x) {
        if (x < 0) return 0;

        T ans = 0;
        T cnt = ((T)1 << sz);
        T mask = 0;

        for (int i = B - 1; i >= 0; i--) {
            if (basis[i]) {
                if ((x >> i) & 1) {
                    ans += (cnt >> 1);
                    if (!(mask >> i & 1)) mask ^= basis[i];
                } else {
                    if (mask >> i & 1) mask ^= basis[i];
                }
                cnt >>= 1;
            } else {
                if (((x >> i) & 1) != ((mask >> i) & 1)) {
                    if ((x >> i) & 1)
                        return ans + cnt;
                    else
                        return ans;
                }
            }
        }
        return ans;
    }
};

void Solve() {
    int n;
    cin >> n;
}

// static bool start = []() {preCalc(); return true; }();
signed main() {
    ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    Fast_IO();

    int tc = 1;
    cin >> tc;

    while (tc--) {
        Solve();
    }

    return 0;
}
