#include <bits/stdc++.h>
using namespace std;

// Assignment of a direction to each undirected edge to be a strongly connected graph
// this cannot be done to every graph, A bridge will make the graph fails

vector<vector<pair<int, int>>> adj;
vector<pair<int, int>> edges;

vector<int> tin, low;
int bridge_cnt;
string orient;
vector<bool> edge_used;

void find_bridges(int v) {
    static int time = 0;
    low[v] = tin[v] = time++;
    for (auto p : adj[v]) {
        if (edge_used[p.second]) continue;
        edge_used[p.second] = true;
        orient[p.second] = v == edges[p.second].first ? '>' : '<';
        int nv = p.first;
        if (tin[nv] == -1) {
            find_bridges(nv);
            low[v] = min(low[v], low[nv]);
            if (low[nv] > tin[v]) {
                bridge_cnt++;
            }
        } else {
            low[v] = min(low[v], tin[nv]);
        }
    }
}

signed main() {
    int n, m;
    cin >> n >> m;
    adj.resize(n);
    tin.resize(n, -1);
    low.resize(n, -1);
    orient.resize(m);
    edges.resize(m);
    edge_used.resize(m);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        adj[a].push_back({b, i});
        adj[b].push_back({a, i});
        edges[i] = {a, b};
    }
    int comp_cnt = 0;
    for (int v = 0; v < n; v++) {
        if (tin[v] == -1) {
            comp_cnt++;
            find_bridges(v);
        }
    }
    // finding a graph orientation so that the number of SCCs is minimal
    // > — left to the right (as in the input)
    // < — the opposite.
    cout << comp_cnt + bridge_cnt << endl << orient << endl;
}
