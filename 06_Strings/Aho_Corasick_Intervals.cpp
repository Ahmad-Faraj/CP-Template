// Aho Corasick Intervals: every occurrence of every pattern in a text, as [start, end] intervals.
// Use when: "cover / delete / count all pattern occurrences", intervals for a sweep or DP.
// Usage: Aho ac(patterns); auto iv = ac.search(text, patterns); // iv = {start, end} pairs
//        (or: Aho ac; ac.insert(p, idx) per pattern; ac.build();)
// Handles: overlapping occurrences, patterns inside patterns. Equal patterns give ONE interval
//          (the last idx inserted wins), so intervals are distinct.
// Time: build O(26 * nodes) | search O(|text| * max |p|) (walks the fail chain per position)
// Indexing: 0-based, inclusive [start, end] in the text; sorted by end, then by decreasing length.
// Note: lowercase 'a'..'z' only. Empty patterns are never reported. Memory: one node per trie
//       character (~240 bytes each). Not copyable. Long fail chains make search slow: fine
//       for short patterns (|p| <= ~100), use Aho_Corasick.cpp output lists otherwise.

#include <bits/stdc++.h>
using namespace std;

struct Aho {
    struct Node {
        Node *fail = nullptr;
        Node *child[26];
        int patIdx = -1; // pattern ending here (one per node), -1 if none
        vector<char> chars; // real children, for build() and the destructor
        Node() { memset(child, 0, sizeof child); }
        ~Node() {
            for (auto c : chars) delete child[(int)c];
        }
    };

    Node *root;
    Aho() { root = new Node(); }
    Aho(const vector<string> &patterns) {
        root = new Node();
        for (int i = 0; i < (int)patterns.size(); i++) insert(patterns[i], i);
        build();
    }
    Aho(const Aho &) = delete;
    Aho &operator=(const Aho &) = delete;
    ~Aho() { delete root; }

    void insert(const string &s, int idx) {
        Node *cur = root;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a';
            if (!cur->child[c]) {
                cur->child[c] = new Node();
                cur->chars.push_back(c);
            }
            cur = cur->child[c];
        }
        cur->patIdx = idx;
    }

    void build() {
        queue<Node *> q;
        for (int i = 0; i < 26; i++) {
            if (root->child[i]) {
                root->child[i]->fail = root;
                q.push(root->child[i]);
            } else
                root->child[i] = root;
        }
        while (q.size()) {
            Node *cur = q.front();
            q.pop();
            for (int i = 0; i < (int)cur->chars.size(); i++) {
                int c = cur->chars[i];
                Node *next = cur->child[c], *fail = cur->fail;
                while (fail != root && !fail->child[c]) fail = fail->fail;
                next->fail = fail->child[c] ? fail->child[c] : root;
                q.push(next);
            }
        }
    }

    // patterns[idx] gives each interval's length; pass the same vector used to insert
    vector<pair<long long, long long>> search(const string &s, const vector<string> &patterns) {
        vector<pair<long long, long long>> intervals;
        Node *cur = root;
        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a';
            while (cur != root && !cur->child[c]) cur = cur->fail;
            if (cur->child[c]) cur = cur->child[c];
            for (Node *t = cur; t != root; t = t->fail)
                if (t->patIdx != -1)
                    intervals.push_back({i - (long long)patterns[t->patIdx].size() + 1, i});
        }
        return intervals;
    }
};
