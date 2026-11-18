// MO: answers offline range queries by moving a window [l, r] in sqrt-sized steps.
// Use when: many "[l, r]" queries, no updates, adding/removing one element is O(1) but ranges
// cannot be merged. The default statistic is sum over values x of cnt(x)^2 * x (CF 86D).
// Usage: MO mo(a, max_val); mo.add_query(l, r) per query; vector<ll> ans = mo.run();
// Handles: any window statistic with O(1) add/remove - edit add(), remove(), cur and cnt.
// Not: updates (use MO with time), tree paths (MO_On_Tree).
// Time: O((n + q) * sqrt(q)) calls to add/remove
// Indexing: 0-based, ranges inclusive; answers come back in the order queries were added
// Note: values index cnt directly, so they must be in [0, max_val] - compress first.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct MO {
    struct Query {
        int l, r, idx;
    };

    int n;
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

    vector<ll> run() {
        vector<ll> ans(qs.size());
        int block = max<int>(1, (int)(n / sqrt((double)qs.size() + 1)));
        sort(qs.begin(), qs.end(), [&](const Query &x, const Query &y) {
            if (x.l / block != y.l / block) return x.l / block < y.l / block;
            return (x.l / block) & 1 ? x.r > y.r : x.r < y.r;
        });
        int l = 0, r = -1;
        for (auto &q : qs) { // grow before shrinking, so the window is never "negative"
            while (r < q.r) add(++r);
            while (l > q.l) add(--l);
            while (r > q.r) remove(r--);
            while (l < q.l) remove(l++);
            ans[q.idx] = cur;
        }
        return ans;
    }
};
