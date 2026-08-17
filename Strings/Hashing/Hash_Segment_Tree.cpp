// Hashing Segment Tree: double-mod polynomial hash of any substring, under single-character updates.
// Use when: "are [l1,r1] and [l2,r2] equal" on a string that keeps changing, palindrome checks with updates.
// Handles: point assign, hash of any range, range equality, hashing a vector of ints as well as a string.
// Time: init O(N) | build O(n) | update O(log n) | query O(log n)
// Indexing: 0-based, ranges inclusive
// Note: call init() once before building. Two hashes compare equal only at equal lengths - equal() checks that.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 2e5 + 5;
const ll B1 = 131, B2 = 137, M1 = 1e9 + 7, M2 = 1e9 + 9;
ll pw1[N], pw2[N];

void init() { // fills both power tables; call once before any tree is built
    pw1[0] = pw2[0] = 1;
    for (int i = 1; i < N; i++) pw1[i] = pw1[i - 1] * B1 % M1, pw2[i] = pw2[i - 1] * B2 % M2;
}

struct node {
    ll h1, h2;
    int len;
    node() : h1(0), h2(0), len(0) {}
    node(ll c) : h1(c), h2(c), len(1) {}
};

node merge(const node &a, const node &b) {
    node r;
    r.h1 = (a.h1 + b.h1 * pw1[a.len]) % M1;
    r.h2 = (a.h2 + b.h2 * pw2[a.len]) % M2;
    r.len = a.len + b.len;
    return r;
}

struct HashSegTree {
    vector<node> tree;
    int n;

    HashSegTree(int n) : tree(4 * n), n(n) {}

    void build(const vector<ll> &a, int id, int l, int r) {
        if (l == r) {
            tree[id] = node(a[l]);
            return;
        }
        int mid = (l + r) >> 1;
        build(a, id << 1, l, mid);
        build(a, id << 1 | 1, mid + 1, r);
        tree[id] = merge(tree[id << 1], tree[id << 1 | 1]);
    }

    void build(const vector<ll> &a) { build(a, 1, 0, n - 1); }

    void build(const string &s) { // maps 'a'..'z' onto 1..26
        vector<ll> a(s.size());
        for (int i = 0; i < (int)s.size(); i++) a[i] = s[i] - 'a' + 1;
        build(a);
    }

    void update(int pos, ll val, int id, int l, int r) {
        if (l == r) {
            tree[id] = node(val);
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid)
            update(pos, val, id << 1, l, mid);
        else
            update(pos, val, id << 1 | 1, mid + 1, r);
        tree[id] = merge(tree[id << 1], tree[id << 1 | 1]);
    }

    void update(int pos, ll val) { update(pos, val, 1, 0, n - 1); }        // a[pos] = val
    void update(int pos, char c) { update(pos, (ll)(c - 'a' + 1)); }       // s[pos] = c

    node query(int x, int y, int id, int l, int r) {
        if (x > r || y < l) return node();
        if (x <= l && r <= y) return tree[id];
        int mid = (l + r) >> 1;
        return merge(query(x, y, id << 1, l, mid), query(x, y, id << 1 | 1, mid + 1, r));
    }

    node query(int x, int y) { return query(x, y, 1, 0, n - 1); } // hash of a[x .. y]

    bool equal(int l1, int r1, int l2, int r2) { // do the two ranges hold the same content
        if (r1 - l1 != r2 - l2) return false;
        node a = query(l1, r1), b = query(l2, r2);
        return a.h1 == b.h1 && a.h2 == b.h2;
    }
};

// Standard problem: type 1 assigns s[pos], type 2 asks whether [l1,r1] and [l2,r2] are the same substring
void solve() {
    init();
    string s;
    int q;
    cin >> s >> q;
    HashSegTree st((int)s.size());
    st.build(s);
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int pos;
            char c;
            cin >> pos >> c;
            st.update(pos - 1, c);
        } else {
            int l1, r1, l2, r2;
            cin >> l1 >> r1 >> l2 >> r2;
            cout << (st.equal(l1 - 1, r1 - 1, l2 - 1, r2 - 1) ? "YES" : "NO") << '\n';
        }
    }
}
