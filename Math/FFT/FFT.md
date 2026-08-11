# Advanced FFT & Convolution Tricks (Grandmaster Notes)

---

# FFT & NTT & FWHT

## 1. What Do it Compute ?
At its heart, any transform in this folder exists to compute a **Convolution** in $\mathcal{O}(N \log N)$ time instead of $\mathcal{O}(N^2)$.

*   **Standard FFT/NTT (Polynomial Multiplication):**
    Computes the standard linear convolution where indices *add up*:
    $$Ans[k] = \sum_{i+j=k} A[i] \times B[j]$$
    
*   **Cross-Correlation (Reversed Array Trick):**
    Computes the correlation where indices have a fixed *difference*:
    $$Ans[k] = \sum_{i-j=k} A[i] \times B[j]$$
    *Implementation:* Compute this using standard FFT by reversing the array $B$. The answer for difference $k$ will be found at `res[k + B.size() - 1]`.

*   **Fast Walsh-Hadamard Transform (FWHT):**
    Computes bitwise convolutions where indices combine via bitwise operators $\oplus \in \{\text{XOR, AND, OR}\}$:
    $$Ans[k] = \sum_{i \oplus j = k} A[i] \times B[j]$$

*   **Base-K FWHT (Ternary / K-ary Convolution):**
    Computes convolutions where digits are added modulo $K$ without carry (e.g., Nim-sum in game theory but base $K$):
    $$Ans[k] = \sum_{(i +_K j) = k} A[i] \times B[j]$$

---

## 2. FFT vs. NTT

### **Fast Fourier Transform (FFT)**
Uses `complex<double>` and $N$-th roots of unity ($e^{2\pi i / N}$). 
*   **When to use:** String matching, Big Integer multiplication, or convolutions with no modulo.
*   **The Danger (Precision Loss):** `double` has ~15-17 significant decimal digits of precision. If the maximum coefficient in your resulting polynomial exceeds $\approx 10^{15}$, the rounding (`llround`) will yield completely wrong answers.
*   **The Fix:** If answers exceed $10^{15}$, you must use Arbitrary Modulo NTT (CRT), or upgrade to `__float128` (extremely slow).

### **Number Theoretic Transform (NTT)**
Operates purely on integers modulo a prime $P$. Uses a primitive root $g$ instead of $e$.
*   **When to use:** When the problem requires the answer modulo a specific prime (e.g., $998244353$).
*   **Is the Modulo NTT-Friendly?**
    An NTT modulo must be of the form $P = c \cdot 2^k + 1$.
    The maximum size of the polynomial $N$ that can be multiplied is strictly bounded by $2^k$.
    *   *Example:* $998244353 = 119 \times 2^{23} + 1$. This means you can multiply polynomials of size up to $2^{23}$ ($\approx 8.3 \times 10^6$).
    *   *Example:* $10^9 + 7 = 5 \times 10^8 \times 2^1 + 1$. The $k$ is only $1$. **You cannot use standard NTT on $10^9+7$!**

### **Arbitrary Modulo NTT (The CRT Trick)**
*   **When to use:** The modulo is $10^9+7$, or coefficients are massive but you want exact integer results.
*   **How it works:** We pick 3 massive, safe NTT-friendly primes:
    1.  $M_1 = 167772161 \quad (root = 3)$
    2.  $M_2 = 469762049 \quad (root = 3)$
    3.  $M_3 = 754974721 \quad (root = 11)$
    We multiply the polynomials under all 3 primes independently. We then use the **Chinese Remainder Theorem (CRT)** to reconstruct the exact coefficient modulo $M_1 \times M_2 \times M_3 \approx 5.9 \times 10^{25}$, guaranteeing zero overflow, and finally take it modulo $10^9+7$.

---

## 3. Primitive Roots & Array Size Limits

