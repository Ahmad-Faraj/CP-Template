#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
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
using namespace __gnu_pbds;
using namespace std;
ll max(ll n1, ll n2) { return (n1 >= n2) ? n1 : n2; }
ll min(ll n1, ll n2) { return (n1 >= n2) ? n2 : n1; }
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}
const int N = 1e5 + 1;
vector<vector<int>> adj;
// tree canonical form
//! we assume here that you have the roots of the trees
// order O(nlogn)
string tree_canonical_form(int node, int par = -1)
{
    vector<string> children;
    for (auto child : adj[node])
    {
        if (child != par)
        {
            children.push_back(tree_canonical_form(child, node));
        }
    }
    sort(children.begin(), children.end());
    string ret = "(";
    for (auto child : children)
    {
        ret += child;
    }
    ret += ")";
    return ret;
}

// tree isomorphism
// if you don't have the roots of the trees
string get_node_representation(int node, vector<vector<string>> &child_representation)
{

    sort(child_representation[node].begin(), child_representation[node].end());
    string ret = "(";
    for (auto child : child_representation[node])
    {
        ret += child;
    }
    ret += ")";
    return ret;
}
string tree_canonical_form_without_root()
{
    int n = sz(adj);
    vector<int> degree(n);
    for (int i = 0; i < n; ++i)
    {
        degree[i] = sz(adj[i]);
    }
    queue<int> leafs;
    int remaining_nodes = n;
    for (int i = 0; i < n; ++i)
    {
        if (degree[i] <= 1)
        {
            leafs.push(i);
        }
    }
    vector<vector<string>> child_representation(n);
    while (remaining_nodes > 2)
    {
        int leafs_size = sz(leafs);
        while (leafs_size--)
        {
            int leaf = leafs.front();
            leafs.pop();
            --remaining_nodes;
            string node_representation = get_node_representation(leaf, child_representation);
            for (auto parent : adj[leaf])
            {
                child_representation[parent].push_back(node_representation);
                if (--degree[parent] == 1)
                {
                    leafs.push(parent);
                }
            }
        }
    }

    // remaining nodes are the roots
    int root1 = leafs.front();
    leafs.pop();
    int root2 = leafs.empty() ? -1 : leafs.front();
    string root1_representation = get_node_representation(root1, child_representation);
    if (root2 == -1)
    {
        return root1_representation;
    }
    string root2_representation = get_node_representation(root2, child_representation);
    // try root1 as root and root2 as leaf
    child_representation[root1].push_back(root2_representation);
    // try root2 as root and root1 as leaf
    child_representation[root2].push_back(root1_representation);
    return min(get_node_representation(root1, child_representation), get_node_representation(root2, child_representation));
}
void solve()
{
    int tree1, tree2;
    cin >> tree1 >> tree2;
    adj.resize(tree1 + 1);
    for (int i = 0; i < tree1 - 1; ++i)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    string tree1_canonical_form = tree_canonical_form_without_root();
    adj.clear();
    adj.resize(tree2 + 1);
    for (int i = 0; i < tree2 - 1; ++i)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    string tree2_canonical_form = tree_canonical_form_without_root();
    cout << tree1_canonical_form << sp << tree2_canonical_form << nl;
    if (tree1_canonical_form == tree2_canonical_form)
    {
        cout << "YES\n";
    }
    else
    {
        cout << "NO\n";
    }
}
int main(void)
{
    fastio();
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i++<<": ";
        solve();
    }
    return 0;
}