#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'

struct Aho {
    int N;
    vector<vector<int>> next, out;
    vector<int> link, out_link, pat_len;

    int node() {
        next.emplace_back(26, 0);
        out.emplace_back(0);
        link.emplace_back(0);
        out_link.emplace_back(0);
        return N++;
    }

    Aho() : N(0) {
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
                    out_link[v] = (!out[link[v]].empty() ? link[v] : out_link[link[v]]);
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

    vector<vector<int>> match(const string &T) // {l, r , idx}
    {
        vector<vector<int>> res;
        int u = 0;
        for (int i = 0; i < T.size(); i++) {
            char c = T[i];
            u = advance(u, c);

            int v = u;
            while (v) {
                for (auto id : out[v]) {

                    int len = pat_len[id];
                    res.push_back({i - len + 1, i, id}); // be careful here (memory)
                }

                v = out_link[v];
            }
        }

        return res;
    }

    int count(const string &s) {
        return match(s).size();
    }

    string smallest(int l) {
        string res = "";
        int u = 0;
        for (int i = 0; i < l; i++) {
            for (int c = 0; c < 26; c++) {
                int v = next[u][c];
                if (out[v].empty()) {
                    res += char('a' + c);
                    u = v;
                    break;
                }
            }
        }

        return res;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string t;
    cin >> t;

    int n;
    cin >> n;

    vector<string> arr(n);
    vector<int> pos(n);
    map<string, int> fm;
    Aho aho;

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        string curr = arr[i];

        // this methods to avoid inserting the same elements (avoid tle)
        if (fm.count(curr)) {
            pos[i] = fm[curr];
        } else {
            pos[i] = fm[curr] = i;

            aho.insert(curr, i);
        }
    }

    aho.compute();
    int u = 0;
    vector<int> freq(n + 5);
    for (int i = 0; i < t.size(); i++) {
        char c = t[i];
        u = aho.advance(u, c);
        int v = u;
        while (v) {
            for (auto &x : aho.out[v]) freq[x]++;
            v = aho.out_link[v];
        }
    }

    for (int i = 0; i < n; i++) {
        cout << freq[pos[i]] << '\n';
    }
}