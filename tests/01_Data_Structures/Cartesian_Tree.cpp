// Test for 01_Data_Structures/Cartesian_Tree.cpp against a recursive O(n^2) build by arg-max.
// Every public member is checked: the constructor (max-heap and min-heap), root, l, r, parent, n,
// for T = int and T = ll with duplicates and negatives, plus the RMQ-as-LCA property.
#include "../../01_Data_Structures/Cartesian_Tree.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(17);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

// oracle: the root of a[lo..hi] (1-based) is the EARLIEST best value; recurse on both sides
template <typename T>
int build(const vector<T> &a, int lo, int hi, bool mx, vector<int> &L, vector<int> &R,
          vector<int> &P, int par) {
    if (lo > hi) return 0;
    int m = lo;
    for (int i = lo + 1; i <= hi; i++)
        if (mx ? a[i - 1] > a[m - 1] : a[i - 1] < a[m - 1]) m = i;
    P[m] = par;
    L[m] = build(a, lo, m - 1, mx, L, R, P, m);
    R[m] = build(a, m + 1, hi, mx, L, R, P, m);
    return m;
}

template <typename T> void one(const vector<T> &a, bool mx, const string &tag) {
    int n = a.size();
    vector<int> L(n + 1, 0), R(n + 1, 0), P(n + 1, 0);
    int root = build(a, 1, n, mx, L, R, P, 0);
    Cartesian_Tree<T> ct = mx ? Cartesian_Tree<T>(a) : Cartesian_Tree<T>(a, false);
    chk(tag + "n", ct.n == n);
    chk(tag + "root", ct.root == root);
    bool okl = true, okr = true, okp = true;
    for (int i = 1; i <= n; i++)
        okl &= ct.l[i] == L[i], okr &= ct.r[i] == R[i], okp &= ct.parent[i] == P[i];
    chk(tag + "l", okl);
    chk(tag + "r", okr);
    chk(tag + "parent", okp);
    if (n) { // lca(i, j) is the position of the best value of a[i..j] (earliest on ties)
        int i = 1 + rng() % n, j = 1 + rng() % n;
        if (i > j) swap(i, j);
        set<int> anc;
        for (int u = i; u; u = ct.parent[u]) anc.insert(u);
        int u = j;
        while (!anc.count(u)) u = ct.parent[u];
        int m = i;
        for (int k = i + 1; k <= j; k++)
            if (mx ? a[k - 1] > a[m - 1] : a[k - 1] < a[m - 1]) m = k;
        chk(tag + "lca=rmq", u == m);
    }
}

int main() {
    for (int it = 0; it < 60000; it++) {
        int n = rng() % 14;
        int range = vector<int>{1, 2, 3, 10, 1000000000}[rng() % 5];
        vector<int> a(n);
        for (auto &x : a) x = (int)(rng() % range) - (rng() % 3 == 0 ? range / 2 : 0);
        one(a, true, "int,max:");
        one(a, false, "int,min:");
        vector<long long> b(n);
        for (auto &x : b) x = (long long)(rng() % 7) * (long long)1e15 - 3 * (long long)1e15;
        one(b, true, "ll,max:");
        one(b, false, "ll,min:");
    }
    // sorted / all-equal arrays: chains of depth n
    for (int n : {1, 2, 50}) {
        vector<int> inc(n), eq(n, 5);
        iota(inc.begin(), inc.end(), 0);
        one(inc, true, "chain:");
        one(inc, false, "chain:");
        one(eq, true, "chain:");
        one(eq, false, "chain:");
    }
    {
        int n = 2000000;
        vector<int> a(n);
        for (auto &x : a) x = rng() % 1000;
        auto t0 = clock();
        Cartesian_Tree<int> ct(a);
        Cartesian_Tree<int> ct2(a, false);
        printf("n=2e6 max-heap + min-heap build: %.3fs\n", double(clock() - t0) / CLOCKS_PER_SEC);
        // every node but the root has exactly one parent
        vector<int> deg(n + 1, 0);
        for (int i = 1; i <= n; i++) deg[ct.parent[i]]++;
        chk("large:one_root", deg[0] == 1 && ct.parent[ct.root] == 0);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
