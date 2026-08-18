# Verification target — CSES 2132, "Increasing Array II"

## ✅ RESULT: ACCEPTED — 2026-08-17

Submitted by Ahmed_Faraj under C++20. All 8 tests passed, slowest **0.07 s** against a 1 s limit.

| Test | What it pinned down |
| --- | --- |
| #1, #6 | degenerate input — all equal, and `n = 1` |
| #3 | `n = 200000` all equal, answer 0 |
| #4 | `1e9` then 200000 ones — the worst case for the heaps |
| #5, #7 | `n = 200000` random, answers `52209998458427` and `52157986384148` — both past 2^31, so the 64-bit accumulation really was exercised |
| #2, #8 | small mixed cases with a non-trivial answer |

`DP/Slope_Trick.cpp` is now proven. The `⚠` has been removed from `CP-Template/README.md` and
the provenance note in `CLAUDE.md` updated.

---

**Submit:** [`test_template.cpp`](test_template.cpp) — unchanged — to
<https://cses.fi/problemset/task/2132>

**Language:** C++17 or C++20 (compiles clean under `g++ -std=c++17 -O2 -Wall -Wextra`)

---

## Problem

You are given an array of `n` integers. You want to modify the array so that it is
increasing — that is, every element is at least as large as the previous one.

On each move, you may increase or decrease the value of any one element by one.

What is the minimum number of moves required?

### Input

```
n
x1 x2 ... xn
```

### Output

One integer: the minimum number of moves.

### Constraints

| | |
| --- | --- |
| `1 <= n <= 2 * 10^5` | so an `O(n^2)` solution will not pass |
| `1 <= xi <= 10^9` | so the answer can exceed `2^31` and needs 64 bits |

### Example

```
Input           Output
5               5
3 2 5 1 7
```

One optimal target array is `2 2 3 3 7`, costing `1 + 0 + 2 + 2 + 0 = 5`.

---

## Why this problem verifies the template

The cost is `sum |a[i] - b[i]|` subject to `b` non-decreasing, which is the canonical
slope-trick DP:

```
f_i(x) = min over y <= x of f_(i-1)(y)  +  |x - a[i]|
```

The first half is `min_left()`, the second is `add_abs()`. So a correct verdict exercises,
together and against real data:

- `min_left()` — the prefix-minimum that enforces "non-decreasing"
- `add_abs()`, and through it `add_x_minus_a()` and `add_a_minus_x()`
- `get_min()`
- both lazy offsets `add_l` / `add_r`, since the heaps are read through `top_l()` / `top_r()`

It also establishes two things the local tests could **not**:

1. **The time bound.** `n = 2 * 10^5` on a 1 second limit. Locally this input runs in
   **169 ms**, but only a judge proves it against their data and machine.
2. **The 64-bit accumulation.** On a random maximal input the answer came out as
   `49988905376614`, well past `2^31`. An `int` anywhere in the accumulation would be
   caught here and nowhere in the small tests.

## Reading the result

- **ACCEPTED** — the template is proven. Drop the `⚠` from `Slope_Trick.cpp` in
  `CP-Template/README.md`, and update the provenance note in `CLAUDE.md` that currently
  says it "has never been through a real submission".
- **WRONG ANSWER** — a logic defect. The local tests matched CSES 2132's sample and two
  hand-checked cases, so a failure means an input shape those missed. Ask for the failing
  test if the judge shows it.
- **TIME LIMIT EXCEEDED** — the operation count is fine in theory; suspect the I/O. The
  submission already sets `sync_with_stdio(false)` and `cin.tie(nullptr)`.

## Integrity of the copy

Everything between the `BEGIN VERBATIM COPY` and `END VERBATIM COPY` markers in
`test_template.cpp` is `CP-Template/DP/Slope_Trick.cpp`, byte for byte — same struct, same
`solve()`, no logic altered. Verified by hash:

```
template sha256 : f3ea8cb2ad5efc893b323391500fd890...
embedded sha256 : f3ea8cb2ad5efc893b323391500fd890...
```

The only addition is the `main()` at the bottom, which the repo's templates omit by
contract. So the verdict applies to the template exactly as it is committed.

To re-check that at any time:

```powershell
# from e:\cp-temp
python verify/check_verbatim.py
```
