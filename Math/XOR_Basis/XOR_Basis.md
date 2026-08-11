# XOR Basis (Standard) 

A linear algebra structure that computes the independent basis of a set of numbers over GF(2). 

## 1. Complete Advanced Function Directory

The following functions can be injected into your core `XorBasis` struct whenever needed.

### **Merging Two Bases**
```cpp
void merge(const XorBasis& other) {
    for (int i = 0; i < LOG; i++) {
        if (other.b[i]) insert(other.b[i]);
    }
}
```

### **Minimum XOR Generated**
Returns the absolute minimum XOR you can generate.
```cpp
int min_xor(int start = 0) const {
    int ans = start;
    for (int i = LOG - 1; i >= 0; i--) {
        ans = min(ans, ans ^ b[i]);
    }
    return ans;
}
```

### **Max XOR Bounded in Range $[min\_val, max\_val]$**
Finds the maximum XOR achievable that strictly lies within $[min\_val, max\_val]$. Requires a DFS branch search.
```cpp
int max_xor_bounded(int min_val, int max_val, int start = 0) const {
    int tmp[LOG]; memcpy(tmp, b, sizeof(tmp));
    for (int i = LOG - 1; i >= 0; i--) { // RREF
        if (!tmp[i]) continue;
        for (int j = i - 1; j >= 0; j--) {
            if (tmp[j] && ((tmp[i] >> j) & 1)) tmp[i] ^= tmp[j];
        }
    }
    vector<int> c;
    for (int i = LOG - 1; i >= 0; i--) if (tmp[i]) c.push_back(tmp[i]);

    int best = -1;
    auto dfs = [&](auto& self, int idx, int current_val) -> void {
        if (idx == c.size()) {
            if (current_val >= min_val && current_val <= max_val) best = max(best, current_val);
            return;
        }
        int max_reach = current_val, min_reach = current_val;
        for (int j = idx; j < c.size(); j++) {
            max_reach = max(max_reach, max_reach ^ c[j]);
            min_reach = min(min_reach, min_reach ^ c[j]);
        }
        if (max_reach < min_val || min_reach > max_val) return; // Pruning
        
        int v1 = current_val ^ c[idx], v2 = current_val;
        if (v1 > v2) {
            self(self, idx + 1, v1);
            if (best != -1) return;
            self(self, idx + 1, v2);
        } else {
            self(self, idx + 1, v2);
            if (best != -1) return;
            self(self, idx + 1, v1);
        }
    };
    dfs(dfs, 0, start);
    return best;
}

```
### **Min XOR Bounded in Range $[min\_val, max\_val]$**
Finds the minimum XOR achievable that strictly lies within $[min\_val, max\_val]$. Requires a DFS branch search.
```cpp
int min_xor_bounded(int min_val, int max_val, int start = 0) const {
    int tmp[LOG]; memcpy(tmp, b, sizeof(tmp));
    for (int i = LOG - 1; i >= 0; i--) { // RREF
        if (!tmp[i]) continue;
        for (int j = i - 1; j >= 0; j--) {
            if (tmp[j] && ((tmp[i] >> j) & 1)) tmp[i] ^= tmp[j];
        }
    }
    vector<int> c;
    for (int i = LOG - 1; i >= 0; i--) if (tmp[i]) c.push_back(tmp[i]);

    int best = -1;
    auto dfs = [&](auto& self, int idx, int current_val) -> void {
        if (idx == c.size()) {
            if (current_val >= min_val && current_val <= max_val) {
                best = (best == -1) ? current_val : min(best, current_val);
            }
            return;
        }
        int max_reach = current_val, min_reach = current_val;
        for (int j = idx; j < c.size(); j++) {
            max_reach = max(max_reach, max_reach ^ c[j]);
            min_reach = min(min_reach, min_reach ^ c[j]);
        }
        if (max_reach < min_val || min_reach > max_val) return; // Pruning
        
        int v1 = current_val ^ c[idx], v2 = current_val;
        if (v1 < v2) {
            self(self, idx + 1, v1);
            if (best != -1) return;
            self(self, idx + 1, v2);
        } else {
            self(self, idx + 1, v2);
            if (best != -1) return;
            self(self, idx + 1, v1);
        }
    };
    dfs(dfs, 0, start);
    return best;
}
```

### **$K$-th Smallest/Greatest UNIQUE XOR Generated**
Returns the $K$-th unique value. Converts to Reduced Row Echelon Form (RREF) first.
```cpp
int get_kth_smallest_xor(long long k) const {
    if (k <= 0) return -1;
    int tmp[LOG]; memcpy(tmp, b, sizeof(tmp));
    for (int i = LOG - 1; i >= 0; i--) { // Convert to RREF
        if (!tmp[i]) continue;
        for (int j = i - 1; j >= 0; j--) {
            if (tmp[j] && ((tmp[i] >> j) & 1)) tmp[i] ^= tmp[j];
        }
    }
    int rank = 0;
    for (int i = 0; i < LOG; i++) if (tmp[i]) rank++;
    if (rank < 63 && k > (1ULL << rank)) return -1;
    
    int ans = 0; k--;
    for (int i = 0; i < LOG; i++) {
        if (tmp[i] && (k & 1)) ans ^= tmp[i];
        k >>= 1;
    }
    return ans;
}

int get_kth_greater_xor(long long k) const {
    long long total = (rk < 63) ? (1ULL << rk) : (1ULL << 63);
    return get_kth_smallest_xor(total - k + 1);
}
```

