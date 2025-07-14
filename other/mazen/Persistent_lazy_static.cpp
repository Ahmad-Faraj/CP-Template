#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ---------------- Persistent Segment Tree with Lazy Propagation ----------------

template<typename T = ll>
struct PersistentSegTree {
    struct Node {
        T    val, lazy;
        Node *l, *r;
        // Empty/sentinel constructor
        Node(): val(0), lazy(0), l(this), r(this) {}
        // Value constructor
        Node(T v): val(v), lazy(0), l(EMPTY), r(EMPTY) {}
        // Copy constructor (cloning)
        Node(const Node &o)
            : val(o.val), lazy(o.lazy), l(o.l), r(o.r) {}
    };

    // One shared EMPTY node
    static Node* EMPTY;

    int N;
    vector<Node*> roots;

    PersistentSegTree(int n): N(n) {
        // Initialize EMPTY on first use
        if (EMPTY == nullptr) {
            EMPTY = new Node();
            EMPTY->l = EMPTY->r = EMPTY;
        }
        // Build version 0 (all zeros)
        roots.push_back(build(1, N));
    }

    // Build an all-zero tree on [L..R]
    Node* build(int L, int R) {
        if (L == R) {
            return new Node(0);
        }
        int M = (L + R) >> 1;
        Node* node = new Node();
        node->l = build(L,   M);
        node->r = build(M+1, R);
        return node;
    }

    // Push lazy from node to children, cloning children
    void push(Node* node, int L, int R) {
        if (node->lazy != 0 && L < R) {
            int M = (L + R) >> 1;
            // Clone children
            node->l = new Node(*node->l);
            node->r = new Node(*node->r);
            // Propagate
            node->l->lazy += node->lazy;
            node->l->val   += node->lazy * (M - L + 1);
            node->r->lazy += node->lazy;
            node->r->val   += node->lazy * (R - M);
            node->lazy = 0;
        }
    }

    // Internal update: returns new root for this version
    Node* update(Node* cur, int L, int R, int i, int j, T v) {
        if (j < L || R < i) {
            // no overlap → reuse
            return cur;
        }
        Node* node = new Node(*cur);  // clone this node
        if (i <= L && R <= j) {
            // fully covered
            node->lazy += v;
            node->val   += v * (R - L + 1);
            return node;
        }
        push(node, L, R);
        int M = (L + R) >> 1;
        node->l = update(node->l, L,   M, i, j, v);
        node->r = update(node->r, M+1, R, i, j, v);
        node->val = node->l->val + node->r->val;
        return node;
    }

    // Public API: create new version by adding `v` on [l..r] to version `ver`
    void update(int ver, int l, int r, T v) {
        roots.push_back(update(roots[ver], 1, N, l, r, v));
    }

    // Internal query (no longer const, since we may push/clone)
    T query(Node* node, int L, int R, int i, int j) {
        if (j < L || R < i) {
            return 0;
        }
        if (i <= L && R <= j) {
            return node->val;
        }
        push(node, L, R);
        int M = (L + R) >> 1;
        return query(node->l, L,   M, i, j)
             + query(node->r, M+1, R, i, j);
    }

    // Public API: query version `ver` on range [l..r]
    T query(int ver, int l, int r) {
        return query(roots[ver], 1, N, l, r);
    }
};

// Initialize the static EMPTY pointer
template<typename T>
typename PersistentSegTree<T>::Node* PersistentSegTree<T>::EMPTY = nullptr;

// ----------------------------- Example Usage -----------------------------

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    // Build a PST for an array of size n (initially all zeros)
    PersistentSegTree<ll> pst(n);

    // We'll track versions: version 0 is the initial all-zero array.
    // After each type-1 update, we get a new version (1, 2, ...).

    for(int _ = 0; _ < q; _++){
        int type;
        cin >> type;
        if (type == 1) {
            // Type 1: update range [l..r] by +v, starting from the latest version
            int l, r; ll v;
            cin >> l >> r >> v;
            int currVer = (int)pst.roots.size() - 1;
            pst.update(currVer, l, r, v);
            cout << "Created version " << pst.roots.size()-1 << "\n";
        }
        else {
            // Type 2: query sum on [l..r] from version ver
            int ver, l, r;
            cin >> ver >> l >> r;
            ll ans = pst.query(ver, l, r);
            cout << ans << "\n";
        }
    }

    return 0;
}