### **How to Find a Primitive Root**
To mathematically verify if $g$ is a primitive root for prime $P$:
1.  Factorize $P - 1$ into its prime factors: $q_1, q_2, \dots, q_m$.
2.  For every prime factor $q_i$, verify that: $g^{(P-1)/q_i} \not\equiv 1 \pmod P$
3.  If this holds for all factors, $g$ is a primitive root! 
```cpp
// Find the primitive root for any MOD dynamically
int ROOT = NTT::generator(MOD);
assert(ROOT != -1);
```

### **How to know if a Modulo $P$ is ENOUGH for your Array Size?**
1. **The $2^k$ Form Requirement:**
   Calculate `__builtin_ctzll(P - 1)`. If this value is $k$, then the **absolute maximum array size** you can ever multiply under this modulo is $2^k$.
2. **The Capacity Requirement:**
   If using NTT to get exact integers, calculate $\max(A) \times \max(B) \times \text{size}$. If this value $\ge P$, the answer will wrap around the modulo. You **must** use Arbitrary Modulo NTT (CRT).

---

# Applications

### **1. Big Integer Multiplication**
Instead of mapping numbers base-10 digit by digit, compress digits into Base-$10^3$ or Base-$10^4$ chunks. This reduces the polynomial size $N$ by a factor of 3 or 4, dramatically speeding up the FFT execution while keeping the maximum coefficient safely below the `double` $10^{15}$ precision limit.
```cpp
// Function to multiply two large integers represented as strings in O(n log n) time using FFT
string mul_big_int(const string &a, const string &b) {
    int n = a.size(), m = b.size();
    vector<int> num1(n), num2(m);
    for (int i = 0; i < n; i++) num1[i] = a[n - 1 - i] - '0';
    for (int i = 0; i < m; i++) num2[i] = b[m - 1 - i] - '0';
    
    vector<int> result = FFT::multiply(num1, num2);
    string res;
    int carry = 0;
    
    for (auto &val : result) {
        val += carry;
        carry = val / 10;
        res.push_back((val % 10) + '0');
    }
    while (carry) {
        res.push_back((carry % 10) + '0');
        carry /= 10;
    }
    
    reverse(res.begin(), res.end());
    for (int i = 0; i < sz(res); i++) {
        if(res[i] != '0') return res.substr(i);
    }
    return "0";
}
```

### **2. String Matching with FFT (Squared Error Trick)**
Find exact occurrences of pattern $P$ in text $T$ where both might contain wildcards (`?`).
*   Map wildcard `?` to $0$. Map standard characters to ASCII values $>0$.
*   A match ending at $i$ is perfect if the squared difference evaluates to $0$:
    $$\sum_{j=0}^{|P|-1} (T[i-j] - P[|P|-1-j])^2 \cdot T[i-j] \cdot P[|P|-1-j] = 0$$
*   By expanding this algebraically, we get $\sum T^3 P - 2 \sum T^2 P^2 + \sum T P^3$. 
```cpp
// 1. Build T1, T2, T3 (Text powers)
// 2. Build P1, P2, P3 (Pattern powers), then reverse them!
vector<int> res1 = FFT::multiply(T3, P1);
vector<int> res2 = FFT::multiply(T2, P2);
vector<int> res3 = FFT::multiply(T1, P3);

for (int i = sz(P) - 1; i < sz(T); i++) {
    long long match_val = res1[i] - 2LL * res2[i] + res3[i];
    if (match_val == 0) cout << "Perfect match ending at index " << i << "\n";
}
```

### **3. Combinatorics & FPS**
*   **Unbounded Knapsack / Coin Change:** 
    If you can use items of weights $W_1, W_2, \dots$ infinitely many times, the generating function is $F(x) = \prod \frac{1}{1 - x^{W_i}}$. Run $\exp(\ln(F(x)))$ via NTT to solve in $\mathcal{O}(N \log N)$.
