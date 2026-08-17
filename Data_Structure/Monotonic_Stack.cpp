// Monotonic Stack: for every index, the nearest index on one side holding a smaller (or larger) value.
// Use when: largest rectangle in a histogram, "next greater element", counting subarrays where a[i] is the minimum.
// Handles: previous / next, smaller / greater, all four directions. Returns indices, with -1 or n when none exists.
// Time: O(n)
// Indexing: 0-based; -1 means nothing to the left, n means nothing to the right
// Note: these are strict - ties do not count. Change <= to < inside to make them non-strict.

#include <bits/stdc++.h>
using namespace std;

vector<int> prev_smaller(const vector<int> &a) { // nearest j < i with a[j] < a[i], else -1
    int n = (int)a.size();
    vector<int> res(n, -1);
    stack<int> s;
    for (int i = 0; i < n; i++) {
        while (!s.empty() && a[s.top()] >= a[i]) s.pop();
        res[i] = s.empty() ? -1 : s.top();
        s.push(i);
    }
    return res;
}

vector<int> next_smaller(const vector<int> &a) { // nearest j > i with a[j] < a[i], else n
    int n = (int)a.size();
    vector<int> res(n, n);
    stack<int> s;
    for (int i = n - 1; i >= 0; i--) {
        while (!s.empty() && a[s.top()] >= a[i]) s.pop();
        res[i] = s.empty() ? n : s.top();
        s.push(i);
    }
    return res;
}

vector<int> prev_greater(const vector<int> &a) { // nearest j < i with a[j] > a[i], else -1
    int n = (int)a.size();
    vector<int> res(n, -1);
    stack<int> s;
    for (int i = 0; i < n; i++) {
        while (!s.empty() && a[s.top()] <= a[i]) s.pop();
        res[i] = s.empty() ? -1 : s.top();
        s.push(i);
    }
    return res;
}

vector<int> next_greater(const vector<int> &a) { // nearest j > i with a[j] > a[i], else n
    int n = (int)a.size();
    vector<int> res(n, n);
    stack<int> s;
    for (int i = n - 1; i >= 0; i--) {
        while (!s.empty() && a[s.top()] <= a[i]) s.pop();
        res[i] = s.empty() ? n : s.top();
        s.push(i);
    }
    return res;
}

// Standard problem: largest rectangle in a histogram
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    vector<int> L = prev_smaller(a), R = next_smaller(a);
    long long best = 0;
    for (int i = 0; i < n; i++) best = max(best, (long long)a[i] * (R[i] - L[i] - 1));
    cout << best << '\n';
}
