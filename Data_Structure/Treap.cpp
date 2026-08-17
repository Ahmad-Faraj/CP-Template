// Treap: a balanced BST keyed by value, with order statistics - k-th smallest and the rank of a value.
// Use when: a sorted multiset that must also answer "k-th smallest" and "how many elements are below x".
// Handles: insert, erase, k-th, lower_bound, upper_bound, duplicates via a per-node frequency count.
// Time: every operation O(log n) expected
// Indexing: kth() is 0-based; lower_bound returns a rank, not an iterator
// Note: EMPTY is one sentinel shared by every treap of a type - never delete it. upper_bound needs an integer type.

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
        node(const DataType &val) : val(val), pri(rnd(eng)), size(1), frq(1), ch{EMPTY, EMPTY}, par(EMPTY) {}

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

    void erase(const DataType &val) { root = erase(root, val); } // remove one copy of val, if present

    DataType kth(node *root, int k) {
        if (root == EMPTY) return DataType();
        if (k < root->ch[L]->size) return kth(root->ch[L], k);
        if (k < root->ch[L]->size + root->frq) return root->val;
        return kth(root->ch[R], k - root->ch[L]->size - root->frq);
    }

    DataType kth(int k) { return kth(root, k); } // 0-based k-th smallest, duplicates counted separately

    int lower_bound(node *root, const DataType &val) {
        if (root == EMPTY) return 0;
        if (val <= root->val) return lower_bound(root->ch[L], val);
        return root->ch[L]->size + root->frq + lower_bound(root->ch[R], val);
    }

    int lower_bound(const DataType &val) { return lower_bound(root, val); } // how many elements are < val

    int upper_bound(const DataType &val) { return lower_bound(val + 1); } // how many elements are <= val

    int size() { return root->size; }

    int count(const DataType &val) { return upper_bound(val) - lower_bound(val); } // copies of val stored

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

    ~Treap() { clear(root); } // EMPTY is shared, so it is deliberately not freed here
};

template <typename DataType>
typename Treap<DataType>::node *Treap<DataType>::EMPTY = new typename Treap<DataType>::node();

// Standard problem: I inserts, D deletes, K reports the k-th smallest, C counts elements below a value
void solve() {
    int q;
    cin >> q;
    Treap<int> tp;
    while (q--) {
        char op;
        int x;
        cin >> op >> x;
        if (op == 'I')
            tp.insert(x);
        else if (op == 'D')
            tp.erase(x);
        else if (op == 'K')
            cout << (x > tp.size() ? -1 : tp.kth(x - 1)) << '\n';
        else
            cout << tp.lower_bound(x) << '\n';
    }
}
