#include <bits/stdc++.h>
using namespace std;

#define int long long

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

const int Mod1 = 1e9 + 7;
const int Mod2 = 1e9 + 9;

int base1, base2;
bool hash_initialized = false;

void init_hash() {
    if (hash_initialized) return;

    base1 = uniform_int_distribution<int>(300, 10000)(rng);
    base2 = uniform_int_distribution<int>(300, 10000)(rng);

    while (base1 == base2) {
        base2 = uniform_int_distribution<int>(300, 10000)(rng);
    }

    hash_initialized = true;
}

struct SegTree {
    vector<pair<int, int>> tree, lazy, pw, prefix;
    vector<int> len;
    int sz;

    SegTree(int n) {
        init_hash();

        tree.assign(4 * n, {0, 0});
        lazy.assign(4 * n, {-1, -1});
        pw.assign(4 * n, {0, 0});
        prefix.assign(4 * n, {0, 0});
        len.assign(4 * n, 0);
        sz = n;

        pw[0] = {1, 1};
        prefix[0] = {1, 1};

        for (int i = 1; i < 4 * n; i++) {
            pw[i].first = (pw[i - 1].first * base1) % Mod1;
            pw[i].second = (pw[i - 1].second * base2) % Mod2;

            prefix[i].first = (pw[i].first + prefix[i - 1].first) % Mod1;
            prefix[i].second = (pw[i].second + prefix[i - 1].second) % Mod2;
        }
    }

    void propagate(int x, int lx, int rx) {
        if (lazy[x].first == -1) return;
        if (lx != rx) {
            lazy[2 * x] = lazy[2 * x + 1] = lazy[x];
        }
        tree[x].first = (lazy[x].first * prefix[rx - lx].first) % Mod1;
        tree[x].second = (lazy[x].second * prefix[rx - lx].second) % Mod2;
        lazy[x] = {-1, -1};
    }

    void merge(int x, int lx, int rx) {
        tree[x].first = (tree[2 * x].first * pw[len[2 * x + 1]].first % Mod1 + tree[2 * x + 1].first) % Mod1;
        tree[x].second = (tree[2 * x].second * pw[len[2 * x + 1]].second % Mod2 + tree[2 * x + 1].second) % Mod2;
        len[x] = len[2 * x] + len[2 * x + 1];
    }

    void build(string &s, int lx, int rx, int x) {
        lazy[x] = {-1, -1};
        if (lx == rx) {
            tree[x].first = s[lx - 1] % Mod1;
            tree[x].second = s[lx - 1] % Mod2;
            len[x] = 1;
            return;
        }

        int mid = (lx + rx) / 2;
        build(s, lx, mid, 2 * x);
        build(s, mid + 1, rx, 2 * x + 1);
        merge(x, lx, rx);
    }

    void build(string &s) { build(s, 1, sz, 1); }

    void update(int l, int r, char c, int lx, int rx, int x) {
        propagate(x, lx, rx);
        if (lx > r || rx < l) return;
        if (lx >= l && rx <= r) {
            lazy[x] = {c % Mod1, c % Mod2};
            propagate(x, lx, rx);
            return;
        }
        int mid = (lx + rx) / 2;
        update(l, r, c, lx, mid, 2 * x);
        update(l, r, c, mid + 1, rx, 2 * x + 1);
        merge(x, lx, rx);
    }

    void update(int l, int r, char c) {
        if (l > r) swap(l, r);
        update(l, r, c, 1, sz, 1);
    }

    pair<pair<int, int>, int> query(int l, int r, int lx, int rx, int x) {
        propagate(x, lx, rx);
        if (lx > r || rx < l) return {{0, 0}, 0};
        if (lx >= l && rx <= r) return {tree[x], len[x]};

        int mid = (lx + rx) / 2;
        auto left = query(l, r, lx, mid, 2 * x);
        auto right = query(l, r, mid + 1, rx, 2 * x + 1);

        if (left.second == 0) return right;
        if (right.second == 0) return left;

        pair<int, int> MergedHash;
        MergedHash.first = (left.first.first * pw[right.second].first % Mod1 + right.first.first) % Mod1;
        MergedHash.second = (left.first.second * pw[right.second].second % Mod2 + right.first.second) % Mod2;

        return {MergedHash, left.second + right.second};
    }

    pair<int, int> query(int l, int r) {
        if (l > r) swap(l, r);
        return query(l, r, 1, sz, 1).first;
    }
};