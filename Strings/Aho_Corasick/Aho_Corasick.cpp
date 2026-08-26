// Aho-Corasick: every occurrence of every pattern from a set, found inside one text in a single pass.
// Use when: "match many patterns against one text at once", forbidden-substring DP, counting dictionary hits.
// Handles: overlapping matches, duplicate patterns, reports start / end / which pattern. Lowercase a-z only.
// Time: build O(sum of pattern lengths) | search O(|s| * L), L = longest chain of fail links that end a pattern
// Indexing: 0-based positions in the text; a pattern is identified by the index you passed to insert()
// Note: call build() once after every insert(). Change 26 and the 'a' offset for a different alphabet.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Aho {
    struct Node {
        Node *fail = nullptr;
        Node *child[26];
        vector<int> pat; // indices ending here; more than one only when the same pattern is given twice
        int patLen = 0;  // a trie node is exactly one string, so one length covers them all
        vector<char> chars;

        Node() { memset(child, 0, sizeof child); }
    };

    Node *root;

    Aho() { root = new Node(); }

    Aho(const vector<string> &patterns) {
        root = new Node();
        for (int i = 0; i < (int)patterns.size(); i++) insert(patterns[i], i);
        build();
    }

    void insert(const string &s, int idx) { // register pattern number idx
        Node *cur = root;
        for (char ch : s) {
            int c = ch - 'a';
            if (!cur->child[c]) cur->child[c] = new Node(), cur->chars.push_back((char)c);
            cur = cur->child[c];
        }
        cur->pat.push_back(idx), cur->patLen = (int)s.size();
    }

    void build() { // link failures; call once, after all inserts
        queue<Node *> q;
        for (int i = 0; i < 26; i++) {
            if (root->child[i])
                root->child[i]->fail = root, q.push(root->child[i]);
            else
                root->child[i] = root;
        }
        while (q.size()) {
            Node *cur = q.front();
            q.pop();
            for (char ch : cur->chars) {
                int c = ch;
                Node *next = cur->child[c], *fail = cur->fail;
                while (fail != root && !fail->child[c]) fail = fail->fail;
                next->fail = fail->child[c] ? fail->child[c] : root;
                q.push(next);
            }
        }
    }

    vector<array<int, 3>> search(const string &s) { // {start, end, pattern index} for every occurrence
        vector<array<int, 3>> hits;
        Node *cur = root;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a';
            while (cur != root && !cur->child[c]) cur = cur->fail;
            if (cur->child[c]) cur = cur->child[c];
            for (Node *t = cur; t != root; t = t->fail)
                for (int idx : t->pat) hits.push_back({i - t->patLen + 1, i, idx});
        }
        return hits;
    }

    ll count_occurrences(const string &s) { return (ll)search(s).size(); } // total matches, overlaps included
};

// Standard problem: n patterns then a text; report every match as "start end pattern"
void solve() {
    int n;
    cin >> n;
    vector<string> patterns(n);
    for (auto &p : patterns) cin >> p;
    string s;
    cin >> s;
    Aho ac(patterns);
    for (auto &h : ac.search(s)) cout << h[0] << ' ' << h[1] << ' ' << h[2] << '\n';
}