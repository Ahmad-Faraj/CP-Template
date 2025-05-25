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
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}
const int N = 1e6;
int spf[N + 1];
vector<int>primes;
vector<int> divisors[N + 1];
// linear sieve
void linear_sieve(){ 
    for (int i=2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
        }
        for (int j = 0; i * primes[j] <= N; ++j) {
            spf[i * primes[j]] = primes[j];
            if (primes[j] == spf[i]) {
                break;
            }
        }
    }
    for(int i = 2; i <= N; i ++){
        for(int j = i; j <= N; j += i){
            divisors[j].emplace_back(i);
        }
    }
}
int mob[N + 1];
void Mobius() // O(n) mobius for range 1 -> n
{
   for(int i = 1; i <= N; i++) {
       if(i == 1) mob[i] = 1;
       else{
           if(spf[i / spf[i]] == spf[i]) mob[i] = 0;
           else
               mob[i] = -1 * mob[i / spf[i]];
       }
   }
}
/*
    mobius function
    mob[i] = 0 if i has a squared prime factor
    mob[i] = 1 if i is a square-free positive integer with an even number of prime factors
    mob[i] = -1 if i is a square-free positive integer with an odd number of prime factors
*/
void solve(int tc)
{
    
}
signed main(void)
{
    fastio();
    int tc = 1;
    //cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}