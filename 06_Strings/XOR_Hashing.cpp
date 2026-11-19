// XOR Hashing: a random 64-bit tag per distinct value, so a SET of values hashes to one XOR.
// Use when: "do prefixes a[0..x] and b[0..y] contain the same set of distinct values", set equality
//           where multiplicity and order do not matter.
// Usage: XorHashing h; auto pa = h.prefix(a), pb = h.prefix(b);  same set <=> pa[x] == pb[y]
// Handles: any integer values (negative, huge), duplicates (counted once), arrays of different
//          lengths.
// Time: prefix O(n log n) (map + set, about 0.6 s per 1e6 elements) | each comparison O(1)
// Indexing: 0-based; pref[i] covers a[0..i]
// Note: every compared array must use the SAME XorHashing object (they share the tag table).
//       Randomised: two different sets collide with probability about 2^-64 per comparison.

#include <bits/stdc++.h>
using namespace std;

struct XorHashing {
    map<long long, unsigned long long> tag;
    mt19937_64 gen{(unsigned long long)chrono::steady_clock::now().time_since_epoch().count()};

    unsigned long long tag_of(long long v) {
        auto it = tag.find(v);
        if (it != tag.end()) return it->second;
        return tag[v] = gen();
    }

    // pref[i] = XOR of the tags of the DISTINCT values in a[0..i]
    template <class T> vector<unsigned long long> prefix(const vector<T> &a) {
        set<long long> seen;
        vector<unsigned long long> pref(a.size(), 0);
        for (int i = 0; i < (int)a.size(); i++) {
            if (seen.insert(a[i]).second) pref[i] = tag_of(a[i]);
            if (i > 0) pref[i] ^= pref[i - 1];
        }
        return pref;
    }
};
