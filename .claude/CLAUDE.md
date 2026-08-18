# CP-Template — Working Rules

A contest reference library. Every file is grabbed under time pressure by someone who
will not read past the header and will not learn the topic now. Optimise for that
reader and nobody else.

## Authorship

NEVER add Claude as co-author, contributor, or generator. No `Co-Authored-By` trailer,
no "Generated with Claude Code", no mention in PR bodies. Commits are authored by the
repo owner alone.

## Provenance — where logic comes from

Never invent an algorithm from scratch. Logic is sourced, not authored.

Allowed: merging two templates into one with more features, extending a template's
functionality, simplifying an existing one.

Not allowed: writing a new algorithm, or silently changing logic to "fix" it. If code
looks wrong, flag it to the owner and leave it alone.

Trusted sources, in order of preference:

1. This repo — source of truth, `other/` included
2. Teammates — `HamzaHassanain/CP_Templates`, `abowahbaz/CP-Templates`
3. `Mazen-Ghanaym/CP-Templates`, `ShahjalalShohag/code-library`
4. cp-algorithms.com, USACO Guide
5. Other established public libraries — KACTL, AtCoder Library, youkn0wwho.academy,
   and Codeforces blogs by a recognised author

Tier 5 exists because ECPC/ACPC reaches topics the first four tiers do not cover. Use it
only when nothing closer has the algorithm, and never as a shortcut past reading `other/`
and `refs/` first.

**Anything from tier 4 or 5, or written from a description rather than copied from code,
is UNPROVEN until it earns a verdict.** Unproven means:

- it carries `⚠` in the README and a provenance note in `CLAUDE.md` naming the exact source
- it is tested against a brute force locally, as everything is, AND
- a submission is prepared under `verify/<Template>/` — `test_template.cpp` embedding the
  template byte for byte plus a `main()`, and `problem.md` naming the judge problem

It stops being unproven when that submission is ACCEPTED. Then the `⚠` comes off and the
provenance note records the judge, problem and date. `DP/Slope_Trick.cpp` is the worked
example: authored, flagged, submitted to CSES 2132, accepted, unflagged.

`python verify/check_verbatim.py` confirms each submission still embeds its template
byte for byte. Run it after touching any template that has one.

A local brute force is not a substitute for the judge. It shares your misunderstanding of
the problem; the judge does not. It also cannot establish the time bound.

## `other/` is frozen

Read it as a source. Never move, delete, rename, or reformat anything under it.

## The file contract

Every template file, in this order:

**1. Header** — first lines of the file, nothing above it. One line each, never wrapping:

```cpp
// <Name>: <what it does, input -> output>
// Use when: <the problem phrasings that should make you reach for this>
// Handles: <what it supports — and what it does not>
// Time: <per operation>
// Indexing: 1-based
// Note: <only real gotchas — destroys the input array, needs MOD, recursion depth>
```

`Use when` is how you find the file without knowing the topic: the keywords and shapes a
statement uses. Write what a problem says, not what the algorithm is.

`Handles` is the capability envelope — negative values, duplicates, online vs offline,
updates, ranges — and ends with what it cannot do, so nobody reaches for the wrong file.

`Indexing` is stated even when it seems obvious. `Note` is omitted when there is nothing
to warn about. Every line earns its place or it goes.

**2. Includes and usings it actually needs.** No macro dumps. Anything the code
references (`ll`, `sz`, `MOD`, `INF`) is defined in the file — a template that does not
stand alone is broken.

**3. The reusable unit** — a struct or a set of free functions.

**4. `solve()`** — solves the topic's standard problem using the template. This is the
usage documentation. `main()` is optional; `solve()` is mandatory.

Any public operation the standard problem does not happen to call still carries its
one-line WHAT comment at its declaration, so nothing the template can do is invisible.

## Comments

- NEVER explain how the algorithm works. No theory, no derivation, no proof, no "this
  works because". The template is a black box.
- The only comment shape allowed inside the unit: one line at a declaration stating
  WHAT it returns — `// kth smallest in [l, r]`.
