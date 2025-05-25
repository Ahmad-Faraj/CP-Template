# Number Theory Knowledge

## Application in Euler's theorem

$\text{if } a \text{ and } m \text{ are relatively prime.}$
$$a^{\phi(m)} \equiv 1 \pmod m \quad $$

In the particular case when $m$ is prime, Euler's theorem turns into **Fermat's little theorem**:

$$a^{m - 1} \equiv 1 \pmod m$$

As immediate consequence we also get the equivalence:

$$a^n \equiv a^{n \bmod \phi(m)} \pmod m$$

This allows computing $x^n \bmod m$ efficiently for $\bf{not}$ $\bf{coprime}$ $x$ and $m$.
For arbitrary $x, m$ and $n \geq \log_2 m$:

$$x^{n}\equiv x^{\phi(m)+[n \bmod \phi(m)]} \mod m$$

> [!NOTE]
> [Problem](https://codeforces.com/gym/100975/problem/F) $(2^{2^n} + 1)\mod k$
>
> Suppose pow = $(\phi(m)+[n \bmod \phi(m)]) \mod \phi(m)$
>
> if (pow == 0) pow = $\phi(m)$

**Why Handle pow = 0?**

In your code, you compute:

$$
\text{pow} = 2^n \mod \phi(k)
$$

However, when $2^n$ is a multiple of $\phi(k)$, the result of $2^n \mod \phi(k)$ will be 0. But this creates an issue when using it in modular exponentiation:

$$
2^{\text{pow}} \mod k
$$

If $\text{pow} = 0$, it means you're computing:

$$
2^0 \mod k
$$

which is always 1. But mathematically, when $n$ is large, we expect:

$$
2^n \equiv 2^{\phi(k) + (n \bmod \phi(k))} \mod k
$$

By Euler’s theorem:

$$
x^{\phi(m)} \equiv 1 \mod m
$$

so we need to adjust by replacing $0$ with $\phi(k)$ because:

$$
2^{\phi(k)} \equiv 1 \mod k
$$

Thus, when $\text{pow} = 0$, setting it to $\phi(k)$ ensures the exponentiation still produces a meaningful result.

---

## Modular Inverse

* For an arbitrary (but coprime) modulus $m$: $a ^ {\phi (m) - 1} \equiv a ^{-1} \mod m$
* For a prime modulus $m$: $a ^ {m - 2} \equiv a ^ {-1} \mod m$
* if a and m are coprime, then $a ^ {-1} \mod m$ can be found using the extended Euclidean algorithm.

## Computing $F(n)$ Modulo a Composite Number

### Notes

1.
   We want to compute:
   $F(n) \bmod C$
   where $C$ is **not** prime.

1. **Why Factor $C$?**  
   * When $C$ is prime, we can use straightforward tools (like Fermat’s Little Theorem) because $\mathbb{Z}/p\mathbb{Z}$ is a field.  
   * For a composite $C$, $\mathbb{Z}/C\mathbb{Z}$ is **not** a field, so direct methods (inverses, etc.) can be more complicated.  

2. **Factor $C$ into Prime Powers**  
   - Express $C$ as:
     $C = p_1^{a_1} \times p_2^{a_2} \times \cdots \times p_k^{a_k}.$
   - Example: If $C = 12$, then $12 = 2^2 \times 3^1$.

3. **Compute $F(n)$ Mod Each Prime Power**  
   - For each prime power $p_i^{a_i}$, compute:
     $F(n) \bmod p_i^{a_i}.$
   - Techniques for prime powers often involve:
     - Euler’s theorem (generalized).
     - Hensel’s lemma (for certain polynomial lifts).
     - Lifting exponent lemmas (for factorials, binomial coefficients, etc.).

4. **Use the Chinese Remainder Theorem (CRT)**  
   - The prime-power factors $p_i^{a_i}$ are pairwise coprime.  
   - By CRT, if you know:
     $$x \equiv r_i \pmod{p_i^{a_i}}$$
     for each $i$, then there is a **unique** solution for
     $$x \bmod \bigl(p_1^{a_1} \cdot p_2^{a_2} \cdots p_k^{a_k}\bigr),$$
     which is $x \bmod C$.

5. **Example: $C = 12$**  
   1. Factor $12$ into prime powers: $12 = 2^2 \times 3^1$.  
   2. Compute:
      $$F(n) \bmod 4, \quad F(n) \bmod 3.$$
   3. Combine the results using CRT:
      - Suppose $F(n) \equiv r_4 \pmod{4}$ and $F(n) \equiv r_3 \pmod{3}$.  
      - There is a unique $r_{12}$ with
        $$r_{12} \equiv r_4 \pmod{4}, \quad r_{12} \equiv r_3 \pmod{3}.$$
      - Then $r_{12} \equiv F(n) \pmod{12}$.

## **Chinese Remainder Theorem (CRT) Usage for Avoiding Overflow**

- We need to compute a function $F(0)$, which can fit in a 32-bit integer.
- However, intermediate calculations may cause overflow if performed directly.

**Approach: Using Modular Arithmetic**
1. **Select a Large Modulus $M$**  
   - Choose $M$ as a product of several prime numbers:  
     $$M = p_1 \times p_2 \times \dots \times p_k$$
   - Ensure that $M$ is large enough (e.g., more than 32 bits).  
   - If $F(0) < M$, then computing $F(0) \mod M$ is equivalent to computing $F(0)$.

2. **Break Down the Computation Using Primes**  
   - Instead of computing $F(0) \mod M$ directly, compute:  
     $$F(0) \mod p_1, \quad F(0) \mod p_2, \quad \dots, \quad F(0) \mod p_k$$
   - Each prime $p_i$ is small enough to avoid overflow.

3. **Example of Prime Factorization**
   - Suppose we choose:
     $$M = 257 \times 263 \times 269 \times 271$$
   - Each prime $p_i$ is small enough for safe modular computations.

4. **Reconstruct the Result Using CRT**  
   - Once we have $F(0) \mod p_i$ for all $p_i$, apply the **Chinese Remainder Theorem (CRT)** to reconstruct $F(0) \mod M$.
   - Since the primes are pairwise coprime, CRT guarantees a unique solution.

## Others

* The Chicken McNugget Theorem states that for any two relatively prime positive integers $m$ and $n$, the greatest integer that $\bf{cannot}$ be written in the form $am + bn$ for $\bf{nonnegative}$ integers $a$ and $b$ is $mn - m - n$.
* A consequence of the theorem is that there are exactly $\frac{(m - 1)(n - 1)}{2}$ positive integers which cannot be expressed in the form $am + bn$.
* The Generalized form of the Chicken McNugget Theorem states that for any two positive integers $m$ and $n$, all multiples of $\gcd(m, n)$ greater than $\textrm{lcm}(m, n)-m-n$ are representable in the form $am+bn$ for some positive integers $a, b$.
