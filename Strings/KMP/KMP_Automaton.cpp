#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'

/*
    KMP Automaton : 
    prefix function, Automaton  O(n)
    KMP O(n + m)
*/

vector<int> prefix_function(const string &pattern) {
    int n = pattern.size();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j && pattern[i] != pattern[j]) j = pi[j - 1];
        if (pattern[i] == pattern[j]) j++;
        pi[i] = j;
    }

    return pi;
}

vector<vector<int>> Automaton(const string &pattern) {
    int n = pattern.size();
    vector<int> pi = prefix_function(pattern);
    vector<vector<int>> go(n + 1, vector<int>(26));
    for (int state = 0; state <= n; state++) {
        for (int c = 0; c < 26; c++) {
            char x = 'a' + c;

            if (state < n && pattern[state] == x)
                go[state][c] = state + 1;
            else if (state == 0)
                go[state][c] = 0;
            else
                go[state][c] = go[pi[state - 1]][c];
        }
    }

    return go;
}

int KMP(const string &txt, const string &pattern) {
    int n = sz(txt);
    int m = sz(pattern);

    if (m > n) return 0;

    vector<vector<int>> go = Automaton(pattern);
    int cnt = 0;

    for (int i = 0, state = 0; i < n; i++) {
        state = go[state][txt[i] - 'a'];

        if (state == m) {
            cnt++;
        }
    }

    return cnt;
}
