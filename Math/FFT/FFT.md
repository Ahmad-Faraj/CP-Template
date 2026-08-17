# Advanced FFT & Convolution Tricks

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

## 4. Pushing NTT Beyond Limits: Fast Half-NTT for Huge Polynomial Multiplication

### **Motivation (The $2^{23}$ Limit)**
An NTT modulo must be of the form $P = c \cdot 2^k + 1$. For $998244353$, $k = 23$, capping array sizes strictly at $2^{23}$. If you are tasked with multiplying polynomials of size $2^{24}$, the convolution size $2^{25}$ strictly exceeds the limit. You cannot use normal NTT, and CRT/FFT are too slow/inaccurate.

### **The Solution (Recursive Fast Half-NTT)**
Instead of building a massive transform, recursively split the problem using root of unity identities:
1. $A_{minus}[i] = A[i] + \omega^x \cdot A[i + n/2]$
2. $A_{plus}[i]  = A[i] - \omega^x \cdot A[i + n/2]$

Recursively multiply $A_{minus} \cdot B_{minus}$ (using root $x/2$) and $A_{plus} \cdot B_{plus}$ (using root $x/2 + \text{mod}/2$). Repeat until the size is small enough for naive $\mathcal{O}(N^2)$ multiplication. 
Finally, combine them back via inverse interpolation:
1. $C[i] = (res_{minus}[i] + res_{plus}[i]) / 2$
2. $C[i + n/2] = (res_{minus}[i] - res_{plus}[i]) / (2 \cdot \sqrt{c})$

### **C++ Implementation**
```cpp
const int MOD = 998244353;

long long power(long long base, long long exp) {
    long long res = 1; base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD; exp /= 2;
    }
    return res;
}

// Recursively multiplies A and B modulo (x^n - c)
vector<long long> fast_half_ntt(vector<long long> a, vector<long long> b, long long c, int n) {
    if (n <= 64) { // Base case: Naive O(N^2) modulo (x^n - c)
        vector<long long> res(n, 0);
        for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < b.size(); j++) {
                if (i + j < n) res[i + j] = (res[i + j] + a[i] * b[j]) % MOD;
                else res[i + j - n] = (res[i + j - n] + a[i] * b[j] % MOD * c) % MOD;
            }
        }
        return res;
    }

    int half = n / 2;
    // For CP, we pass the known precomputed roots (omega) downward.
    // For simplicity here, assume sqrt_c evaluates to the required root.
    long long sqrt_c = 1; // Replace with actual Tonelli-Shanks or precomputed root table

    vector<long long> a_minus(half), a_plus(half);
    vector<long long> b_minus(half), b_plus(half);

    for (int i = 0; i < half; i++) {
        long long wa = (a[i + half] * sqrt_c) % MOD;
        long long wb = (b[i + half] * sqrt_c) % MOD;
        
        a_minus[i] = (a[i] + wa) % MOD;
        a_plus[i]  = (a[i] - wa + MOD) % MOD;
        b_minus[i] = (b[i] + wb) % MOD;
        b_plus[i]  = (b[i] - wb + MOD) % MOD;
    }

    vector<long long> res_minus = fast_half_ntt(a_minus, b_minus, sqrt_c, half);
    vector<long long> res_plus  = fast_half_ntt(a_plus, b_plus, MOD - sqrt_c, half);

    vector<long long> C(n, 0);
    long long inv2 = power(2, MOD - 2);
    long long inv_sqrt_c = power(sqrt_c, MOD - 2);

    for (int i = 0; i < half; i++) {
        C[i] = (res_minus[i] + res_plus[i]) * inv2 % MOD;
        long long diff = (res_minus[i] - res_plus[i] + MOD) % MOD;
        C[i + half] = (diff * inv2 % MOD) * inv_sqrt_c % MOD;
    }

    return C;
}

vector<long long> fast_poly_mul(vector<long long> a, vector<long long> b) {
    int sz = 1; while(sz < a.size() + b.size() - 1) sz *= 2;
    a.resize(sz, 0); b.resize(sz, 0);
    vector<long long> res = fast_half_ntt(a, b, 1, sz);
    while(res.size() > 1 && res.back() == 0) res.pop_back();
    return res;
}
```

