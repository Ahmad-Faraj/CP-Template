#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}

template <typename T = int, bool VAL_ON_EDGE = false>
class MoTree
{
public:
    struct Query
    {
        int l, r, k, lca, queryIdx;
        int64_t ord;

        Query(vector<T> &S, vector<T> &E, int L = 0, int R = 0, int QueryIdx = 0, int LCA = 0, int HilbertPow = 0)
        {
            if (S[L] > S[R])
                swap(L, R);
            if (LCA == L)
                l = S[L] + VAL_ON_EDGE, r = S[R], lca = -1, queryIdx = QueryIdx;
            else
                l = E[L], r = S[R], lca = LCA, queryIdx = QueryIdx;
            calcOrder(HilbertPow);
        }

        void calcOrder(int hilbert_pow)
        {
            ord = MoTree::hilbertOrder(l, r, hilbert_pow, 0);
        }

        bool operator<(const Query &rhs) const
        {
            return ord < rhs.ord;
        }
    };

    MoTree(int N, int M, vector<vector<pair<int, int>>> &G, vector<T> V = vector<T>(), int root = 1) : curr_l(1), curr_r(0), n(N), m(M), SqrtN(n / sqrt(m) + 1), timer(1), ans(0), answers(M), val(V), adj(G)
    {
        LOG = calcLog(N);
        helbertPow = calcHilbertPow(2 * N + 1);
        nodeFreq = S = E = dep = vector<int>(n + 5);
        FT = vector<int>(2 * n + 5);
        val = vector<T>(n + 5);
        anc = vector<vector<int>>(n + 5, vector<int>(LOG));
        dfs(root);
    }

    static inline int64_t hilbertOrder(int x, int y, int pow, int rotate)
    {
        if (pow == 0)
            return 0;
        int hpow = 1 << (pow - 1);
        int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
        seg = (seg + rotate) & 3;
        const int rotateDelta[4] = {3, 0, 0, 1};
        int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
        int nrot = (rotate + rotateDelta[seg]) & 3;
        int64_t subSquareSize = int64_t(1) << (2 * pow - 2);
        int64_t ordd = seg * subSquareSize;
        int64_t add = hilbertOrder(nx, ny, pow - 1, nrot);
        ordd += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
        return ordd;
    }

    void getData()
    {
        for (int i = 0, u, v; i < m && cin >> u >> v; i++)
            queries.emplace_back(S, E, u, v, i, getLCA(u, v), helbertPow);
        process();
    }

    void process()
    {
        sort(queries.begin(), queries.end());
        // for (auto &i : FT)
        //     cout << i << sp;
        // cout << nl;
        // for (int i = 1; i <= n; i++)
        //     cout << val[i] << sp << S[i] << sp << E[i] << nl;
        // cout << nl;
        // start with the first query
        curr_l = queries[0].l, curr_r = queries[0].l - 1;

        for (auto &q : queries)
        {
            // cout << q.l << sp << q.r << sp << q.lca << sp << q.queryIdx << nl;
            setRange(q);

            // if lca is -1 then the two nodes are in the same subtree
            if (~q.lca && !VAL_ON_EDGE)
                add(q.lca);

            answers[q.queryIdx] = ans;

            if (~q.lca && !VAL_ON_EDGE)
                remove(q.lca);
        }
    }

    vector<T> getAnswers() const
    {
        return answers;
    }
    MEX<int> Mex;
private:
    int curr_l, curr_r, n, m, SqrtN, timer, LOG, helbertPow;
    T ans;
    vector<T> answers, val;
    vector<int> dep, S, E, FT, nodeFreq;
    vector<vector<pair<int, int>>> adj;
    vector<vector<int>> anc;
    vector<Query> queries;
    void dfs(int u, int p = -1)
    {
        S[u] = timer;
        FT[timer++] = u;
        for (auto &[v, w] : adj[u])
        {
            if (v == p)
                continue;
            dep[v] = dep[u] + 1;
            anc[v][0] = u;
            for (int bit = 1; bit < LOG; bit++)
                anc[v][bit] = anc[anc[v][bit - 1]][bit - 1];
            val[v] = w;
            dfs(v, u);
        }
        E[u] = timer;
        FT[timer++] = u;
    }

    int kthAncestor(int u, int k) const
    {
        if (dep[u] < k)
            return -1;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (k & (1 << bit))
                u = anc[u][bit];
        return u;
    }

    int getLCA(int u, int v) const
    {
        if (dep[u] < dep[v])
            swap(u, v);
        u = kthAncestor(u, dep[u] - dep[v]);
        if (u == v)
            return u;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (anc[u][bit] != anc[v][bit])
                u = anc[u][bit], v = anc[v][bit];
        return anc[u][0];
    }

    void setRange(Query &q)
    {
        while (curr_l > q.l)
            operation(--curr_l);
        while (curr_r < q.r)
            operation(++curr_r);
        while (curr_l < q.l)
            operation(curr_l++);
        while (curr_r > q.r)
            operation(curr_r--);
    }
    
    inline void add(int u)
    {
        
    }

    inline void remove(int u)
    {
       
    }

    inline void operation(int idx)
    {
        int u = FT[idx];
        nodeFreq[u] ^= 1;
        if (nodeFreq[u] == 1)
        {
            // add u to the path
            add(u);
        }
        else
        {
            // remove u from the path
            remove(u);
        }
    }

    int calcLog(int max_n) const
    {
        int log = 0;
        while ((1 << log) <= max_n)
            log++;
        return log;
    }

    int calcHilbertPow(int max_n) const
    {
        int pow = 0;
        while ((1 << pow) < max_n)
            pow++;
        return pow;
    }
};

void solve(int tc)
{
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }
    MoTree<int, true> mo(n, m, adj);
    mo.getData();
    for (auto &i : mo.getAnswers())
        cout << i << nl;
    // cout << "Case #" << tc << ": ";
}
signed main(void)
{
    fastio();
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}