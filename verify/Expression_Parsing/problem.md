# Verification target — a calculator-style problem

**Submit:** [`test_template.cpp`](test_template.cpp)
**Suggested:** SPOJ **ONP — Transform the Expression** — <https://www.spoj.com/problems/ONP/>

**Status:** ⚠ UNPROVEN — tier 5 source, awaiting a verdict.

---

## Check these first

Neither was confirmed against a live problem page, so read the statement before submitting.

1. **Output format.** `solve()` prints one **value** per line via `evaluate()`. SPOJ ONP wants
   the **postfix string with no spaces**. For that problem, swap `solve()` to use
   `to_postfix(line)` and remove the spaces — the token order is already correct, only the
   separator differs.
2. **Unary minus vs `^`.** This template reads `-2^2` as `-(2^2) = -4`, matching Python and
   normal mathematical convention. If a judge wants `(-2)^2 = 4`, the dial is the unary rank
   in `precedence()`.

## Why this template needed a gate

Shunting-yard is standard, but it is not in our archive or any of the four reference repos,
so it is tier 5 and starts unproven regardless of how routine it looks.

## Local verification already done

The strong part is **generating random expression trees**, whose value is known by
construction, rendering each to text with random spacing and redundant brackets, then checking
the parser recovers the same number: **20000 trees, 0 failures**. That tests the parser against
something that never went through a parser.

Plus 26 hand-computed cases: `1+2*3`, `(1+2)*3`, `2^3^2 = 512` vs `(2^3)^2 = 64`, `100/7`,
`100%7`, `--5`, `-(3+4)`, `3--2`, `-3*-4`, deep nesting, and stray whitespace.

That caught one real bug: `-2^2` returned **4**. Unary minus was ranked above `^`, and a prefix
operator was also allowed to pop operators before its own operand had been read. Both fixed —
prefix operators now never pop, and unary sits between `*` and `^`.

## On the verdict

- **ACCEPTED** — drop the `⚠` from `Expression_Parsing.cpp` in `CP-Template/README.md` and
  record judge, problem and date in `CLAUDE.md`.
- **WRONG ANSWER** — check the two conventions above before suspecting the algorithm; the tree
  test makes a arithmetic error unlikely.
