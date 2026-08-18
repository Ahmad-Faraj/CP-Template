# Verification target — CSES 1735, "Range Updates and Sums"

**Submit:** [`test_template.cpp`](test_template.cpp) to <https://cses.fi/problemset/task/1735>

**Status:** ⚠ UNPROVEN — tier 5 source. Correctness established locally; the **time bound** is
what a verdict decides.

---

## Already confirmed here

The statement was read from the problem page directly, so there is no unverified convention:
`1 a b x` adds, `2 a b x` assigns, `3 a b` sums, and `solve()` matches.

**The official sample passes.** Running the submission file itself:

```
input                 official      this file
  6 5                     7             7
  2 3 1 1 5 3            11            11
  3 3 5                  15            15
  1 2 4 2
  3 3 5
  2 2 4 5
  3 3 5
```

Plus the structure against a plain array: 4000 random sequences × 40 operations mixing
**assign, range add and sum**, checking point reads after every operation, range sums, value
counts, and that `for_each_run` tiles the queried range exactly. 0 failures.

## What the submission actually decides

Not correctness — that is covered above. **The time bound.**

This structure is amortised *only because assignment collapses runs*. CSES 1735 deliberately
mixes range add with range assign. If the test data is assign-poor, the runs never merge and
each query drifts toward O(n).

So:

- **ACCEPTED** — the amortisation holds on real data. Drop the `⚠` from
  `CP-Template/README.md` and record judge, problem and date in `CLAUDE.md`.
- **TIME LIMIT EXCEEDED** — *informative, not a bug.* It means this data is adversarial for a
  run-based structure and the problem wants
  `Range_Query/Segment_Tree/Segment_Tree_Lazy.cpp`. The template header already warns that
  without assignments it degrades. If this happens, record it in `CLAUDE.md` as a known
  limit and keep the `⚠`, or retarget the submission at an assign-only problem.
- **WRONG ANSWER** — would be surprising given the sample and the brute-force runs. Check for
  overflow first: values reach 1e6 and n, q reach 2e5, so sums reach ~1e16 and need 64 bits.

## Constraints

`1 ≤ n, q ≤ 2·10⁵`, values and `x` up to `10⁶`, `1 ≤ a ≤ b ≤ n`.
