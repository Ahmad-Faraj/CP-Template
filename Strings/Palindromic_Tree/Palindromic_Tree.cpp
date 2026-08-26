#include "../../core.h"

struct PalindromicTree {
    struct Node {
        vector<int> next; // transitions
        int link;         // suffix link
        int len;          // length of palindrome
        long long occ;    // occurrence count
        int firstPos;     // ending position of first occurrence (1-based)
        int num;          // number of palindromic suffixes ending at this node

        Node(int sigma = 0, int L = 0) : next(sigma, 0), link(0), len(L), occ(0), firstPos(-1), num(0) {}
    };

    vector<Node> tree;
    string s;  // processed string (with dummy prefix)
    int last;  // node for longest suffix palindrome so far
    int SIGMA; // alphabet size
    char BASE; // offset character

    // maxLen = expected max length of the string,
    // sigma = alphabet size,
    // base  = first character in your alphabet (e.g. 'a' or '0')
    PalindromicTree(int maxLen, int sigma, char base) : s("#"), last(1), SIGMA(sigma), BASE(base) {
        tree.reserve(maxLen + 3);
        tree.emplace_back(sigma, -1); // imaginary root (len = -1)
        tree.emplace_back(sigma, 0);  // empty root     (len = 0)
        tree[0].link = 0;
        tree[1].link = 0;
        tree[0].num = 0;
        tree[1].num = 0;
    }

    // Add s[pos]=ch (pos is 1-based)
    void addChar(char ch, int pos) {
        int c = ch - BASE;
        s.push_back(ch);
        int cur = last;
        // 1) Find the longest suffix-palindrome that can be extended
        while (true) {
            int curlen = tree[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == ch) break;
            cur = tree[cur].link;
        }
        // 2) If the extension already exists, just bump occ:
        if (tree[cur].next[c]) {
            last = tree[cur].next[c];
            tree[last].occ++;
            return;
        }
        // 3) Otherwise create a new node:
        last = tree[cur].next[c] = tree.size();
        tree.emplace_back(SIGMA, tree[cur].len + 2);
        tree[last].occ = 1;
        tree[last].firstPos = pos;

        // 4) Set its suffix link:
        if (tree[last].len == 1) {
            // single-char palindrome always links to empty root
            tree[last].link = 1;
        } else {
            int linkCandidate = tree[cur].link;
            while (true) {
                int candLen = tree[linkCandidate].len;
                if (pos - 1 - candLen >= 0 && s[pos - 1 - candLen] == ch) {
                    tree[last].link = tree[linkCandidate].next[c];
                    break;
                }
                linkCandidate = tree[linkCandidate].link;
            }
        }

        // 5) Set number of distinct palindromic suffixes
        tree[last].num = tree[tree[last].link].num + 1;
    }

    // After all addChar calls, propagate occ from longer to shorter palindromes.
    // O(V) Optimization: Nodes are mathematically guaranteed to be created in topologically
    // increasing order of length! We can drop std::sort entirely and simply iterate backwards!
    void propagateOccurrences() {
        for (int i = (int)tree.size() - 1; i > 1; i--) {
            tree[tree[i].link].occ += tree[i].occ;
        }
    }

    // Number of distinct palindromic substrings
    int distinctCount() const { return (int)tree.size() - 2; }

    // Total occurrences of all palindromes
    long long totalOccurrences() const {
        long long sum = 0;
        for (int i = 2; i < (int)tree.size(); i++) sum += tree[i].occ;
        return sum;
    }
};
