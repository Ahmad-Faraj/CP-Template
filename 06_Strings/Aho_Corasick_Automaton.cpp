// Aho-Corasick Automaton: the full goto automaton of a pattern set (array goto + fail links).
// Use when: DP over "the last few characters typed" - count / build strings that avoid or contain
//           patterns, max score of a length-L string, matrix power on states; per-pattern hits.
// Usage: AhoCorasickAutomaton ac(patterns);  s = ac.go(s, ch);  ac.cnt[s] = patterns ending here
//        (or: AhoCorasickAutomaton ac; ac.insert(p, idx) for each pattern; ac.build();)
// Handles: duplicate patterns (each counted), patterns inside patterns, empty pattern set.
// Time: build O(K * nodes) | go O(1) | total_matches O(|text|) | occurrences O(|text| + nodes)
// Indexing: state 0 = root (empty string); states 0..size()-1. Pattern indices are those given to
//           insert (vector order in the ctor); node_of[idx] = its terminal state.
// Note: alphabet 'a'..'z' (change K / BASE). Call build() once, after the last insert.
//       cnt[s] = patterns that are suffixes of state s (own count summed along fail links).
//       order = BFS order: link[v] comes before v, so fold the fail tree with a reverse loop.
// Source: cp-algorithms "Aho-Corasick algorithm" (BFS construction of the automaton).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct AhoCorasickAutomaton {
    static constexpr int K = 26;
    static constexpr char BASE = 'a';
    vector<array<int, K>> nxt; // trie edges before build(), full goto function after
    vector<int> link, cnt, order, node_of;

    AhoCorasickAutomaton() { add_node(); }
    AhoCorasickAutomaton(const vector<string> &patterns) {
        add_node();
        for (int i = 0; i < (int)patterns.size(); i++) insert(patterns[i], i);
        build();
    }

    int add_node() {
        array<int, K> a;
        a.fill(-1);
        nxt.push_back(a), link.push_back(0), cnt.push_back(0);
        return (int)nxt.size() - 1;
    }
    int size() const { return nxt.size(); }

    int insert(const string &s, int idx) { // returns the terminal state of s
        int v = 0;
        for (char ch : s) {
            int c = ch - BASE;
            if (nxt[v][c] == -1) {
                int u = add_node(); // add_node may reallocate nxt: no reference held across it
                nxt[v][c] = u;
            }
            v = nxt[v][c];
        }
        cnt[v]++;
        if ((int)node_of.size() <= idx) node_of.resize(idx + 1, -1);
        node_of[idx] = v;
        return v;
    }

    void build() {
        order.assign(1, 0);
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            for (int c = 0; c < K; c++) {
                int v = nxt[u][c];
                if (v != -1) {
                    link[v] = u == 0 ? 0 : nxt[link[u]][c];
                    cnt[v] += cnt[link[v]];
                    order.push_back(v);
                } else nxt[u][c] = u == 0 ? 0 : nxt[link[u]][c];
            }
        }
    }

    int go(int s, char ch) const { return nxt[s][ch - BASE]; }

    ll total_matches(const string &text) const { // occurrences of all patterns, overlaps included
        ll res = 0;
        for (int i = 0, s = 0; i < (int)text.size(); i++) s = go(s, text[i]), res += cnt[s];
        return res;
    }

    vector<ll> occurrences(const string &text) const { // res[idx] = occurrences of pattern idx
        vector<ll> hit(size(), 0);
        for (int i = 0, s = 0; i < (int)text.size(); i++) hit[s = go(s, text[i])]++;
        for (int i = size() - 1; i > 0; i--) hit[link[order[i]]] += hit[order[i]];
        vector<ll> res(node_of.size(), 0);
        for (int i = 0; i < (int)node_of.size(); i++)
            if (node_of[i] != -1) res[i] = hit[node_of[i]];
        return res;
    }
};
