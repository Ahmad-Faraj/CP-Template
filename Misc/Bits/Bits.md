# Advanced Bitwise Tricks & Properties

### **1. Basic Bitwise Functions & Custom Helpers**
Instead of using macros, use these simple inline functions. We also include the standard C++ built-ins.
*   `__builtin_popcount(n)`: Number of set bits (use `ll` for 64-bit).
*   `__builtin_ctz(n)`: Count trailing zeros (index of lowest set bit). **Undefined for 0!**
*   `__builtin_clz(n)`: Count leading zeros. **Undefined for 0!**
*   `__builtin_parity(n)`: Number of set bits modulo 2.

```cpp
inline int set_bit( int n , int i) { return n | (1ll << i); }
inline int clear_bit( int n , int i) { return n & ~(1ll << i); }
inline int toggle_bit( int n , int i) { return n ^ (1ll << i); }
inline int check_bit( int n , int i) { return (n >> i) & 1; }
inline int isolate_lowest( int n ) { return n & -n; }
inline int clear_lowest( int n ) { return n & (n - 1); }

inline int count_bits(int n) { return __builtin_popcount(n); }
inline int count_bits(long long n) { return __builtin_popcountll(n); }
inline int count_zeros(int n) { return 32 - __builtin_popcount(n); }
inline int count_zeros(long long n) { return 64 - __builtin_popcountll(n); }
```

### **2. Algebraic & Arithmetic Identities**
*   **Addition:** $a + b = (a \oplus b) + 2(a \& b)$
*   **Addition (OR form):** $a + b = (a | b) + (a \& b)$
*   **Subtraction:** $a - b = (a \oplus (a \& b)) - ((a | b) \oplus a)$
*   **Symmetry of OR/AND/XOR:** $a | b = a \oplus b + a \& b$
*   **Union & Intersection:** $a \oplus b = (a | b) \setminus (a \& b) = (a | b) \oplus (a \& b)$

### **3. Power of 2 Validations & Tricks**
*   **Check if power of 2:** `(n > 0) && (n & (n - 1)) == 0`
*   **Next Power of 2 (32-bit):**
    ```cpp
    n--; n |= n >> 1; n |= n >> 2; n |= n >> 4; n |= n >> 8; n |= n >> 16; n++;
    ```
*   **LSB (Lowest Set Bit):** `n & -n`
*   **MSB (Most Significant Bit):** `31 - __builtin_clz(n)` (or 63 for `ll`).
*   **Clear LSB:** `n & (n - 1)`

### **4. Range XOR Utilities**
Finding the XOR sum of all numbers in a range `[L, R]` in $\mathcal{O}(1)$.
```cpp
// XOR of all elements from 0 to x
inline long long xor_upto(long long x) {
    if (x < 0) return 0;
    long long rem = x % 4;
    if (rem == 0) return x;
    if (rem == 1) return 1;
    if (rem == 2) return x + 1;
    return 0;
}

// XOR of all elements in range [l, r]
inline long long xor_range(long long l, long long r) { 
    if (l > r) return 0;
    return xor_upto(r) ^ xor_upto(l - 1); 
}
```

### **5. Subset Iteration (Gosper's Hack & Submasks)**
*   **Iterating all submasks of $M$:**
    Traverses all subsets of bits that are ON in $M$ in $\mathcal{O}(2^{\text{popcount}(M)})$.
    ```cpp
    for (int s = M; s > 0; s = (s - 1) & M) {
        // 's' is a valid submask
    }
    // Note: To include 0, you can do: for(int s=M; ; s=(s-1)&M) { ... if(!s) break; }
    ```
*   **Gosper's Hack (Next Permutation of bits):**
    Generates the next number with exactly $K$ bits set in $\mathcal{O}(1)$. Useful for combinatorial brute forces across fixed sizes.
    ```cpp
    int next_comb(int v) {
        int c = v & -v;          // Lowest set bit
        int r = v + c;           // Flip lowest contiguous block of 1s
        return (((r ^ v) >> 2) / c) | r;
    }
    ```

### **6. Sum Over Subsets (SOS DP)**
Compute $F(mask) = \sum_{sub \subseteq mask} A[sub]$ in $\mathcal{O}(N \cdot 2^N)$.
If done naively via submask iteration, it takes $\mathcal{O}(3^N)$.
```cpp
// F array initially contains A
for(int i = 0; i < N; ++i) {
    for(int mask = 0; mask < (1 << N); ++mask) {
        if(mask & (1 << i)) {
            F[mask] += F[mask ^ (1 << i)];
        }
    }
}
```
*Can easily be inverted to compute supersets by iterating `if(!(mask & (1<<i)))`.*

### **7. Basis of a Vector Space (XOR Basis)**
Any set of numbers can be compressed into a basis of maximum size $\log_2(\max A)$. The basis spans the exact same XOR space as the original array.
```cpp
int basis[60]; // Size depends on max bits (e.g., 60 for long long)
void insertVector(int mask) {
    for (int i = 59; i >= 0; i--) {
        if ((mask >> i) & 1) {
            if (!basis[i]) { basis[i] = mask; return; }
            mask ^= basis[i];
        }
    }
}
bool checkXor(int mask) {
    for (int i = 59; i >= 0; i--) {
        if ((mask >> i) & 1) {
            if (!basis[i]) return false;
            mask ^= basis[i];
        }
    }
    return true; // Can be formed by the basis
}
```

### **8. Brian Kernighan's Algorithm**
Clears the lowest set bit. Excellent for iterating only the 1-bits directly without checking 0s!
```cpp
while (n > 0) {
    int lowest_bit = n & -n;
    // process lowest_bit
    n &= (n - 1); // clears lowest_bit
}
```

### **9. Maximum XOR Pair / Subarray (Bitwise Trie)**
To find the maximum XOR of a given number $X$ with any number in an array $A$, insert all elements of $A$ into a binary Trie. Then, query $X$ by always greedily traversing towards the opposite bit of $X$ at the current level.
```cpp
const int LOG = 30;
int trie[100005 * LOG][2], nxt = 1;

void insert(int num) {
    int node = 0;
    for (int i = LOG - 1; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (!trie[node][bit]) trie[node][bit] = nxt++;
        node = trie[node][bit];
    }
}

int query_max_xor(int num) {
    int node = 0, ans = 0;
    for (int i = LOG - 1; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (trie[node][bit ^ 1]) {
            ans |= (1 << i);
            node = trie[node][bit ^ 1];
        } else {
            node = trie[node][bit];
        }
    }
    return ans;
}
```

### **10. Bitwise Lifting on Fenwick Tree (BIT)**
Find the smallest index $idx$ such that the prefix sum is $\ge V$ in $\mathcal{O}(\log N)$ without binary search.
```cpp
int get_kth(int v) {
    int sum = 0, pos = 0;
    // 20 is log2(MAX_N)
    for(int i = 20; i >= 0; i--) {
        if(pos + (1 << i) < MAX_N && sum + tree[pos + (1 << i)] < v) {
            sum += tree[pos + (1 << i)];
            pos += (1 << i);
        }
    }
    return pos + 1;
}
```