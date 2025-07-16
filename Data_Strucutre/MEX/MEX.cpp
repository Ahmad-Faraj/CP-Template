#include <bits/stdc++.h>
using namespace std;

int mex(vector<int> const &A) {
    set<int> b(A.begin(), A.end());
    int result = 0;
    while (b.count(result)) ++result;
    return result;
}

const int MAX_N = 1e5 + 5;
int mex(vector<int> const &A) {
    static bool used[MAX_N + 1] = {0};
    for (int x : A) {
        if (x <= MAX_N) used[x] = true;
    }
    int result = 0;
    while (used[result]) ++result;
    for (int x : A) {
        if (x <= MAX_N) used[x] = false;
    }
    return result;
}