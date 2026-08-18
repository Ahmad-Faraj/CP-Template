# Verification target — CSES 1694, "Download Speed"

**Submit:** [`test_template.cpp`](test_template.cpp) to <https://cses.fi/problemset/task/1694>

**Status:** correctness and timing checked locally; awaiting a verdict.

---

## Checked here before you spend a submission

- **Official sample passes** — the file prints `6`, which is the official answer.
- **Timing at the ceiling** — generated `n = 500`, `m = 1000`, capacities up to `1e9`
  (the exact constraint limits) and it finished in **~130 ms**. The time limit is not the
  risk on this one.

## Why this problem for this template

Max flow is among the least brute-forceable things in the library. The local suite compares
the flow value against a brute-force **minimum cut over every vertex subset** — which is
exact, but only feasible for 6–7 node graphs.

This runs the same code at 500 nodes with capacities at `1e9`, where the total flow passes
`2^31` and the `long long` accounting has to be right. That is the part small random tests
cannot reach.

## Constraints

`1 ≤ n ≤ 500`, `1 ≤ m ≤ 1000`, `1 ≤ c ≤ 10^9`. Worst-case total flow is about `10^12`,
so 64-bit accumulation is required — `Dinic.cpp` uses `long long` throughout and
`flow_through` was changed from `int` during normalisation for exactly this reason.

## On the verdict

- **ACCEPTED** — record judge, problem and date next to `Dinic.cpp` in `CLAUDE.md`. That
  raises it from "brute-forced on tiny graphs" to "judge-proven at scale".
- **WRONG ANSWER** — would point at the residual-graph or level-BFS logic, not the model;
  the sample and the min-cut comparison both pass.
- **TIME LIMIT** — unlikely given 130 ms locally, but would indicate the judge's data is
  worse than random for Dinic's phase count.
