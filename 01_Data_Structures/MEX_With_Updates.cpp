// MEX With Updates: the mex of an array under point assignment a[idx] = value.
// Use when: "set a[i] = x, then print the mex of the whole array", many times.
// Usage: Mex m(a); m.update(idx, val); int x = m.mex();
// Handles: duplicates, negatives, values of any size.
// Time: build O(n log n) | update O(log n) | mex O(1)
// Indexing: 0-based positions

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

    int mex() { return *missing_numbers.begin(); } // smallest non-negative value not present

    void update(int idx, int new_value) { // A[idx] = new_value
        int old = A[idx];
        if (--frequency[old] == 0 && 0 <= old && old <= (int)A.size()) missing_numbers.insert(old);
        A[idx] = new_value;
        ++frequency[new_value];
        missing_numbers.erase(new_value);
    }
};
