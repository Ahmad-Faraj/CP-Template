/*
    [1] DESCRIPTION
    A self-contained Multiset implemented directly using a 1-based
    Binary Indexed Tree with binary lifting for order statistics.
    Supports element insertion/deletion, frequency counts, k-th element
    queries, and relative order queries without external dependencies.

    [2] TIME & SPACE COMPLEXITY
    Insert / Erase: O(log MAX_VAL)
    Count / Order Queries: O(log MAX_VAL)
    at(k) [k-th element]: O(log MAX_VAL) via Binary Lifting
    Space: O(MAX_VAL)

    [3] LIMITS & NOTES
    Supports values in 1-based range [1, MAX_VAL].
    Uses 32-bit integer array internally to pass strict memory limits (<= 28 MB).
*/

#include "../../core.h"

class Fenwick_Multiset {
   private:
    vector<int> BIT;
    int max_val;
    int total_size;

    // helper functions
    void add(int idx, int delta) {
        for (; idx <= max_val; idx += (idx & -idx)) BIT[idx] += delta;
    }

    int query(int idx) const {
        int sum = 0;
        for (int i = min((int)idx, max_val); i > 0; i -= (i & -i)) sum += BIT[i];
        return sum;
    }

   public:
    Fenwick_Multiset() : max_val(0), total_size(0) {}

    // Initialize to hold 1-based values in range [1, _max_val]
    Fenwick_Multiset(int _max_val) { init(_max_val); }

    void init(int _max_val) {
        max_val = _max_val;
        total_size = 0;
        BIT.assign(max_val + 5, 0);
    }

    // Insert 'cnt' instances of 1-based value v (1 <= v <= max_val)
    void insert(int v, int cnt = 1) {
        add(v, cnt);
        total_size += cnt;
    }

    // Erase 'cnt' instances of 1-based value v if present
    void erase(int v, int cnt = 1) {
        int avail = count(v);
        int to_remove = std::min((long long)avail, (long long)cnt);
        if (to_remove > 0) {
            add(v, -to_remove);
            total_size -= to_remove;
        }
    }

    // Frequency count of 1-based value v
    int count(int v) const { return query(v) - query(v - 1); }

    // Total count of elements in the multiset
    int size() const { return total_size; }

    // Find 1-based k-th smallest element (1 <= k <= size) via Binary Lifting
    int at(int k) const {
        int target = k;
        int idx = 0;

        int mask = 1;
        while (mask <= max_val) mask <<= 1;

        for (mask >>= 1; mask > 0; mask >>= 1) {
            int nxt = idx + mask;
            if (nxt <= max_val && BIT[nxt] < target) {
                target -= BIT[nxt];
                idx = nxt;
            }
        }

        return idx + 1;  // Returns 1-based element
    }

    // Number of elements strictly less than 1-based value v
    int order_of_key(int v) const { return query(v - 1); }

    // Relative ordering queries (for 1-based value v)
    int lessThanMe(int v) const { return query(v - 1); }

    int lessThanOrEqualMe(int v) const { return query(v); }

    int largerThanMe(int v) const { return total_size - query(v); }

    int largerThanOrEqualMe(int v) const { return total_size - query(v - 1); }
};