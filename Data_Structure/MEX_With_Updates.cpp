// MEX with Updates: the mex of an array that keeps changing by point assignment.
// Use when: "set a[i] = v, then report the mex", repeated many times over the same array.
// Handles: point assignment, mex query, duplicates, values of any size. Frequencies are tracked, so repeats are safe.
// Time: build O(n log n) | update O(log n) | mex O(1)
// Indexing: 0-based
// Note: the mex can never exceed n, so the missing set is seeded with 0..n and larger values never matter.

#include <bits/stdc++.h>
using namespace std;

struct Mex {
    map<int, int> frequency;
    set<int> missing_numbers;
    vector<int> A;

    Mex(const vector<int> &A) : A(A) {
        for (int i = 0; i <= (int)A.size(); i++) missing_numbers.insert(i);
        for (int x : A) {
            ++frequency[x];
            missing_numbers.erase(x);
        }
    }

    int mex() { return *missing_numbers.begin(); } // smallest non-negative integer not present

    void update(int idx, int new_value) { // A[idx] = new_value
        int old = A[idx], n = (int)A.size();
        if (--frequency[old] == 0 && 0 <= old && old <= n) missing_numbers.insert(old);
        A[idx] = new_value;
        ++frequency[new_value];
        missing_numbers.erase(new_value);
    }
};

// Standard problem: q assignments, each followed by the mex of the whole array
void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    Mex mx(a);
    while (q--) {
        int idx, val;
        cin >> idx >> val;
        mx.update(idx, val);
        cout << mx.mex() << '\n';
    }
}
