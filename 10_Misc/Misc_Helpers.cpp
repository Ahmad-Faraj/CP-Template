// Misc Helpers: substring test, closed-form sums of 1..x / odds / evens, and logarithms.
// Use when: quick "is s1 inside s2", sum of 1..n without a loop, floor(log_b(a)).
// Usage: issubstring(a, b); sumall(n); sumodd(n); sumeven(n); log_floor(a, b); get_log(a, b)
// Handles: x = 0, x up to the largest value whose sum still fits in ll (see Note).
// Time: issubstring O(|s1| * |s2|) worst case; the rest O(1), log_floor O(log a)
// Indexing: none
// Note: sumall(x) = 1+..+x fits for x <= 4.29e9; sumodd(x) = sum of odd k <= x and
//       sumeven(x) = sum of even k <= x fit for x <= 6074000997. All need x >= 0.
//       get_log is floating point: floor(get_log(1000, 10)) == 2. Use log_floor for exact answers.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

bool issubstring(const string &s1, const string &s2) { return s2.find(s1) != string::npos; }

ll sumall(ll x) { return x % 2 == 0 ? x / 2 * (x + 1) : (x + 1) / 2 * x; }

ll sumodd(ll x) { // 1 + 3 + 5 + ... (odd numbers <= x)
    ll res = (x + 1) / 2;
    return res * res;
}

ll sumeven(ll x) { // 2 + 4 + 6 + ... (even numbers <= x)
    ll m = x / 2;
    return m * (m + 1);
}

// get logb(a)
double get_log(ll a, ll b) { return log(a) / log(b); }

int log_floor(ll a, ll b) { // largest k with b^k <= a; a >= 1, b >= 2
    int k = 0;
    while (a >= b) a /= b, k++;
    return k;
}
