# Hashing

### **1. Multiset / Subarray Hashing (Zobrist-like)**
Used to check if the multiset of values in subarray `A[l...r]` is identical to the multiset of values in `B[L...R]` in $\mathcal{O}(1)$ time after $\mathcal{O}(N)$ preprocessing.
*   **Approach:** Assign each unique value $x$ a completely random vector of $K$ large integers. The hash of a multiset is simply the element-wise sum of the vectors of its elements.
*   **Prefix Sums:** Compute prefix sums of these vectors. The multiset hash of subarray `[l...r]` is `Prefix[r] - Prefix[l-1]`.
*   *(Note: 5 random 64-bit integers are used here to completely eliminate collision probability).*

```cpp
#include <bits/stdc++.h>
using namespace std;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
inline long long random_val(long long l, long long r) {
    return rnd() % (r - l + 1) + l;
}

struct MultisetNode {
    long long a, b, c, d, e;
    MultisetNode() {
        a = random_val(1, 1e9);
        b = random_val(1, 1e9);
        c = random_val(1, 1e9);
        d = random_val(1, 1e9);
        e = random_val(1, 1e9);
    }
    MultisetNode(long long _a, long long _b, long long _c, long long _d, long long _e) 
        : a(_a), b(_b), c(_c), d(_d), e(_e) {}
    
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

// Example usage checking A[l..r] == B[L..R]
void solve() {
    int n, q;
    cin >> n >> q;
    vector<MultisetNode> A(n + 1, MultisetNode(0, 0, 0, 0, 0));
    vector<MultisetNode> B(n + 1, MultisetNode(0, 0, 0, 0, 0));
    map<long long, MultisetNode> mp;

    for (int i = 1; i <= n; i++) {
        long long x; cin >> x;
        if (mp.find(x) == mp.end()) mp[x] = MultisetNode();
        A[i] = A[i - 1] + mp[x];
    }
    for (int i = 1; i <= n; i++) {
        long long x; cin >> x;
        if (mp.find(x) == mp.end()) mp[x] = MultisetNode();
        B[i] = B[i - 1] + mp[x];
    }
    
    while (q--) {
        int l, r, L, R;
        cin >> l >> r >> L >> R;
        if (A[r] - A[l - 1] == B[R] - B[L - 1]) cout << "YES\n";
        else cout << "NO\n";
    }
}
```
