#include <bits/stdc++.h>
using namespace std;
#define ll long long

// Find MEX of L to R
struct SegmentTree {
    int n;
    vector<int> tree;
    SegmentTree(int sz) {
        n = 1;
        while (n < sz) {
            n <<= 1;
        }
        tree.assign(2 * n, 0);
    }
    void set(int ind, int val) {
        ind += n;
        tree[ind] = val;
        ind >>= 1;
        while (ind > 0) {
            tree[ind] = min(tree[2 * ind], tree[2 * ind + 1]);
            ind >>= 1;
        }
    }
    int get(int x) {
        // return the first index i, such that s[i] < x
        int node = 1;
        while (node < n) {
            int left = (node << 1);
            int right = (node << 1) + 1;
            if (tree[left] < x) {
                node = left;
            } else {
                node = right;
            }
        }
        return (node - n);
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    int q;
    cin >> q;
    vector<vector<pair<int, int>>> queries(n + 1);
    for (int i = 0; i < q; ++i) {
        int l, r;
        cin >> l >> r;
        queries[r].push_back({l, i});
    }

    vector<int> res(q);

    SegmentTree s(n + 1); // Make it size of max of a[i] to avoid RTE
    for (int i = 1; i <= n; ++i) {
        // set the last occurence of a[i] to i
        s.set(a[i], i);
        for (auto [l, ind] : queries[i]) {
            // find the smallest x, such that last occurence of x < l
            res[ind] = s.get(l);
        }
    }
    for (int elem : res) {
        cout << elem << '\n';
    }
    cout << '\n';

    return 0;
}