// Implicit Treap: an array you can splice - insert or erase anywhere, reverse or rotate a range, all in O(log n).
// Use when: an array needs insert/erase in the middle, range reverse, or cyclic shift alongside range queries.
// Handles: insert at a position, erase a range, range sum/min/max, range add, range assign, reverse, cyclic shift.
// Time: every operation O(log n) expected
// Indexing: 0-based, ranges inclusive
// Note: get_pos(value) needs distinct values - with duplicates it reports the most recently inserted one.

#include <bits/stdc++.h>
using namespace std;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

struct treap {
    struct node {
        int val, sz, prior, lazy, sum, mx, mn, repl;
        bool repl_flag, rev;
        node *l, *r, *par;
        node() : val(0), sz(0), prior(0), lazy(0), sum(0), mx(0), mn(0), repl(0), repl_flag(0), rev(0),
                 l(NULL), r(NULL), par(NULL) {}
        node(int _val) : val(_val), sz(1), prior(rnd()), lazy(0), sum(_val), mx(_val), mn(_val), repl(0),
                         repl_flag(0), rev(0), l(NULL), r(NULL), par(NULL) {}
    };
    typedef node *pnode;

    pnode root;
    map<int, pnode> position; // value -> node, for get_pos

    void clear() {
        root = NULL;
        position.clear();
    }

    treap() { clear(); }

    int size(pnode t) { return t ? t->sz : 0; }
    void update_size(pnode &t) { if (t) t->sz = size(t->l) + size(t->r) + 1; }

    void update_parent(pnode &t) {
        if (!t) return;
        if (t->l) t->l->par = t;
        if (t->r) t->r->par = t;
    }

    void lazy_sum_upd(pnode &t) {
        if (!t or !t->lazy) return;
        t->sum += t->lazy * size(t);
        t->val += t->lazy;
        t->mx += t->lazy;
        t->mn += t->lazy;
        if (t->l) t->l->lazy += t->lazy;
        if (t->r) t->r->lazy += t->lazy;
        t->lazy = 0;
    }

    void lazy_repl_upd(pnode &t) {
        if (!t or !t->repl_flag) return;
        t->val = t->mx = t->mn = t->repl;
        t->sum = t->val * size(t);
        if (t->l) t->l->repl = t->repl, t->l->repl_flag = true;
        if (t->r) t->r->repl = t->repl, t->r->repl_flag = true;
        t->repl_flag = false;
        t->repl = 0;
    }

    void lazy_rev_upd(pnode &t) {
        if (!t or !t->rev) return;
        t->rev = false;
        swap(t->l, t->r);
        if (t->l) t->l->rev ^= true;
        if (t->r) t->r->rev ^= true;
    }

    void reset(pnode &t) {
        if (!t) return;
        t->sum = t->val, t->mx = t->val, t->mn = t->val;
    }

    void combine(pnode &t, pnode l, pnode r) {
        if (!l) return void(t = r);
        if (!r) return void(t = l);
        t->sum = l->sum + r->sum; // t may alias l or r, so order matters here
        t->mx = max(l->mx, r->mx);
        t->mn = min(l->mn, r->mn);
    }

    void operation(pnode &t) {
        if (!t) return;
        reset(t);
        lazy_rev_upd(t->l), lazy_rev_upd(t->r);
        lazy_repl_upd(t->l), lazy_repl_upd(t->r);
        lazy_sum_upd(t->l), lazy_sum_upd(t->r);
        combine(t, t->l, t);
        combine(t, t, t->r);
    }

    void split(pnode t, pnode &l, pnode &r, int k, int add = 0) { // first k+1 elements go left
        if (t == NULL) return void(l = r = NULL);
        lazy_rev_upd(t), lazy_repl_upd(t), lazy_sum_upd(t);
        int idx = add + size(t->l);
        if (t->l) t->l->par = NULL;
        if (t->r) t->r->par = NULL;
        if (idx <= k)
            split(t->r, t->r, r, k, idx + 1), l = t;
        else
            split(t->l, l, t->l, k, add), r = t;
        update_parent(t);
        update_size(t);
        operation(t);
    }