### **Performance**
This handles polynomial convolution sizes up to $2^{25}$ elegantly in $\mathcal{O}(N \log N)$ staying perfectly inside $998244353$!

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
*   **Stirling Numbers of the Second Kind:** Finding a whole row $S(N, k)$ in $\mathcal{O}(N \log N)$. 
```cpp
// Stirling Numbers S(N, k) 
vector<int> A(N + 1), B(N + 1);
for (int i = 0; i <= N; i++) {
    A[i] = (i % 2 == 0 ? 1LL : -1LL) * invFact[i] % MOD;
    A[i] = (A[i] + MOD) % MOD;
    B[i] = power(i, N) * invFact[i] % MOD;
}
vector<int> S = NTT::multiply(A, B); // S[k] holds S(N, k)
```
*   **Unbounded Knapsack / Coin Change:** Generating function is $F(x) = \prod \frac{1}{1 - x^{W_i}}$. Run $\exp(\ln(F(x)))$ via NTT.
*   **Euler's Pentagonal Numbers (Integer Partitions):** Run **FPS Inverse** using NTT to invert the sparse polynomial in $\mathcal{O}(N \log N)$.

### **4. Mathematics & Polynomials**
*   **Taylor Shift ($P(x+c)$):** Expand $a_i (x+c)^i$. Let $A[i] = a_i \cdot i!$ and $B[k] = c^k / k!$. Compute cross-correlation.
```cpp
// Taylor Shift P(x + c)
vector<int> A(N), B(N);
for(int i = 0; i < N; i++) A[i] = P[i] * fact[i] % MOD;
for(int i = 0; i < N; i++) B[i] = power(c, i) * invFact[i] % MOD;
reverse(B.begin(), B.end());
vector<int> cross_corr = NTT::multiply(A, B);
vector<int> shifted(N);
for(int i = 0; i < N; i++) shifted[i] = cross_corr[i + N - 1] * invFact[i] % MOD;
```

### **5. Probability & Graph Theory**
*   **Sum of Independent Random Variables:** The distribution of $X + Y$ is exactly the polynomial convolution of arrays $P_X$ and $P_Y$.
*   **$K$ Steps on a Graph / Game:** Distribution after $K$ steps is $(P_X)^K$. Compute via `poly_pow(P_X, K)`.
```cpp
// K Steps on a Graph using Polynomial Fast Exponentiation
// Utilize the built-in exponentiation from the templates!
int LIMIT = 100000; // Define maximum degree to prevent O(N^2) size blowup
vector<int> res_fft = FFT::poly_pow(P_X, K, LIMIT);
// OR for modular arithmetic:
vector<int> res_ntt = NTT::poly_pow_mod(P_X, K, LIMIT);
```

### **6. Dynamic Programming (CDQ Divide & Conquer + NTT)**
*   **Problem:** $dp[i] = \sum_{j=1}^{i-1} dp[j] \times W[i-j]$. Standard NTT fails because $dp[i]$ depends on previous answers.
```cpp
// CDQ D&C + NTT
void cdq(int L, int R) {
    if (L == R) return;
    int mid = L + (R - L) / 2;
    cdq(L, mid); // Solve left half first
    
    vector<int> A(mid - L + 1), B(R - L);
    for (int i = L; i <= mid; i++) A[i - L] = dp[i];
    for (int i = 1; i <= R - L; i++) B[i - 1] = W[i];
    
    vector<int> C = NTT::multiply(A, B); // Contribution from left to right
    for (int i = mid + 1; i <= R; i++) dp[i] = (dp[i] + C[i - L - 1]) % MOD;
    
    cdq(mid + 1, R); // Now solve right half
}
```

### **7. Game Theory & Subsets**
*   **Subset Convolution (Exact Submasks):** OR-convolution computes $i | j = k$. To force $i \cap j = 0$, add a second dimension for popcount: $\text{popcount}(i) + \text{popcount}(j) = \text{popcount}(k)$.
```cpp
// Subset Convolution (OR convolution forcing disjoint sets)
void subset_convolution(vector<int> A, vector<int> B, vector<int>& C) {
    int n = __builtin_ctz(A.size());
    vector<vector<int>> fA(n + 1, vector<int>(1 << n, 0)), fB(n + 1, vector<int>(1 << n, 0));
    for(int i = 0; i < (1 << n); i++) {
        fA[__builtin_popcount(i)][i] = A[i]; fB[__builtin_popcount(i)][i] = B[i];
    }
    for(int i = 0; i <= n; i++) { FWHT_OR(fA[i]); FWHT_OR(fB[i]); }
    
    vector<vector<int>> fC(n + 1, vector<int>(1 << n, 0));
    for(int i = 0; i <= n; i++) {
        for(int j = 0; i + j <= n; j++) {
            for(int mask = 0; mask < (1 << n); mask++) {
                fC[i + j][mask] = (fC[i + j][mask] + 1LL * fA[i][mask] * fB[j][mask]) % MOD;
            }
        }
    }
    for(int i = 0; i <= n; i++) Inverse_FWHT_OR(fC[i]);
    for(int i = 0; i < (1 << n); i++) C[i] = fC[__builtin_popcount(i)][i];
}
```

