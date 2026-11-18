// Monotonic Queue: a FIFO queue that also reports its maximum, all in O(1) amortized.
// Use when: sliding-window maximum, "max of the last k elements" as the window advances.
// Usage: Monotonic_Queue<int> q; q.push(x); if (window too big) q.pop(); q.top();
// Handles: duplicates, negatives. For a minimum, push -x and negate top() (or flip <= in push).
// Time: push O(1) amortized | pop O(1) | top O(1)
// Indexing: none
// Note: pop() removes the OLDEST element (not the max). top() on an empty queue is undefined.
// Note: memory grows with the number of pushes (popped slots at the front are not reused).

#include <bits/stdc++.h>
using namespace std;

template <typename T = int> struct Monotonic_Queue {
    // strictly decreasing values; b[i] = how many older, smaller elements a[i] absorbed
    vector<T> a;
    vector<int> b;
    int l = 0;

    void push(T val) {
        int cnt = 0;
        while ((int)a.size() > l && a.back() <= val) {
            cnt += b.back() + 1;
            a.pop_back(), b.pop_back();
        }
        a.push_back(val), b.push_back(cnt);
    }
    T top() { return a[l]; } // max of everything in the queue
    void pop() {             // drop the oldest element
        if (l >= (int)a.size()) return;
        if (b[l] > 0) {
            b[l]--;
            return;
        }
        l++;
    }
    bool empty() { return l >= (int)a.size(); }
};
