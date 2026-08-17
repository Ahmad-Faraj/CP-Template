# Number Theory

### **1. Divisibility Rules**
Quick tests to check if $N$ is divisible by $D$:
*   **2:** Last digit is even.
*   **3:** Sum of digits is divisible by $3$.
*   **4:** Last two digits form a number divisible by $4$.
*   **5:** Last digit is $0$ or $5$.
*   **6:** Divisible by both $2$ and $3$.
*   **7:** Double the last digit, subtract it from the truncated number. The result must be divisible by $7$.
*   **8:** Last three digits form a number divisible by $8$.
*   **9:** Sum of digits is divisible by $9$.
*   **10:** Last digit is $0$.
*   **11:** Difference between the sum of digits in odd positions and even positions is divisible by $11$.
*   **12:** Divisible by both $3$ and $4$.

### **2. Primes & Factorization**
*   **Number of Divisors $\tau(n)$:** If $n = p_1^{a_1} p_2^{a_2} \dots p_k^{a_k}$, then $\tau(n) = (a_1+1)(a_2+1)\dots(a_k+1)$.
*   **Sum of Divisors $\sigma(n)$:** $\sigma(n) = \prod \frac{p_i^{a_i+1} - 1}{p_i - 1}$.
*   **Sieve of Eratosthenes:** Finds all primes up to $N$ in $\mathcal{O}(N \log \log N)$.
    ```cpp
    vector<int> sieve(int n) {
        vector<bool> is_prime(n + 1, true);
        vector<int> primes;
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i <= n; i++) {
            if (is_prime[i]) {
                primes.push_back(i);
                for (long long j = 1LL * i * i; j <= n; j += i)
                    is_prime[j] = false;
            }
        }
        return primes;
    }
    ```
*   **Smallest Prime Factor (SPF):** Precomputing the SPF allows factoring any $X \le N$ in $\mathcal{O}(\log X)$.
    ```cpp
    vector<int> spf(MAX_N + 1);
    void build_spf() {
        iota(spf.begin(), spf.end(), 0);
        for (int i = 2; i * i <= MAX_N; i++) {
            if (spf[i] == i) {
                for (int j = i * i; j <= MAX_N; j += i)
                    if (spf[j] == j) spf[j] = i;
            }
        }
    }
    vector<int> factorize(int x) {
        vector<int> factors;
        while (x != 1) {
            factors.push_back(spf[x]);
            x /= spf[x];
        }
        return factors;
    }
    ```

### **3. GCD & Extended Euclidean Algorithm**
*   **Euclidean Algorithm:** $\gcd(A, B) = \gcd(B, A \pmod B)$. Runs in $\mathcal{O}(\log(\min(A,B)))$.
*   **LCM:** $\text{lcm}(A, B) = \frac{A \cdot B}{\gcd(A,B)}$.
*   **Bézout's Identity:** For any integers $A$ and $B$, there exist integers $X$ and $Y$ such that $AX + BY = \gcd(A, B)$.
*   **Extended GCD:** Returns $X$, $Y$, and $\gcd(A,B)$.
    ```cpp
    long long extGCD(long long a, long long b, long long &x, long long &y) {
        if (b == 0) { x = 1; y = 0; return a; }
        long long x1, y1;
        long long d = extGCD(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return d;
    }
    ```

### **4. Modular Arithmetic & Inverse**
*   **Addition:** $(A + B) \pmod M = (A \pmod M + B \pmod M) \pmod M$
*   **Multiplication:** $(A \cdot B) \pmod M = (A \pmod M \cdot B \pmod M) \pmod M$
*   **Subtraction:** $(A - B) \pmod M = (A \pmod M - B \pmod M + M) \pmod M$
*   **Division:** $\frac{A}{B} \pmod M = (A \cdot B^{-1}) \pmod M$. 
    *   $B^{-1}$ is the modular multiplicative inverse of $B$ modulo $M$.
    *   It exists $\iff \gcd(B, M) = 1$.
*   **Fermat's Little Theorem:** If $M$ is prime and $B$ is not divisible by $M$, then $B^{M-1} \equiv 1 \pmod M$.
    *   Implies $B^{-1} \equiv B^{M-2} \pmod M$. Can be found via binary exponentiation in $\mathcal{O}(\log M)$.
    ```cpp
    long long mod_pow(long long a, long long b, long long m) {
        long long res = 1;
        while (b) {
            if (b & 1) res = res * a % m;
            a = a * a % m;
            b >>= 1;
        }
        return res;
    }
    long long mod_inv(long long a, long long m) { return mod_pow(a, m - 2, m); }
    ```
