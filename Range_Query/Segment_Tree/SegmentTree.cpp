#include "../../core.h"
/*
    [1] Definition
    Data structure for range queries and point updates on an array.

    [2] Time & Space Complexity
    Build: O(N)
    Update: O(log N)
    Query: O(log N)
    Space: O(N)

    [3] Notes
    Uses 1-based indexing for queries.
    Default node value is the identity node.
    Tree size is rounded up to the nearest power of 2.
*/

const int INF = 1e9 + 7;
const long long INFLL = 1e18;

// Tree node
struct Node {
    ll sum;

    // max-min in segment
    int maxi, mini;

    // count kth-zero
    int cnt_zero;

    // max sub array
    ll pref, suff, mxsub;

    Node() {
        sum = 0;

        mini = INF;
        maxi = -INF;
        cnt_zero = 0;

        pref = -INFLL;
        suff = -INFLL;
        mxsub = -INFLL;
    }

    Node(int x) {
        sum = x;

        mini = x;
        maxi = x;
        cnt_zero = (x == 0 ? 1 : 0);

        // if segment may be empty all should be max(0LL, x)
        pref = x;
        suff = x;
        mxsub = x;
    }

    void update(int x) {
        sum = x;

        mini = x;
        maxi = x;
        cnt_zero = (x == 0 ? 1 : 0);

        // if segment may be empty all should be max(0LL, x)
        pref = x;
        suff = x;
        mxsub = x;
    }
};

// 1-based indexing
class Segment_Tree {
  private:
    int size;
    vector<Node> tree;

    Node merge(const Node &l, const Node &r) {
        if (l.mxsub == -INFLL) return r;
        if (r.mxsub == -INFLL) return l;

        Node res;
        res.sum = l.sum + r.sum;
        res.mini = min(l.mini, r.mini);
        res.maxi = max(l.maxi, r.maxi);
        res.cnt_zero = l.cnt_zero + r.cnt_zero;

        res.pref = max(l.pref, l.sum + r.pref);
        res.suff = max(r.suff, r.sum + l.suff);
        res.mxsub = max({l.mxsub, r.mxsub, l.suff + r.pref});

        return res;
    }

    void init(const vector<int> &nums, int id, int lx, int rx) {
        if (lx == rx) {
            if (lx <= sz(nums)) tree[id] = Node(nums[lx - 1]);
            return;
        }

        int mid = (lx + rx) / 2;
        init(nums, 2 * id, lx, mid);
        init(nums, 2 * id + 1, mid + 1, rx);

        tree[id] = merge(tree[2 * id], tree[2 * id + 1]);
    }

    // Update node at index
    void set(const int idx, const int v, int id, int lx, int rx) {
        if (lx == rx) {
            tree[id].update(v);
            return;
        }

        int mid = (lx + rx) / 2;
        if (idx <= mid)
            set(idx, v, 2 * id, lx, mid);
        else
            set(idx, v, 2 * id + 1, mid + 1, rx);

        tree[id] = merge(tree[2 * id], tree[2 * id + 1]);
    }

    // Query range [l, r]
    Node get(const int l, const int r, int id, int lx, int rx) {
        if (l > rx || r < lx) return Node();

        if (l <= lx && r >= rx) return tree[id];

        int mid = (lx + rx) / 2;

        Node ln = get(l, r, 2 * id, lx, mid);
        Node rn = get(l, r, 2 * id + 1, mid + 1, rx);

        return merge(ln, rn);
    }

    // Helper: Find k-th zero
    int find_kth_zero(int k, int id, int lx, int rx) {
        if (tree[id].cnt_zero < k) return -1;

        if (lx == rx) return lx;

        int mid = (lx + rx) / 2;
        int left_zeros = tree[2 * id].cnt_zero;

        if (k <= left_zeros)
            return find_kth_zero(k, 2 * id, lx, mid);
        else
            return find_kth_zero(k - left_zeros, 2 * id + 1, mid + 1, rx);
    }

    // Helper: Find first element in [l, r] that is >= x
    int get_first_P(int l, int r, int x, int id, int lx, int rx) {
        if (lx > r || rx < l) return -1;

        if (tree[id].maxi < x) return -1;

        if (lx == rx) return lx;

        int mid = (lx + rx) / 2;
        int left = get_first_P(l, r, x, 2 * id, lx, mid);
        if (left != -1) return left;

        return get_first_P(l, r, x, 2 * id + 1, mid + 1, rx);
    }

  public:
    Segment_Tree(int n) {
        size = 1;
        while (size < n) size <<= 1;
        tree = vector<Node>(2 * size, Node());
    }

    // Build tree
    void init(const vector<int> &nums) {
        init(nums, 1, 1, size);
    }

    // Update value at index
    void update(const int idx, const int v) {
        set(idx, v, 1, 1, size);
    }

    // query [l , r]
    ll query(int l, int r) {
        return get(l, r, 1, 1, size).mxsub;
    }

    // query kth zero position
    int kth_zero(int kth) {
        return find_kth_zero(kth, 1, 1, size);
    }

    // query first element's position in [l, r] such that is >= x
    int get_first(int l, int r, int x) {
        return get_first_P(l, r, x, 1, 1, size);
    }
};