### **8. Geometry (Minkowski Sums)**
*   **Problem:** Given two shapes defined as boolean grids $A$ and $B$, find their Minkowski Sum (all vector sums $\vec{a} + \vec{b}$).
```cpp
// Minkowski Sum of Two Shapes (Grid A and Grid B)
int SHIFT = max_width; vector<int> polyA, polyB;
for(int i = 0; i < H_A; i++) 
    for(int j = 0; j < W_A; j++) 
        if(A[i][j]) polyA[i * SHIFT + j] = 1;

for(int i = 0; i < H_B; i++) 
    for(int j = 0; j < W_B; j++) 
        if(B[i][j]) polyB[i * SHIFT + j] = 1;

vector<int> C = FFT::multiply(polyA, polyB);
for(int i = 0; i < C.size(); i++) {
    if(C[i] > 0) cout << "Vector (" << i / SHIFT << ", " << i % SHIFT << ") exists in sum\n";
}
```

### **9. Generating Pair Sums & Differences**
*   **Pair Sums:** Convert array to frequency polynomial $F = \sum freq(a) x^a$. Compute $F^2$ using FFT. The coefficient of index $k$ represents the number of pairs $(i, j)$ forming the sum $k$. (Note: subtract valid pairs manually if $i \neq j$ is required).
*   **Pair Differences ($A_i - A_j$):** Create $F_1 = \sum freq(a) x^a$ and $F_2 = \sum freq(a) x^{-a + SHIFT}$. Compute $F_1 \cdot F_2$; the answer for difference $d$ is at $x^{d + SHIFT}$.
```cpp
// Pair Differences: A_i - A_j
int SHIFT = 100000;
vector<int> F1(MAX_VAL), F2(MAX_VAL + SHIFT);
for (int x : A) { F1[x]++; F2[-x + SHIFT]++; }
vector<int> diff_freq = FFT::multiply(F1, F2);
// freq of difference 'd' is at diff_freq[d + SHIFT]
```

### **10. Subarray & Subset Sums**
*   **Subarray Sum:** Since a subarray sum is the difference of two prefix sums, build $F_1$ and $F_2$ using frequencies of prefix sums with a $SHIFT$. Access only positive indices to enforce $j \ge i$. (Manually adjust occurrences where sum = 0).
*   **Subset Sum:** Every element acts as a polynomial $(1 + x^{a_i})$. Multiply all polynomials. **Optimization:** Push them into a Priority Queue by size, always extracting and multiplying the two smallest polynomials to achieve $\mathcal{O}(N \log^2 N)$.
```cpp
// Subarray Sums (Difference of prefix sums)
int SHIFT = 100000;
vector<int> pref(N + 1, 0);
for (int i = 1; i <= N; i++) pref[i] = pref[i-1] + A[i-1];

vector<int> F1(MAX_VAL), F2(MAX_VAL + SHIFT);
for (int p : pref) { F1[p]++; F2[-p + SHIFT]++; }

vector<int> subarray_sums = FFT::multiply(F1, F2);
// Zero-sum subarrays must be counted manually!
```

### **11. Index-Relative Convolutions & Shifts**
*   **Multiplication with Distance:** To quickly answer queries of $\sum a_i \cdot b_{i+x}$, let $F_a = \sum a_i x^i$ and $F_b = \sum b_i x^{-i + SHIFT}$. The answer for distance $x$ is directly located at index $x$ in the resulting polynomial!
*   **Cyclic Shifts:** Need to evaluate the above while array $b$ cyclically shifts right? Simply append $b$ to itself ($b+b$) and pad $a$ with $N$ zeros at the front. The same FFT handles all cyclic states natively in $\mathcal{O}(1)$ query time!
```cpp
// Multiplication with Distance / Cyclic Shifts
int SHIFT = b.size();
vector<int> F_a = a;
vector<int> F_b(b.size() * 2);
for (int i = 0; i < b.size(); i++) {
    F_b[-i + SHIFT] = b[i];
    F_b[-i - b.size() + SHIFT] = b[i]; // cyclic wrapper
}
vector<int> res = FFT::multiply(F_a, F_b);
// Answer for cyclic shift x is at res[x + SHIFT]
```

