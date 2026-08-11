/*
    [1] Definition
    Wavelet Tree over a value range [lo, hi] that recursively partitions an array of integers
    based on whether elements are <= mid. Supports range query operations such as range k-th
    smallest, count of elements <= k, sum of elements <= k, and frequency of k.

    [2] Time & Space Complexity
    Build: O(N log(MAXV))
    k-th Smallest Query: O(log(MAXV))
    Less-Than-or-Equal Count (LTE): O(log(MAXV))
    Sum of Elements <= k: O(log(MAXV))
    Frequency Query: O(log(MAXV))
    Space: O(N log(MAXV))

    [3] Notes
    Uses 1-based indexing for range queries [l, r].
    Values in the array must be within [lo, hi] provided during initialization.
    `b` vector acts as a bitmap prefix sum for elements routed to the left child.
    `c` vector acts as a prefix sum array over elements at the current node.
*/

#include "../../core.h"

class wavelet_tree {
   private:
    int lo, hi;
    wavelet_tree *left, *right;
    int *b, *c, bSz, cSz;  // c holds prefix sum of elements

   public:
    wavelet_tree() : lo(1), hi(0), bSz(0), cSz(0), left(nullptr), right(nullptr) {}

    void init(int* from, int* to, int lx, int rx) {
        lo = lx, hi = rx;
        if (from >= to) return;

        int mid = (lo + hi) >> 1;

        b = (int*)malloc((to - from + 2) * sizeof(int));
        c = (int*)malloc((to - from + 2) * sizeof(int));
        bSz = 0, cSz = 0;
        b[bSz++] = 0, c[cSz++] = 0;

        auto sm = [mid](int x) { return x <= mid; };

        for (auto it = from; it != to; it++) {
            b[bSz] = (b[bSz - 1] + sm(*it));
            c[cSz] = (c[cSz - 1] + (*it));
            bSz++, cSz++;
        }

        if (hi == lo) return;

        auto pivot =
            stable_partition(from, to, sm);  // this function splite the them into two arrays based on condition
        left = new wavelet_tree(), right = new wavelet_tree();
        left->init(from, pivot, lo, mid), right->init(pivot, to, mid + 1, hi);
    }

    // kth smallest element
    int kth(int l, int r, int k) {
        if (l > r) return 0;

        if (lo == hi) return lo;

        int inLeft = b[r] - b[l - 1];
        int lb = b[l - 1], rb = b[r];

        if (k <= inLeft)
            return this->left->kth(lb + 1, rb, k);
        else
            return this->right->kth(l - lb, r - rb, k - inLeft);
    }

    // count of numbers in [l, r] Less than or equal to k
    int LTE(int l, int r, int k) {
        if (l > r || k < lo) return 0;

        if (hi <= k) return r - l + 1;

        int lb = b[l - 1], rb = b[r];

        return this->left->LTE(lb + 1, rb, k) + this->right->LTE(l - lb, r - rb, k);
    }

    // sum of numbers in [l ,r] less than or equal to k
    int sum(int l, int r, int k) {
        if (l > r || k < lo) return 0;

        if (hi <= k) return c[r] - c[l - 1];

        int lb = b[l - 1], rb = b[r];

        return this->left->sum(lb + 1, rb, k) + this->right->sum(l - lb, r - rb, k);
    }

    // count of numbers in [l, r] equal to k
    int freq(int l, int r, int k) {
        if (l > r || k < lo) return 0;

        if (lo == hi) return r - l + 1;

        int lb = b[l - 1], rb = b[r], mid = (lo + hi) >> 1;

        if (k <= mid)
            return this->left->freq(lb + 1, rb, k);
        else
            return this->right->freq(l - lb, r - rb, k);
    }

    ~wavelet_tree() {
        delete left;
        delete right;
    }
};

const int MAXN = 2e5 + 5;
const int MAXV = 1e9;
wavelet_tree wt;

int arr[MAXN];

/*
signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; i++) cin >> arr[i];

    wt.init(arr + 1, arr + n + 1, 0, MAXV);

    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        l++, k++;
        cout << wt.kth(l, r, k) << nl;
    }

    return 0;
}
*/