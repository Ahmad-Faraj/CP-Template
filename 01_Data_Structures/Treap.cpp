// Treap: a balanced BST keyed by value, with order statistics - k-th smallest and the rank of a
//        value.
// Use when: a sorted multiset that must also answer "k-th smallest" and "how many elements are
//           below x".
// Usage: Treap<int> t; t.insert(x); t.erase(x); t.kth(k); t.lower_bound(x); t.count(x); t.size();
// Handles: insert, erase, k-th, lower_bound, upper_bound, duplicates.
// Not: sequences by position (insert at i, reverse [l, r)) - use Treap_Implicit.cpp.
// Time: every operation O(log n) expected
// Indexing: kth() is 0-based; lower_bound returns a rank, not an iterator
// Note: upper_bound and count need an integer type.

#include <bits/stdc++.h>
using namespace std;

mt19937 eng(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution<int> rnd{INT_MIN + 1, INT_MAX};

template <typename DataType> struct Treap {
    enum DIR { L, R };

    struct node {
        DataType val;
        int pri, size, frq;
        node *ch[2], *par;

        node() : pri(INT_MIN), size(0), frq(0), ch{this, this}, par(this) {}
        node(const DataType &val)
            : val(val), pri(rnd(eng)), size(1), frq(1), ch{EMPTY, EMPTY}, par(EMPTY) {}

        void upd() { size = ch[L]->size + frq + ch[R]->size; }
    };

    static node *EMPTY;
    node *root = EMPTY;

    Treap() {}

    void link(node *p, node *c, int dir) {
        if (c != EMPTY) c->par = p;
        if (p != EMPTY) {
            p->ch[dir] = c;
            p->upd();
        }
    }

    int getDir(node *p, node *c) { return p->ch[R] == c; }

    void rotate(node *q) {
        node *p = q->par;
        int d = getDir(p, q);
        node *gp = p->par;
        int gd = getDir(gp, p);
        node *b = q->ch[!d];
        link(p, b, d);
        link(q, p, !d);
        link(gp, q, gd);
    }

    node *balance(node *root, int d) {
        if (root->ch[d]->pri > root->pri) {
            node *tmp = root->ch[d];
            rotate(tmp);
            return tmp;
        }
        return root;
    }

    node *insert(node *root, const DataType &val) {
        if (root == EMPTY) return new node(val);
        if (root->val == val) {
            root->frq++;
            root->upd();
            return root;
        }
        int d = val > root->val;
        link(root, insert(root->ch[d], val), d);
        return balance(root, d);
    }

    void insert(const DataType &val) { root = insert(root, val); } // add one copy of val

    node *erase(node *root, const DataType &val) {
        if (root == EMPTY) return root;
        if (root->val == val) {
            if (root->frq > 1) {
                root->frq--;
                root->upd();
                return root;
            }
            int mxcd = 0;
            for (int d = 0; d < 2; d++)
                if (root->ch[d] == EMPTY) {
                    node *tmp = root->ch[!d];
                    tmp->par = EMPTY;
                    delete root;
                    return tmp;
                } else if (root->ch[d]->pri > root->ch[mxcd]->pri)
                    mxcd = d;
            node *tmp = root->ch[mxcd];
            rotate(tmp);
            link(tmp, erase(tmp->ch[!mxcd], val), !mxcd);
            return tmp;
        }
        int d = val > root->val;
        link(root, erase(root->ch[d], val), d);
        return root;
    }

    // remove one copy of val, if present
    void erase(const DataType &val) { root = erase(root, val); }

    DataType kth(node *root, int k) {
        if (root == EMPTY) return DataType();
        if (k < root->ch[L]->size) return kth(root->ch[L], k);
        if (k < root->ch[L]->size + root->frq) return root->val;
        return kth(root->ch[R], k - root->ch[L]->size - root->frq);
    }

    // 0-based k-th smallest, duplicates counted separately
    DataType kth(int k) { return kth(root, k); }

    int lower_bound(node *root, const DataType &val) {
        if (root == EMPTY) return 0;
        if (val <= root->val) return lower_bound(root->ch[L], val);
        return root->ch[L]->size + root->frq + lower_bound(root->ch[R], val);
    }

    // how many elements are < val
    int lower_bound(const DataType &val) { return lower_bound(root, val); }

    // how many elements are <= val
    int upper_bound(const DataType &val) { return lower_bound(val + 1); }

    int size() { return root->size; }

    // copies of val stored
    int count(const DataType &val) { return upper_bound(val) - lower_bound(val); }

    void print(node *root, int d = 0) {
        if (root == EMPTY) return;
        print(root->ch[L], d + 1);
        cout << root->val << ' ' << root->frq << ' ' << d << '\n';
        print(root->ch[R], d + 1);
    }

    void print() { print(root); } // in-order dump: value, count, depth

    void clear(node *root) {
        if (root == EMPTY) return;
        clear(root->ch[L]);
        clear(root->ch[R]);
        delete root;
    }

    void clear() {
        clear(root);
        root = EMPTY;
    }

    ~Treap() { clear(root); }
};

template <typename DataType>
typename Treap<DataType>::node *Treap<DataType>::EMPTY = new typename Treap<DataType>::node();
