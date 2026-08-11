/*
    [1] Definition
    Persistent Segment Tree maintaining multiple versions of a segment tree over time.

    [2] Time & Space Complexity
    Build: O(N)
    Update: O(log N)
    Query: O(log N)
    Space: O(N + Q log N)

    [3] Notes
    Uses 1-based indexing for queries.
    Stores root pointers to support querying historical versions.
    Default value (0) is the identity for sum queries.
*/

#include "../../core.h"

// 1-based indexing
template <typename T = int>
class PST {
   private:
    // Tree node
    struct Node {
        T val;
        Node *left, *right;

        Node(T _val = 0) {
            this->val = _val;
            left = right = this;
        }

        Node(Node* node, Node* l = new Node, Node* r = new Node) {
            val = node->val;
            left = l;
            right = r;
        }
    };

    // new Node to be 1-based Indexing
    vector<Node*> roots = {new Node()};  // Root pointers for each version
    int N;
    T Lx, Rx;

    // Build initial version recursively
    Node* build(const vector<T>& nums, T l, T r) {
        if (l == r) return new Node(nums[l - 1]);
        T mx = l + (r - l) / 2;
        Node* L = build(nums, l, mx);
        Node* R = build(nums, mx + 1, r);
        return new Node(merge(L, R), L, R);
    }

    // Combine left and right nodes
    Node* merge(Node* a, Node* b) {
        Node* node = new Node();
        node->val = a->val + b->val;
        return node;
    }

    // Create new path with updated node
    Node* update(Node* root, int idx, T val, T lx, T rx) {
        if (idx < lx || idx > rx) return root;
        if (lx == rx) return new Node(val);
        T mx = lx + (rx - lx) / 2;
        Node* L = update(root->left, idx, val, lx, mx);
        Node* R = update(root->right, idx, val, mx + 1, rx);
        return new Node(merge(L, R), L, R);
    }

    // Query range [l, r] on a version
    Node* query(Node* root, int l, int r, T lx, T rx) {
        if (root == nullptr) return new Node();
        if (lx > r || l > rx) return new Node();
        if (lx >= l && rx <= r) return root;
        int mx = lx + (rx - lx) / 2;
        Node* L = query(root->left, l, r, lx, mx);
        Node* R = query(root->right, l, r, mx + 1, rx);
        return merge(L, R);
    }

    int count_greater(Node* l, Node* r, int k, int lx, int rx) {
        if (k < lx) return r->val - l->val;

        if (k >= rx) return 0;

        int mid = lx + (rx - lx) / 2;

        return count_greater(l->left, r->left, k, lx, mid) + count_greater(l->right, r->right, k, mid + 1, rx);
    }

    T kth(Node* l, Node* r, int k, T lx, T rx) {
        if (lx == rx) return lx;
        T m = lx + (rx - lx) / 2;
        int count = r->left->val - l->left->val;  // Count of elements in the left subtree
        if (count >= k)
            return kth(l->left, r->left, k, lx, m);
        else
            return kth(l->right, r->right, k - count, m + 1, rx);
    }

   public:
    PST(int n = 0, T lx = -1e9, T rx = 1e9) : N(n), Lx(lx), Rx(rx) {}

    // Build and store initial version
    void build(const vector<T>& nums) { roots.push_back(build(nums, Lx, Rx)); }

    // Query version for range [l, r]
    T query(int l, int r, int version) { return query(roots[version], l, r, Lx, Rx)->val; }

    // Create new version with updated value
    void update(int idx, T val) { roots.push_back(update(roots.back(), idx, val, Lx, Rx)); }

    // Overwrite to update specific version
    void update_version(int idx, T val, int version) { roots[version] = (update(roots[version], idx, val, Lx, Rx)); }

    // Duplicate a version
    void copy(int version) { roots.push_back(roots[version]); }

    // Repush the last version
    void clone() { roots.push_back(roots.back()); }

    // Get value at index and version [version, idx]
    T get(int time, int idx) { return query(idx, idx, time)->val; }

    // count the number of elements greater than k in the range [l, r] don't forget notes
    int count_greater(int l, int r, int v) { return count_greater(roots[l - 1], roots[r], v, Lx, Rx); }

    // find index of the k-th smallest element in the range [l, r]
    T kth(int l, int r, int k) { return kth(roots[l - 1], roots[r], k, Lx, Rx); }
};