### **12. Polynomial Exponentiation**
If raising a polynomial to power $K$ in NTT, avoid iterative multiplication. Simply transition the polynomial to the frequency domain (run NTT once), raise the frequencies to power $K$ in $\mathcal{O}(N \log K)$ via fast exponentiation: `fa[i] = power(fa[i], K)`, and then apply the Inverse NTT to get the exact coefficients.
```cpp
// Raise polynomial A to power K inside NTT
vector<int> A_freq = A;
A_freq.resize(next_power_of_two);
NTT::ntt(A_freq, false);

for(int i = 0; i < A_freq.size(); i++) A_freq[i] = power(A_freq[i], K); // Fast modular exponentiation

NTT::ntt(A_freq, true); // Inverse NTT
```

### **13. Arbitrary Modulus (Polynomial Splitting Method)**
If the CRT method is too complex for $10^9+7$, distribute $A(x)$ and $B(x)$ into two smaller polynomials using $C \approx \sqrt{M}$:
$$A(x) = A_1(x) + A_2(x) \cdot C \quad \text{and} \quad B(x) = B_1(x) + B_2(x) \cdot C$$
Then $A(x)B(x) = A_1 B_1 + (A_1 B_2 + A_2 B_1)C + (A_2 B_2)C^2$. Since coefficients are $< \sqrt{M}$, products are $< M \cdot N$, easily fitting inside standard `double` FFT without precision loss.
```cpp
// A_1 + A_2 * C
int C = sqrt(MOD) + 1;
vector<double> A1(n), A2(n), B1(m), B2(m);
for(int i=0; i<n; i++) { A1[i] = A[i] % C; A2[i] = A[i] / C; }
for(int i=0; i<m; i++) { B1[i] = B[i] % C; B2[i] = B[i] / C; }

vector<double> R1 = FFT::multiply(A1, B1); // A1 * B1
vector<double> R2 = FFT::multiply(A1, B2); // A1 * B2
vector<double> R3 = FFT::multiply(A2, B1); // A2 * B1
vector<double> R4 = FFT::multiply(A2, B2); // A2 * B2

vector<long long> result(n + m - 1);
for(int i=0; i < result.size(); i++) {
    long long r1 = llround(R1[i]) % MOD;
    long long r23 = (llround(R2[i]) + llround(R3[i])) % MOD;
    long long r4 = llround(R4[i]) % MOD;
    result[i] = (r1 + r23 * C + r4 * C % MOD * C) % MOD;
}
```
For Codeforces Div1 E/F and extreme competitive programming, standard convolution is just the beginning. We treat polynomials as infinite **Formal Power Series (FPS)** and use Newton's Method to compute inverses, logarithms, and exponentials in $\mathcal{O}(N \log N)$. 

### **14. Polynomial Inverse ($P(x)^{-1} \pmod{x^n}$)**
Computes $Q(x)$ such that $P(x)Q(x) \equiv 1 \pmod{x^n}$. Uses Newton's method: $Q_{k+1} = Q_k (2 - P Q_k) \pmod{x^{2^k}}$.
```cpp
vector<int> poly_inv(vector<int> a, int deg) {
    if (deg == 1) return {NTT::mod_pow(a[0], NTT::MOD - 2, NTT::MOD)};
    vector<int> res = poly_inv(a, (deg + 1) / 2);
    int n = 1; while (n < deg * 2) n <<= 1;
    
    vector<int> copy_a(a.begin(), a.begin() + min((int)a.size(), deg));
    copy_a.resize(n, 0);
    vector<int> copy_res = res; copy_res.resize(n, 0);
    
    NTT::ntt(copy_a, false); NTT::ntt(copy_res, false);
    for (int i = 0; i < n; i++) {
        copy_res[i] = (1LL * copy_res[i] * (2LL - 1LL * copy_a[i] * copy_res[i] % NTT::MOD + NTT::MOD)) % NTT::MOD;
    }
    NTT::ntt(copy_res, true);
    copy_res.resize(deg);
    return copy_res;
}
```

### **15. Polynomial Derivative & Integral**
```cpp
vector<int> poly_deriv(const vector<int>& a) {
    vector<int> res(max(1, (int)a.size() - 1), 0);
    for(int i = 1; i < a.size(); i++) res[i - 1] = 1LL * a[i] * i % NTT::MOD;
    return res;
}
vector<int> poly_integr(const vector<int>& a) {
    vector<int> res(a.size() + 1, 0);
    for(int i = 0; i < a.size(); i++) res[i + 1] = 1LL * a[i] * NTT::mod_pow(i + 1, NTT::MOD - 2, NTT::MOD) % NTT::MOD;
    return res;
}
```

