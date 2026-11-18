// Li Chao Persistent: Li Chao tree where every insert makes a new version; query min at x in any.
// Use when: lines live on a tree path ("max of x*V + y over the scrolls from here to the root"),
// "go back to state k", CHT on a rooted tree (Harbingers), insert into an old version (branching).
// Usage: Li_Chao_Persistent t(-1e6, 1e6); int v = t.add_line(t.latest(), {m, c}); t.query(v, x);
// Handles: any slope/query order, equal lines, negative m/c/x, inserting into any older version
// (versions form a tree). Version 0 is empty: query returns INF. Max: insert {-m, -c}, negate.
// Time: add_line O(log C) time and new nodes | query O(log C), C = R - L + 1
// Indexing: versions 0..count-1 (0 = empty), add_line returns the new id; integer x in [L, R]
// Note: each insert adds <= log2(C) + 2 nodes of 24-32 bytes (1e5 lines, C = 2e6: <= 2.3e6 nodes,
// ~75 MB max); t.reserve(...) skips vector regrowth. Keep |m * x + c| < INF = 2e18 on [L, R].
// Source: Shohag's code library "Persistent Li Chao Tree" (globals -> vectors, floor mid).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Li_Chao_Persistent {
    static constexpr ll INF = 2e18;
    struct Line {
        ll m, c;
        ll eval(ll x) const { return m * x + c; }
    };
    struct Node {
        Line line;
        int l = 0, r = 0; // child ids, 0 = none
    };

    ll L, R;
    vector<Node> t;    // t[0] = empty sentinel (line = INF everywhere)
    vector<int> roots; // roots[v] = root node of version v

    Li_Chao_Persistent(ll L = -1e9, ll R = 1e9) : L(L), R(R), t{Node{{0, INF}}}, roots{0} {}

    int latest() const { return (int)roots.size() - 1; }
    int count() const { return (int)roots.size(); }

    // copy node pre, keep the winner at the middle, push the loser into one child (path copying)
    int upd(int pre, Line nw, ll l, ll r) {
        ll m = l + (r - l) / 2; // floor, also for negative l
        int id = t.size();
        t.push_back(t[pre]);
        bool lef = nw.eval(l) < t[id].line.eval(l);
        bool mid = nw.eval(m) < t[id].line.eval(m);
        if (mid) swap(t[id].line, nw);
        if (l == r) return id;
        if (lef != mid) {
            int c = t[pre].l ? upd(t[pre].l, nw, l, m) : new_leaf(nw);
            t[id].l = c;
        } else {
            int c = t[pre].r ? upd(t[pre].r, nw, m + 1, r) : new_leaf(nw);
            t[id].r = c;
        }
        return id;
    }
    int new_leaf(Line ln) {
        t.push_back(Node{ln});
        return (int)t.size() - 1;
    }

    int add_line(int ver, Line ln) { // new version = version ver + ln; returns its id
        int root = upd(roots[ver], ln, L, R);
        roots.push_back(root);
        return latest();
    }

    ll query(int ver, ll x) const { // min over the lines of version ver at x; INF if none
        ll ans = INF, l = L, r = R;
        for (int cur = roots[ver]; cur;) {
            ans = min(ans, t[cur].line.eval(x));
            ll m = l + (r - l) / 2;
            if (x <= m) cur = t[cur].l, r = m;
            else cur = t[cur].r, l = m + 1;
        }
        return ans;
    }
};