*   **Generalized Modular Inverse:** For any coprime modulus $M$, $B^{-1} \equiv B^{\phi(M)-1} \pmod M$.

### **5. Euler's Totient Function $\phi(n)$**
Counts the integers $\le n$ that are coprime to $n$.
*   **Formula:** $\phi(n) = n \prod_{p|n} \left(1 - \frac{1}{p}\right)$.
    ```cpp
    long long phi(long long n) {
        long long result = n;
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                while (n % i == 0) n /= i;
                result -= result / i;
            }
        }
        if (n > 1) result -= result / n;
        return result;
    }
    ```
*   **Euler's Theorem:** If $\gcd(A, M) = 1$, then $A^{\phi(M)} \equiv 1 \pmod M$.
    *   This is the generalization of Fermat's Little Theorem to non-prime moduli.
*   **Exponent Reduction (Coprime):** $A^N \equiv A^{N \bmod \phi(M)} \pmod M$.
*   **Exponent Reduction (Not Coprime):** For arbitrary $A, M$ and $N \ge \log_2 M$:
    $$ A^N \equiv A^{\phi(M) + [N \bmod \phi(M)]} \pmod M $$
    *(Note: When doing exponentiation, if $N \bmod \phi(M) == 0$, you MUST use $\phi(M)$ as the exponent, not $0$. Computing $A^0 \pmod M$ incorrectly returns 1 if $A$ and $M$ share factors).*

### **6. Linear Diophantine Equations (LDE)**
Finds integer solutions to $Ax + By = C$.
*   **Condition:** A solution exists $\iff \gcd(A, B)$ divides $C$.
*   **Base Solution:** Let $g = \text{extGCD}(A, B, x_0, y_0)$. The base solution is $x = x_0 \cdot \frac{C}{g}$ and $y = y_0 \cdot \frac{C}{g}$.
*   **All Solutions:** 
    $x_k = x + k \cdot \frac{B}{g}$
    $y_k = y - k \cdot \frac{A}{g}$

### **7. Chinese Remainder Theorem (CRT)**
Solves systems of congruences $x \equiv a_i \pmod{m_i}$ for $i=1 \dots K$.
*   If all $m_i$ are pairwise coprime, a unique solution exists modulo $M = \prod m_i$.
*   **Formula:** $x = \left(\sum a_i \cdot M_i \cdot M_i^{-1}\right) \pmod M$, where $M_i = \frac{M}{m_i}$ and $M_i^{-1}$ is the inverse of $M_i \pmod{m_i}$.
    ```cpp
    long long congurence_system(vector<long long> &rems, vector<long long> &mods) {
        long long rem = rems[0], mod = mods[0];
        for (int i = 1; i < (int)rems.size(); i++) {
            long long x, y, found, a = mods[i], b = -mods[i], c = rems[i] - rem;
            long long g = LDE(a, b, c, x, y, found);
            if (!found) return -1; // no solution
            rem += mod * x;
            mod = (mod / g) * mods[i];
            rem = (rem % mod + mod) % mod;
        }
        return rem;
    }
    ```
*   **Computing $F(n)$ Modulo a Composite $C$:**
    1. Factor $C$ into prime powers $C = p_1^{a_1} p_2^{a_2} \dots p_k^{a_k}$.
    2. Compute $F(n) \bmod p_i^{a_i}$ independently.
    3. Use CRT to combine the results into a unique answer modulo $C$.
*   **Using CRT to Avoid Overflow:**
    If computing a huge value $F$ that fits in a 64-bit integer, but intermediate steps overflow:
    1. Pick $k$ small primes such that $P = p_1 \cdot p_2 \dots p_k > F$.
    2. Compute $F \bmod p_i$ for each prime (safe from overflow).
    3. Combine using CRT to get the exact un-moduloed value of $F$.

