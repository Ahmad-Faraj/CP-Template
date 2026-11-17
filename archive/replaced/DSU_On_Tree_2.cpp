#include <bits/stdc++.h>

using namespace std;

const int MAX_N = 2e5;

// nodes will be 1-indexed like in the problem
vector<int> adj[MAX_N + 1];

set<int> colors[MAX_N + 1];
int distinct_num[MAX_N + 1];

void dsu_on_trees(int node, int par)
{
    for (int child : adj[node])
    {
        if (child != par)
        {
            dsu_on_trees(child, node);
            // make node the larger set always
            if (colors[node].size() < colors[child].size())
            {
                swap(colors[node], colors[child]);
            }
            for (int item : colors[child])
            {
                colors[node].insert(item);
            }
        }
    }
    distinct_num[node] = colors[node].size();
}

int main()
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        int a;
        cin >> a;
        colors[i].insert(a);
    }
    for (int i = 1; i < n; i++)
    {
        int a;
        int b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    dsu_on_trees(1, 0);
    for (int i = 1; i <= n; i++)
    {
        cout << distinct_num[i] << (i < n ? " " : "\n");
    }
}