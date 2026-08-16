/* Topic: Single String Hash
 * Description: Single String Hash uses a standard polynomial rolling hash for quickly checking
 *   string equivalence. Often implemented modulo a large prime to achieve high
 *   reliability in substring comparison tasks.
 * Usage: SingleHash sh(s); sh.query(l, r); sh.merge(l1, r1, l2, r2);
 */
#include "../../core.h"

// Time Complexity: O(N) build, O(1) query
// Space Complexity: O(N)
struct SingleHash {
    static int get_rand_base(int min_val = 313, int max_val = 1e9) {
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        return uniform_int_distribution<int>(min_val, max_val)(rng);
    }

    static int get_rand_prime() {
        static const int primes[] = {(int)1e9 + 513, (int)1e9 + 531, (int)1e9 + 579, (int)1e9 + 607, (int)1e9 + 613,
                                     (int)1e9 + 637, (int)1e9 + 663, (int)1e9 + 711, (int)1e9 + 753, (int)1e9 + 787};
        return primes[get_rand_base() % 10];
    }

    static inline int B1 = 0, M1 = 0;

    static void reassign() {
        B1 = get_rand_base();
        M1 = get_rand_prime();
    }

    vector<int> h1, p1;

    SingleHash(string s) : h1(sz(s) + 1), p1(sz(s) + 1, 1) {
        if (B1 == 0) reassign();
        for (int i = 0; i < sz(s); i++) {
            h1[i + 1] = (h1[i] * B1 + s[i]) % M1;
            p1[i + 1] = (p1[i] * B1) % M1;
        }
    }

    int query(int l, int r) {  // 0-indexed, inclusive
        return (h1[r + 1] - (h1[l] * p1[r - l + 1]) % M1 + M1) % M1;
    }

    int merge(int h_left, int h_right, int len_right) { return (h_left * p1[len_right] % M1 + h_right) % M1; }

    int merge(int l1, int r1, int l2, int r2) { return merge(query(l1, r1), query(l2, r2), r2 - l2 + 1); }
};
