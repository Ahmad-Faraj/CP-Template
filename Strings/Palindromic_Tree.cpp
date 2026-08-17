// Palindromic Tree (Eertree): every distinct palindromic substring of a string, and how often each occurs.
// Use when: "count distinct palindromic substrings", total palindromic occurrences, longest palindromic suffix.
// Handles: any alphabet via sigma and base, occurrence counts, first-occurrence position. Characters append only.
// Time: addChar O(1) amortized | propagateOccurrences O(k log k), k = number of distinct palindromes
// Indexing: characters are added at 1-based positions; nodes 0 and 1 are the two roots, not real palindromes
// Note: call propagateOccurrences() once after the last addChar, before reading occ or totalOccurrences().

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct PalindromicTree {
    struct Node {
        vector<int> next; // transitions
        int link;         // suffix link
        int len;          // length of this palindrome
        ll occ;           // occurrences
        int firstPos;     // 1-based end position of the first occurrence

        Node(int sigma = 0, int L = 0) : next(sigma, 0), link(0), len(L), occ(0), firstPos(-1) {}
    };

    vector<Node> tree;
    string s;  // processed characters, with a dummy prefix
    int last;  // node of the longest palindromic suffix so far
    int SIGMA; // alphabet size
    char BASE; // first character of the alphabet

    PalindromicTree(int maxLen, int sigma, char base) : s("#"), last(1), SIGMA(sigma), BASE(base) {
        tree.reserve(maxLen + 3);
        tree.emplace_back(sigma, -1); // imaginary root
        tree.emplace_back(sigma, 0);  // empty root
        tree[0].link = 0;
        tree[1].link = 0;
    }

    void addChar(char ch, int pos) { // append ch at 1-based position pos
        int c = ch - BASE;
        s.push_back(ch);
        int cur = last;
        while (true) {
            int curlen = tree[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == ch) break;
            cur = tree[cur].link;
        }
        if (tree[cur].next[c]) {
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
    }

    void build(const string &str) { // add every character of str in order
        for (int i = 0; i < (int)str.size(); i++) addChar(str[i], i + 1);
        propagateOccurrences();
    }

    void propagateOccurrences() { // push occurrence counts from longer palindromes down to shorter ones
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

    ll totalOccurrences() const { // total palindromic substrings, occurrences counted separately
        ll sum = 0;
        for (int i = 2; i < (int)tree.size(); i++) sum += tree[i].occ;
        return sum;
    }
};

// Standard problem: report how many distinct palindromic substrings s has, and how many in total
void solve() {
    string s;
    cin >> s;
    PalindromicTree pt((int)s.size(), 26, 'a');
    pt.build(s);
    cout << pt.distinctCount() << '\n' << pt.totalOccurrences() << '\n';
}
