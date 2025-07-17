# Pushing NTT Beyond Limits: Fast Half-NTT for Huge Polynomial Multiplication


## Motivation: The Limits of NTT

NTT (Number Theoretic Transform) is a powerful tool for multiplying polynomials efficiently under a modulus, with time complexity $O(n \log n)$. It requires a special kind of prime modulus:

$$
\text{mod} = k \cdot 2^s + 1
$$

A popular one is:

$$
\boxed{998244353 = 119 \cdot 2^{23} + 1}
$$

This prime allows NTT to work efficiently up to size $2^{23} = 8,388,608$, which is great — until it's not.

What if you're given two large polynomials with degrees:

* $n = 2^{24}$
* $m = 2^{24}$

Then the convolution size becomes:

$$
n + m - 1 \approx 2^{25}
$$

This **exceeds the NTT limit** for `998244353`, making standard NTT **inapplicable**.

---

## The Problem

We want to perform polynomial multiplication under modulus `998244353`, but input size is larger than $2^{23}$, the maximum NTT-friendly length.

### You **can’t**:

* Use normal NTT — the root of unity for such size doesn’t exist under this modulus.

### You **don’t want**:

* Floating-point FFT (inaccurate for exact integer modulo arithmetic)
* CRT with multiple moduli (extra complexity and memory)

---

## The Solution: Recursive Fast Half-NTT

### Key Idea

Instead of building a massive NTT transform, we:

* **Recursively split** the problem into two subproblems using root of unity identities.
* **Solve smaller convolutions** (within size limit).
* **Combine** them using inverse interpolation.

This method:

* Never requires an NTT of size > $2^{23}$
* Keeps all operations within `998244353`
* Works efficiently even for huge inputs

---

## How It Works

### Step 1: Root-Based Decomposition

Given a polynomial $A(x)$ of degree $n$, split it like this:

```text
A_minus[i] = A[i] + ω^x * A[i + n/2]
A_plus[i]  = A[i] - ω^x * A[i + n/2]
```

Where:

* $ω$ is a primitive root of unity modulo `998244353`
* $x$ determines which power of root to use at each level

Do the same for $B(x)$

### Step 2: Recursive Multiplication

Now multiply:

* $A_{-} \cdot B_{-}$ using `x/2`
* $A_{+} \cdot B_{+}$ using `x/2 + mod/2`

Repeat this recursively until size ≤ threshold (`lim`, e.g. 64), then use naive $O(n^2)$ multiplication.

### Step 3: Interpolate

After computing the two products, combine them back:

```cpp
C[i]           = (res_minus[i] + res_plus[i]) / 2
C[i + n/2]     = (res_minus[i] - res_plus[i]) / (2 * sqrt_c)
```

All operations are done modulo `998244353`.

---

## Code Skeleton

```cpp
vector<int> fast_ntt_poly_mul(vector<int>& a, vector<int>& b) {
    // Resize a, b to next power of two above n + m - 1
    // Recursively multiply
    // Return trimmed result
}
```

This is a clean and efficient recursive algorithm with minimal overhead.

---

## Performance

* Handles sizes up to $2^{25}$ with **no problem**
* Memory-efficient: uses divide-and-conquer strategy
* Competitive with iterative NTT for large inputs

---

## When Should You Use This?

 When:

* You have large polynomials (bigger than $2^{23}$)
* You want to stay within modulus `998244353`
* You don’t want to deal with CRT

 Avoid if:

* Your inputs are small (use iterative NTT instead)
* You don't have a good NTT-friendly modulus (e.g., $10^9 + 7$)

---

## Can This Be Generalized?

### Yes, **but only to other NTT-friendly primes**.

Requirements:

* The modulus $p = k \cdot 2^s + 1$
* Existence of roots of unity for various powers
* Ability to divide by 2 (modular inverse of 2 exists)

Examples:

* 998244353
* 167772161
* 7340033

### Not applicable for:

* $10^9 + 7$, $10^9 + 9$, etc.
* Non-NTT-friendly primes

In such cases, you’ll need multi-modulus + CRT or float FFT.

---

## Summary

| Feature         | Recursive Fast-NTT     |
| --------------- | ---------------------- |
| Max size        | Any (adaptive)         |
| Modulus         | 998244353 (or similar) |
| Time            | $O(n \log n)$          |
| Space           | $O(n)$                 |
| Uses CRT?       |  No                   |
| Works with FFT? |  No                   |

---