### **8. Multiplicative Functions & Dirichlet Convolution**
A function is multiplicative if $f(a \cdot b) = f(a) \cdot f(b)$ for $\gcd(a,b)=1$.
*   **Dirichlet Convolution:** $(f * g)(n) = \sum_{d|n} f(d) g\left(\frac{n}{d}\right)$.
*   **Möbius Function $\mu(n)$:**
    *   $\mu(n) = 1$ if $n$ is square-free with an even number of prime factors.
    *   $\mu(n) = -1$ if $n$ is square-free with an odd number of prime factors.
    *   $\mu(n) = 0$ if $n$ has a squared prime factor.
*   **Fundamental Property:** $\sum_{d|n} \mu(d) = 1$ if $n=1$, else $0$.

### **9. Möbius Inversion**
Allows simplifying sums over divisors.
If $g(n) = \sum_{d|n} f(d)$, then $f(n) = \sum_{d|n} \mu(d) g\left(\frac{n}{d}\right)$.
*(Also applies to summation over multiples: If $g(n) = \sum_{n|d} f(d)$, then $f(n) = \sum_{n|d} \mu\left(\frac{d}{n}\right) g(d)$).*

### **10. Primitive Roots & Discrete Logarithm**
*   **Primitive Root:** A number $g$ is a primitive root modulo $M$ if its powers $g^1, g^2, \dots, g^{\phi(M)}$ generate all numbers coprime to $M$. (Exists only for $M = 2, 4, p^k, 2p^k$).
*   **Discrete Logarithm (Baby-Step Giant-Step):** Solves $A^X \equiv B \pmod M$ for $X$ in $\mathcal{O}(\sqrt{M})$.
    *   Let $S = \lceil \sqrt{M} \rceil$. Rewrite $X = p \cdot S - q$.
    *   Equation becomes $A^{p \cdot S} \equiv B \cdot A^q \pmod M$.
    *   Precompute the RHS for all $q \in [0, S)$ into a Hash Map. Then iterate $p \in [1, S]$ and check for a match.

### **11. Advanced Primality Testing & Factorization**
*   **Miller-Rabin:** Probabilistic primality test checking $A^{D} \equiv 1 \pmod P$ or $A^{2^r D} \equiv -1 \pmod P$. Determines if $N \le 10^{18}$ is prime in $\mathcal{O}(k \log^3 N)$.
*   **Pollard's Rho Algorithm:** Finds a non-trivial factor of $N$ in expected $\mathcal{O}(N^{1/4})$ time. Uses Floyd's cycle-finding on the pseudorandom sequence $x_{i+1} = (x_i^2 + c) \pmod N$ checking $\gcd(|x_i - x_j|, N) > 1$.

### **12. Lifting The Exponent Lemma (LTE)**
Finds the highest power of a prime $p$ dividing $x^n \pm y^n$. Let $v_p(x)$ be the exponent of $p$ in the prime factorization of $x$.
*   **Conditions:** $p$ is prime, $p \nmid x$, $p \nmid y$.
*   **For $p > 2$ and $p \mid (x-y)$:**
    $$ v_p(x^n - y^n) = v_p(x - y) + v_p(n) $$
*   **For $p = 2$:**
    *   If $n$ is even and $2 \mid (x-y)$: $v_2(x^n - y^n) = v_2(x - y) + v_2(x + y) + v_2(n) - 1$
    *   If $n$ is odd and $2 \mid (x-y)$: $v_2(x^n - y^n) = v_2(x - y)$
*   **For $p > 2$ and $p \mid (x+y)$ with $n$ odd:**
    $$ v_p(x^n + y^n) = v_p(x + y) + v_p(n) $$

### **13. Zsigmondy's Theorem**
For any coprime integers $a > b > 0$, the sequence $a^n - b^n$ has at least one prime factor that does not divide $a^k - b^k$ for any $k < n$.
**Exceptions:**
1. $a=2, b=1, n=6 \implies 2^6 - 1^6 = 63$ (factors 3 and 7 already divide $2^2-1$ and $2^3-1$).
2. $a+b$ is a power of 2, and $n=2 \implies a^2 - b^2 = (a+b)(a-b)$.

### **14. Quadratic Residues & Tonelli-Shanks**
*   **Euler's Criterion:** An integer $A$ is a quadratic residue modulo an odd prime $P$ (i.e., $X^2 \equiv A \pmod P$ has a solution) if and only if:
    $$ A^{\frac{P-1}{2}} \equiv 1 \pmod P $$
    (If it equals $-1$, it is a non-residue. If $0$, $P \mid A$).
