#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'

struct Aho {
    int N, P;
    vector<vector<int>> next, out;
    vector<int> link, out_link, pat_len;

    int node() {
        next.emplace_back(26, 0);
        out.emplace_back(0);
        link.emplace_back(0);
        out_link.emplace_back(0);
        return N++;
    }

    Aho() : N(0), P(0) {
        node();
    }

    inline int get(char c) {
        return c - 'a';
    }

    void insert(const string &pattern, int idx) {
        int u = 0;
        for (auto &c : pattern) {
            if (!next[u][get(c)]) next[u][get(c)] = node();

            u = next[u][get(c)];
        }

        out[u].push_back(idx);

        if (idx >= (int)pat_len.size()) pat_len.resize(idx + 1);
        pat_len[idx] = (int)pattern.size();
    }

    void compute() {
        queue<int> q;

        for (int c = 0; c < 26; c++) {
            if (next[0][c]) q.push(next[0][c]);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int c = 0; c < 26; c++) {
                int v = next[u][c];
                if (!v) {
                    next[u][c] = next[link[u]][c];
                } else {
                    link[v] = (u ? next[link[u]][c] : 0);
                    int l = link[v];
                    out_link[v] = (!out[l].empty() ? l : out_link[l]);
                    q.push(v);
                }
            }
        }
    }

    int advance(int u, char c) {
        while (u && !next[u][get(c)]) u = link[u];
        u = next[u][get(c)];
        return u;
    }

    vector<array<int, 3>> match(const string &T) {
        vector<array<int, 3>> res;
        int u = 0;
        for (int i = 0; i < T.size(); i++) {
            u = next[u][get(T[i])];
            for (int temp = u; temp; temp = out_link[temp]) {
                for (auto id : out[temp]) {
                    int len = pat_len[id];
                    res.push_back({i - len + 1, i, id});
                }
            }
        }

        return res;
    }

    void mark_matches(const string &T, vector<bool> &good) {
        int u = 0;
        vector<bool> visited(N, false);

        for (int i = 0; i < (int)T.size(); i++) {
            u = next[u][get(T[i])];

            int temp = u;
            while (temp && !visited[temp]) {
                visited[temp] = true;
                for (auto id : out[temp]) good[id] = true;

                temp = out_link[temp];
            }
        }
    }

    int count(const string &s) {
        return match(s).size();
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string t;
    cin >> t;

    int n;
    cin >> n;
    Aho aho;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        aho.insert(s, i);
    }

    aho.compute();

    vector<array<int, 3>> arr = aho.match(t);
    for (auto [l, r, id] : arr) cout << l << " " << r << " " << id << nl;
}