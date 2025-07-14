#include <bits/stdc++.h>
using namespace std;
#define ll long long
const int N = 2e5 + 5, SQ = 448;
ll arr[N], blk[SQ], n, q;
vector<vector<ll>> blocks(SQ);

void process()
{
    for (int i = 0; i < n; i++)
    {
        blocks[i / SQ].push_back(arr[i]);
        blk[i / SQ] += arr[i];
    }
}

void update(int idx, ll val)
{
    blk[idx / SQ] -= arr[idx];
    arr[idx] = val;
    blk[idx / SQ] += arr[idx];
    blocks[idx / SQ][idx % SQ] = arr[idx];
}

ll query(int l, int r)
{
    ll ret = 0;
    while (l <= r)
    {
        if (l % SQ == 0 && l + SQ <= r)
        {
            ret += blk[l / SQ];
            l += SQ;
        }
        else
        {
            ret += arr[l];
            l++;
        }
    }
    return ret;
}
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}

int main()
{
    fastio();
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    cin >> n >> q;
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }
    process();
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int idx;
            ll val;
            cin >> idx >> val;
            update(--idx, val);
        }
        else
        {
            int l, r;
            cin >> l >> r;
            cout << query(--l, --r) << '\n';
        }
    }
}