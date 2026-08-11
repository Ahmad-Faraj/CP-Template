/*
    [1] Definition
    Persistent Segment Tree with Lazy Propagation maintaining multiple versions
    of a segment tree with range updates and range queries over time.

    [2] Time & Space Complexity
    Build: O(N)
    Update: O(log N)
    Query: O(log N)
    Space: O(N + Q log N)

    [3] Notes
    Uses 1-based indexing for queries.
    Stores root pointers to support querying historical versions.
    Supports range lazy propagation for range updates.
    Default value (0) is the identity for sum queries.
*/

#include "../../core.h"

// 1-based indexing
template <typename T = int>
class PersistentSegTree {
   private:
    // Tree node
    struct Node {
        T val, lazy;
        Node *l, *r;

        Node() : val(0), lazy(0), l(this), r(this) {}

        Node(T v) : val(v), lazy(0), l(DEFAULT), r(DEFAULT) {}

        Node(const Node& o) : val(o.val), lazy(o.lazy), l(o.l), r(o.r) {}
    };

    static Node* DEFAULT;

    int N;
    vector<Node*> roots;  // Root pointers for each version

    // Build initial version recursively
    Node* build(int L, int R, vector<int>& nums) {
        if (L == R) return new Node(nums[L - 1]);
        int M = (L + R) >> 1;
        Node* node = new Node();
        node->l = build(L, M, nums);
        node->r = build(M + 1, R, nums);
        node->val = node->l->val + node->r->val;
        return node;
    }

    // Push lazy tag down to children by cloning them
    void push(Node* node, int L, int R) {
        if (node->lazy && L < R) {
            int M = (L + R) >> 1;
            node->l = new Node(*node->l);
            node->r = new Node(*node->r);
            node->l->lazy += node->lazy;
            node->l->val += node->lazy * (M - L + 1);
            node->r->lazy += node->lazy;
            node->r->val += node->lazy * (R - M);
            node->lazy = 0;
        }
    }

    // Query range [i, j] on a version
    T query(Node* node, int L, int R, int i, int j) {
        if (j < L || R < i) return 0;
        if (i <= L && R <= j) return node->val;
        push(node, L, R);
        int M = (L + R) >> 1;
        return query(node->l, L, M, i, j) + query(node->r, M + 1, R, i, j);
    }

    // Create new path with updated range [i, j]
    Node* update(Node* cur, int L, int R, int i, int j, T v) {
        if (j < L || R < i) return cur;
        Node* node = new Node(*cur);
        if (i <= L && R <= j) {
            node->lazy += v;
            node->val += v * (R - L + 1);
            return node;
        }
        push(node, L, R);
        int M = (L + R) >> 1;
        node->l = update(node->l, L, M, i, j, v);
        node->r = update(node->r, M + 1, R, i, j, v);
        node->val = node->l->val + node->r->val;
        return node;
    }

   public:
    PersistentSegTree(int n, vector<int> nums) : N(n) {
        if (DEFAULT == nullptr) {
            DEFAULT = new Node();
            DEFAULT->l = DEFAULT->r = DEFAULT;
        }
        roots.push_back(build(1, N, nums));
    }

    // Range update on a specific version
    void update(int l, int r, int v) { roots.push_back(update(roots.back(), 1, N, l, r, v)); }

    // Overwrite to update specific version
    void update(int l, int r, int v, int version) { roots.push_back(update(roots[version], 1, N, l, r, v)); }

    // normal query [l , r]
    T query(int l, int r) { return query(roots.back(), 1, N, l, r); }

    // Query version for range [l, r]
    T query(int ver, int l, int r) { return query(roots[ver], 1, N, l, r); }

    // resize root
    void reSize(int k) { roots.resize(k + 1); }
};

template <typename T>
typename PersistentSegTree<T>::Node* PersistentSegTree<T>::DEFAULT = nullptr;