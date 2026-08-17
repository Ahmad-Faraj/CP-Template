// Dynamic Aho-Corasick: add and remove patterns online, then count their matches in any text.
// Use when: the pattern set changes between queries - "add word", "delete word", "how many words occur in s".
// Handles: online insert and delete, total match count, per-pattern match positions. Lowercase a-z only.
// Time: insert O(L log P) amortized | count O(|s| log P) | delete O(size of the affected bucket)
// Indexing: 0-based positions in the text; patterns are numbered in insertion order
// Note: rebuilds in log-sized buckets, so one insert can be slow on its own while the amortized cost stays low.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define sz(x) (int)(x).size()

struct AhoBucket { // a static automaton over one bucket of patterns
    struct node {
        vector<int> occ; // pattern indices ending exactly here
        int child[26], cnt, link;
        node() : cnt(0), link(-1) { memset(child, -1, sizeof child); }
    };
    vector<node> tree;

    AhoBucket() { tree = vector<node>(1); }

    AhoBucket(const vector<pair<string, int>> &patterns) {
        tree = vector<node>(1);
        for (auto &[s, idx] : patterns) {
            int cur = 0;
            for (char c : s) {
                int to = c - 'a';
                if (tree[cur].child[to] == -1) {
                    tree[cur].child[to] = sz(tree);
                    tree.emplace_back(node());
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
        while (sz(q)) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < 26; i++) {
                int v = tree[u].child[i];
                if (~v) {
                    int j = tree[u].link;
                    while (~j and !~tree[j].child[i]) j = tree[j].link;
                    tree[v].link = (~j ? tree[j].child[i] : 0);
                    tree[v].cnt += tree[tree[v].link].cnt; // cnt accumulates in O(1); occ is walked instead
                    q.push(v);
                }
            }
        }
    }

    ll count_occurrences(const string &s) { // total matches of this bucket's patterns inside s
        ll ans = 0;
        int cur = 0;
        for (char c : s) {
            int to = c - 'a';
            while (~cur and !~tree[cur].child[to]) cur = tree[cur].link;
            cur = (~cur ? tree[cur].child[to] : 0);
            ans += tree[cur].cnt;
        }
        return ans;
    }

    void get_occ(const string &s, vector<vector<int>> &ret) { // ret[pattern] gets each 0-based end position
        int cur = 0;
        for (int i = 0; i < sz(s); i++) {
            int to = s[i] - 'a';
            while (~cur and !~tree[cur].child[to]) cur = tree[cur].link;
            cur = (~cur ? tree[cur].child[to] : 0);
            for (int t = cur; t > 0; t = tree[t].link)
                for (int idx : tree[t].occ) ret[idx].push_back(i);
        }
    }
};

struct AhoDynamic {
    static const int LOG = 20;
    int curr_size = 0;
    AhoBucket aho[LOG];
    vector<pair<string, int>> list[LOG];

    int get_nxt() {
        for (int i = 0; i < LOG; i++)
            if (list[i].empty()) return i;
        return 0;
    }

    void insert(const string &s) { // add a pattern; it gets the next index
        int idx = get_nxt();
        for (int bef = 0; bef < idx; bef++) {
            for (auto &it : list[bef]) list[idx].push_back(it);
            list[bef].clear();
            aho[bef] = AhoBucket();
        }
        list[idx].emplace_back(s, curr_size++);
        aho[idx] = AhoBucket(list[idx]);
    }

    void delete_word(const string &s) { // remove one copy of a pattern, if present
        for (int l = 0; l < LOG; l++) {
            auto it = find_if(list[l].begin(), list[l].end(), [&](const pair<string, int> &p) { return p.first == s; });
            if (it != list[l].end()) {
                list[l].erase(it);
                aho[l] = AhoBucket(list[l]);
                break;
            }
        }
    }

    ll count(const string &s) { // total occurrences of every stored pattern inside s
        ll ans = 0;
        for (int i = 0; i < LOG; i++)
            if (!list[i].empty()) ans += aho[i].count_occurrences(s);
        return ans;
    }

    vector<vector<int>> get_occ(const string &s) { // per pattern index, the 0-based end positions in s
        vector<vector<int>> ans(curr_size);
        for (int i = 0; i < LOG; i++)
            if (!list[i].empty()) aho[i].get_occ(s, ans);
        return ans;
    }
};

// Standard problem: type 1 adds a word, type 2 removes one, type 3 reports total occurrences inside s
void solve() {
    int q;
    cin >> q;
    AhoDynamic ac;
    while (q--) {
        int t;
        string s;
        cin >> t >> s;
        if (t == 1)
            ac.insert(s);
        else if (t == 2)
            ac.delete_word(s);
        else
            cout << ac.count(s) << '\n';
    }
}
