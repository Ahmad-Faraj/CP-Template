/*
 * Topic: Boolean FFT
 * Description: Boolean Convolution using Bitsets
 * Very Very Very Fast
 */
#include "../../core.h"

struct FFT_boolean {

    // Function to multiply two bitsets (Boolean Convolution)
    // Time Complexity: O(S * N / 64) where S is the number of set bits in B.
    // Worst-Case Time: O(N^2 / 64) when B is full of 1s.
    // Space Complexity: O(N / 64) to store the result bitset.
    bitset<N> multiply(const bitset<N>& A, const bitset<N>& B) {
        bitset<N> res;
        // B._Find_first() and B._Find_next() only iterate over the bits that are 1.
        for (int i = B._Find_first(); i < N; i = B._Find_next(i)) {
            res |= (A << i);  // Shifts and ORs N bits simultaneously in chunks of 64
        }
        return res;
    }

    // Function to calculate (base)^k using Binary Exponentiation
    // Time Complexity: O( ( N^2 / 64 ) * log k )
    //   - The loop runs log2(k) times.
    //   - Each iteration does up to 2 bitset multiplications of worst-case O(N^2 / 64).
    // Space Complexity: O(N / 64) memory overhead for local bitsets.
    bitset<N> power(bitset<N> base, int k) {
        bitset<N> res;
        res.set(0);  // Base case: Sum of 0 elements is 0 (x^0 = 1 in polynomials)

        while (k > 0) {
            if (k % 2 == 1) { res = multiply(res, base); }
            if (k > 1) { base = multiply(base, base); }
            k /= 2;
        }
        return res;
    }
};
