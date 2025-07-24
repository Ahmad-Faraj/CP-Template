#include <bits/stdc++.h>
using namespace std;
#define ll long long

// Get vector with the prime factors of number / sqrt(N)
vector<int> prime_factorization(ll n) {
    vector<int> factors;
    while (n % 2 == 0) factors.push_back(2), n /= 2;
    for (int i = 3; i <= sqrt(n); i += 2)
        while (n % i == 0) factors.push_back(i), n /= i;
    if (n > 2) factors.push_back(n);
    return factors;
}

// <prime factor,frq> / sqrt(N)
vector<pair<int, int>> factors;
void get_factors(int n) {
    factors.clear();
    int d = 1;
    for (int i = 2; i * i <= n; i += d, d = 2)
        if (n % i == 0) {
            factors.push_back(make_pair(i, 0));
            while (n % i == 0) {
                n /= i;
                factors.back().second++;
            }
        }
    if (n != 1) factors.push_back(make_pair(n, 1));
}

// sqrt(N)
vector<ll> get_divisors(ll n) {
    vector<ll> divs;
    for (ll i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            divs.push_back(i);
            if (i != n / i) divs.push_back(n / i);
        }
    }
    sort(divs.begin(), divs.end());
    return divs;
}

// sqrt(N)
ll get_sum_div(ll x) {
    ll ans = 0;
    for (ll left = 1, right; left <= x; left = right + 1) {
        right = x / (x / left);
        ans += (x / left) * (left + right) * (right - left + 1) / 2;
        cout << (x / left) * (left + right) * (right - left + 1) / 2 << endl;
        cout << ans << endl;
    }
    return ans;
}

ll count_divisors(ll n) {
    ll cnt = 0;
    for (ll i = 1; i <= sqrt(n); i++) {
        if (n % i == 0) {
            if (n / i == i)
                cnt++;
            else
                cnt = cnt + 2;
        }
    }
    return cnt;
}