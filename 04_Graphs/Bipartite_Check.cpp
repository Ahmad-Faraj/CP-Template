#include <bits/stdc++.h>
using namespace std;
#define ll long long
const int MAX_N = 1e5 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;

int n, m, color[MAX_N];
vector<int> adj[MAX_N];

void bfs(int s) {
    queue<int> q;
    color[s] = 1;
    q.push(s);
    while (q.size()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (!color[v]) {
                color[v] = color[u] % 2 + 1;
                q.push(v);
            } else if (color[v] == color[u]) {
                cout << "IMPOSSIBLE\n";
                exit(0);
            }
        }
    }
}

void solve() {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 1; i <= n; i++)
        if (!color[i]) bfs(i);
    for (int i = 1; i <= n; i++) cout << color[i] << " ";
    cout << "\n";
}