#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<int> apply_permutation(const vector<int> &a, const vector<int> &p) { // result[i] = a[p[i]]
    int n = (int)a.size();
    vector<int> res(n);
    for (int i = 0; i < n; i++) res[i] = a[p[i]];
    return res;
}

vector<int> pow_permutation(vector<int> a, vector<int> p, ll k) { // a after applying p k times
    while (k > 0) {
        if (k & 1) a = apply_permutation(a, p);
        p = apply_permutation(p, p);
        k >>= 1;
    }
    return a;
}