// XOR Basis Range: an XOR basis that also remembers how recent each vector is, so a suffix can be queried.
// Use when: "largest XOR of a subset of a[l..r]" with r fixed at the end, or many queries sharing a right endpoint.
// Handles: duplicates and zeros, any suffix start l, and rank or reachability restricted to that suffix.
// Time: add O(BITS) | every suffix query O(BITS)
// Indexing: 0-based positions, added in increasing order; queries take the suffix start l
// Note: this answers SUFFIX queries [l, current end). For arbitrary [l, r] add elements up to r, then query with l.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll, int BITS = 62> struct XOR_Basis_Range {
    array<T, BITS> basis{};
    array<int, BITS> newest{}; // newest[i] = the largest position that contributed this basis vector
    ll count = 0;

    XOR_Basis_Range() {
        basis.fill(0);
        newest.fill(-1);
    }

    // add a[pos]; positions must arrive in increasing order
    void add(T x, int pos) {
        count++;
        for (int i = BITS - 1; i >= 0; i--) {
            if (!((x >> i) & 1)) continue;
            if (newest[i] < pos) { // keep the fresher vector here and push the older one down
                swap(basis[i], x);
                swap(newest[i], pos);
            }
            if (!basis[i]) { // the slot was empty, so x settled here
                basis[i] = x;
                newest[i] = pos;
                return;
            }
            x ^= basis[i];
            if (!x) return;
        }
    }

    int rank_from(int l) const { // how many basis vectors are usable for the suffix starting at l
        int total = 0;
        for (int i = 0; i < BITS; i++)
            if (basis[i] && newest[i] >= l) total++;
        return total;
    }

    T max_xor_from(int l) const {
        T res = 0;
        for (int i = BITS - 1; i >= 0; i--)
            if (basis[i] && newest[i] >= l && (res ^ basis[i]) > res) res ^= basis[i];
        return res;
    }

    bool can_make_from(T x, int l) const {
        for (int i = BITS - 1; i >= 0; i--)
            if (((x >> i) & 1) && basis[i] && newest[i] >= l) x ^= basis[i];
        return x == 0;
    }

    ll distinct_count_from(int l) const { return 1LL << rank_from(l); }
};

// Standard problem: n values then q queries of l - report the largest XOR of a subset of a[l..n-1]
void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (ll &x : a) cin >> x;
    XOR_Basis_Range<ll> b;
    for (int i = 0; i < n; i++) b.add(a[i], i);
    while (q--) {
        int l;
        cin >> l;
        cout << b.max_xor_from(l) << '\n';
    }
}