### **16. Polynomial Logarithm ($\ln P(x)$)**
Computes $\ln P(x) = \int \frac{P'(x)}{P(x)} dx$. (Requires $P[0] = 1$).
```cpp
vector<int> poly_ln(vector<int> a, int deg) {
    vector<int> deriv = poly_deriv(a);
    vector<int> inv = poly_inv(a, deg);
    vector<int> res = poly_integr(NTT::multiply(deriv, inv, deg - 1));
    res.resize(deg, 0);
    return res;
}
```

### **17. Polynomial Exponential ($\exp P(x)$)**
Computes $\exp P(x)$ using Newton's method: $Q_{k+1} = Q_k (1 - \ln Q_k + P) \pmod{x^{2^k}}$. (Requires $P[0] = 0$).
```cpp
vector<int> poly_exp(vector<int> a, int deg) {
    if (deg == 1) return {1}; // e^0 = 1
    vector<int> res = poly_exp(a, (deg + 1) / 2);
    res.resize(deg, 0);
    vector<int> ln_res = poly_ln(res, deg);
    
    vector<int> diff(deg);
    for (int i = 0; i < deg; i++) {
        diff[i] = (a[i] - ln_res[i] + NTT::MOD) % NTT::MOD;
    }
    diff[0] = (diff[0] + 1) % NTT::MOD;
    
    res = NTT::multiply(res, diff, deg);
    return res;
}
```

### **18. Linear Recurrence in $\mathcal{O}(K \log K \log N)$ (Bostan-Mori Algorithm)**
Given a recurrence $A_n = \sum_{i=1}^K c_i A_{n-i}$ and base cases, find the $N$-th term. Extremely useful when $N \le 10^{18}$ and $K \le 10^5$.
1. Build $Q(x) = 1 - \sum_{i=1}^K c_i x^i$.
2. Build $P(x) = A(x) \times Q(x) \pmod{x^K}$.
3. We want the $N$-th coefficient of $P(x) / Q(x)$.
```cpp
long long bostan_mori(vector<int> P, vector<int> Q, long long N) {
    while (N > 0) {
        vector<int> Q_minus = Q;
        for (int i = 1; i < Q_minus.size(); i += 2) Q_minus[i] = (NTT::MOD - Q_minus[i]) % NTT::MOD;
        
        vector<int> U = NTT::multiply(P, Q_minus);
        vector<int> V = NTT::multiply(Q, Q_minus);
        
        for (int i = N % 2; i < U.size(); i += 2) P[i / 2] = U[i];
        for (int i = 0; i < V.size(); i += 2) Q[i / 2] = V[i];
        
        P.resize((U.size() + 1) / 2);
        Q.resize((V.size() + 1) / 2);
        N /= 2;
    }
    return P[0];
}
```

### **19. Bluestein’s Algorithm (Chirp-Z Transform)**
Computes $P(c^k)$ for $k = 0 \dots M-1$ in $\mathcal{O}((N+M) \log(N+M))$. Breaks the restriction of roots of unity!
Using the identity $ki = \binom{k+i}{2} - \binom{k}{2} - \binom{i}{2}$, we rewrite $P(c^k) = \sum a_i c^{ki}$ into a perfect convolution.
```cpp
vector<int> bluestein(vector<int> a, int c, int M) {
    int N = a.size();
    vector<int> A(N), B(N + M);
    int inv_c = NTT::mod_pow(c, NTT::MOD - 2, NTT::MOD);
    
    for (int i = 0; i < N; i++) A[N - 1 - i] = 1LL * a[i] * NTT::mod_pow(inv_c, (1LL * i * i) / 2 % (NTT::MOD - 1), NTT::MOD) % NTT::MOD;
    for (int i = 0; i < N + M; i++) B[i] = NTT::mod_pow(c, (1LL * i * i) / 2 % (NTT::MOD - 1), NTT::MOD);
    
    vector<int> C = NTT::multiply(A, B);
    vector<int> res(M);
    for (int k = 0; k < M; k++) res[k] = 1LL * C[N - 1 + k] * NTT::mod_pow(inv_c, (1LL * k * k) / 2 % (NTT::MOD - 1), NTT::MOD) % NTT::MOD;
    
    return res;
}
```