- No commented-out code and no `/* ... */` graveyards. A variant worth keeping becomes
  its own file; a variant not worth keeping is deleted.
- No unreachable code used to express a variant (e.g. a second `return` after the
  first). If two behaviours matter, expose both as callable options.

## Scannability

Reading only the header must answer: what is this, how fast is it, how do I index it.
Reading only `solve()` must answer: how do I call it. Neither may require knowing the
topic.

## One Main + variants

Each topic has exactly one `Topic.cpp` — the simplest and most standard one. Simple does
not mean feature-poor: the Main template must do everything the topic can do.

Variants are suffixed: `Topic_Persistent.cpp`, `Topic_2D.cpp`, `Topic_Dynamic.cpp`.

Redundancy is acceptable. Ambiguity about which file to grab is not.

## Naming

`Title_Snake_Case.cpp`, for files and directories alike. No spaces, no hyphens, no
lowercase-only names, no CamelCase runs (`Bellman_Ford`, never `BellmanFord`).

Spell the name out. If a reader has to decode it, it is wrong: `Coordinate_Compression`,
not `cord_comp`. The only permitted abbreviations are acronyms every competitor reads at a
glance — DP, DSU, KMP, LCA, LCT, HLD, MEX, MO, MST, CRT, GCD, LDE, FFT, NTT, FWHT, CHT,
SOS, SQRT, 2_SAT.

Variants extend the Main's name so the family sorts together in a directory listing:
`Segment_Tree`, `Segment_Tree_Lazy`, `Segment_Tree_Persistent`.

## Layout

One directory per topic. A directory gains subdirectories only when it grows past roughly
15 files — below that, nesting costs more than it saves. Subdirectories are named for the
question they answer (`Shortest_Path`, `Connectivity`, `Hashing`), never for the technique
that happens to implement them.

A file belongs to the directory matching the question it answers, not the machinery it uses:
a hashing segment tree is a Strings file, an XOR basis is a Math file, Kruskal is a Graph
file. Keep siblings together — splitting a family across directories costs more than any
tidiness it buys.

`Misc/` is for what genuinely resists categorisation, and stays small. A file landing there
is usually a sign the right directory has not been named yet.

## Formatting

- `.clang-format` at repo root is authoritative: 4 spaces, attached braces, 120 columns.
- Must compile clean under `g++ -std=c++17 -O2 -Wall`.

## Verification

Compiling is not evidence. A template is finished only once it has been **run**, against
a real judge sample and against a randomized brute-force comparison.

```powershell
pwsh tools/check.ps1 -Path <dir>     # header contract + compiles clean
```

The checker catches missing headers, missing `solve()`, dead code and compile errors. It
cannot catch wrong answers, and wrong answers were the majority of what was found here:
an all-private `class`, a `class` member read before initialization, a 64 MB object built
on the stack, `int` arithmetic overflowing a hash, a mex corrupted by negative values, a
destructor freeing a sentinel shared by every other instance. Each compiled without a
warning.

So: append a `main()` to a scratch copy, run it, and report the failure count. Never claim
a template works because it built.

Three habits that paid off repeatedly:

- **Doubt the test before the template.** Several "failures" were bugs in the test —
  a stale global between iterations, `rand()` silently capping at 32767, comparing
  indices whose identity was not observable. Prove which side is wrong.
- **Test what the structure promises, not just its return value.** That a centroid
  tree's depth really is O(log n), that a diameter path really is a walk, that
  `get_components()` really is a partition, that an old version really is unchanged.
- **Call every member a `template` declares.** A member function of a class template is
  only compiled when something calls it, so a broken one is invisible — an undefined
  `sz(nums)` in a segment tree's `build()`, a type error in a persistent tree's `get()`,
  a shadowed macro in a trie's `erase()`. Four files hid a real error this way. The test
  must exercise the whole advertised surface, not just the path `solve()` happens to take.

## Scope

Skip naive and trivial topics. A file that is a solved contest problem rather than a
reusable unit either becomes a template or leaves the topic directory.
