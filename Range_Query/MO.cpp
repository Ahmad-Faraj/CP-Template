// MO's Algorithm: answers many offline range queries by sliding a window in sqrt-sized steps.
// Use when: "q queries of [l, r]" with no updates, where add/remove of one element is O(1) but ranges cannot be merged.
// Handles: any window statistic with O(1) add/remove - distinct count, frequency sums, mode. Offline only, no updates.
// Time: O((n + q) * sqrt(q)) calls to add/remove
// Indexing: 0-based, ranges inclusive
// Note: edit add(), remove(), cur and cnt per problem. run() returns answers in the order queries were added.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct MO {
    struct Query {
        int l, r, idx;
    };

    int n, block = 1;
    vector<ll> a, cnt;
    vector<Query> qs;
    ll cur = 0;

    MO(const vector<ll> &v, int max_val) : n((int)v.size()), a(v), cnt(max_val + 1, 0) {}

    void add(int i) { // bring a[i] into the window
        cur -= cnt[a[i]] * cnt[a[i]] * a[i];
        cnt[a[i]]++;
        cur += cnt[a[i]] * cnt[a[i]] * a[i];
    }

    void remove(int i) { // drop a[i] from the window
        cur -= cnt[a[i]] * cnt[a[i]] * a[i];
        cnt[a[i]]--;
        cur += cnt[a[i]] * cnt[a[i]] * a[i];
    }

    void add_query(int l, int r) { qs.push_back({l, r, (int)qs.size()}); }

    vector<ll> run() { // answers indexed by the order add_query() was called
        vector<ll> ans(qs.size());
        block = max(1, (int)(n / sqrt((double)qs.size() + 1)));
        sort(qs.begin(), qs.end(), [&](const Query &x, const Query &y) {
            if (x.l / block != y.l / block) return x.l / block < y.l / block;
            return (x.l / block) & 1 ? x.r > y.r : x.r < y.r;
        });
        int l = 0, r = -1;
        for (auto &q : qs) {
            while (r < q.r) add(++r);
            while (l > q.l) add(--l);
            while (r > q.r) remove(r--);
            while (l < q.l) remove(l++);
            ans[q.idx] = cur;
        }
        return ans;
    }
};

// Standard problem: for each query report the sum of cnt(x)^2 * x over [l, r] (CF 86D - Powerful Array)
void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    ll mx = 0;
    for (auto &x : a) cin >> x, mx = max(mx, x);
    MO mo(a, (int)mx);
    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        mo.add_query(l - 1, r - 1);
    }
    for (ll v : mo.run()) cout << v << '\n';
}
