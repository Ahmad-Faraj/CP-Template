// Palindromic Tree (Eertree): one node per distinct palindromic substring, with occurrence counts.
// Use when: "count distinct palindromic substrings", total palindromic substrings, palindromic
//           suffixes.
// Usage: PalindromicTree pt(n, 26, 'a'); pt.build(s);  (or addChar(s[i], i + 1) per i, then
//        propagateOccurrences())  -> pt.distinctCount(), pt.totalOccurrences(), pt.tree[v].occ
// Handles: any contiguous alphabet via sigma and base; per-node len, occ, firstPos, suffix link.
// Time: addChar O(1) amortized | propagateOccurrences O(k log k), k = distinct palindromes
// Indexing: characters go in at 1-based positions 1, 2, 3, ...; nodes 0 (len -1) and 1 (len 0) are
//           the
//           two roots, real palindromes are nodes 2 .. tree.size() - 1; firstPos = 1-based END
//           position
// Note: call propagateOccurrences() exactly once, after the last addChar, before reading occ.
//       Memory: sigma ints per node (26 * 4 bytes * n), about 100 MB at n = 1e6.
//       Characters must be in [base, base + sigma) and must not be '#'.

#include <bits/stdc++.h>
using namespace std;

struct PalindromicTree {
    struct Node {
        vector<int> next; // transitions
        int link;         // suffix link
        int len;          // length of the palindrome
        long long occ;    // occurrence count (after propagateOccurrences)
        int firstPos;     // 1-based end position of the first occurrence

        Node(int sigma = 0, int L = 0) : next(sigma, 0), link(0), len(L), occ(0), firstPos(-1) {}
    };

    vector<Node> tree;
    string s;  // processed string, with a dummy prefix
    int last;  // node of the longest suffix palindrome so far
    int SIGMA; // alphabet size
    char BASE; // first character of the alphabet

    PalindromicTree(int maxLen, int sigma, char base) : s("#"), last(1), SIGMA(sigma), BASE(base) {
        tree.reserve(maxLen + 3);
        tree.emplace_back(sigma, -1); // imaginary root (len = -1)
        tree.emplace_back(sigma, 0);  // empty root     (len = 0)
        tree[0].link = 0;
        tree[1].link = 0;
    }

    void addChar(char ch, int pos) { // append s[pos] = ch, pos is 1-based and increasing
        int c = ch - BASE;
        s.push_back(ch);
        int cur = last;
        while (true) { // longest suffix palindrome that can be extended by ch
            int curlen = tree[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == ch) break;
            cur = tree[cur].link;
        }
        if (tree[cur].next[c]) { // already exists
            last = tree[cur].next[c];
            tree[last].occ++;
            return;
        }
        last = tree[cur].next[c] = tree.size();
        tree.emplace_back(SIGMA, tree[cur].len + 2);
        tree[last].occ = 1;
        tree[last].firstPos = pos;
        if (tree[last].len == 1) {
            tree[last].link = 1;
        } else {
            int cand = tree[cur].link;
            while (true) {
                int candLen = tree[cand].len;
                if (pos - 1 - candLen >= 0 && s[pos - 1 - candLen] == ch) {
                    tree[last].link = tree[cand].next[c];
                    break;
                }
                cand = tree[cand].link;
            }
        }
    }

    void build(const string &str) { // addChar for every character, then propagate
        for (int i = 0; i < (int)str.size(); i++) addChar(str[i], i + 1);
        propagateOccurrences();
    }

    void propagateOccurrences() { // push occ from longer palindromes to their suffix links
        int k = tree.size();
        vector<int> order(k);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) { return tree[a].len > tree[b].len; });
        for (int v : order) {
            if (v <= 1) continue;
            tree[tree[v].link].occ += tree[v].occ;
        }
    }

    int distinctCount() const { return (int)tree.size() - 2; } // distinct palindromic substrings

    long long totalOccurrences() const { // palindromic substrings counted with multiplicity
        long long sum = 0;
        for (int i = 2; i < (int)tree.size(); i++) sum += tree[i].occ;
        return sum;
    }
};