*   **Tonelli-Shanks Algorithm:** Finds $X$ such that $X^2 \equiv A \pmod P$ in $\mathcal{O}(\log^2 P)$.

### **15. Pell's Equation**
Finds integer solutions to $x^2 - d \cdot y^2 = 1$ for a non-square integer $d$.
*   **Fundamental Solution:** Found using the continued fraction expansion of $\sqrt{d}$. Let $\frac{h_i}{k_i}$ be the convergents. The first convergent satisfying the equation gives the fundamental solution $(x_1, y_1)$.
*   **Generating All Solutions:**
    $$ x_k + y_k \sqrt{d} = (x_1 + y_1 \sqrt{d})^k $$

### **16. Sum of Two Squares Theorem (Fermat)**
An integer $N$ can be expressed as the sum of two squares $N = A^2 + B^2$ if and only if every prime factor $p$ of $N$ where $p \equiv 3 \pmod 4$ occurs with an **even** exponent in the prime factorization of $N$.
*(If finding $A$ and $B$, use Thue's Lemma: solve $x^2 \equiv -1 \pmod p$ using Tonelli-Shanks, then apply the Euclidean algorithm step until remainders drop below $\sqrt{p}$).*

### **17. Wilson's Theorem**
An integer $P > 1$ is a prime number if and only if:
$$ (P-1)! \equiv -1 \pmod P $$
*Useful trick:* $(P-k)! \equiv \frac{(-1)^k}{(k-1)!} \pmod P$.

### **18. Carmichael Function $\lambda(n)$**
The smallest positive integer $m$ such that $A^m \equiv 1 \pmod n$ for every integer $A$ coprime to $n$.
*   $\lambda(p^k) = \phi(p^k)$ for odd primes, and for $p=2$ with $k \le 2$.
*   $\lambda(2^k) = \frac{1}{2} \phi(2^k) = 2^{k-2}$ for $k \ge 3$.
*   $\lambda(p_1^{a_1} \dots p_k^{a_k}) = \text{lcm}(\lambda(p_1^{a_1}), \dots, \lambda(p_k^{a_k}))$.

### **19. Min_25 Sieve (Lucy Hedgehog Sieve)**
Computes the prefix sum of a multiplicative function $\sum_{i=1}^N f(i)$ in $\mathcal{O}(N^{3/4} / \log N)$ or $\mathcal{O}(N^{2/3})$.
*   **Requirement:** $f(p)$ is a low-degree polynomial, and $f(p^k)$ can be computed quickly.
*   **Mechanism:** Uses DP to calculate the sum over primes $\le \sqrt{N}$ and smoothly transitions to composite numbers.

### **20. Harmonic Lemma & Divisor Summatory Function**
The sum $\sum_{i=1}^N \lfloor \frac{N}{i} \rfloor$ (which counts the total number of divisors of all numbers up to $N$) can be computed in $\mathcal{O}(\sqrt{N})$ using the Harmonic Lemma:
$$ \sum_{i=1}^N \lfloor \frac{N}{i} \rfloor = 2 \sum_{i=1}^{\lfloor \sqrt{N} \rfloor} \lfloor \frac{N}{i} \rfloor - \lfloor \sqrt{N} \rfloor^2 $$
*Used heavily in Dirichlet hyperbola method and Möbius inversion block-jumping (where $\lfloor N/i \rfloor$ takes at most $2\sqrt{N}$ distinct values).*

### **21. Dirichlet Hyperbola Method**
Used to compute the prefix sum of a Dirichlet convolution $h = f * g$ up to $N$ in $\mathcal{O}(\sqrt{N})$ or $\mathcal{O}(N^{2/3})$:
$$ \sum_{n=1}^N h(n) = \sum_{i=1}^{\lfloor \sqrt{N} \rfloor} f(i) G\left(\lfloor \frac{N}{i} \rfloor\right) + \sum_{j=1}^{\lfloor \sqrt{N} \rfloor} g(j) F\left(\lfloor \frac{N}{j} \rfloor\right) - F(\lfloor \sqrt{N} \rfloor) G(\lfloor \sqrt{N} \rfloor) $$
*(Where $F$ and $G$ are the prefix sums of $f$ and $g$ respectively).*
