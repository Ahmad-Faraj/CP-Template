#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define all(x) x.begin(), x.end()

// print all permutation of an array
void Print_Permutation(vector<int> &nums) {
    sort(all(nums));
    do {
        for (auto &i : nums) cout << i << " ";
        cout << "\n";
    } while (next_permutation(nums.begin(), nums.end()));
}

// print all permutation of a string
void Print_Permutation(string s) {
    sort(all(s));
    do {
        cout << s << "\n";
    } while (next_permutation(s.begin(), s.end()));
}

// Applies one permutation to a sequence
vector<int> applyPermutation(vector<int> sequence, vector<int> permutation) {
    vector<int> newSequence(sequence.size());
    for (int i = 0; i < sequence.size(); i++) {
        newSequence[i] = sequence[permutation[i]];
    }
    return newSequence;
}

// Applies a permutation k times using binary exponentiation
vector<int> permute(vector<int> sequence, vector<int> permutation, long long k) {
    while (k > 0) {
        if (k & 1) {
            sequence = applyPermutation(sequence, permutation);
        }
        permutation = applyPermutation(permutation, permutation);
        k >>= 1;
    }
    return sequence;
}