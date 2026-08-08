#include <bits/stdc++.h>
using namespace std;

#define int long long
const int mod = 1e9 + 7;
const int base = 313;

/*
 * Segment Tree for Polynomial Rolling Hash (String Hashing) with Lazy Propagation.
 *
 * - 1-based Range Indexing: Updateates and queries expect ranges from 1 to N.
 * - 1-based Tree Nodes Indexing: The root starts at 1, with children at 2*i and 2*i+1.
 * - 0-based String Indexing: The input string is processed correctly starting from index 0.
 * - Note: This implementation assumes `#define int long long` is used globally to prevent overflow.
 */
struct segtree
{
      vector<int> tree, lazy, len;
      vector<int> pw, prefix;
      int sz;

      segtree(int n)
      {
            pw = prefix = len = tree = vector<int>(4 * n, 0);
            lazy = vector<int>(4 * n, -1);
            sz = n;
            prefix[0] = pw[0] = 1;
            for (int i = 1; i < 4 * n; i++)
            {
                  pw[i] = (pw[i - 1] * base) % mod;
                  prefix[i] += pw[i] + prefix[i - 1];
                  prefix[i] %= mod;
            }
      }

      void propagate(int x, int lx, int rx)
      {
            if (lazy[x] == -1)
                  return;
            if (lx != rx)
            {
                  lazy[2 * x] = lazy[2 * x + 1] = lazy[x];
            }
            tree[x] = lazy[x] * prefix[rx - lx] % mod;
            lazy[x] = -1;
      }

      void merge(int x, int lx, int rx)
      {
            tree[x] = (tree[2 * x] * pw[len[2 * x + 1]] % mod + tree[2 * x + 1]) % mod;
            len[x] = len[2 * x] + len[2 * x + 1];
      }

      void build(string &s, int lx, int rx, int x)
      {
            lazy[x] = -1;
            if (lx == rx)
            {
                  tree[x] = s[lx - 1] % mod;
                  len[x] = 1;
                  return;
            }
            int mid = (lx + rx) / 2;
            build(s, lx, mid, 2 * x);
            build(s, mid + 1, rx, 2 * x + 1);
            merge(x, lx, rx);
      }
      void build(string &s)
      {
            build(s, 1, sz, 1);
      }

      void update(int l, int r, char c, int lx, int rx, int x)
      {
            propagate(x, lx, rx);
            if (lx > r || rx < l)
                  return;
            if (lx >= l && rx <= r)
            {
                  lazy[x] = c % mod;
                  propagate(x, lx, rx);
                  return;
            }
            int mid = (lx + rx) / 2;
            update(l, r, c, lx, mid, 2 * x);
            update(l, r, c, mid + 1, rx, 2 * x + 1);
            merge(x, lx, rx);
      }

      void update(int l, int r, char c)
      {
            update(l, r, c, 1, sz, 1);
      }

      pair<int, int> query(int l, int r, int lx, int rx, int x)
      {
            propagate(x, lx, rx);
            if (lx > r || rx < l)
                  return {0, 0};
            if (lx >= l && rx <= r)
            {
                  return {tree[x], len[x]};
            }
            int mid = (lx + rx) / 2;
            auto le = query(l, r, lx, mid, 2 * x);
            auto ri = query(l, r, mid + 1, rx, 2 * x + 1);

            // .second represents the length. If length is 0, return the other side.
            if (!le.second)
                  return ri;
            if (!ri.second)
                  return le;

            // Merge hash using .first (hash) and .second (length)
            return {(le.first * pw[ri.second] % mod + ri.first) % mod, le.second + ri.second};
      }

      int query(int l, int r)
      {
            if (l > r)
                  return 0;
            return query(l, r, 1, sz, 1).first;
      }
};

void solve()
{
      int n, q;
      cin >> n >> q;
      string s;
      cin >> s;
      segtree seg(n);
      seg.build(s);

      while (q--)
      {
            string type;
            cin >> type;
            int l, r;
            char c;
            if (type == "QUERY")
            {
                  cin >> l >> r;
                  if (l > r)
                        swap(l, r);
                  if (r - l + 1 == 1)
                        cout << "YES\n";
                  else if (r - l + 1 == 2)
                        cout << (seg.query(l, l) != seg.query(r, r) ? "YES\n" : "NO\n");
                  else
                  {
                        if (seg.query(l, r - 2) == seg.query(l + 2, r) && seg.query(l, r - 1) != seg.query(l + 1, r))
                              cout << "YES\n";
                        else
                              cout << "NO\n";
                  }
            }
            else
            {
                  cin >> l >> r >> c;
                  if (l > r)
                        swap(l, r);
                  seg.update(l, r, c);
            }
      }
}
signed main()
{

      ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
      freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif

      int t = 1;
      cin >> t;
      while (t--)
      {
            solve();
            cout << "\n";
      }
}