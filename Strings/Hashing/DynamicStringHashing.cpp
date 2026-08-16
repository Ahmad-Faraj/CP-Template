/* Topic: Dynamic String Hashing
 * Description: Dynamic String Hashing maintains the polynomial rolling hash of a string
 *   dynamically using a Segment Tree or Fenwick Tree. Allows for O(log N) string
 *   substring equivalence checks after character modifications.
 * Usage: DynamicStringHashing hs(s); hs.update(idx, c); hs.query(l, r);
 */
#include "../../core.h"

// Time Complexity: O(N) build, O(log N) update/query
// Space Complexity: O(N)
struct DynamicStringHashing {
    static const int M1 = 1e9 + 7, M2 = 1e9 + 9, B1 = 313, B2 = 317;

    struct Node {
        int h1, h2, len;
    };

    int n;
    vector<Node> tree;
    vector<int> p1, p2;

    Node merge(Node a, Node b) {
        if (!a.len) return b;
        if (!b.len) return a;
        return {(a.h1 * p1[b.len] + b.h1) % M1, (a.h2 * p2[b.len] + b.h2) % M2, a.len + b.len};
    }

    void build(int node, int l, int r, const string& s) {
        if (l == r) {
            tree[node] = {s[l], s[l], 1};
            return;
        }
        int mid = l + (r - l) / 2;
        build(2 * node, l, mid, s);
        build(2 * node + 1, mid + 1, r, s);
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    DynamicStringHashing(string s) : n(sz(s)), tree(4 * n), p1(n + 1, 1), p2(n + 1, 1) {
        for (int i = 1; i <= n; i++) p1[i] = (p1[i - 1] * B1) % M1, p2[i] = (p2[i - 1] * B2) % M2;
        if (n) build(1, 0, n - 1, s);
    }

    void update(int node, int l, int r, int idx, char c) {
        if (l == r) {
            tree[node] = {c, c, 1};
            return;
        }
        int mid = l + (r - l) / 2;
        idx <= mid ? update(2 * node, l, mid, idx, c) : update(2 * node + 1, mid + 1, r, idx, c);
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    void update(int idx, char c) { update(1, 0, n - 1, idx, c); }

    Node query(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return {0, 0, 0};
        if (ql <= l && r <= qr) return tree[node];
        int mid = l + (r - l) / 2;
        return merge(query(2 * node, l, mid, ql, qr), query(2 * node + 1, mid + 1, r, ql, qr));
    }

    pair<int, int> query(int l, int r) {  // 0-indexed, inclusive
        Node res = query(1, 0, n - 1, l, r);
        return {res.h1, res.h2};
    }
};
