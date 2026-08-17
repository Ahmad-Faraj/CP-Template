# Combinatorics

### **0. Fundamental Rules**
*   **Permutations ($P(n, r)$):** Arrange $r$ distinct items from $n$ distinct items. Formula: $\frac{n!}{(n-r)!}$
*   **Combinations ($\binom{n}{r}$):** Choose $r$ distinct items from $n$ distinct items (order doesn't matter). Formula: $\frac{n!}{r!(n-r)!}$
*   **Multiset Permutations:** Arrange $N$ items where group 1 has $k_1$ identical items, group 2 has $k_2$, etc. Formula: $\frac{N!}{k_1! k_2! \dots}$
*   **Circular Permutations:** Arrange $N$ distinct items in a circle. Formula: $(N-1)!$. *(If reflections are identical, e.g., a necklace, divide by 2: $\frac{(N-1)!}{2}$)*
*   **Stars and Bars (Standard):** Distribute $N$ identical balls into $K$ distinct bins (each bin can have $\ge 0$). Formula: $\binom{N+K-1}{K-1}$
*   **Stars and Bars (Positive):** Distribute $N$ identical balls into $K$ distinct bins (each bin must have $\ge 1$). Formula: $\binom{N-1}{K-1}$
*   **Pigeonhole Principle:** If $N$ items are put into $K$ containers, at least one container must hold $\lceil N/K \rceil$ items.

### **1. Core Binomial Properties**
*   **Symmetry:** $\binom n k = \binom n {n-k}$
    *Meaning: Choosing $k$ items to KEEP is the exact same as choosing $n-k$ items to THROW AWAY.*
*   **Factoring in (Absorption):** $\binom n k = \frac n k \binom {n-1} {k-1}$
    *Meaning: Selecting a team of $k$ people and then picking a captain is the same as first picking the captain from $n$ people, then choosing the remaining $k-1$ team members.*
*   **Sum over $k$:** $\sum_{k = 0}^n \binom n k = 2 ^ n$
    *Meaning: The total number of subsets of any size you can form from $n$ items is exactly $2^n$ (each item is either IN or OUT).*
*   **Sum over $n$ (Hockey Stick):** $\sum_{m = k}^n \binom m k = \binom {n + 1} {k + 1}$
    *Meaning: If you want to choose $k+1$ items from $n+1$ items, imagine the highest indexed item you picked is at position $m+1$. Then you must choose $k$ items from the remaining $m$ items before it.*
*   **Vandermonde's Identity:** $\sum_{k = 0}^r  \binom {n} k \binom {m} {r-k} = \binom {n + m} r$
    *Meaning: Choosing $r$ items from a combined pool of $n+m$ is the same as choosing $k$ from the first pool and $r-k$ from the second pool, summed over all possible values of $k$.*
*   **Sum of the squares:** $\sum_{k=0}^n \binom n k^2 = \binom {2n} n$
    *Meaning: A direct result of Vandermonde's Identity when splitting a pool of $2n$ into two halves of $n$.*
*   **Weighted sum:** $\sum_{k=1}^n k \binom n k = n 2^{n-1}$
    *Meaning: You want to pick a subset and elect a leader. You can either pick the leader first ($n$ choices) and then invite anyone else ($2^{n-1}$ subsets), OR pick a subset of size $k$ first ($\binom{n}{k}$) and then pick one of them to lead ($k$).*
*   **Fibonacci Connection:** $\sum_{k=0}^{\lfloor n/2 \rfloor} \binom{n-k}{k} = F_{n+1}$
    *Meaning: The sum of the shallow diagonals in Pascal's triangle naturally generates the Fibonacci sequence.*

### **2. The Twelvefold Way (Balls into Boxes)**
Placing $N$ balls into $K$ boxes. The formulas change drastically based on whether the balls/boxes look identical or unique.

**Scenario A: BOTH are Unique (Distinguishable Balls & Boxes)**
*   **Any arrangement:** $K^N$ (Each of the $N$ balls has $K$ distinct choices of which box to go into).
*   **$\le 1$ ball per box (Injective):** $P(K, N) = \frac{K!}{(K-N)!}$ (Pick $N$ ordered boxes for the $N$ balls).
*   **$\ge 1$ ball per box (Surjective):** $K! \cdot S(N, K)$ (Partition $N$ balls into $K$ non-empty sets, then assign each set to a unique box).

**Scenario B: Balls are Identical, Boxes are Unique**
*   **Any arrangement (Stars & Bars):** $\binom{N+K-1}{K-1}$ (Arrange $N$ identical stars and $K-1$ bars to divide them into $K$ distinct boxes).
*   **$\ge 1$ ball per box:** $\binom{N-1}{K-1}$ (Give 1 ball to each box first to guarantee they aren't empty, then distribute the remaining $N-K$ balls using Stars & Bars).
*   **$\le 1$ ball per box:** $\binom{K}{N}$ (Simply choose which $N$ boxes get exactly 1 ball).

**Scenario C: Balls are Unique, Boxes are Identical**
*   **Any arrangement:** $\sum_{i=1}^K S(N, i)$ (Partition $N$ balls into up to $K$ sets. If $K=N$, this sum is the **Bell Number** $B_N$).
*   **$\ge 1$ ball per box:** $S(N, K)$ (Stirling Number of the Second Kind: partition $N$ unique balls into exactly $K$ identical unlabelled sets).
*   **$\le 1$ ball per box:** $1$ if $N \le K$, else $0$ (Since boxes are identical, there's only 1 valid way to put them in separate boxes).

**Scenario D: BOTH are Identical**
*   **Any arrangement:** $P_K(N)$ (The number of integer partitions of $N$ into $\le K$ parts).
*   **$\ge 1$ ball per box:** $P_K(N - K)$ (Give 1 to each box, then partition the remaining $N-K$ balls).
*   **$\le 1$ ball per box:** $1$ if $N \le K$, else $0$.

### **3. Stars and Bars with Upper Bounds**
Find the number of solutions to $x_1 + x_2 + \cdots + x_n = S$ with $0 \le x_i \le U$.
We use Inclusion-Exclusion to subtract invalid combinations where some $x_i \ge U+1$:
$$ F(n,S;U) = \sum_{r = 0}^{\lfloor S/(U+1)\rfloor} (-1)^r \binom{n}{r} \binom{n + S - r(U+1) - 1}{S - r(U+1)} $$

**Special Cases:**
1. **Binary Variables ($U=1$):** Each $x_i \in \{0,1\}$. Reduces to $\binom{n}{S}$.
2. **Ternary Bound ($U=2$):** Each $x_i \in \{0,1,2\}$. Extremely common subproblem (e.g., subset sum variations):
    $F(n,S;2) = \sum_{r=0}^{\lfloor S/3\rfloor} (-1)^r \binom{n}{r} \binom{n + S - 3r - 1}{S - 3r}$

**Implementation ($\mathcal{O}(S / U)$):**
```cpp
long long countBounded(int n, int S, int U) {
    if (S < 0 || S > 1LL * n * U) return 0;
    int rmax = S / (U + 1);
    long long ans = 0;
    for(int r = 0; r <= rmax; r++) {
        long long choose = nCr(n, r);
        int rem = S - r * (U + 1);
        long long ways = nCr(n + rem - 1, rem);
        long long term = (choose * ways) % MOD;
        if (r & 1) term = (MOD - term) % MOD;
        ans = (ans + term) % MOD;
    }
    return ans;
}
```
*Note: If upper bound $U$ differs for each variable, apply PIE over subsets of variables exceeding their individual bound. If $n \le 20$, use bitmasks.*

### **4. Lucas Theorem**
Calculates $\binom{N}{K} \pmod P$ for massive $N, K$ (e.g. $10^{18}$) but small prime $P$ ($< 10^5$) in $\mathcal{O}(P + \log_P N)$.
$$ \binom{N}{K} \equiv \prod_{i} \binom{n_i}{k_i} \pmod P $$
Where $n_i$ and $k_i$ are the digits of $N$ and $K$ in base $P$.
```cpp
long long lucas(long long n, long long k, int p) {
    if (k == 0) return 1;
    int ni = n % p, ki = k % p;
    if (ki > ni) return 0;
    return lucas(n / p, k / p, p) * nCr(ni, ki) % p; 
    // nCr precomputed up to P
}
```

### **5. Stirling Numbers of the Second Kind $S(N, K)$**
The number of ways to partition $N$ labeled elements into exactly $K$ unlabelled non-empty subsets.
*   **Recurrence:** $S(N, K) = K \cdot S(N-1, K) + S(N-1, K-1)$
*   **Explicit Formula (via PIE):** $S(N, K) = \frac{1}{K!} \sum_{i=0}^K (-1)^{K-i} \binom{K}{i} i^N$
*   *Can be calculated for a fixed $N$ over all $K$ in $\mathcal{O}(N \log N)$ using FFT (it's a convolution).*

### **6. Stirling Numbers of the First Kind $c(N, K)$**
The number of permutations of length $N$ with exactly $K$ cycles.
*   **Recurrence:** $c(N, K) = (N-1) \cdot c(N-1, K) + c(N-1, K-1)$
*   **Generating Function:** $\sum_{K=0}^N c(N, K) x^K = x(x+1)(x+2)\cdots(x+N-1)$

### **7. Derangements (No Fixed Points)**
The number of permutations of $N$ items where no item appears in its original position.
*   **Formula:** $D_N = N! \sum_{i=0}^N \frac{(-1)^i}{i!}$
*   **Recurrence:** $D_N = (N-1)(D_{N-1} + D_{N-2})$ with $D_0=1, D_1=0$.

### **8. Burnside's Lemma & Pólya Enumeration Theorem**
Finds the number of unique objects considering symmetries (e.g., coloring a necklace up to rotations).
**Burnside's Lemma:**
$$ |X/G| = \frac{1}{|G|} \sum_{g \in G} |X^g| $$
*(Number of unique orbits = Average number of fixed points over all symmetry operations $g$)*
**Pólya Enumeration:**
If coloring with $C$ colors, the number of fixed points under operation $g$ is $C^{cycles(g)}$, where $cycles(g)$ is the number of independent cycles the elements form under $g$.

### **9. Cayley's Formula & Prüfer Sequences**
*   **Cayley's Formula:** The number of labeled trees on $N$ vertices is exactly $N^{N-2}$.
*   **Forest of $K$ rooted trees:** The number of forests on $N$ labeled vertices consisting of $K$ rooted trees (where specific $K$ vertices are the roots) is $K \cdot N^{N-K-1}$.
*   **Prüfer Sequence:** A bijection between labeled trees on $N$ vertices and sequences of length $N-2$ with elements from $\{1, \dots, N\}$. The degree of vertex $v$ in the tree is $1 + (\text{occurrences of } v \text{ in the sequence})$.

### **10. Catalan Numbers $C_n$**
Counts the number of valid bracket sequences, triangulations of convex polygons, Dyck paths from $(0,0)$ to $(2n, 0)$, and binary trees with $n$ nodes.
*   **Formula:** $C_n = \frac{1}{n+1} \binom{2n}{n} = \binom{2n}{n} - \binom{2n}{n+1}$
*   **Recurrence:** $C_n = \sum_{i=0}^{n-1} C_i C_{n-1-i}$
*   **Generating Function:** $C(x) = \frac{1 - \sqrt{1 - 4x}}{2x}$

### **11. Kummer's Theorem & Legendre's Formula**
*   **Legendre's Formula:** The highest power of a prime $p$ dividing $N!$ is $\sum_{k=1}^\infty \lfloor \frac{N}{p^k} \rfloor$. *(Calculated in $\mathcal{O}(\log_p N)$).*
*   **Kummer's Theorem:** The highest power of $p$ dividing $\binom{N}{K}$ is exactly the number of **carries** when adding $K$ and $N-K$ in base $p$.
    *Implication: $\binom{N}{K}$ is odd $\iff$ $K$ is a submask of $N$ (i.e., `(N & K) == K`).*

### **12. Matrix Tree Theorem (Kirchhoff's Theorem)**
Finds the number of spanning trees in a general multigraph.
1. Build the Laplacian Matrix $L = D - A$, where $D$ is the degree matrix (diagonal) and $A$ is the adjacency matrix.
2. Delete any 1 row and 1 column (usually the last row and column) to form $L^*$.
3. **Number of spanning trees = $\det(L^*)$**.
*Note: For directed graphs (counting arborescences rooted at $r$), delete the $r$-th row and $r$-th column, and $D_{i,i}$ must be the **in-degree** of vertex $i$.*

### **13. Lindström-Gessel-Viennot Lemma**
Finds the number of non-intersecting path systems between a set of start nodes $A = \{a_1, \dots, a_n\}$ and end nodes $B = \{b_1, \dots, b_n\}$ in a Directed Acyclic Graph (DAG).
1. Let $e(a_i, b_j)$ be the number of paths from $a_i$ to $b_j$ (usually calculated via DP).
2. Construct the $n \times n$ matrix $M$ where $M_{i,j} = e(a_i, b_j)$.
3. **Number of non-intersecting path configurations = $\det(M)$**.

### **14. Eulerian Numbers $\left\langle \begin{matrix} n \\ k \end{matrix} \right\rangle$**
Counts the number of permutations of length $n$ with exactly $k$ ascents (indices where $P_i < P_{i+1}$).
*   **Recurrence:** $\left\langle \begin{matrix} n \\ k \end{matrix} \right\rangle = (k+1) \left\langle \begin{matrix} n-1 \\ k \end{matrix} \right\rangle + (n-k) \left\langle \begin{matrix} n-1 \\ k-1 \end{matrix} \right\rangle$
*   **Symmetry:** $\left\langle \begin{matrix} n \\ k \end{matrix} \right\rangle = \left\langle \begin{matrix} n \\ n-1-k \end{matrix} \right\rangle$
*   **Explicit Formula:** $\left\langle \begin{matrix} n \\ k \end{matrix} \right\rangle = \sum_{j=0}^{k+1} (-1)^j \binom{n+1}{j} (k+1-j)^n$

### **15. Bell Numbers $B_n$**
Counts the number of ways to partition a set of $n$ elements into any number of non-empty, unlabelled subsets.
*   **Formula:** $B_n = \sum_{k=0}^n S(n, k)$ (Sum of Stirling numbers of the second kind).
*   **Recurrence (Bell Triangle):** $B_{n+1} = \sum_{k=0}^n \binom{n}{k} B_k$
*   **Touchard's Congruence:** $B_{p+n} \equiv B_n + B_{n+1} \pmod p$ for prime $p$.

### **16. Lah Numbers $L(n, k)$**
Counts the number of ways to partition a set of $n$ elements into exactly $k$ **ordered** lists.
*   **Formula:** $L(n, k) = \binom{n-1}{k-1} \frac{n!}{k!}$

### **17. Inclusion-Exclusion (PIE Min-Max & Posets)**
*   **Min-Max Theorem (Expected Values):** $\max(A_1, \dots, A_n) = \sum_{i} A_i - \sum_{i<j} \min(A_i, A_j) + \sum_{i<j<k} \min(A_i, A_j, A_k) \dots$
    *Highly useful for calculating expected time to complete tasks (e.g., Coupon Collector variants).*
*   **Möbius Inversion on Divisibility Poset:** $g(n) = \sum_{d \mid n} f(d) \iff f(n) = \sum_{d \mid n} \mu\left(\frac{n}{d}\right) g(d)$

### **18. MacMahon's Master Theorem**
Equates the coefficient of $x_1^{k_1} x_2^{k_2} \dots x_n^{k_n}$ in the expansion of $\prod (\dots)$ to the coefficients in the inverse of the determinant of $(I - X A)$, where $A$ is the coefficient matrix. Used in multi-variable recurrences.

### **19. Faulhaber's Formula**
Calculates the sum of the $p$-th powers of the first $n$ positive integers: $\sum_{k=1}^n k^p$.
It uses Bernoulli numbers $B_j$:
$$ \sum_{k=1}^n k^p = \frac{1}{p+1} \sum_{j=0}^p \binom{p+1}{j} B_j n^{p+1-j} $$

### **20. Rencontres Numbers (Partial Derangements)**
The number of permutations of $N$ items with exactly $K$ fixed points.
$$ D_{N, K} = \binom{N}{K} D_{N-K} $$
*(Where $D_{N-K}$ is the standard derangement number).*

### **21. Multinomial Coefficients**
The number of ways to divide $N$ distinct items into $M$ distinct groups of sizes $k_1, k_2, \dots, k_m$ (where $\sum k_i = N$).
$$ \binom{N}{k_1, k_2, \dots, k_m} = \frac{N!}{k_1! k_2! \cdots k_m!} $$

### **22. Euler's Totient Function $\phi(n)$**
The number of integers up to $n$ that are relatively prime to $n$.
$$ \phi(n) = n \prod_{p|n} \left(1 - \frac{1}{p}\right) $$
*   **Property:** $\sum_{d|n} \phi(d) = n$
*   **Euler's Theorem:** $a^{\phi(n)} \equiv 1 \pmod n$ (for $\gcd(a,n)=1$).

### **23. Pentagonal Number Theorem**
Relates to the partition function $P(n)$, the number of ways to write $n$ as a sum of positive integers.
$$ P(n) = \sum_{k \neq 0} (-1)^{k-1} P\left(n - \frac{k(3k-1)}{2}\right) $$
*(The sequence of offsets is $1, 2, 5, 7, 12, 15, \dots$ which are the generalized pentagonal numbers. It allows computing $P(n)$ in $\mathcal{O}(n \sqrt{n})$).*

### **24. Hook Length Formula**
Finds the number of Standard Young Tableaux of a given shape (useful for counting specific types of paths or permutations).
$$ f^\lambda = \frac{n!}{\prod_{x \in Y(\lambda)} \text{hook}(x)} $$
*(Where $\text{hook}(x)$ is the number of cells in the same row to the right of $x$, plus the cells in the same column below $x$, plus $1$ for the cell itself).*

### **25. Frobenius Coin Problem (Chicken McNugget Theorem)**
For two relatively prime integers $a$ and $b$:
*   **Largest unformable amount:** $ab - a - b$
*   **Number of unformable amounts:** $\frac{(a-1)(b-1)}{2}$

### **26. Pick's Theorem**
Calculates the area $A$ of a polygon whose vertices lie on a regular lattice grid.
$$ A = I + \frac{B}{2} - 1 $$
*(Where $I$ is the number of interior lattice points and $B$ is the number of boundary lattice points).*

### **27. Euler's Formula for Planar Graphs**
For any connected planar graph (drawn without edge intersections):
$$ V - E + F = 2 $$
*(Where $V$ is vertices, $E$ is edges, and $F$ is faces/regions, including the infinite exterior face).*

### **28. Handshaking Lemma**
In any undirected graph, the sum of all vertex degrees is exactly twice the number of edges.
$$ \sum_{v \in V} \deg(v) = 2|E| $$
*Implication: The number of vertices with an odd degree must be even.*

### **29. Narayana Numbers $N(n, k)$**
Counts the number of valid parenthesis expressions with $n$ pairs of brackets and exactly $k$ distinct nesting levels (or peaks in a Dyck path).
$$ N(n, k) = \frac{1}{n} \binom{n}{k} \binom{n}{k-1} $$

### **30. Motzkin Numbers $M_n$**
Counts the number of paths from $(0,0)$ to $(n,0)$ taking steps $(1,1), (1,-1), (1,0)$, and never dropping below the x-axis ($y<0$).
*   **Recurrence:** $M_{n+1} = M_n + \sum_{i=0}^{n-1} M_i M_{n-1-i}$

### **31. Delannoy Numbers $D(n, m)$**
Counts the number of paths from $(0,0)$ to $(n,m)$ using only steps $(1,0)$ (East), $(0,1)$ (North), and $(1,1)$ (Northeast).
$$ D(n, m) = \sum_{k=0}^{\min(n,m)} \binom{n}{k} \binom{m+n-k}{n} $$

### **32. Schröder Numbers $S_n$**
Counts the number of paths from $(0,0)$ to $(2n,0)$ taking steps $(1,1), (1,-1), (2,0)$ and never dropping below the x-axis.
*   **Recurrence:** $S_n = S_{n-1} + \sum_{i=0}^{n-1} S_i S_{n-1-i}$

### **33. Rook Polynomials**
Calculates the number of ways $r_k$ to place $k$ non-attacking rooks on an arbitrary board shape $B$.
*   **Property:** If a board can be split into two disjoint non-interacting boards $A$ and $B$, their generating polynomials multiply: $R_{A \cup B}(x) = R_A(x) \times R_B(x)$.

### **34. Lovász Local Lemma**
Used to prove that a specific combinatorial configuration exists (the probability of no "bad events" happening is $>0$).
If each bad event $A_i$ occurs with probability $\le p$, and each $A_i$ is mutually independent of all but at most $d$ other bad events, then if:
$$ e \cdot p \cdot (d + 1) \le 1 $$
There is a strictly positive probability that **none** of the bad events occur.

### **35. Choosing Non-Adjacent Items (Linear & Circular)**
*   **Linear Selection:** The number of ways to choose $k$ items from $n$ items arranged in a line such that no two chosen items are adjacent:
    $$ \binom{n-k+1}{k} $$
*   **Circular Selection (Kaplansky’s Theorem):** The number of ways to choose $k$ items from $n$ items arranged in a circle such that no two are adjacent:
    $$ \frac{n}{n-k}\binom{n-k}{k} $$

### **36. Multinomial Theorem**
Calculates the coefficient of $x_1^{k_1} x_2^{k_2} \dots x_m^{k_m}$ in the expansion of $(x_1 + x_2 + \dots + x_m)^N$:
$$ \frac{N!}{k_1! k_2! \dots k_m!} $$
*(Where $\sum k_i = N$. This directly links algebra to placing $N$ items into $m$ specific subsets).*

### **37. Generating Functions for Coin Change**
The number of ways to make change for $N$ using a set of coin values $\{c_1, c_2, \dots \}$ is exactly the coefficient of $x^N$ in the expansion of:
$$ \prod_{i} \frac{1}{1-x^{c_i}} = \prod_{i} (1 + x^{c_i} + x^{2c_i} + \dots) $$

### **38. Erdős-Gallai Theorem**
Determines if a given sequence of integers $d_1 \ge d_2 \ge \dots \ge d_n$ can represent the degrees of a simple undirected graph. The sequence is valid if and only if $\sum d_i$ is even, and for every $k$ from $1$ to $n$:
$$ \sum_{i=1}^k d_i \le k(k-1) + \sum_{i=k+1}^n \min(d_i, k) $$

### **39. Chromatic Polynomial $P(G, k)$**
The number of valid ways to color the vertices of a graph $G$ using up to $k$ colors such that no two adjacent vertices share the same color.
*   **Deletion-Contraction Recurrence:** $P(G, k) = P(G - e, k) - P(G / e, k)$
    *(Where $G - e$ is the graph with edge $e$ deleted, and $G / e$ is the graph with edge $e$ contracted/merged).*

### **40. Menage Problem (Couples at a Circular Table)**
The number of ways to seat $N$ couples around a circular table such that men and women alternate, and no husband sits next to his own wife.
$$ 2 \cdot N! \sum_{k=0}^N (-1)^k \frac{2N}{2N-k} \binom{2N-k}{k} (N-k)! $$

### **41. Fuss-Catalan Numbers $C_m(n)$**
A generalization of Catalan numbers counting the number of $m$-ary trees with $n$ internal nodes, or the number of paths that never cross the diagonal $y = (m-1)x$.
$$ C_m(n) = \frac{1}{mn+1} \binom{mn+1}{n} $$
