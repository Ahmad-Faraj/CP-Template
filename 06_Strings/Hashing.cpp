// Hashing: double-mod polynomial hash of any substring of a string or vector, O(1) per query.
// Use when: "are these two substrings equal", distinct substrings, binary search on a length.
// Usage: Hash<> h(s); h.equal(l1, r1, l2, r2); h.sub(l, r); h.merge_hash(l1, r1, l2, r2); h.at(i);
// Handles: strings (any char) or vector<T> (any values, negatives too). Random base and mods.
// Time: build O(n) | sub / at / equal / merge_hash O(1)
// Indexing: queries are ALWAYS 1-based, l <= r. Base = 0: input is 0-based (s[0] is position 1);
//           Base = 1: input is 1-based (s[0] is a dummy and is skipped).
// Note: sub() / merge_hash() values are comparable only for equal lengths (equal() checks it).
//       p, m are static: every Hash<T, Base> of the same type shares them. T must be 64-bit.

#include <bits/stdc++.h>
using namespace std;

template <typename T = long long, int Base = 0> struct Hash {
    int n;
    vector<T> pow1, pow2, h1, h2;
    static constexpr array<T, 6> powers = {307, 509, 1009, 2003, 3001, 4001};
    static constexpr array<T, 6> mods = {1000000007, 1000000009, 1000000021,
                                         1000000033, 1000000087, 1000000093};
    static T p1, p2;
    static T m1, m2;

    Hash() { initialize_static(); }

    Hash(const string &s) {
        initialize_static();
        initialize(s.size() - Base);
        calculate_powers();
        calculate_hashes(s);
    }

    Hash(const vector<T> &vec) {
        initialize_static();
        initialize(vec.size() - Base);
        calculate_powers();
        calculate_hashes(vec);
    }

    static void initialize_static() {
        static once_flag flag;
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        call_once(flag, []() {
            uniform_int_distribution<int> dist(0, 5);
            int a = dist(rng), b = (a + 1 + dist(rng) % 5) % 6;
            p1 = powers[dist(rng)];
            p2 = powers[dist(rng)];
            m1 = mods[a];
            m2 = mods[b];
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
            T c = (unsigned char)s[i - !Base];
            h1[i] = (h1[i - 1] * p1 + c) % m1;
            h2[i] = (h2[i - 1] * p2 + c) % m2;
        }
    }

    void calculate_hashes(const vector<T> &vec) {
        h1[0] = h2[0] = 0;
        for (int i = 1; i <= n; ++i) {
            T v = vec[i - !Base];
            h1[i] = (h1[i - 1] * p1 + (v % m1 + m1) % m1) % m1;
            h2[i] = (h2[i - 1] * p2 + (v % m2 + m2) % m2) % m2;
        }
    }

    pair<T, T> sub(int l, int r) const {
        T F = (h1[r] - (h1[l - 1] * pow1[r - l + 1] % m1) + m1) % m1;
        T S = (h2[r] - (h2[l - 1] * pow2[r - l + 1] % m2) + m2) % m2;
        return {F, S};
    }

    pair<T, T> merge_hash(int l1, int r1, int l2, int r2) const { // hash of s[l1..r1] + s[l2..r2]
        auto a = sub(l1, r1), b = sub(l2, r2);
        T F = ((a.first * pow1[r2 - l2 + 1]) + b.first) % m1;
        T S = ((a.second * pow2[r2 - l2 + 1]) + b.second) % m2;
        return {F, S};
    }

    pair<T, T> at(int idx) const { return sub(idx, idx); }

    bool equal(int l1, int r1, int l2, int r2) const {
        return r1 - l1 == r2 - l2 && sub(l1, r1) == sub(l2, r2);
    }
};

template <typename T, int Base> constexpr array<T, 6> Hash<T, Base>::powers;
template <typename T, int Base> constexpr array<T, 6> Hash<T, Base>::mods;
template <typename T, int Base> T Hash<T, Base>::p1 = 0;
template <typename T, int Base> T Hash<T, Base>::p2 = 0;
template <typename T, int Base> T Hash<T, Base>::m1 = 0;
template <typename T, int Base> T Hash<T, Base>::m2 = 0;
