# Applications of FFT

1. Ploynomials Multiplication
<details>
    <summary>statement</summary>
    <p>Given two polynomials of length n and m output the coefficients of their product polynomial.</p>
    template solution
</details>
2. Ploynomials power
   <details>
       <summary>statement</summary>
       <p>Given a polynomial of length n and an integer k output the coefficients of the polynomial raised to the power k. or first m coefficients if the number of coefficients is very large.</p>
       template solution

   > [!NOTE]
   > Make sure to use the limit on the number of coefficients.

   > [!WARNING]check correctness
   > if raising to power k in NTT, you can raise the coefficients to the power k in the frequency domain and then apply the inverse NTT to get the coefficients of the polynomial raised to the power k.
   >
   > ```cpp
   > for (int i = 0; i < n; i++)
   >    fa[i] = (int)(fa[i] * fb[i] % MOD);
   > ```

   </details>

3. Multiply large numbers
<details>
    <summary>statement</summary>
    <p>Given two large numbers represented as strings, output their product.</p>
    template solution
</details>

4. Generate all pair sum
   <details>
   <summary>statement</summary>

   Given an array of integers, output the sum of all pairs of integers in the array. $0 <= a_i <= 10^5$
   </details>
   <details>
   <summary>solution</summary>

   we will convert the array to be in form of $F = \sum_{a \in A}freq(a)x^a$
   then we will compute $F^2$ using FFT, each coefficient represent the number of pairs $(i, j)$ to form the sum of the corresponding index. and no guarantee that $i \neq j$.
   if you want to guarantee a condition compute the total and then remove the invalid pairs.
   </details>

5. Generate all pair difference $ai - aj$
   <details>
   <summary>statement</summary>

   Given an array of integers, output the difference of all pairs of integers in the array. $0 <= a_i <= 10^5$
   </details>
   <details>
   <summary>solution</summary>

   we will convert the array to be in form of $F1 = \sum_{a \in A}freq(a)x^a$ and $F2 = \sum_{a \in A}freq(a)x^{-a + SHIFT}$

   > [!NOTE]
   > Make sure to use a large enough SHIFT to avoid negative indices.
   > we use SHIFT to keep it polynomial, so we can use FFT.
   > but the exponent if the multiplication will be $a_i - a_j + SHIFT$ so to access the value add SHIFT any where we access the index.
   > and in general if you use any SHIFT in any polynomial, the result will be shifted by the total SHIFT.

   then we will compute $F1 * F2$ using FFT, each coefficient represent the number of pairs $(i, j)$ to form the difference of the corresponding index $-SHIFT$. and no guarantee that $i \neq j$.
   </details>

6. Generate all subarray sum
   <details>
   <summary>statement</summary>

   Given an array of integers, Generate all subarray sums. $0 <= a_i <= 10^5, \sum_{i = 1}^n a_i <= 10^5$
   </details>
   <details>
   <summary>solution</summary>

   the sub array sum can be represented as difference of two prefix sums, so we can use the same idea as the previous problem.
   we will convert the array to be in form of $F1 = \sum_{pref \in A}freq(pref)x^{pref}$ and $F2 = \sum_{pref \in A}freq(pref)x^{-pref + SHIFT}$, and note that the polynomial multiplication doesn't guarantee that $pref[j] - pref[i]$ and $j >= i$ and this is wrong so we have a condition that $a_i >= 0$ so we will access only the positive indices because it will counted correctly, because all the prefixes are non-negative and the positive indices guarantee that $j >= i$ and $pref[j] - pref[i]$ is non-negative.
   then we will compute $F1 * F2$ using FFT, each coefficient represent the number of pairs $(i, j)$ to form the difference of the corresponding index $-SHIFT$. but the FFT will overcount the subarray sums that have sum = 0, because the sum 0 can be formed from $j > i \mid j < i \mid j = i$ so compute the sumarrays that have sum = 0 manually
   > [!NOTE]
   > don't forget to mark the sum of 0 to frequency 1, because the empty subarray is a valid subarray and its sum is 0.
   </details>

