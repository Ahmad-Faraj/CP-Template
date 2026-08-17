/* Topic: Double String Hash
 * Description: Double Hash is a string hashing technique that computes two separate polynomial
 *   hashes with different moduli or bases to minimize the probability of collisions
 *   when comparing strings.
 * Usage: DoubleHash dh(s); dh.query(l, r); dh.merge(l1, r1, l2, r2);
 */
#include "../../core.h"

// Time Complexity: O(N) build, O(1) query
// Space Complexity: O(N)
struct DoubleHash {
    static int get_rand_base(int min_val = 313, int max_val = 1e9) {
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        return uniform_int_distribution<int>(min_val, max_val)(rng);
    }

    static int get_rand_prime() {
        static const int primes[] = { 1e9 + 513 ,  1e9 + 531 ,  1e9 + 579 ,  1e9 + 607 ,  1e9 + 613 ,
                                      1e9 + 637 ,  1e9 + 663 ,  1e9 + 711 ,  1e9 + 753 ,  1e9 + 787 } ;
        return primes[get_rand_base() % 10];
    }

    static inline int B1 = 0, B2 = 0, M1 = 0, M2 = 0;

    static void reassign() {
        B1 = get_rand_base();
        B2 = get_rand_base();
        M1 = get_rand_prime();
        M2 = get_rand_prime();
        while (M2 == M1) M2 = get_rand_prime();
    }

    vector<int> h1, h2, p1, p2;

    DoubleHash(string s) : h1(sz(s) + 1), h2(sz(s) + 1), p1(sz(s) + 1, 1), p2(sz(s) + 1, 1) {
        if (B1 == 0) reassign();
        for (int i = 0; i < sz(s); i++) {
            h1[i + 1] = (1LL * h1[i] * B1 + s[i]) % M1;
            h2[i + 1] = (1LL * h2[i] * B2 + s[i]) % M2;
            p1[i + 1] = (1LL * p1[i] * B1) % M1;
            p2[i + 1] = (1LL * p2[i] * B2) % M2;
        }
    }

    pair<int, int> query(int l, int r) {  // 0-indexed, inclusive
        int v1 = (h1[r + 1] - (1LL * h1[l] * p1[r - l + 1]) % M1 + M1) % M1;
        int v2 = (h2[r + 1] - (1LL * h2[l] * p2[r - l + 1]) % M2 + M2) % M2;
        return {v1, v2};
    }

    pair<int, int> merge(pair<int, int> h_left, pair<int, int> h_right, int len_right) {
        int v1 = (1LL * h_left.first * p1[len_right] % M1 + h_right.first) % M1;
        int v2 = (1LL * h_left.second * p2[len_right] % M2 + h_right.second) % M2;
        return {v1, v2};
    }

    pair<int, int> merge(int l1, int r1, int l2, int r2) { return merge(query(l1, r1), query(l2, r2), r2 - l2 + 1); }
};
