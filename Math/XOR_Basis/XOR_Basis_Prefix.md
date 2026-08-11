# Prefix XOR Basis (Range Queries) - Grandmaster Notes

A specialized linear algebra structure. Instead of maintaining a static basis, we maintain the "latest possible index" (`pos`) for each bit in the basis. This allows us to answer offline and online queries of the form: "What is the maximum XOR subset in the range $A[L \dots R]$?"

## 1. Complete Advanced Function Directory

### **Minimum XOR Generated in $[L, R]$**
```cpp
int min_xor(int l, int start = 0) const {
    int ans = start;
    for (int i = LOG - 1; i >= 0; i--) {
        if (pos[i] >= l) ans = min(ans, ans ^ b[i]);
    }
    return ans;
}
```

### **$K$-th Smallest Unique XOR Generated in $[L, R]$**
Requires reducing the valid basis vectors (where `pos[i] >= l`) into RREF.
```cpp
int kth_smallest_unique(long long k, int l) const {
    if (k <= 0) return -1;
    int tmp[LOG];
    for (int i = 0; i < LOG; i++) {
        tmp[i] = (pos[i] >= l) ? b[i] : 0;
    }
    
    // Convert valid subset to RREF
    for (int i = LOG - 1; i >= 0; i--) {
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
```

### **Check if $X$ can be formed strictly using elements from $[L, R]$**
```cpp
bool can_make(int x, int l) const {
    int ans = x;
    for (int i = LOG - 1; i >= 0; i--) {
        if (pos[i] >= l) ans = min(ans, ans ^ b[i]);
    }
    return ans == 0;
}
```


### **Max/Min XOR Bounded in $[min\_val, max\_val]$ strictly inside subarray $[L, R]$**
Finds the maximum (or minimum) XOR achievable using only elements from $A[L \dots R]$ that strictly lies within $[min\_val, max\_val]$. Requires a DFS branch search.
```cpp
int max_xor_bounded(int min_val, int max_val, int l, int start = 0) const {
    int tmp[LOG];
    for (int i = 0; i < LOG; i++) tmp[i] = (pos[i] >= l) ? b[i] : 0;
    
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

int min_xor_bounded(int min_val, int max_val, int l, int start = 0) const {
    int tmp[LOG];
    for (int i = 0; i < LOG; i++) tmp[i] = (pos[i] >= l) ? b[i] : 0;
    
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
---

## 2. Grandmaster CP Applications

### **A. Offline Subsegment Maximum XOR $[L, R]$**
**Problem:** Given an array $A$ of size $N$ and $Q$ queries of the form $(L, R)$, find the maximum XOR subset inside $A[L \dots R]$. 
**Implementation:** By sorting queries, we achieve $\mathcal{O}(N \log X + Q \log X)$ which destroys Mo's algorithm overhead.
```cpp
struct Query { int l, r, id; };

int n, q; cin >> n >> q;
vector<int> a(n + 1);
for (int i = 1; i <= n; i++) cin >> a[i];

vector<Query> queries(q);
for (int i = 0; i < q; i++) {
    cin >> queries[i].l >> queries[i].r;
    queries[i].id = i;
}

// Sort queries by their right endpoint (R) ascending
sort(queries.begin(), queries.end(), [](const Query& A, const Query& B) {
    return A.r < B.r;
});

vector<int> ans(q);
XorBasisPrefix<62> basis;
int curr_r = 1;

for (int i = 0; i < q; i++) {
    while (curr_r <= n && curr_r <= queries[i].r) {
        basis.insert(a[curr_r], curr_r); // Insert with 1-based index
        curr_r++;
    }
    ans[queries[i].id] = basis.max_xor(queries[i].l);
}

for (int i = 0; i < q; i++) cout << ans[i] << "\n";
```

### **B. Online Subsegment Queries (Bypassing Mo's Entirely)**
**Problem:** The exact same query structure, but queries are heavily encrypted (online), so you cannot sort them by $R$.
**Implementation:** Create a persistent history of prefix bases. Drops complexity to $\mathcal{O}(N \log X + Q \log X)$ entirely online!
```cpp
int n, q; cin >> n >> q;
vector<XorBasisPrefix<62>> pref(n + 1);

for (int i = 1; i <= n; i++) {
    int x; cin >> x;
    pref[i] = pref[i - 1]; // Clone the entire previous basis state
    pref[i].insert(x, i);  // Insert current element, pushing older duplicates out
}

int last_ans = 0;
while (q--) {
    int l, r; cin >> l >> r;
    
    // Online Encryption (Example)
    l = (l ^ last_ans) % n + 1;
    r = (r ^ last_ans) % n + 1;
    if (l > r) swap(l, r);
    
    // Answer query instantly using the basis state saved exactly at index R
    last_ans = pref[r].max_xor(l);
    cout << last_ans << "\n";
}
```
