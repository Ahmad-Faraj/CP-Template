#include "../../core.h"
/*
[1] Definition:
    Mo's Algorithm processes range queries [L, R] offline.
    It sorts queries by block index (sqrt(N)) to minimize overall pointer movement.

    [2] Time & Space Complexity:
    - Time Complexity: O((N + Q) * sqrt(N))
    - Space Complexity: O(N + Q)

    [3] Important Notes:
    - Works for offline queries only (no updates to array).
    - Uses 1-based indexing.
    - Set SQ = sqrt(N) for best performance.
    - 0-based indexing (array - queries)
    */

const int N = 200000 + 5;
const int SQ = 450;
const int HILBERT_POW = 21;

// static inline int64_t hilbertOrder(int x, int y, int pow, int rotate)
// {
//     if (pow == 0)
//         return 0;
//     int hpow = 1 << (pow - 1);
//     int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
//     seg = (seg + rotate) & 3;
//     const int rotateDelta[4] = {3, 0, 0, 1};
//     int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
//     int nrot = (rotate + rotateDelta[seg]) & 3;
//     int64_t subSquareSize = int64_t(1) << (2 * pow - 2);
//     int64_t ordd = seg * subSquareSize;
//     int64_t add = hilbertOrder(nx, ny, pow - 1, nrot);
//     ordd += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
//     return ordd;
// }

// struct Query
// {
//     int l, r, q_idx, blk_idx;
//     int64_t ord;

//     Query() {}

//     Query(int l, int r, int q_idx)
//     {
//         this->l = l;
//         this->r = r;
//         this->q_idx = q_idx;
//         blk_idx = l / SQ;

//         ord = hilbertOrder(l - 1, r - 1, HILBERT_POW, 0);
//     }

//     bool operator<(const Query &other) const
//     {
//         return ord < other.ord;
//     }
// };

struct Query {
    int l, r, q_idx, blk_idx;

    Query() {
    }

    Query(int l, int r, int q_idx) {
        this->l = l;
        this->r = r;
        this->q_idx = q_idx;
        blk_idx = l / SQ;
    }

    bool operator<(const Query &other) const {
        if (blk_idx != other.blk_idx) return blk_idx < other.blk_idx;
        return r < other.r;
    }
};

ll n, q, arr[N], ans[200005];
Query query[200005];

// change these depending on problem
ll vis[1000006], res = 0;

// Adds element at arr[idx] to current range
void add(int idx) {
    res -= (vis[arr[idx]] * vis[arr[idx]] * arr[idx]);
    vis[arr[idx]]++;
    res += (vis[arr[idx]] * vis[arr[idx]] * arr[idx]);
}

// Removes element at arr[idx] from current range
void remove(int idx) {
    res -= (vis[arr[idx]] * vis[arr[idx]] * arr[idx]);
    vis[arr[idx]]--;
    res += (vis[arr[idx]] * vis[arr[idx]] * arr[idx]);
}

// Processes all queries offline
void procces() {
    sort(query, query + q);
    int l = 1, r = 0;

    for (int i = 0; i < q; i++) {
        while (l < query[i].l) remove(l++);
        while (l > query[i].l) add(--l);
        while (r < query[i].r) add(++r);
        while (r > query[i].r) remove(r--);
        ans[query[i].q_idx] = res;
    }
}

// don't forget to resizing arrays based on problem's constrains