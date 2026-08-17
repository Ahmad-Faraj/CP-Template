// Coordinate Compression: replaces values by their rank, so a sparse range of coordinates fits an array.
// Use when: indices reach 1e9 but there are only 1e5 distinct ones - segment trees, sweeps, offline range queries.
// Handles: duplicates, negatives, values added after the build, and mapping ranks back to the original values.
// Time: build O(n log n) | rank O(log n)
// Indexing: ranks are 0-based and dense, so a value's rank is a valid array index straight away
// Note: rank() of a value never added returns where it WOULD sit, which is rarely wanted - call contains first.

#include <bits/stdc++.h>
using namespace std;

template <typename T = int> struct Coordinate_Compression {
    vector<T> values;
    bool built = false;

    Coordinate_Compression() {}
    Coordinate_Compression(const vector<T> &v) : values(v) { build(); }

    void add(T x) {
        values.push_back(x);
        built = false;
    }

    void add(const vector<T> &v) {
        values.insert(values.end(), v.begin(), v.end());
        built = false;
    }

    void build() { // sort and drop duplicates; safe to call more than once
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());
        built = true;
    }

    int rank(T x) { // 0-based position of x among the distinct values
        if (!built) build();
        return (int)(lower_bound(values.begin(), values.end(), x) - values.begin());
    }

    bool contains(T x) { // whether x was ever added; check this before trusting rank()
        if (!built) build();
        auto it = lower_bound(values.begin(), values.end(), x);
        return it != values.end() && *it == x;
    }

    T value_at(int r) { // the original value with this rank, the inverse of rank()
        if (!built) build();
        return values[r];
    }

    vector<int> compress(const vector<T> &v) { // every element replaced by its rank
        if (!built) build();
        vector<int> out;
        out.reserve(v.size());
        for (const T &x : v) out.push_back(rank(x));
        return out;
    }

    int count_below(T x) { // how many distinct values are strictly less than x
        if (!built) build();
        return (int)(lower_bound(values.begin(), values.end(), x) - values.begin());
    }

    int count_at_most(T x) { // how many distinct values are <= x
        if (!built) build();
        return (int)(upper_bound(values.begin(), values.end(), x) - values.begin());
    }

    int size() {
        if (!built) build();
        return (int)values.size();
    }
};

// Standard problem: n values - print how many distinct there are, then each value's rank
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    Coordinate_Compression<int> cc(a);
    cout << cc.size() << '\n';
    vector<int> ranks = cc.compress(a);
    for (int i = 0; i < n; i++) cout << ranks[i] << " \n"[i + 1 == n];
}
