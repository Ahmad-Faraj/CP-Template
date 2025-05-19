#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

int N, M;
vector<string> stands;
unordered_map<int, int> memo;

int minStands(int mask) {
    // Base case: if mask is 0, no stands are needed
    if (mask == 0) return 0;
    // If already computed, return the stored result
    if (memo.find(mask) != memo.end()) return memo[mask];
    
    int result = INT_MAX;
    // Iterate over each stand
    for (int i = 0; i < N; ++i) {
        int flavorMask = 0;
        // Create a bitmask for the flavors this stand sells
        for (int j = 0; j < M; ++j) {
            if (stands[i][j] == 'o') {
                flavorMask |= (1 << j);
            }
        }
        // If the stand contributes to the current mask
        if ((mask & flavorMask) != 0) {
            // Recursively find the minimum stands needed for the new mask
            result = min(result, 1 + minStands(mask & ~flavorMask));
        }
    }
    // Memoize the result
    memo[mask] = result;
    return result;
}

int main() {
    cin >> N >> M;
    stands.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> stands[i];
    }

    // Call the recursive function with all flavors' mask
    int result = minStands((1 << M) - 1);
    cout << result << endl;

    return 0;
}
