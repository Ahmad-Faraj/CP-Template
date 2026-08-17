#include "../../core.h"

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
*/

class wavelet_tree {
  private:
    int lo, hi;
    wavelet_tree *left, *right;
    int *b, *c, *xr, bSz, cSz, xrSz; // c holds prefix sum of elements

  public:
    wavelet_tree()
        : lo(1), hi(0), bSz(0), cSz(0), xrSz(0), left(nullptr), right(nullptr), b(nullptr), c(nullptr), xr(nullptr) {
    }

    void init(int *from, int *to, int lx, int rx) {
        lo = lx, hi = rx;
        if (from >= to) return;

        int mid = (lo + hi) >> 1;

        b = (int *)malloc((to - from + 2) * sizeof(int));
        c = (int *)malloc((to - from + 2) * sizeof(int));
        xr = (int *)malloc((to - from + 2) * sizeof(int));

        bSz = 0, cSz = 0, xrSz = 0;
        b[bSz++] = 0, c[cSz++] = 0, xr[xrSz++] = 0;

        auto sm = [mid](int x) { return x <= mid; };

        for (auto it = from; it != to; it++) {
            b[bSz] = (b[bSz - 1] + sm(*it));
            c[cSz] = (c[cSz - 1] + (*it));
            xr[xrSz] = (xr[xrSz - 1] ^ (*it));

            bSz++, cSz++, xrSz++;
        }

        if (hi == lo) return;

        auto pivot = stable_partition(from, to, sm); // this function splite the them into two arrays based on condition
        left = new wavelet_tree(), right = new wavelet_tree();
        left->init(from, pivot, lo, mid), right->init(pivot, to, mid + 1, hi);
    }

    // kth smallest element
    int kth_smallest(int l, int r, int k) {
        if (l > r) return 0;

        if (lo == hi) return lo;

        int inLeft = b[r] - b[l - 1];
        int lb = b[l - 1], rb = b[r];

        if (k <= inLeft)
            return this->left->kth_smallest(lb + 1, rb, k);
        else
            return this->right->kth_smallest(l - lb, r - rb, k - inLeft);
    }

    // COUNTING QUERIES
    // count of numbers in [l, r] Less than or equal to (<= k)
    int count_LTE(int l, int r, int k) {
        if (l > r || k < lo) return 0;

        if (hi <= k) return r - l + 1;

        int lb = b[l - 1], rb = b[r];

        return this->left->count_LTE(lb + 1, rb, k) + this->right->count_LTE(l - lb, r - rb, k);
    }

    // Count of numbers in [l, r] Greater Than or Equal to (k <=)
    int count_GTE(int l, int r, int k) {
        return (r - l + 1) - count_LTE(l, r, k - 1);
    }

    // Count of numbers in [l, r] Strictly Less Than  (< k)
    int count_LT(int l, int r, int k) {
        return count_LTE(l, r, k - 1);
    }

    // Count of numbers in [l, r] Strictly Greater Than  (k <)
    int count_GT(int l, int r, int k) {
        return (r - l + 1) - count_LTE(l, r, k);
    }

    // Count of numbers in [l, r] Equal to k (== k)
    int freq_equal(int l, int r, int k) {
        if (l > r || k < lo || k > hi) return 0;

        if (lo == hi) return r - l + 1;

        int lb = b[l - 1], rb = b[r], mid = (lo + hi) >> 1;

        if (k <= mid)
            return this->left->freq_equal(lb + 1, rb, k);
        else
            return this->right->freq_equal(l - lb, r - rb, k);
    }

    // SUM QUERIES
    // sum of numbers in [l ,r] <= k
    ll sum_LTE(int l, int r, int k) {
        if (l > r || k < lo) return 0;

        if (hi <= k) return c[r] - c[l - 1];

        int lb = b[l - 1], rb = b[r];

        return this->left->sum_LTE(lb + 1, rb, k) + this->right->sum_LTE(l - lb, r - rb, k);
    }

    // Sum of numbers in [l, r] >= k
    ll sum_GTE(int l, int r, int k) {
        return (c[r] - c[l - 1]) - sum_LTE(l, r, k - 1);
    }

    // Sum of numbers in [l, r] < k
    ll sum_LT(int l, int r, int k) {
        return sum_LTE(l, r, k - 1);
    }

    // Sum of numbers in [l, r] > k
    ll sum_GT(int l, int r, int k) {
        return (c[r] - c[l - 1]) - sum_LTE(l, r, k);
    }

    // Sum of numbers in [l, r] == k
    ll sum_EQ(int l, int r, int k) {
        return 1LL * freq_equal(l, r, k) * k;
    }

    // XOR QUERIES
    // XOR sum of numbers in [l, r] <= k
    int xor_sum_LTE(int l, int r, int k) {
        if (l > r || k < lo) return 0;

        if (hi <= k) return xr[r] ^ xr[l - 1];

        int mid = lo + ((hi - lo) >> 1);
        int lb = b[l - 1], rb = b[r];

        if (k <= mid)
            return this->left->xor_sum_LTE(lb + 1, rb, k);
        else {
            int left_xor = (this->left != nullptr && rb > lb) ? (this->left->xr[rb] ^ this->left->xr[lb]) : 0;

            return left_xor ^ this->right->xor_sum_LTE(l - lb, r - rb, k);
        }
    }

    // XOR sum of numbers in [l, r] >= k
    int xor_sum_GTE(int l, int r, int k) {
        return (xr[r] ^ xr[l - 1]) ^ xor_sum_LTE(l, r, k - 1);
    }

    // XOR sum of numbers in [l, r] < k
    int xor_sum_LT(int l, int r, int k) {
        return xor_sum_LTE(l, r, k - 1);
    }

    // XOR sum of numbers in [l, r] > k
    int xor_sum_GT(int l, int r, int k) {
        return (xr[r] ^ xr[l - 1]) ^ xor_sum_LTE(l, r, k);
    }

    // XOR sum of numbers in [l, r] == k
    int xor_sum_EQ(int l, int r, int k) {
        int count = freq_equal(l, r, k);

        return (count & 1) ? k : 0;
    }

    ~wavelet_tree() {
        delete left;
        delete right;

        free(b);
        free(c);
        free(xr);
    }
};

const int MAXN = 2e5 + 5;
const int MAXV = 1e9;
wavelet_tree wt;

int arr[MAXN];

// signed main() {
//     ios_base::sync_with_stdio(false);
//     cin.tie(nullptr);
//
//    int n, q;
//    cin >> n;
//
//  for (int i = 1; i <= n; i++) cin >> arr[i];
//
//  wt.init(arr + 1, arr + n + 1, 0, MAXV);
// cin >> q;
// while (q--) {
//  int l, r, k;
// cin >> l >> r >> k;
//
//      cout << wt.sum_LTE(l, r, k) << nl;
//}
//
//   return 0;
//}