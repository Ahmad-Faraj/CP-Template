#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define ull unsigned long long
#define ld long double
#define nl "\n"
#define oo 1e9 + 1
#define sp ' '
#define sz(x) int(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)

void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}

const int N = 1e5 + 5;
vector<vector<int>> adj;
// tree diameter
// diameter either pass with me (sum of 2 highest sub-trees) or don't, then diameter pass with a children
// you can't know which two nodes are the diameter endpoints, so you need to midify the algorithm
// return pair of (diameter, height)
pair<int, int> diameter(int node, int parent = -1)
{
    int diam = 0;
    int max_height[3] = {-1, -1, -1};
    for (auto child : adj[node])
    {
        if (child == parent)
            continue;
        auto [d, h] = diameter(child, node);
        diam = max(diam, d);
        max_height[0] = h + 1; // if it's weighted tree, add weight of edge
        sort(max_height, max_height + 3);
    }

    for (auto &i : max_height)
        i = max(i, 0);

    diam = max(diam, max_height[1] + max_height[2]);
    return {diam, max_height[2]};
}

// another way to find diameter using BFS
// first BFS to find the farthest node from any node
// second BFS to find the farthest node from the farthest node
// the distance between them is the diameter
// O(n)
int diameter_bfs(int node)
{
    vector<int> dist(sz(adj), -1);
    queue<int> q;
    q.push(node);
    dist[node] = 0;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto v : adj[u])
        {
            if (dist[v] == -1)
            {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    int farthest_node = -1, max_dist = -1;
    for (int i = 0; i < sz(adj); i++)
    {
        if (dist[i] > max_dist)
        {
            max_dist = dist[i];
            farthest_node = i;
        }
    }



    dist.assign(sz(adj), -1);
    q.push(farthest_node);
    dist[farthest_node] = 0;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto v : adj[u])
        {
            if (dist[v] == -1)
            {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    int diameter = -1;
    for (int i = 0; i < sz(adj); i++)
        diameter = max(diameter, dist[i]);
    return diameter;
}

// on other side, we can get the worst nodes throught the center nodes.
// A worst node is noe that is an endpoint of the diameter, so it has the worst tree height
int main(void)
{
    fastio();
    int n;
    cin >> n;
    adj.resize(n + 1);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v), adj[v].push_back(u);
    }
    cout << diameter_bfs(1) << nl;
    return 0;
}
