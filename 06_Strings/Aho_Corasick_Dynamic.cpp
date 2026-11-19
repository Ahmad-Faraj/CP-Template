// Aho Corasick Dynamic: a pattern set with online insert / delete, counting matches in a text.
// Use when: "add a pattern", "remove a pattern", "how many pattern occurrences in this text".
// Usage: aho_corasick ac; ac.insert(p); ac.delete_word(p); ac.count(t); auto occ = ac.get_occ(t);
// Handles: duplicate patterns (counted once per copy), deleting one copy, deleting a missing word.
// Time: insert amortised O(|p| * 26 * log n); count / get_occ O(|t| * log n + matches);
//       delete_word rebuilds the whole bucket holding the word (O(bucket total length * 26)).
// Indexing: patterns get ids 0, 1, 2, ... in insertion order; get_occ(t)[id] lists end positions
//           (0-based index in t of the last character) of every match of that pattern.
// Note: lowercase 'a'..'z' only; no empty patterns. Ids of deleted words stay reserved.
//       Each node copies the ids of every pattern that is a suffix of it (for get_occ), even if
//       you only call count(): many short / repeated patterns (a, a, aa, ...) blow up time+memory.
//       One delete_word hit can cost ~50 ms on 1e5 words. For many deletes of live words, insert
//       them into a second aho_corasick del instead and answer ac.count(t) - del.count(t).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class aho_corasick_node {
    struct node {
        vector<int> occ;
        int child[26], cnt, link;
        node() : cnt(0), link(-1) { memset(child, -1, sizeof child); }
    };
    vector<node> tree;

public:
    aho_corasick_node() = default;

    aho_corasick_node(const vector<pair<string, int>> &patterns) {
        tree = vector<node>(1);
        for (auto &[s, idx] : patterns) {
            int cur = 0;
            for (char c : s) {
                int to = c - 'a';
                if (tree[cur].child[to] == -1) {
                    tree[cur].child[to] = (int)tree.size();
                    tree.emplace_back();
                }
                cur = tree[cur].child[to];
            }
            tree[cur].cnt++;
            tree[cur].occ.push_back(idx);
        }
        build_links();
    }

    void build_links() {
        queue<int> q;
        q.push(0);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < 26; i++) {
                int v = tree[u].child[i];
                if (v == -1) continue;
                int j = tree[u].link;
                while (j != -1 && tree[j].child[i] == -1) j = tree[j].link;
                tree[v].link = (j != -1 ? tree[j].child[i] : 0);
                tree[v].cnt += tree[tree[v].link].cnt;
                auto &lo = tree[tree[v].link].occ;
                tree[v].occ.insert(tree[v].occ.end(), lo.begin(), lo.end());
                q.push(v);
            }
        }
    }

    ll count_occurrences(const string &s) {
        ll ans = 0;
        int cur = 0;
        for (char c : s) {
            int to = c - 'a';
            while (cur != -1 && tree[cur].child[to] == -1) cur = tree[cur].link;
            cur = (cur != -1 ? tree[cur].child[to] : 0);
            ans += tree[cur].cnt;
        }
        return ans;
    }

    void get_occ(const string &s, vector<vector<int>> &ret) {
        int cur = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            int to = s[i] - 'a';
            while (cur != -1 && tree[cur].child[to] == -1) cur = tree[cur].link;
            cur = (cur != -1 ? tree[cur].child[to] : 0);
            for (int idx : tree[cur].occ) ret[idx].push_back(i);
        }
    }
};

class aho_corasick {
    int curr_size = 0;
    static const int LOG = 20;
    aho_corasick_node aho[LOG];
    vector<pair<string, int>> list[LOG];

    int get_nxt() {
        for (int i = 0; i < LOG; i++)
            if (list[i].empty()) return i;
        return 0;
    }

public:
    void insert(const string &s) {
        int idx = get_nxt();
        for (int bef = 0; bef < idx; bef++) {
            for (auto &it : list[bef]) list[idx].push_back(it);
            list[bef].clear();
            aho[bef] = aho_corasick_node();
        }
        list[idx].emplace_back(s, curr_size++);
        aho[idx] = aho_corasick_node(list[idx]);
    }

    // total occurrences of all live patterns in s (with multiplicity)
    ll count(const string &s) {
        ll ans = 0;
        for (int i = 0; i < LOG; i++)
            if (!list[i].empty()) ans += aho[i].count_occurrences(s);
        return ans;
    }

    // result[id] = end positions in s of pattern id (empty for deleted ids)
    vector<vector<int>> get_occ(const string &s) {
        vector<vector<int>> ans(curr_size);
        for (int i = 0; i < LOG; i++)
            if (!list[i].empty()) aho[i].get_occ(s, ans);
        return ans;
    }

    // removes one copy of s if present
    void delete_word(const string &s) {
        for (int l = 0; l < LOG; l++) {
            auto it = find_if(list[l].begin(), list[l].end(),
                              [&](const pair<string, int> &p) { return p.first == s; });
            if (it != list[l].end()) {
                list[l].erase(it);
                aho[l] = aho_corasick_node(list[l]);
                break;
            }
        }
    }
};
