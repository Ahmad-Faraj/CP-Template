# Verification target — AtCoder ABC 218 H, "Red and Blue Lamps"

**Submit:** [`test_template.cpp`](test_template.cpp) to <https://atcoder.jp/contests/abc218/tasks/abc218_h>

**Status:** ⚠ UNPROVEN — written from the standard formulation, no tier 1–4 source exists.

---

## Why this one is unproven

Searched `other/` and all four `refs/` for the Aliens trick by name (`aliens`, `wqs`, `lambda`,
`penalty`, `Lagrange`) and by shape. The only hit was a variable called `penalty` inside
`refs/mazen/Graph_Algorithms/Dijkstra.cpp`. Shohag has a `Math/Lagrange Multiplier.cpp`, but that is a
continuous optimisation problem solved in doubles — not this technique.

So the wrapper was written from the standard formulation. Under the provenance rule that makes it
tier 5, and it stays flagged until this submission is ACCEPTED.

## Why ABC 218 H is the right target

It is an exactly-k problem whose optimum is concave in k, which is precisely the premise the trick
needs, and its input is small enough that the driver is obviously faithful: `N R` then `A_1..A_{N-1}`.

## Already confirmed here

**All three official samples pass**, running the submission file itself:

| input | official | this file |
| --- | --- | --- |
| `6 2` / `3 1 4 1 5` | 11 | 11 |
| `7 6` / `2 7 1 8 2 8` | 10 | 10 |
| `11 7` / `12345 678 …` | 46207983 | 46207983 |

**Brute force, 4000 random cases per direction** (`tests/Aliens_Trick.cpp`), 0 failures:

| what was compared | failures |
| --- | --- |
| minimisation, exactly k parts, vs the exact O(n²k) DP with k as a real dimension | 0 |
| the penalty it reports actually makes k optimal | 0 |
| maximisation, exactly k picks, vs sorting | 0 |
| `is_convex` agrees with the real cost curve | 0 |

The reference is the DP that carries the piece count as a dimension — the very thing the trick
exists to remove — so it shares nothing with the wrapper under test.

## Note on the driver

The template ships an O(n²) demo solver inside its `solve()`, because the point of the file is the
wrapper. This submission's `main()` supplies an O(n) solver instead. That is the intended use and
changes nothing above the marker; `verify/check_verbatim.py` proves it.
