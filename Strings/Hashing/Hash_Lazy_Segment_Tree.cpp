// Hash Lazy Segment Tree: double-mod hash of any substring, under range-assign updates.
// Use when: "set s[l..r] all to one character" alongside "are these two ranges equal" or palindrome checks.
// Handles: range assign of one character, range hash, double mod. Point assign is update(i, i, c).
// Time: precompute O(N) once | build O(n) | update O(log n) | query O(log n)
// Indexing: 1-based, ranges inclusive; set Base = 1 if the string you hand it is itself 1-based
// Note: call precompute() once before constructing. Characters map to 1..26 and -1 is reserved for "no lazy".

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 1e6 + 5;
const ll mod1 = 1e9 + 7, mod2 = 1e9 + 9, p1 = 31, p2 = 37;
const ll p_1inv = 233333335, p_2inv = 972222231; // (p1 - 1)^-1 mod mod1, (p2 - 1)^-1 mod mod2
ll p_pow[2][N];

ll mul(ll a, ll b, ll m) { return (a * b) % m; }
ll add(ll a, ll b, ll m) { return (a + b) % m; }

void precompute() { // fills both power tables; call once before building any tree
    p_pow[0][0] = p_pow[1][0] = 1;
    for (int i = 1; i < N; i++)
        p_pow[0][i] = mul(p_pow[0][i - 1], p1, mod1), p_pow[1][i] = mul(p_pow[1][i - 1], p2, mod2);
}

template <typename T = ll, bool Base = 0> struct Hash_LazySegmentTree {
    string s;
    int n, size;
    vector<T> lazy;
    vector<pair<T, T>> tree;

#define LEFT (idx << 1)
#define RIGHT ((idx << 1) | 1)

    struct Item {
        T len;
        pair<T, T> ans;
        Item() : len(0), ans({0, 0}) {}
        Item(pair<T, T> ans, T len) : len(len), ans(ans) {}
    };

    explicit Hash_LazySegmentTree(int N_, const string &s) : s(s), n(N_) {
        size = 1;
        while (size < n) size *= 2;
        lazy = vector<T>(2 * size, -1);
        tree = vector<pair<T, T>>(2 * size, {0, 0});
        build(1, 1, n);
    }

    pair<T, T> merge(const pair<T, T> &a, const pair<T, T> &b, T len) { // len = length of a
        return {add(a.first, mul(b.first, p_pow[0][len], mod1), mod1),
                add(a.second, mul(b.second, p_pow[1][len], mod2), mod2)};
    }

    pair<T, T> lazy_process(T val, T len) { // hash of val repeated len times
        T h1 = mul(val, (p_pow[0][len] - 1 + mod1) % mod1, mod1);
        T h2 = mul(val, (p_pow[1][len] - 1 + mod2) % mod2, mod2);
        return {mul(h1, p_1inv, mod1), mul(h2, p_2inv, mod2)};
    }

    void propagate(int idx, int l, int r) {
        if (lazy[idx] == -1) return;
        tree[idx] = lazy_process(lazy[idx], r - l + 1);
        if (l != r) lazy[LEFT] = lazy[RIGHT] = lazy[idx];
        lazy[idx] = -1;
    }

    void build(int idx, int l, int r) {
        if (l == r) return void(tree[idx] = {s[l - !Base] - 'a' + 1, s[l - !Base] - 'a' + 1});
        int m = (l + r) / 2;
        build(LEFT, l, m);
        build(RIGHT, m + 1, r);
        tree[idx] = merge(tree[LEFT], tree[RIGHT], m - l + 1);
    }

    void update(int idx, int l, int r, int L, int R, T val) {
        propagate(idx, l, r);
        if (l > R or r < L) return;
        if (l >= L and r <= R) {
            lazy[idx] = val;
            propagate(idx, l, r);
            return;
        }
        int m = (l + r) / 2;
        update(LEFT, l, m, L, R, val);
        update(RIGHT, m + 1, r, L, R, val);
        tree[idx] = merge(tree[LEFT], tree[RIGHT], m - l + 1);
    }

    Item query(int idx, int l, int r, int L, int R) {
        propagate(idx, l, r);
        if (l > R or r < L) return Item();
        if (l >= L and r <= R) return Item(tree[idx], r - l + 1);
        int m = (l + r) / 2;
        Item left = query(LEFT, l, m, L, R);
        Item right = query(RIGHT, m + 1, r, L, R);
        return Item(merge(left.ans, right.ans, left.len), left.len + right.len);
    }

    void update(int l, int r, T val) { update(1, 1, n, l, r, val); } // s[l .. r] = val, val in 1..26

    pair<T, T> query(int l, int r) { return query(1, 1, n, l, r).ans; } // hash of s[l .. r]

    bool equal(int l1, int r1, int l2, int r2) { // do the two ranges hold the same content
        if (r1 - l1 != r2 - l2) return false;
        return query(l1, r1) == query(l2, r2);
    }

#undef LEFT
#undef RIGHT
};

// Standard problem: type 1 sets s[l..r] to one letter, type 2 asks whether [l1,r1] and [l2,r2] match
void solve() {
    precompute();
    string s;
    int q;
    cin >> s >> q;
    Hash_LazySegmentTree<> st((int)s.size(), s);
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int l, r;
            char c;
            cin >> l >> r >> c;
            st.update(l, r, c - 'a' + 1);
        } else {
            int l1, r1, l2, r2;
            cin >> l1 >> r1 >> l2 >> r2;
            cout << (st.equal(l1, r1, l2, r2) ? "YES" : "NO") << '\n';
        }
    }
}
