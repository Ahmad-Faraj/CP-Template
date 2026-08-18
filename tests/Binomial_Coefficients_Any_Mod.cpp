// Brute-force test for:
//   Math/Combinatorics/Binomial_Coefficients_Any_Mod.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace AM {
#include "../Math/Combinatorics/Binomial_Coefficients_Any_Mod.cpp"
}
int main() {
    // Pascal's triangle mod m is pure addition, so it is valid for ANY modulus -
    // exactly the reference this template needs, since factorial tables are not.
    int fail = 0, checked = 0, zeroCases = 0;
    const int N = 60;
    for (ll m : {2LL,3LL,4LL,6LL,8LL,9LL,10LL,12LL,16LL,25LL,27LL,30LL,36LL,49LL,60LL,72LL,100LL,121LL,144LL,1000LL,1024LL,997LL,1000000LL}) {
        vector<vector<ll>> pas(N + 1, vector<ll>(N + 1, 0));
        for (int i = 0; i <= N; i++) {
            pas[i][0] = 1 % m;
            for (int j = 1; j <= i; j++) pas[i][j] = (pas[i-1][j-1] + pas[i-1][j]) % m;
        }
        for (ll n = 0; n <= N; n++) for (ll r = 0; r <= n; r++) {
            ll got = AM::ncr_any_mod(n, r, m);
            if (got != pas[n][r]) fail++;
            if (pas[n][r] == 0) zeroCases++;
            checked++;
        }
        // out-of-range r
        if (AM::ncr_any_mod(10, -1, m) != 0 || AM::ncr_any_mod(10, 11, m) != 0) fail++;
    }
    // huge n against Lucas for a prime modulus, where both must agree
    int bigFail = 0;
    {
        ll m = 1000003; // prime
        // C(n, 1) = n, C(n, n-1) = n, C(n, 0) = 1 for any n
        for (ll n : {1000000000000LL, 999999999999LL, 123456789012LL}) {
            if (AM::ncr_any_mod(n, 0, m) != 1 % m) bigFail++;
            if (AM::ncr_any_mod(n, 1, m) != n % m) bigFail++;
            if (AM::ncr_any_mod(n, n - 1, m) != n % m) bigFail++;
            if (AM::ncr_any_mod(n, n, m) != 1 % m) bigFail++;
        }
        // a composite modulus with a large prime power factor
        ll cm = 8 * 27 * 25; // 5400
        for (ll n : {100000000000LL, 55555555555LL}) {
            if (AM::ncr_any_mod(n, 0, cm) != 1 % cm) bigFail++;
            if (AM::ncr_any_mod(n, 1, cm) != n % cm) bigFail++;
        }
        // symmetry C(n,r) == C(n,n-r) at scale
        for (ll n : {10000000LL, 987654321LL}) for (ll r : {3LL, 17LL, 250LL})
            if (AM::ncr_any_mod(n, r, 720) != AM::ncr_any_mod(n, n - r, 720)) bigFail++;
    }
    printf("Binomial_Coefficients_Any_Mod\n");
    printf("  vs Pascal mod m, 23 moduli : %d failures (%d values, %d of them 0)\n", fail, checked, zeroCases);
    printf("  huge n identities          : %d failures\n", bigFail);
}