    void merge(pnode &t, pnode l, pnode r) {
        lazy_rev_upd(l), lazy_rev_upd(r);
        lazy_repl_upd(l), lazy_repl_upd(r);
        lazy_sum_upd(l), lazy_sum_upd(r);
        if (!l) return void(t = r);
        if (!r) return void(t = l);
        if (l->prior > r->prior)
            merge(l->r, l->r, r), t = l;
        else
            merge(r->l, l, r->l), t = r;
        update_parent(t);
        update_size(t);
        operation(t);
    }

    void insert(int pos, int val) { // put val at index pos, shifting the rest right
        if (root == NULL) {
            root = new node(val);
            position[val] = root;
            return;
        }
        pnode l, r, mid = new node(val);
        position[val] = mid;
        split(root, l, r, pos - 1);
        merge(l, l, mid);
        merge(root, l, r);
    }

    void erase(int qL, int qR) { // drop indices [qL, qR], shifting the rest left
        pnode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        merge(root, l, r);
    }

    int query(int qL, int qR) { return range(qL, qR, 0); }     // sum over [qL, qR]
    int query_max(int qL, int qR) { return range(qL, qR, 1); } // max over [qL, qR]
    int query_min(int qL, int qR) { return range(qL, qR, 2); } // min over [qL, qR]

    int range(int qL, int qR, int what) {
        pnode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        int answer = what == 0 ? mid->sum : what == 1 ? mid->mx : mid->mn;
        merge(r, mid, r);
        merge(root, l, r);
        return answer;
    }

    void update(int qL, int qR, int val) { // add val to every element of [qL, qR]
        pnode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        lazy_repl_upd(mid);
        mid->lazy += val;
        merge(r, mid, r);
        merge(root, l, r);
    }

    void replace(int qL, int qR, int v) { // set every element of [qL, qR] to v
        pnode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        lazy_sum_upd(mid);
        mid->repl_flag = 1;
        mid->repl = v;
        merge(r, mid, r);
        merge(root, l, r);
    }

    void reverse(int qL, int qR) { // reverse the order of [qL, qR]
        pnode l, r, mid;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        mid->rev ^= 1;
        merge(r, mid, r);
        merge(root, l, r);
    }

    void cyclic_shift(int qL, int qR, int k, bool left_shift = 0) { // rotate [qL, qR] by k
        if (qL == qR) return;
        k %= (qR - qL + 1);
        pnode l, r, mid, fh, sh;
        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);
        if (left_shift == 0)
            split(mid, fh, sh, (qR - qL + 1) - k - 1);
        else
            split(mid, fh, sh, k - 1);
        merge(mid, sh, fh);
        merge(r, mid, r);
        merge(root, l, r);
    }

    bool exist;

    int get_pos(pnode curr, pnode son = nullptr) {
        if (exist == 0) return 0;
        if (curr == NULL) return exist = 0;
        if (!son) {
            if (curr == root) return size(curr->l);
            return size(curr->l) + get_pos(curr->par, curr);
        }
        if (curr == root) return son == curr->l ? 0 : size(curr->l) + 1;
        if (curr->l == son) return get_pos(curr->par, curr);
        return get_pos(curr->par, curr) + size(curr->l) + 1;
    }

    int get_pos(int value) { // index of value, or -1 if absent
        if (position.find(value) == position.end()) return -1;
        exist = 1;
        int x = get_pos(position[value]);
        return exist == 0 ? -1 : x;
    }

    int get_val(int pos) { return query(pos, pos); } // element at index pos
    int size() { return size(root); }
    bool find(int val) { return get_pos(val) != -1; }

    void inorder(pnode cur) {
        if (cur == NULL) return;
        operation(cur);
        inorder(cur->l);
        cout << cur->val << ' ';
        inorder(cur->r);
    }

    void print_array() { // the current contents, in order
        inorder(root);
        cout << '\n';
    }
};

// Standard problem: build an array, then apply range add / assign / reverse / rotate and report range sums
void solve() {
    int n, q;
    cin >> n >> q;
    treap t;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        t.insert(i, x);
    }
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 1) {
            int v;
            cin >> v;
            t.update(l, r, v);
        } else if (type == 2) {
            int v;
            cin >> v;
            t.replace(l, r, v);
        } else if (type == 3) {
            t.reverse(l, r);
        } else {
            cout << t.query(l, r) << '\n';
        }
    }
    t.print_array();
}
