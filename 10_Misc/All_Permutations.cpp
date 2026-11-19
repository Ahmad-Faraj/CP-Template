// All Permutations: visit every distinct permutation of an array or string, and apply one k times.
// Use when: brute force over all orders (n <= ~10), "list all distinct rearrangements of a string",
//   "where is every element after k identical shuffles".
// Usage: all_permutations(v, [&](const auto &p) { ... }); auto r = permute(seq, perm, k);
// Handles: duplicates (each distinct order once, in sorted order), strings, k = 0, k up to 1e18.
// Time: all_permutations O(n * distinct permutations); applyPermutation O(n); permute O(n log k)
// Indexing: 0-based; applyPermutation(s, p)[i] = s[p[i]] (position i reads from p[i])
// Note: for p's cycles, order, or p^k as cycles use Permutation_Cycles.cpp.

#include <bits/stdc++.h>
using namespace std;

template <class C, class F> void all_permutations(C a, F visit) { // C = vector<...> or string
    sort(a.begin(), a.end());
    do {
        visit(a);
    } while (next_permutation(a.begin(), a.end()));
}

// Applies one permutation to a sequence
vector<int> applyPermutation(const vector<int> &sequence, const vector<int> &permutation) {
    vector<int> newSequence(sequence.size());
    for (int i = 0; i < (int)sequence.size(); i++) newSequence[i] = sequence[permutation[i]];
    return newSequence;
}

// Applies a permutation k times
vector<int> permute(vector<int> sequence, vector<int> permutation, long long k) {
    while (k > 0) {
        if (k & 1) sequence = applyPermutation(sequence, permutation);
        permutation = applyPermutation(permutation, permutation);
        k >>= 1;
    }
    return sequence;
}
