/*
 * Topic: XOR Basis Offline Range Queries
 * Description: Boilerplate to answer Q offline range [L, R] queries in O(N log X + Q log X)
 *   by sorting queries by R and using the Prefix Basis.
 */
#include "XorBasisPrefix.cpp"

struct Query {
    int l, r, id;
};

// Answers offline queries asking for the Maximum XOR in subarray A[L...R]
vector<int> solve_offline_queries(const vector<int> &a, vector<Query> &queries) {
    int n = a.size();
    int q = queries.size();

    // Sort queries by their right endpoint in ascending order
    sort(queries.begin(), queries.end(), [](const Query &A, const Query &B) { return A.r < B.r; });

    vector<int> ans(q);
    XorBasisPrefix<62> basis;

    int curr_r = 0;
    for (int i = 0; i < q; i++) {
        // Insert elements into the prefix basis until we reach the query's right bound
        while (curr_r < n && curr_r <= queries[i].r) {
            basis.insert(a[curr_r], curr_r);
            curr_r++;
        }
        // Answer the query using the left bound
        ans[queries[i].id] = basis.max_xor(queries[i].l);
    }
    return ans;
}
