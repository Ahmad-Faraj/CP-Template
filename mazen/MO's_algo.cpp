#include<bits/stdc++.h>
#define ll long long
using namespace std;

const int N = 200000 + 5;
const int SQ = 450;
struct Query
{
    int l, r, q_idx, blk_idx;
    Query(){}
    Query(int l, int r, int q_idx)
    {
        this->l = l;
        this->r = r;
        this->q_idx = q_idx;
        blk_idx = l / SQ;
    }
    bool operator<(const Query &other) const
    {
        if (blk_idx != other.blk_idx)
            return blk_idx < other.blk_idx;
        return r < other.r;
    }
};
ll n, q, arr[N], vis[1000006], ans[200005], res = 0;
Query query[200005];
void add(int idx)
{
    res -= (vis[arr[idx]] * vis[arr[idx]] * arr[idx]);
    vis[arr[idx]]++;
    res += (vis[arr[idx]] * vis[arr[idx]] * arr[idx]);
}
void remove(int idx)
{
    res -= (vis[arr[idx]] * vis[arr[idx]] * arr[idx]);
    vis[arr[idx]]--;
    res += (vis[arr[idx]] * vis[arr[idx]] * arr[idx]);
}
void procces(){
    sort(query, query + q);
    int l = 1, r = 0;
    for (int i = 0; i < q; i++){
        while(l < query[i].l)
            remove(l++);
        while(l > query[i].l)
            add(--l);
        while(r < query[i].r)
            add(++r);
        while(r > query[i].r)
            remove(r--);
        ans[query[i].q_idx] = res;
    }
}
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}
void solve(int tc){
    cin >> n >> q;
    for (int i = 0; i < n; i++)
        cin >> arr[i];
    for (int i = 0; i < q; i++){
        int l, r;
        cin >> l >> r;
        query[i] = Query(--l, --r, i); // 0 based indexing
    }
    procces();
    for (int i = 0; i < q; i++)
        cout << ans[i] << "\n";
}
int main()
{
    fastio();
    int t = 1;
    //cin >> t;
    int i = 1;
    while(t--){
        solve(i++);
    }
    return 0;
}