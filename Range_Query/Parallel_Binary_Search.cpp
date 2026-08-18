// Parallel Binary Search: binary searches every query at once over one shared timeline of events.
// Use when: each query asks "after how many events does my condition first hold", and rebuilding per query is too slow.
// Handles: any number of queries, queries whose condition never holds (-1), and any structure you can reset and replay.
// Time: O(log E) rounds, each replaying every event once, so O((E + Q) log E) times the cost of one event
// Indexing: events are 1..events and queries are 0..queries-1; the answer is the event count, not an index
// Note: you supply reset/apply/test. apply is only ever called forwards from a reset, so the structure needs no undo.

#include <bits/stdc++.h>
using namespace std;

// For each query, the smallest t in [1, events] for which test(query) holds after applying
// events 1..t; -1 when it never holds.
//   reset()        put the structure back to empty
//   apply(t)       apply event t (called with t increasing, always after a reset)
//   test(query)    is this query satisfied by the events applied so far
template <typename Reset, typename Apply, typename Test>
vector<int> parallel_binary_search(int queries, int events, Reset reset, Apply apply, Test test) {
    vector<int> lo(queries, 1), hi(queries, events + 1); // hi == events + 1 means "never"
    vector<vector<int>> pending(events + 2);
    while (true) {
        bool active = false;
        for (int i = 0; i < queries; i++)
            if (lo[i] < hi[i]) {
                pending[(lo[i] + hi[i]) / 2].push_back(i);
                active = true;
            }
        if (!active) break;

        reset();
        for (int t = 1; t <= events; t++) {
            apply(t);
            for (int i : pending[t]) {
                if (test(i))
                    hi[i] = t;
                else
                    lo[i] = t + 1;
            }
            pending[t].clear();
        }
        for (int t = 0; t <= events + 1; t++) pending[t].clear(); // anything past the last event
    }
    vector<int> answer(queries);
    for (int i = 0; i < queries; i++) answer[i] = (hi[i] == events + 1) ? -1 : hi[i];
    return answer;
}

// A Fenwick tree, since range-add point-read is what these problems almost always need
struct Fenwick {
    int n;
    vector<long long> t;
    Fenwick(int n = 0) { init(n); }
    void init(int m) {
        n = m;
        t.assign(n + 2, 0);
    }
    void add(int i, long long v) {
        for (; i <= n; i += i & -i) t[i] += v;
    }
    void add_range(int l, int r, long long v) { // needs r >= l
        add(l, v);
        add(r + 1, -v);
    }
    long long point(int i) { // the value at i, when built by add_range
        long long s = 0;
        for (; i > 0; i -= i & -i) s += t[i];
        return s;
    }
};

// Standard problem: POI Meteors - m sectors on a ring owned by n states, q falls each adding to a
// range of sectors; for each state report the first fall after which it has collected enough.
void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> owned(n);
    for (int i = 0; i < m; i++) {
        int owner;
        cin >> owner;
        owned[owner - 1].push_back(i + 1);
    }
    vector<long long> want(n);
    for (long long &w : want) cin >> w;
    int q;
    cin >> q;
    vector<array<long long, 3>> fall(q + 1);
    for (int i = 1; i <= q; i++) cin >> fall[i][0] >> fall[i][1] >> fall[i][2];

    Fenwick bit(m);
    vector<int> ans = parallel_binary_search(
        n, q, [&]() { bit.init(m); },
        [&](int t) {
            long long l = fall[t][0], r = fall[t][1], v = fall[t][2];
            if (l <= r)
                bit.add_range((int)l, (int)r, v);
            else // the ring wraps around
                bit.add_range((int)l, m, v), bit.add_range(1, (int)r, v);
        },
        [&](int state) {
            long long total = 0;
            for (int sector : owned[state]) {
                total += bit.point(sector);
                if (total >= want[state]) return true; // stop early, the sums can be huge
            }
            return total >= want[state];
        });

    for (int i = 0; i < n; i++) {
        if (ans[i] == -1)
            cout << "NIE\n";
        else
            cout << ans[i] << '\n';
    }
}
