// String Hashing: double-mod polynomial hash of any substring, base and mod drawn at random each run.
// Use when: "are these two substrings equal", distinct substrings, comparing rotations, binary searching a length.
// Handles: strings or vectors, substring hash, concatenation, equality. Random params defeat anti-hash tests.
// Time: build O(n) | sub / at / equal / merge_hash O(1)
// Indexing: queries are ALWAYS 1-based; the Base parameter only says whether the input itself is 0- or 1-based
// Note: two hashes are comparable only at equal lengths. Params are shared by every Hash of the same type.

#include <bits/stdc++.h>
using namespace std;

template <typename T = long long, int Base = 0> struct Hash {
    int n;
    vector<T> pow1, pow2, h1, h2;
    static constexpr array<T, 6> powers = {307, 509, 1009, 2003, 3001, 4001};
    static constexpr array<T, 6> mods = {1000000007, 1000000009, 1000000021, 1000000033, 1000000087, 1000000093};
    static T p1, p2, m1, m2;

    Hash() { initialize_static(); }

    Hash(const string &s) {
        initialize_static();
        initialize(s.size());
        calculate_powers();
        calculate_hashes(s);
    }

    Hash(const vector<T> &vec) {
        initialize_static();
        initialize(vec.size());
        calculate_powers();
        calculate_hashes(vec);
    }

    static void initialize_static() {
        static once_flag flag;
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        call_once(flag, []() {
            uniform_int_distribution<int> dist(0, 5);
            p1 = powers[dist(rng)];
            p2 = powers[dist(rng)];
            m1 = mods[dist(rng)];
            m2 = mods[dist(rng)];
        });
    }

    void initialize(int size) {
        n = size;
        pow1.assign(n + 5, 0);
        pow2.assign(n + 5, 0);
        h1.assign(n + 5, 0);
        h2.assign(n + 5, 0);
    }

    void calculate_powers() {
        pow1[0] = pow2[0] = 1;
        for (int i = 1; i <= n; ++i) {
            pow1[i] = (pow1[i - 1] * p1) % m1;
            pow2[i] = (pow2[i - 1] * p2) % m2;
        }
    }

    void calculate_hashes(const string &s) {
        h1[0] = h2[0] = 0;
        for (int i = 1; i <= n; ++i) {
            h1[i] = (h1[i - 1] * p1 + s[i - !Base]) % m1;
            h2[i] = (h2[i - 1] * p2 + s[i - !Base]) % m2;
        }
    }

    void calculate_hashes(const vector<T> &vec) {
        h1[0] = h2[0] = 0;
        for (int i = 1; i <= n; ++i) {
            h1[i] = (h1[i - 1] * p1 + vec[i - !Base]) % m1;
            h2[i] = (h2[i - 1] * p2 + vec[i - !Base]) % m2;
        }
    }

    pair<T, T> sub(int l, int r) const { // hash of the substring [l, r]
        T F = (h1[r] - (h1[l - 1] * pow1[r - l + 1] % m1) + m1) % m1;
        T S = (h2[r] - (h2[l - 1] * pow2[r - l + 1] % m2) + m2) % m2;
        return {F, S};
    }

    pair<T, T> merge_hash(int l1, int r1, int l2, int r2) const { // hash of [l1,r1] followed by [l2,r2]
        auto a = sub(l1, r1), b = sub(l2, r2);
        T F = ((a.first * pow1[r2 - l2 + 1]) + b.first) % m1;
        T S = ((a.second * pow2[r2 - l2 + 1]) + b.second) % m2;
        return {F, S};
    }

    pair<T, T> at(int idx) const { return sub(idx, idx); } // hash of one position

    bool equal(int l1, int r1, int l2, int r2) const { // do the two ranges hold the same content
        if (r1 - l1 != r2 - l2) return false;
        return sub(l1, r1) == sub(l2, r2);
    }
};

template <typename T, int Base> constexpr array<T, 6> Hash<T, Base>::powers;
template <typename T, int Base> constexpr array<T, 6> Hash<T, Base>::mods;
template <typename T, int Base> T Hash<T, Base>::p1 = 0;
template <typename T, int Base> T Hash<T, Base>::p2 = 0;
template <typename T, int Base> T Hash<T, Base>::m1 = 0;
template <typename T, int Base> T Hash<T, Base>::m2 = 0;

// Standard problem: q queries asking whether s[l1..r1] and s[l2..r2] are the same substring
void solve() {
    string s;
    int q;
    cin >> s >> q;
    Hash<> h(s);
    while (q--) {
        int l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;
        cout << (h.equal(l1, r1, l2, r2) ? "YES" : "NO") << '\n';
    }
}
