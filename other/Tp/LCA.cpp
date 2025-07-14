#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define PI acos(-1)
#define all(x) x.begin(), x.end()
#define clr(v, d) memset(v, d, sizeof(v))
#define endl "\n"
#define sz(x) (int)x.size()
#define vl vector<ll>
#define pb push_back
#define mod 1000000007

void M7MDAN_FARAG() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}

const ll N = 1e5 + 5;

template <typename T = int>
struct LCA {

    int N, LOG;
    vector<vector<T>> anc;
    vector<vector<pair<T, T>>> adj;
    vector<int> dep;

    LCA(int n = 0) {
        N = n + 10, LOG = 0;
        while ((1 << LOG) <= N) LOG++;
        dep = vector<int>(N);
        adj = vector<vector<pair<T, T>>>(N);
        anc = vector<vector<T>>(N, vector<T>(LOG));
    }

    void add_edge(int u, int v) {
        adj[u].push_back({v, 0});
        adj[v].push_back({u, 0});
    }

    void build_adj(int edges) {
        for (int i = 0, u, v; i < edges && cin >> u >> v; i++)
            add_edge(u, v);
    }

    void dfs(int u, int p = 0) {
        for (auto& edge : adj[u]) {
            int v = edge.first;
            if (v == p) continue;
            dep[v] = dep[u] + 1, anc[v][0] = u;
            for (int bit = 1; bit < LOG; bit++)
                anc[v][bit] = anc[anc[v][bit - 1]][bit - 1];
            dfs(v, u);
        }
    }

    int kth_ancestor(int u, int k) {
        if (dep[u] < k)
            return -1;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (k & (1 << bit))
                u = anc[u][bit];
        return u;
    }

    int get_lca(int u, int v) {
        if (dep[u] < dep[v])
            swap(u, v);
        u = kth_ancestor(u, dep[u] - dep[v]);
        if (u == v)
            return u;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (anc[u][bit] != anc[v][bit])
                u = anc[u][bit], v = anc[v][bit];
        return anc[u][0];
    }

    T query(int u, int v) {
        int lca = get_lca(u, v);
        return dep[u] + dep[v] - 2 * dep[lca];
    }

    void build(int root = 1) {
        dfs(root);
    }
};

vector<vector<pair<ll, ll>>> adj(N);
vl fools(N), roads(N);

void dfss(ll node, ll par = -1) {
    for (auto child : adj[node]) {
        ll v = child.first, idx = child.second;
        if (v == par) continue;
        dfss(v, node);
        roads[node] += roads[v];
        fools[idx] += roads[v];
    }
}

void solve() {
ll n, k;
cin >> n;

LCA<ll> lca(n);

for(int i = 1; i < n; i++){
    ll u, v;
    cin >> u >> v;
    adj[u].pb({v, i});
    adj[v].pb({u, i});
}

lca.adj = adj;

lca.build();

cin >> k;

while(k--){
    ll u, v;
    cin >> u >> v;

    ll lca_node = lca.get_lca(u, v);

    roads[u]++;
    roads[v]++;
    roads[lca_node] -= 2;
}

dfss(1);

for(int i = 1; i < n; i++)
    cout << fools[i] << " ";

cout << endl;
}

signed main() {
    M7MDAN_FARAG();
    ll t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
}
