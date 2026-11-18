#include <bits/stdc++.h>
using namespace std;
#define ll long long

bool issubstring(string s1, string s2) {
    if (s2.find(s1) != string::npos) {
        return 1;
    }
    return 0;
}

ll sumall(ll x) {
    ll res = (x * (x + 1)) / 2;
    return res;
}

ll sumodd(ll x) {
    ll res = (x + 1) / 2;
    return res * res;
}

ll sumeven(ll x) { return x * (x + 1); }

// get logb(a)
double get_log(ll a, int b) { return log(a) / log(b); }