*   **Stirling Numbers of the Second Kind:** 
    Finding a whole row $S(N, k)$ in $\mathcal{O}(N \log N)$. $S(N, k) = \sum_{i=0}^k \left( \frac{(-1)^i}{i!} \right) \times \left( \frac{(k-i)^N}{(k-i)!} \right)$. 
*   **Euler's Pentagonal Numbers (Integer Partitions):**
    Compute partitions of $N$. The denominator of the partition generating function is $1 - x - x^2 + x^5 + x^7 \dots$ Run **FPS Inverse** using NTT to invert this sparse polynomial in $\mathcal{O}(N \log N)$.

### **4. Mathematics & Polynomials**
*   **Taylor Shift ($P(x+c)$):** 
    Expand $a_i (x+c)^i$ using the Binomial Theorem. This forms a cross-correlation! Let $A[i] = a_i \cdot i!$ and $B[k] = c^k / k!$. Compute cross-correlation using the reversed-array FFT trick, then multiply the $j$-th term by $1/j!$.
*   **Multipoint Evaluation & Interpolation:** 
    Evaluate $P(x)$ at $N$ arbitrary points in $\mathcal{O}(N \log^2 N)$. Build a Segment Tree of polynomials bottom-up: $M_v(x) = \prod(x - x_i)$ using NTT. Pass $P(x)$ down the tree, taking the polynomial modulo the child's $M_v(x)$ using FPS Division.

### **5. Probability & Graph Theory**
*   **Sum of Independent Random Variables:**
    The distribution of $X + Y$ is exactly the polynomial convolution of arrays $P_X$ and $P_Y$.
*   **$K$ Steps on a Graph / Game:**
    If you take $K$ steps, the new probability distribution is $(P_X)^K$. Compute via `poly_pow(P_X, K)` in $\mathcal{O}(N \log N \log K)$.
*   **Counting Paths of Length $L$ in a Tree:**
    At each centroid, multiply subtree depth-frequency polynomials via FFT. Drops complexity from $\mathcal{O}(N^2)$ to $\mathcal{O}(N \log^2 N)$.

### **6. Dynamic Programming (CDQ Divide & Conquer + NTT)**
*   **Problem:** $dp[i] = \sum_{j=1}^{i-1} dp[j] \times W[i-j]$. Standard NTT fails because $dp[i]$ depends on previous answers.
*   **Blueprint:** Use CDQ Divide & Conquer in $\mathcal{O}(N \log^2 N)$:
    1. `solve(L, R)` -> `mid = (L + R) / 2` -> `solve(L, mid)`
    2. Extract polynomial $A = dp[L \dots mid]$. Extract $B = W[1 \dots R - L]$.
    3. $C = NTT(A, B)$.
    4. For $i$ in $[mid+1 \dots R]$, add $C[i - L - 1]$ to $dp[i]$.
    5. `solve(mid + 1, R)`

### **7. Game Theory & Subsets**
*   **Sprague-Grundy State Reachability (Nim-Sums):**
    Construct boolean array $A$ where $A[g] = 1$ if Grundy value $g$ is reachable. For $K$ turns: Run `FWHT_XOR(A)`. Set $A[i] = A[i]^K$. Run `Inverse_FWHT_XOR`.
*   **Subset Convolution (Exact Submasks):**
    Standard OR-convolution computes $i | j = k$. To force $i \cap j = 0$ (disjoint), add a second dimension for popcount: $\text{popcount}(i) + \text{popcount}(j) = \text{popcount}(k)$.

### **8. Geometry (Minkowski Sums)**
*   **Problem:** Given two shapes defined as boolean grids $A$ and $B$, find their Minkowski Sum (all vector sums $\vec{a} + \vec{b}$).
*   **Blueprint:** Flatten the 2D grid into a 1D array by padding each row with enough zeros (width $\ge W_A + W_B$) to prevent row-wrap-around. Run standard 1D `FFT_boolean`. If $Ans[i] > 0$, the vector exists in the Minkowski sum.
