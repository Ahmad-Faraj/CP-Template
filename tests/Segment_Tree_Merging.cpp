// Brute-force test for:
//   Range_Query/Segment_Tree/Segment_Tree_Merging.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace SM {
#include "../Range_Query/Segment_Tree/Segment_Tree_Merging.cpp"
}
int main() {
    mt19937_64 gen(5150);
    int cntFail = 0, kthFail = 0, freqFail = 0, mergeFail = 0, treeFail = 0, delFail = 0, cases = 0;

    // ---- one tree at a time: counts, kth, most frequent, against a plain multiset
    for (int it = 0; it < 4000; it++) {
        int V = 1 + (int)(gen() % 10), ops = 1 + (int)(gen() % 15);
        SM::Segment_Tree_Merging st(V, 0); // reserve 0 on purpose: the pool reallocates constantly
        int root = 0;
        multiset<int> ref;
        cases++;
        for (int o = 0; o < ops; o++) {
            int v = 1 + (int)(gen() % V);
            if (!ref.empty() && gen() % 4 == 0) { // deletions too
                auto it2 = ref.find(v);
                if (it2 != ref.end()) { ref.erase(it2); root = st.update(root, v, -1); }
            } else {
                ref.insert(v);
                root = st.update(root, v, 1);
            }
            if (st.size(root) != (int)ref.size()) delFail++;
            for (int l = 1; l <= V; l++)
                for (int r = l; r <= V; r++) {
                    int want = 0;
                    for (int x : ref) want += (x >= l && x <= r);
                    if (st.count(root, l, r) != want) cntFail++;
                }
            vector<int> sorted(ref.begin(), ref.end());
            for (int k = 1; k <= (int)sorted.size() + 1; k++) {
                int got = st.kth(root, k);
                int want = k <= (int)sorted.size() ? sorted[k - 1] : -1;
                if (got != want) kthFail++;
            }
            int best = 0; ll bsum = 0;
            for (int x = 1; x <= V; x++) { int c = (int)ref.count(x); if (c > best) best = c, bsum = x; else if (c == best && c > 0) bsum += x; }
            if (best == 0) bsum = 0;
            if (st.max_frequency(root) != best || st.frequent_sum(root) != bsum) freqFail++;
        }
    }

    // ---- merging: many trees folded together must equal the union
    for (int it = 0; it < 3000; it++) {
        int V = 1 + (int)(gen() % 12), parts = 1 + (int)(gen() % 5);
        SM::Segment_Tree_Merging st(V, 0);
        vector<int> roots(parts, 0);
        multiset<int> all;
        for (int p = 0; p < parts; p++)
            for (int j = 0, c = (int)(gen() % 5); j < c; j++) {
                int v = 1 + (int)(gen() % V);
                roots[p] = st.update(roots[p], v, 1);
                all.insert(v);
            }
        int r = 0;
        for (int p = 0; p < parts; p++) r = st.merge(r, roots[p]);
        if (st.size(r) != (int)all.size()) mergeFail++;
        for (int x = 1; x <= V; x++) if (st.count(r, x, x) != (int)all.count(x)) mergeFail++;
        vector<int> sorted(all.begin(), all.end());
        for (int k = 1; k <= (int)sorted.size(); k++) if (st.kth(r, k) != sorted[k - 1]) mergeFail++;
        int best = 0; ll bsum = 0;
        for (int x = 1; x <= V; x++) { int c = (int)all.count(x); if (c > best) best = c, bsum = x; else if (c == best && c > 0) bsum += x; }
        if (best == 0) bsum = 0;
        if (st.max_frequency(r) != best || st.frequent_sum(r) != bsum) mergeFail++;
    }

    // ---- the payload: CF 600E, against an O(n^2) per-subtree recount
    for (int it = 0; it < 2000; it++) {
        int n = 1 + (int)(gen() % 10);
        vector<int> colour(n + 1), parent(n + 1, 0);
        vector<vector<int>> adj(n + 1);
        for (int v = 1; v <= n; v++) colour[v] = 1 + (int)(gen() % n);
        for (int v = 2; v <= n; v++) { parent[v] = 1 + (int)(gen() % (v - 1)); adj[parent[v]].push_back(v); }
        SM::Segment_Tree_Merging st(n, 0);
        vector<int> root(n + 1, 0);
        vector<ll> got(n + 1);
        for (int u = n; u >= 1; u--) { // children have larger ids than parents here
            root[u] = st.update(root[u], colour[u], 1);
            got[u] = st.frequent_sum(root[u]);
            if (parent[u]) root[parent[u]] = st.merge(root[parent[u]], root[u]);
        }
        for (int u = 1; u <= n; u++) {
            map<int, int> cnt; // brute force: walk the whole subtree
            vector<int> st2{u};
            while (!st2.empty()) { int x = st2.back(); st2.pop_back(); cnt[colour[x]]++; for (int c : adj[x]) st2.push_back(c); }
            int best = 0; ll want = 0;
            for (auto [c, k] : cnt) { if (k > best) best = k, want = c; else if (k == best) want += c; }
            if (got[u] != want) treeFail++;
        }
    }

    printf("Segment_Tree_Merging  (%d random multisets, pool reserve 0)\n", cases);
    printf("  count(l, r) vs multiset    : %d failures\n", cntFail);
    printf("  kth, incl. k out of range  : %d failures\n", kthFail);
    printf("  size after deletions       : %d failures\n", delFail);
    printf("  most frequent + ties       : %d failures\n", freqFail);
    printf("  merge == union of parts    : %d failures\n", mergeFail);
    printf("  CF 600E vs subtree recount : %d failures\n", treeFail);
}
