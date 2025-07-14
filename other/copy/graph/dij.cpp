#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#define int long long

#define ull unsigned long long int
#define ld long double
#define ll long long
#define PI acos(-1)
#define endl "\n"
#define sz(x) (int)x.size()
#define all(x) x.begin(), x.end()
#define clr(arr, val) memset(dp, -1, sizeof(dp))
#define fixed(n) cout << fixed << setprecision(n)
#define loop(i, n) for (int i = 0; i < (int)n; i++)
#define ordered_set tree<ll, null_type, less_equal<>, rb_tree_tag, tree_order_statistics_node_update>

const int N = 2e5 + 1;
const double EPS = 1e-6;
const int MOD = 1000000007;
const int OO = 0x3f3f3f3f;
const ll INF = LLONG_MAX;
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

void Ahmed_Faraj()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}

template <typename T = int>
istream &operator>>(istream &in, vector<T> &v)
{
    for (auto &x : v)
        in >> x;
    return in;
}

template <typename T = int>
ostream &operator<<(ostream &out, const vector<T> &v)
{
    for (const T &x : v)
        out << x << ' ';
    return out;
}

vector<pair<int, int>> adj[N];
vector<int> vis(N);
vector<int> ans(N, INF);
vector<int> path(N, -1);
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

void dij()
{
    ans[1] = 0;
    pq.push(make_pair(0, 1));

    while (!pq.empty())
    {
        auto [cost, node] = pq.top();
        pq.pop();

        if (vis[node])
            continue;
        vis[node] = 1;

        if (ans[node] < cost)
            continue;

        for (auto [u, c] : adj[node])
        {
            if (!vis[u] && cost + c < ans[u])
            {
                ans[u] = cost + c;
                pq.push({ans[u], u});
            }
        }
    }
}

void solve()
{
    int n;
    cin >> n;

    for (int i = 1; i < n; i++)
    {
        int a, b, x;
        cin >> a >> b >> x;
        adj[i].emplace_back(make_pair(i + 1, a));
        adj[i].emplace_back(make_pair(x, b));
    }

    dij();

    cout << ans[n] << endl;
}

signed main()
{
    Ahmed_Faraj();
    int test = 1;
    // cin >> test;
    for (int tc = 1; tc <= test; tc++)
    {
        // cout << "Case #" << tc << ": ";
        solve();
    }
    return 0;
}