7. Generate all subset sum
   <details>
   <summary>statement</summary>

   Given an array of integers, Generate all subset sums.

   > [!NOTE]
   > take care of the constraints, if the sum of the array is very large, you may need to use a different approach. or if the problem satisfies a limit on the number of coefficients, you can use that to limit the number of coefficients in the polynomial.

   </details>
   <details>
   <summary>solution</summary>

   the subset is like take or leave the element, so we can represent each element as a polynomial of the form $F = (1 + x^{a_i})$ where $a_i$ is the value of the element, and then we can multiply all the polynomials to get the subset sums.

   > [!NOTE]
   > take care of the distinct condition, if you only take the element once, but with different ways like number of occurrences you can represent the element as $F = (1 + freq(a_i) * x^{a_i})$ where $freq(a_i)$ is the number of occurrences of the element.
   > or if you can take the element multiple times, you can represent each occurrence as $F = (1 + x^{a_i})$.

   > [!TIP]
   > for this problem the multiplication of the polynomials will take time if you multiply in order, so to optimize use priority queue to multiply the first two polynomials with small size, store only size and index rather than the whole vector, also after multiplication store the result in any of the two polynomials and push the result back to the queue, and repeat until you have one polynomial left.

   </details>

### FFT relative to index

Some ideas to use the index as the exponent in the polynomial and the element as the coefficient

1. Multiplication with distance
   <details>
   <summary>statement</summary>

   Given two arrays $a$ and $b$ of integers, and q queries, each query you will be given $x$ and you need to output the value of $\sum_{i = 1}^n a_i * b_{i + x}$ and if $i + x > n$ assume $b_{i + x} = 0$.
   </details>
   <details>
   <summary>solution</summary>

   we can represent the two arrays as polynomials $F_a = \sum_{i = 1}^n a_i x^i$ and $F_b = \sum_{i = 1}^n b_i x^{-i + SHIFT}$ and then we can compute the product of the two polynomials $F_a * F_b$ using FFT, and then we can answer each query in O(1) by accessing the coefficient of the polynomial at index $x$.
   </details>

2. same as above but different representation
   <details>
   <summary>statement</summary>

   Given two arrays $a$ and $b$ of integers, and $q$ queries, each query will be one of the following:

   1. output the value of $\sum_{i = 1}^n a_i * b_{i}$.
   2. cyclic shift the array $b$ to the right by $x$.
   </details>
   <details>
   <summary>solution</summary>

   to solve this problem $b = b + b$ and $a = {0,0,...,0} + a$ append $b$ at the front of $b$ and append $n$ zeros at the front of $a$ thit guarantee that if the array $b$ is shifted right the value will be correct, and then we can represent the two arrays as polynomials $F_a = \sum_{i = 1}^n a_i x^i$ and $F_b = \sum_{i = 1}^n b_i x^{-i + SHIFT}$ and then we can compute the product of the two polynomials $F_a * F_b$ using FFT, and then we can answer each query in $O(1)$ by accessing the coefficient of the polynomial at index $x$.
   make sure to accumulate the total cyclic shifts and take modulo $n$ to get the correct index.
   </details>