### **Can Make / Count Ways to Make**
```cpp
bool can_make(int x, int total_inserted, bool exclude_empty_set = false) const {
    if (exclude_empty_set && x == 0) return total_inserted > rk;
    return reduce(x) == 0;
}

// Returns the number of subsets that XOR to exactly X
long long count_equal_to_x(int x, int total_inserted) const {
    if (!can_make(x, total_inserted, false)) return 0;
    return (1ULL << (total_inserted - rk)); // Assumes no overflow
}

// Total distinct numbers the basis can generate
long long get_span_of_Matrix() const {
    return (1ULL << rk);
}
```

---

## 2. CP Applications

### **1. Choose $A[i]$ or $0$ (Maximum XOR Subset)**
**Problem:** Given an array $A$, choose for each index either $A[i]$ or $0$ to maximize the XOR sum. This is identical to asking for the maximum XOR subset of $A$.
```cpp
int n; cin >> n;
XorBasis<62> basis;
for (int i = 0; i < n; i++) {
    int x; cin >> x;
    basis.insert(x); // Picking 0 mathematically means not picking x
}
cout << basis.max_xor(0) << "\n";
```

### **2. Choose EXACTLY ONE of $A[i]$ or $B[i]$**
**Problem:** For every index $i \le N$, you must select exactly one value (either $A[i]$ or $B[i]$). Maximize the total XOR sum.
```cpp
int n; cin >> n;
vector<int> a(n), b(n);
int base_xor = 0;
for (int i = 0; i < n; i++) {
    cin >> a[i];
    base_xor ^= a[i]; // Assume we pick A[i] for all i
}

XorBasis<62> basis;
for (int i = 0; i < n; i++) {
    cin >> b[i];
    // Swapping our choice from A[i] to B[i] changes the XOR sum by A[i] ^ B[i]
    basis.insert(a[i] ^ b[i]); 
}
// Maximize the base sum using the swap choices
cout << basis.max_xor(base_xor) << "\n";
```

### **3. Maximum XOR Subset from TWO Arrays (Merged Bases)**
**Problem:** Choose any subset from array $A$ and any subset from array $B$ to maximize the combined XOR sum.
```cpp
int n, m; cin >> n >> m;
XorBasis<62> A, B;
for (int i = 0; i < n; i++) { int x; cin >> x; A.insert(x); }
for (int i = 0; i < m; i++) { int x; cin >> x; B.insert(x); }

A.merge(B); // Combines both independent vector spaces
cout << A.max_xor(0) << "\n";
```
> [!WARNING]
> If a problem strictly asks to pick **exactly one** element from A and **exactly one** element from B (total 2 elements), this is a **Binary Trie** problem, NOT an XOR Basis problem!

### **4. Maximum XOR Path in a Graph (Cycles & DFS)**
**Problem:** Find the maximum XOR path from node $1$ to node $N$ in an undirected graph where cycles are allowed.
```cpp
vector<pair<int, int>> adj[N];
int dist[N];
bool vis[N];
XorBasis<62> basis;

void dfs(int u, int p, int current_xor) {
    vis[u] = true;
    dist[u] = current_xor;
    for (auto& edge : adj[u]) {
        int v = edge.first, w = edge.second;
        if (v == p) continue;
        if (vis[v]) {
            // Found a cycle! Cost to enter/exit cancels out, leaving only cycle weight
            basis.insert(current_xor ^ w ^ dist[v]); 
        } else {
            dfs(v, u, current_xor ^ w);
        }
    }
}

int n, m; cin >> n >> m;
for (int i = 0; i < m; i++) {
    int u, v, w; cin >> u >> v >> w;
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
}
dfs(1, 0, 0); // Find an arbitrary path to N and collect all cycles
cout << basis.max_xor(dist[n]) << "\n";
```

### **5. Nim Game & Game Theory**
**Problem:** You are playing Nim. Find how many subsets of piles result in a winning state (XOR sum > 0).
```cpp
int n; cin >> n;
XorBasis<62> basis;
for (int i = 0; i < n; i++) {
    int piles; cin >> piles;
    basis.insert(piles);
}
// Number of subsets resulting in a winning state (XOR > 0)
long long total_subsets = (1LL << n);
long long losing_subsets = (1LL << (n - basis.rk)); // Subsets that XOR to 0
cout << total_subsets - losing_subsets << "\n";
```

### **6. Bitset XOR Basis (For $N > 64$)**
If you need to process binary strings of length $1000$:
```cpp
template <size_t LOG>
struct XorBasisBitset {
    bitset<LOG> b[LOG];
    int rk = 0;
    
    bool insert(bitset<LOG> curr) {
        for (int i = LOG - 1; i >= 0; i--) {
            if (!curr[i]) continue;
            if (!b[i][i]) { b[i] = curr; rk++; return true; }
            curr ^= b[i];
        }
        return false;
    }
};
```
