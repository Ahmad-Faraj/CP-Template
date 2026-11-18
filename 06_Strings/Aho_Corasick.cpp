// Aho-Corasick: finds every occurrence of every pattern of a fixed set inside a text in one pass.
// Use when: "match many patterns against one text", dictionary hits, which patterns end at each
//           position.
// Usage: AhoCorasick ac(patterns);  auto res = ac.search(text);  res[i] = patterns ending at
//        text[i]
//        (or: AhoCorasick ac; ac.insert(p, idx) for each pattern; ac.build();)
// Handles: overlapping matches, duplicate patterns (both indices reported), patterns inside
//          patterns.
// Time: build O(26 + sum |p| + copied indices) | search O(|text| + number of matches)
// Indexing: 0-based text positions; res[i] lists pattern indices whose occurrence ENDS at i
//           (start = i - |pattern| + 1). Indices are those given to insert (vector order in the
//           ctor).
// Note: lowercase 'a'..'z' only, non-empty patterns. Call build() once, after the last insert.
//       Output (and memory: each node copies its fail chain's indices) can reach |text| * sqrt(2 *
//       sum |p|).

#include <bits/stdc++.h>
using namespace std;

struct AhoCorasick {
    struct Node {
        Node *fail;
        Node *child[26];
        vector<int> patIdx; // patterns ending here, including those reached through fail links
        vector<char> chars; // real trie children
        Node() {
            memset(child, 0, sizeof(child));
            fail = 0;
        }
    };
    Node *root;

    AhoCorasick() { root = new Node(); }

    AhoCorasick(const vector<string> &patterns) {
        root = new Node();
        for (int i = 0; i < (int)patterns.size(); i++) insert(patterns[i], i);
        build();
    }

    void insert(const string &s, int idx) {
        Node *cur = root;
        for (char ch : s) {
            int c = ch - 'a';
            if (!cur->child[c]) {
                cur->child[c] = new Node();
                cur->chars.push_back(c);
            }
            cur = cur->child[c];
        }
        cur->patIdx.push_back(idx);
    }

    void build() {
        queue<Node *> q;
        for (int i = 0; i < 26; i++) {
            if (root->child[i]) root->child[i]->fail = root, q.push(root->child[i]);
            else root->child[i] = root;
        }
        while (!q.empty()) {
            Node *cur = q.front();
            q.pop();
            for (int c : cur->chars) {
                Node *next = cur->child[c], *fail = cur->fail;
                while (fail != root && !fail->child[c]) fail = fail->fail;
                next->fail = fail->child[c] ? fail->child[c] : root;
                for (int j : next->fail->patIdx) next->patIdx.push_back(j);
                q.push(next);
            }
        }
    }

    vector<vector<int>> search(const string &s) {
        vector<vector<int>> res(s.size());
        Node *cur = root;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a';
            while (cur != root && !cur->child[c]) cur = cur->fail;
            if (cur->child[c]) cur = cur->child[c];
            for (int j : cur->patIdx) res[i].push_back(j);
        }
        return res;
    }
};