3. Problem 3
   <details>
   <summary>statement</summary>

   Given an array $a$ of integers consist of only $2$ or $3$, and $q$ queries, each query will be one of the following:

   1. 2 $x$ output the number of $2s$ pairs that have difference between their indices equal to $x$.
   2. 3 $x$ output the number of $3s$ pairs that have difference between their indices equal to $x$.
   </details>
   <details>
   <summary>solution</summary>

   we will solve for each number separately, so we will have two polynomials $F_2$ and $F_3$.
   $F_2$ will be the polynomial of ${0, 1}$ each coefficient represent that the index is $2$ or not, and $F_3$ will be the polynomial of ${0, 1}$ each coefficient represent that the index is $3$ or not.

   solve for $2$ as above problem $F_2 * F_2$ one with $i$ and other with $-i + SHIFT$ and then multiply the two polynomials to get the result.

   for $3$ same as $2$ and answer the queries in $O(1)$ by accessing the coefficient of the polynomial at index $x$.

   > [!NOTE]Bonus
   > if we have one more type of query of the form `2 3 x` output the number of pairs of $2s$ and $3s$ that have difference between their indices equal to $x$.
   > we can solve this by using the same idea as above, but we will have two polynomials $F_2$ and $F_3$ and we will compute the product of the two polynomials $F_2 * F_3$ using FFT, and then we can answer each query in $O(1)$ by accessing the coefficient of the polynomial at index $x$.
   > but make sure one with $i$ and other with $-i + SHIFT$ and then multiply the two polynomials to get the result.

   </details>

### String matching

1. Match a pattern consist of a wildcard character
   <details>
   <summary>statement</summary>

   Given a string $s$ and a pattern $p$ that contains a wildcard character `#` that can match any character, output the number of occurrences of the pattern in the string.
   </details>
   <details>
   <summary>solution</summary>

   we will solve for each character from `a` to `z` separately,
   so we will have for each character polynomial for the string $s$ and a polynomial for the pattern $p$.
   each polynomial's coefficients will be ${0, 1}$
   where the coefficient at index $i$ is $1$ if the character at index $i$ in the string or pattern is equal to the character we are solving for, and $0$ otherwise.

   then we will compute the product of the two polynomials using FFT one has $i$ the pattern has $-i + SHIFT$ and accumulate the answers in a global vector for all characteres from 'a' to 'z'.
   `ans[i + SHIFT]` is the number of matched characters at index $i$
   </details>

## Multiplication with arbitrary modulus

Here we want to achieve the same goal as in previous section.
Multiplying two polynomial $A(x)$ and $B(x)$, and computing the coefficients modulo some number $M$.
The number theoretic transform only works for certain prime numbers.
What about the case when the modulus is not of the desired form?

One option would be to perform multiple number theoretic transforms with different prime numbers of the form $c 2^k + 1$, then apply the [Chinese Remainder Theorem](chinese-remainder-theorem.md) to compute the final coefficients.

Another options is to distribute the polynomials $A(x)$ and $B(x)$ into two smaller polynomials each

$$\begin{align}
A(x) &= A_1(x) + A_2(x) \cdot C \\
B(x) &= B_1(x) + B_2(x) \cdot C
\end{align}$$

with $C \approx \sqrt{M}$.

Then the product of $A(x)$ and $B(x)$ can then be represented as:

$$A(x) \cdot B(x) = A_1(x) \cdot B_1(x) + \left(A_1(x) \cdot B_2(x) + A_2(x) \cdot B_1(x)\right)\cdot C + \left(A_2(x) \cdot B_2(x)\right)\cdot C^2$$

$A_1(x)$ -> is the polynomial containing the remiander of the coefficients of $A(x)$ when divided by $C$.

$A_2(x)$ -> is the polynomial containing the coefficients of $A(x)$ divided by $C$.

$B_1(x)$ -> is the polynomial containing the remiander of the coefficients of $B(x)$ when divided by $C$.

$B_2(x)$ -> is the polynomial containing the coefficients of $B(x)$ divided by $C$.

The polynomials $A_1(x)$, $A_2(x)$, $B_1(x)$ and $B_2(x)$ contain only coefficients smaller than $\sqrt{M}$, therefore the coefficients of all the appearing products are smaller than $M \cdot n$, which is usually small enough to handle with typical floating point types.

This approach therefore requires computing the products of polynomials with smaller coefficients (by using the normal FFT and inverse FFT), and then the original product can be restored using modular addition and multiplication in $O(n)$ time.
