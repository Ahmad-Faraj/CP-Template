## Matrix Representation & Exponentiation ($O(\log N)$)

**Core Idea:** Represent the recurrence relation as a linear transition matrix. This turns computing any term, range update, or state shift into matrix exponentiation.

* **The Companion Transition Matrix:**

$$\begin{bmatrix} 1 & 1 \\ 1 & 0 \end{bmatrix}^n = \begin{bmatrix} \text{fib}(n + 1) & \text{fib}(n) \\ \text{fib}(n) & \text{fib}(n - 1) \end{bmatrix}$$

* **Determinant & Invariant (Yields Cassini's Identity):**

$$\det \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n = \left(\det \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}\right)^n = (-1)^n \implies \text{fib}(n + 1)\text{fib}(n - 1) - \text{fib}(n)^2 = (-1)^n$$



## Index Shifting & Linear Decomposition

* **Forward Shift (Standard Basis):**

$$\text{fib}(n + z) = \text{fib}(z - 1)\text{fib}(n) + \text{fib}(z)\text{fib}(n + 1)$$


* **Forward Shift (Backward Basis):**

$$\text{fib}(n + z) = \text{fib}(z + 1)\text{fib}(n) + \text{fib}(z)\text{fib}(n - 1)$$


* **Negative / Backward Shift:**

$$\text{fib}(n - z) = (-1)^z \Big( \text{fib}(z + 1)\text{fib}(n) - \text{fib}(z)\text{fib}(n + 1) \Big)$$


* **D'Ocagne's Identity:**

$$\text{fib}(m)\text{fib}(n + 1) - \text{fib}(m + 1)\text{fib}(n) = (-1)^n \text{fib}(m - n)$$


* **Even Index Formula:**

$$\text{fib}(2k) = \text{fib}(k) \cdot \Big( 2\text{fib}(k + 1) - \text{fib}(k) \Big)$$


* **Odd Index Formula:**

$$\text{fib}(2k + 1) = \text{fib}(k + 1)^2 + \text{fib}(k)^2$$


* **Tripling Formulas:**

$$\text{fib}(3k) = 5\text{fib}(k)^3 + 3(-1)^k \text{fib}(k)$$


$$\text{fib}(3k) = \text{fib}(k + 1)^3 + \text{fib}(k)^3 - \text{fib}(k - 1)^3$$



## Prefix Sums & Range Accumulation

* **Standard Prefix Sum:**

$$\sum_{i=1}^{n} \text{fib}(i) = \text{fib}(n + 2) - 1$$


* **Odd Indices Sum:**

$$\sum_{i=1}^{n} \text{fib}(2i - 1) = \text{fib}(2n)$$


* **Even Indices Sum:**

$$\sum_{i=1}^{n} \text{fib}(2i) = \text{fib}(2n + 1) - 1$$


* **Alternating Signs Sum:**

$$\sum_{i=1}^{n} (-1)^{i+1}\text{fib}(i) = (-1)^{n+1}\text{fib}(n - 1) + 1$$


* **Sum of Squares:**

$$\sum_{i=1}^{n} \text{fib}(i)^2 = \text{fib}(n) \cdot \text{fib}(n + 1)$$


* **Sum of Products of Adjacent Terms:**

$$\sum_{i=1}^{n} \text{fib}(i)\text{fib}(i + 1) = \begin{cases} \text{fib}(n + 1)^2 - 1 & \text{if } n \text{ is odd} \\ \text{fib}(n + 1)^2 & \text{if } n \text{ is even} \end{cases}$$


* **Weighted Linear Sum ($\sum i \cdot F_i$):**

$$\sum_{i=1}^{n} i \cdot \text{fib}(i) = n \cdot \text{fib}(n + 2) - \text{fib}(n + 3) + 2$$



## Non-Linear & Determinant Identities

* **Cassini's Identity:**

$$\text{fib}(n - 1)\text{fib}(n + 1) - \text{fib}(n)^2 = (-1)^n$$


* **Catalan's Identity (Cassini Generalization):**

$$\text{fib}(n - r)\text{fib}(n + r) - \text{fib}(n)^2 = (-1)^{n - r + 1}\text{fib}(r)^2$$


* **Gelin-Cesàro Identity:**

$$\text{fib}(n - 2)\text{fib}(n - 1)\text{fib}(n + 1)\text{fib}(n + 2) - \text{fib}(n)^4 = -1$$


* **Convolution Identity:**

$$\sum_{i=0}^{n} \text{fib}(i)\text{fib}(n - i) = \frac{n \cdot \text{fib}(n + 1) + 2(n + 1)\text{fib}(n)}{5}$$



## Number Theory & Divisibility Properties

* **GCD Identity (Crucial in CP):**

$$\gcd(\text{fib}(a), \text{fib}(b)) = \text{fib}(\gcd(a, b))$$


* **Trick:** Never compute GCD on large values; replace with index GCD: $\text{fib}(\gcd(a, b)) \pmod M$.
  


* **Successive Coprimality:**

$$\gcd(\text{fib}(n), \text{fib}(n + 1)) = 1 \quad \forall \, n \ge 1$$


* **Divisibility Condition:**

$$\text{fib}(a) \mid \text{fib}(b) \iff a \mid b \quad (\text{for } a \ge 3)$$


* **Factorization over Powers ($F_{nk}$):**

$$\text{fib}(n) \mid \text{fib}(nk) \quad \forall \, k \ge 1$$



## Pisano Period ($\pi(M)$) & Huge Exponent Reductions

**Core Idea:** The sequence is strictly periodic modulo any integer $M$. This allows computing $\text{fib}(N) \pmod M$ when $N = a^b$ or $N$ has millions of digits.

* **Core Reduction Rule:**

$$\text{fib}(N) \equiv \text{fib}(N \pmod{\pi(M)}) \pmod M$$


* **Pisano Period Bounds:**
* $\pi(M) \le 6M$, with equality achieved if and only if $M = 2 \times 5^k$.
* For prime $p$:
* If $p \equiv \pm 1 \pmod 5 \implies \pi(p) \mid (p - 1)$
* If $p \equiv \pm 2 \pmod 5 \implies \pi(p) \mid (2p + 2)$




* **Key Precomputed Moduli for CP:**
* $\pi(10^9 + 7) = 2{,}000{,}000{,}016$
* $\pi(998244353) = 998{,}244{,}352 \times 2$
* $\pi(10^k) = 1.5 \times 10^k \quad (\text{for } k \ge 3)$



## Zeckendorf's Theorem & Greedy Representation

**Core Idea:** Every positive integer can be uniquely represented as the sum of one or more non-consecutive Fibonacci numbers.

* **Statement:**

$$N = \sum_{i=1}^{k} \text{fib}(c_i) \quad \text{where } c_1 \ge 2, \; c_{i+1} \ge c_i + 2$$


* **Algorithm (Greedy):** Find the largest $\text{fib}(k) \le N$, subtract it, and repeat with the remainder.
* **Application:** Used in Fibonacci Nim (game theory) where the unique winning move for the first player is removing the smallest Fibonacci component in the Zeckendorf representation of the pile size.