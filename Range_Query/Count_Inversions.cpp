// Count Inversions: the number of pairs i < j with a[i] > a[j], by merge sort.
// Use when: "how many pairs are out of order", "minimum adjacent swaps to sort", measuring how unsorted an array is.
// Handles: duplicates, negatives, any comparable type, a custom comparator. Whole-array only - not a range query.
// Time: O(n log n)
// Indexing: 0-based, the whole vector
// Note: the count reaches n*(n-1)/2, so it is long long. count_inversions_inplace also sorts the vector it is given.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T, typename Cmp = less<T>>
ll count_inversions_inplace(vector<T> &a, Cmp cmp = Cmp()) { // counts, and leaves a sorted
    int n = (int)a.size();
    if (n < 2) return 0;
    vector<T> buf(n);
    ll total = 0;
    for (int width = 1; width < n; width *= 2) {
        for (int lo = 0; lo < n - width; lo += 2 * width) {
            int mid = lo + width, hi = min(lo + 2 * width, n);
            int i = lo, j = mid, k = lo;
            while (i < mid && j < hi) {
                if (cmp(a[j], a[i])) {
                    total += mid - i; // every remaining left element beats a[j]
                    buf[k++] = a[j++];
                } else
                    buf[k++] = a[i++];
            }
            while (i < mid) buf[k++] = a[i++];
            while (j < hi) buf[k++] = a[j++];
            for (int t = lo; t < hi; t++) a[t] = buf[t];
        }
    }
    return total;
}

template <typename T, typename Cmp = less<T>>
ll count_inversions(vector<T> a, Cmp cmp = Cmp()) { // counts, leaving the caller's vector alone
    return count_inversions_inplace(a, cmp);
}

// Standard problem: SPOJ INVCNT - report the number of inversions of each array
void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int &x : a) cin >> x;
        cout << count_inversions(a) << '\n';
    }
}
