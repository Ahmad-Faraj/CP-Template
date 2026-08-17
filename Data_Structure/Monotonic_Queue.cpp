// Monotonic Queue: the maximum of a queue, under push at the back and pop from the front, O(1) amortized.
// Use when: sliding-window maximum, "max of the last k elements" as the window advances.
// Handles: push, pop the oldest element, read the max. Grows on demand; flip the comparison for a minimum.
// Time: push O(1) amortized | pop O(1) | top O(1)
// Indexing: none
// Note: pop() removes the oldest pushed element, not the maximum. top() on an empty queue is undefined.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct MonotonicQueue {
    deque<pair<ll, int>> d; // {value, how many smaller values it absorbed}

    void push(ll val) { // append val to the back of the queue
        int cnt = 0;
        while (!d.empty() && d.back().first <= val) cnt += d.back().second + 1, d.pop_back();
        d.push_back({val, cnt});
    }

    void pop() { // drop the oldest element still in the queue
        if (d.empty()) return;
        if (d.front().second > 0)
            d.front().second--;
        else
            d.pop_front();
    }

    ll top() { return d.front().first; } // maximum of everything currently queued

    bool empty() { return d.empty(); }
};

// Standard problem: report the maximum of every window of k consecutive elements
void solve() {
    int n, k;
    cin >> n >> k;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    MonotonicQueue q;
    for (int i = 0; i < n; i++) {
        q.push(a[i]);
        if (i >= k) q.pop();
        if (i >= k - 1) cout << q.top() << ' ';
    }
    cout << '\n';
}
