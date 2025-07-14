#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;

// Non-Deterministic Miller-Rabin:
// - Probabilistic algorithm: small chance of error (error probability ≤ 4^(-iterations)).
// - Uses randomly selected bases for each iteration (faster for fewer iterations).
// - Time Complexity: O(iterations * log^3(n)), where iterations = number of random tests (e.g., 5).
// - Suitable for testing primality of numbers beyond 10^18 or when speed is prioritized.
// - Can reduce error probability further by increasing the number of iterations.
// Deterministic Miller-Rabin:
// - Deterministic algorithm: guarantees correctness for numbers up to 10^18.
// - Uses a fixed set of bases (e.g., {2, 3, 5, 7, ...}) to ensure accuracy.
// - Time Complexity: O(fixed_bases * log^3(n)), where fixed_bases = number of pre-determined bases.
// - Suitable for applications where absolute certainty is required within the supported range.
// - Slightly slower as it always tests all fixed bases, regardless of input.
struct Miller_Rabin
{
    using u64 = uint64_t;
    using u128 = __uint128_t;

    u64 binpower(u64 base, u64 e, u64 mod)
    {
      u64 result = 1;
      base %= mod;
      while (e)
      {
        if (e & 1)
          result = (u128)result * base % mod;
        base = (u128)base * base % mod;
        e >>= 1;
      }
      return result;
    }

    bool check_composite(u64 n, u64 a, u64 d, int s)
    {
      u64 x = binpower(a, d, n);
      if (x == 1 || x == n - 1)
        return false;
      for (int r = 1; r < s; r++)
      {
        x = (u128)x * x % n;
        if (x == n - 1)
          return false;
      }
      return true;
    };

    bool is_prime_probabilistic(u64 n, int iter = 5)
    { // returns true if n is probably prime, else returns false.
      if (n < 4)
        return n == 2 || n == 3;

      int s = 0;
      u64 d = n - 1;
      while ((d & 1) == 0)
      {
        d >>= 1;
        s++;
      }

      for (int i = 0; i < iter; i++)
      {
        int a = 2 + rand() % (n - 3);
        if (check_composite(n, a, d, s))
          return false;
      }
      return true;
    }

    bool is_prime_deterministic(u64 n)
    { // returns true if n is prime, else returns false.
      if (n < 2)
        return false;

      int r = 0;
      u64 d = n - 1;
      while ((d & 1) == 0)
      {
        d >>= 1;
        r++;
      }

      for (int a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
      {
        if (n == a)
          return true;
        if (check_composite(n, a, d, r))
          return false;
      }
      return true;
    }
};

void solve(int tc)
{
    Miller_Rabin mr;
    int n;
    cin >> n;
    cout << (mr.is_prime_deterministic(n) ? "Prime" : "Not Prime") << nl;
    cout << (mr.is_prime_probabilistic(n) ? "Probably Prime" : "Not Prime") << nl;
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (tc--)
    {
      // cout<<"Case #"<<i<<": ";
      solve(i++);
    }
    return 0;
}