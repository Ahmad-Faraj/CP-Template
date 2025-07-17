# Subarray Hashing

## Problem Statement

Given two arrays `A` and `B` and `q` queries, each query consists of four integers $l_i$ $r_i$ $L_i$ $R_i$ such that $1 \leq l_i \leq r_i \leq n$ and $1 \leq L_i \leq R_i \leq n$. For each query, we need to check if the multiset of the subarray `A[l_i...r_i]` is equal to the multiset of the subarray `B[L_i...R_i]`.

## Approach

Give each value five random numbers, and make five prefix sums over each value and only compare the sum of the subarrays.

## Implementation

```cpp
#include <bits/stdc++.h>
using namespace std;
#define int long long
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
inline int random(int l, int r){
    return rnd() % (r - l + 1) + l;
}

struct MultisetNode {
    ll a, b, c, d, e;
    MultisetNode() {
        a = random(1, 1e9);
        b = random(1, 1e9);
        c = random(1, 1e9);
        d = random(1, 1e9);
        e = random(1, 1e9);
    }
    MultisetNode (ll _a, ll _b, ll _c, ll _d, ll _e) : a(_a), b(_b), c(_c), d(_d), e(_e) {}
    MultisetNode operator+(const MultisetNode &other) const {
        return MultisetNode(a + other.a, b + other.b, c + other.c, d + other.d, e + other.e);
    }
    MultisetNode operator-(const MultisetNode &other) const {
        return MultisetNode(a - other.a, b - other.b, c - other.c, d - other.d, e - other.e);
    }
    bool operator==(const MultisetNode &other) const {
        return a == other.a && b == other.b && c == other.c && d == other.d && e == other.e;
    }
};
void solve(){
    int n, q;
    cin >> n >> q;
    vector<MultisetNode> A(n + 1), B(n + 1);
    A[0] = B[0] = MultisetNode(0, 0, 0, 0, 0);
    map<ll, MultisetNode> mp;
    for(int i = 1; i <= n; i++){
        ll x;
        cin >> x;
        if(mp.find(x) == mp.end()){
            mp[x] = MultisetNode();
        }
        A[i] = A[i - 1] + mp[x];
    }
    for(int i = 1; i <= n; i++){
        ll x;
        cin >> x;
        if(mp.find(x) == mp.end()){
            mp[x] = MultisetNode();
        }
        B[i] = B[i - 1] + mp[x];
    }
    while(q--){
        int l, r, L, R;
        cin >> l >> r >> L >> R;
        if(A[r] - A[l - 1] == B[R] - B[L - 1]){
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
}
```
