# Verification target — CSES 1133, "Tree Distances II"

**Submit:** [`test_template.cpp`](test_template.cpp) to <https://cses.fi/problemset/task/1133>

**Status:** ⚠ UNPROVEN — written from the standard formulation, no tier 1–4 source exists.

---

## Why this one is unproven

Rerooting is the only template here with **no source at all** in the trusted tiers. Searched
by filename and by content across `other/` and all four `refs/` (hamza, abowahbaz, mazen,
shohag): every hit was a false positive — `SquareRootDecomposition.cpp` matching "root",
`Cactus Graph.cpp` matching `up[`. Shohag has Virtual Tree, Block Cut Tree and Dominator
Tree, but nothing that re-roots a DP.

So it was written from the standard down-pass / up-pass formulation. Under the provenance
rule that makes it tier 5, and it stays flagged until this submission is ACCEPTED.

## Why CSES 1133 is the right target

It *is* the rerooting problem: "for each node, the sum of distances to all other nodes."
The statement was read from the problem page, so the I/O convention is not guessed —
n, then n−1 edge lines, then n answers on one line.

## Already confirmed here

**Official sample passes**, running the submission file itself:

```
input          official     this file
  5
  1 2         6 9 5 8 8    6 9 5 8 8
  1 3
  3 4
  3 5
```

**Brute force, 4000 random trees, every node checked as root.** The reference re-runs the
down-DP explicitly rooted at each vertex, which shares nothing with the up pass — the only
part that can be wrong. Four different DP shapes, so the test does not just confirm one
special case:

| what was compared | failures |
| --- | --- |
| distance sum, unweighted | 0 |
| distance sum, weighted edges | 0 |
| farthest node — a `max` merge, not a sum | 0 |
| `add_node` depending on the node id | 0 |
| `answer[root] == down[root]` at run()'s root | 0 |
| star and path shapes, n = 1..60 exhaustively | 0 |

The farthest-node case also runs `run()` from a **random** root each iteration, so a
dependence on which vertex the passes start from would show up.

## The open question is the time bound

n = 200000, the full constraint:

| tree shape | time |
| --- | --- |
| path | 293 ms |
| star | 353 ms |
| random | 510–670 ms |

The limit is 1.00 s. The margin is real but not generous, and the cost is the
`std::function` indirection — every merge is an indirect call. Three vector allocations
per node were removed already (798 ms → ~510 ms on the random tree, output byte-identical).

**If this TLEs, the fix is known and does not change the algorithm:** make the four
operations template parameters instead of `std::function` members, so they inline. That
was traded away for the simpler call site. Report the verdict either way.
