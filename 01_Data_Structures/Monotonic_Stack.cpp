// Monotonic Stack: for each index, the nearest index left / right with a smaller / greater value.
// Use when: largest rectangle in a histogram, "next greater element", count subarrays where a[i] is
//           the min / max (contribution technique), stock span.
// Usage: vector<int> L = prev_smaller(a), R = next_smaller(a);  a[i] = min of a[L[i]+1 .. R[i]-1]
// Handles: all four directions, duplicates, n = 0, any comparable T.
// Time: O(n) each
// Indexing: 0-based; -1 means nothing to the left, n means nothing to the right
// Note: strict - equal values do not count. For "smaller or equal" flip >= to > (and <= to <).
// Note: contribution counting with duplicates: use one strict side and one non-strict side.

#include <bits/stdc++.h>
using namespace std;

template <typename T> vector<int> prev_smaller(const vector<T> &a) { // nearest j < i, a[j] < a[i]
    int n = (int)a.size();
    vector<int> res(n), s;
    for (int i = 0; i < n; i++) {
        while (!s.empty() && a[s.back()] >= a[i]) s.pop_back();
        res[i] = s.empty() ? -1 : s.back();
        s.push_back(i);
    }
    return res;
}

template <typename T> vector<int> next_smaller(const vector<T> &a) { // nearest j > i, a[j] < a[i]
    int n = (int)a.size();
    vector<int> res(n), s;
    for (int i = n - 1; i >= 0; i--) {
        while (!s.empty() && a[s.back()] >= a[i]) s.pop_back();
        res[i] = s.empty() ? n : s.back();
        s.push_back(i);
    }
    return res;
}

template <typename T> vector<int> prev_greater(const vector<T> &a) { // nearest j < i, a[j] > a[i]
    int n = (int)a.size();
    vector<int> res(n), s;
    for (int i = 0; i < n; i++) {
        while (!s.empty() && a[s.back()] <= a[i]) s.pop_back();
        res[i] = s.empty() ? -1 : s.back();
        s.push_back(i);
    }
    return res;
}

template <typename T> vector<int> next_greater(const vector<T> &a) { // nearest j > i, a[j] > a[i]
    int n = (int)a.size();
    vector<int> res(n), s;
    for (int i = n - 1; i >= 0; i--) {
        while (!s.empty() && a[s.back()] <= a[i]) s.pop_back();
        res[i] = s.empty() ? n : s.back();
        s.push_back(i);
    }
    return res;
}
