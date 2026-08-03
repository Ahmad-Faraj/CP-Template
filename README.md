

# CP-Template

A comprehensive C++ template library and reference cheat sheet for Competitive Programming, curated by Ahmad Faraj. This repository provides optimized, contest-ready implementations of essential algorithms, data structures, mathematical utilities, and common problem-solving boilerplates.

## 📁 Repository Structure

The repository is organized by topic for quick navigation during contests and practice:

- **`Data_Structure/`** - Binary Trie, Standard Trie
- **`DP/`** - Convex Hull Trick (CHT), Digit DP
- **`Game_Theory/`** - Combinatorial Game Theory on Graphs (Winning/Losing states)
- **`Geometry/`** - 1D Objects, Point & Line operations, Intersections, Rotations
- **`Graph/`** - 2-SAT, Bipartite Matching, Articulation Points, Bellman-Ford, Shortest Path utilities
- **`Math/`** - FFT with Modulo, Chinese Remainder Theorem (CRT), Linear Diophantine Equations, Catalan Numbers, Numerical Differentiation
- **`Misc/`** - Base Conversions, Coordinate Compression, Tree Diameter, Meet-in-the-Middle
- **`notes/`** - Bitwise operation properties & GCC `__builtin` functions cheat sheet
- **`other/`** - Utility functions, Fast I/O templates, Segment Trees with Lazy Propagation, and default contest boilerplate

## 🚀 Usage

This repository is designed as a **reference and template library** for competitive programming. It is not meant to be installed as a package, but rather used as a snippet repository.

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Ahmad-Faraj/CP-Template.git
   ```
2. **Set up your IDE:**
   - Open the project folder in **Visual Studio Code**.
   - The included `.vscode/settings.json` automatically configures file associations and syntax highlighting for C++ headers and PBDS extensions.
3. **Use during contests:**
   - Copy the required algorithm files directly into your submission project.
   - Start with the default boilerplate (`other/default/template.cpp`). It includes fast I/O, `long long` type aliases, common macros (`sz`, `all`, `fixed`), and local file redirection.
   - Use `Flow_Type.md` as a quick decision matrix for selecting the right algorithm based on graph type, edge weights, and problem goals.
   - Refer to `notes/Bits.md` for bitwise math identities and compiler builtin references.

## 📖 Quick Reference Guides

| Resource | Description |
|----------|-------------|
| [`Flow_Type.md`](./Flow_Type.md) | Algorithm selection table covering matching, flows, assignment problems, and their time complexities |
| [`notes/Bits.md`](./notes/Bits.md) | Bitwise arithmetic identities and GCC `__builtin` function reference |
| [`other/default/template.cpp`](./other/default/template.cpp) | Standard contest starting template with Fast I/O and modular structure |

## 📝 Notes

- All implementations are optimized for standard CP constraints ($N \le 10^5$, $O(N \log N)$ or better where applicable).
- Consistent use of modular arithmetic macros, fast I/O, and `long long` aliases across all files.
- Feel free to fork, modify, or contribute your own optimized implementations!
