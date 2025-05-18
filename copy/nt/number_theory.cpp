#include <bits/stdc++.h>

#define ll long long
#define ld long double

#define nl "\n"
#define sp " "

#define fixed(n) fixed << setprecision(n)


using namespace std;

void Fast_IO(){
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}

// ----------------------------- {code} ----------------------------




struct MyMathForNT{
    const int MAXRange = 1e6;
    vector<short> isPrime;
    vector<long long> phi;
    vector<int> moebius;
    MyMathForNT(){}

    void buildIsPrime(){
        isPrime.assign(MAXRange + 5, 1);
        for(int i = 2; i <= MAXRange; i++){
            if(isPrime[i]){
                for(int j = i * 2; j <= 1e6; j += i){ isPrime[j] = 0; }
            }
        }
    }

    ll extended_euclid(ll a, ll b, ll &x, ll &y){
        if(a < 0){
            ll r = extended_euclid(-a, b, x, y);
            x *= -1;
            return r;
        }
        if(b < 0){
            ll r = extended_euclid(a, -b, x, y);
            y *= -1;
            return r;
        }
        if(b == 0){
            x = 1, y = 0;
            return a;
        }
        ll g = extended_euclid(b , a % b, y , x);
        y -= (a / b) * x;
        return g;
    }

    
    ll ldioph(ll a, ll b, ll c, ll &x, ll &y, ll &found){
        ll g = extended_euclid(a, b, x, y);
        if((found = c % g == 0)){
            x *= (c / g), y *= (c / g);
        }
        return g;
    }

    void shift_solution(ll & x, ll & y, ll a, ll b, ll cnt) {
        x += cnt * b;
        y -= cnt * a;
    }

    ll find_all_solutions(ll a, ll b, ll c, ll minx, ll maxx, ll miny, ll maxy) {
        ll x, y, found, g = ldioph(a, b, c, x, y, found);
        if (!found) return 0;

        a /= g;
        b /= g;

        ll sign_a = a > 0 ? +1 : -1;
        ll sign_b = b > 0 ? +1 : -1;

        // find the interval of x:
        shift_solution(x, y, a, b, (minx - x) / b);
        if (x < minx)
            shift_solution(x, y, a, b, sign_b);
        if (x > maxx)
            return 0;
        ll lx1 = x;

        shift_solution(x, y, a, b, (maxx - x) / b);
        if (x > maxx)
            shift_solution(x, y, a, b, -sign_b);
        ll rx1 = x;



        // find the interval of y:
        shift_solution(x, y, a, b, -(miny - y) / a);
        if (y < miny)
            shift_solution(x, y, a, b, -sign_a);
        if (y > maxy)
            return 0;
        ll lx2 = x;

        shift_solution(x, y, a, b, -(maxy - y) / a);
        if (y > maxy)
            shift_solution(x, y, a, b, sign_a);
        ll rx2 = x;


        if (lx2 > rx2) swap(lx2, rx2);

        // get intersection:
        ll lx = max(lx1, lx2);
        ll rx = min(rx1, rx2);

        if (lx > rx) return 0;

        // todo: create vector for the solutions:

        return (rx - lx) / abs(b) + 1;
    }

    vector<long long> modularEquation(ll a, ll b, ll n){
        // solves the equation ax = b (mod n)
        vector<ll> solutions;
        ll x, y, g;
        g = extended_euclid(a, n, x, y);
        if(b % g != 0) return solutions; // * no solutions

        x = ( (x * b / g) % n + n) % n; // find positive x between 0 and n

        for(int i = 0; i < g; ++i)
            solutions.push_back((x + i * n / g) % n);

        // * sort the solution if needed to be sorted
        sort(solutions.begin(), solutions.end());

        return solutions;
    }


    vector<ll> solveSystemOfCongruencesGeneral(vector<ll> &rems, vector<ll> &mods){
        
        // T = rem_0 (mod MODs_0) --> T = MODs_0 * x + rem_0
        ll rem = rems[0], mod = mods[0];

        // solve with prev equ, get new congruence equ
        for(int i = 1; i < rems.size(); i++){
            // T = rem_i (MOD MODs_i) --> T = MODs_i * y + rem_i
            // MODs_0 * x + (-MODs_i) * y = rem_i - rem_0
            // a           b               c 
            ll x, y, found, a = mod, b = -mods[i], c = rems[i] - rem;
            ll g = ldioph(a, b, c, x, y, found);

            if(!found) return {-1, -1, -1};

            rem += mod * x; 
            mod = mod / g * mods[i]; // * merged mod --> lcm mods
            rem = (rem % mod + mod) % mod; // * merge rem
        }

        //  T, rem, mod
        //  T = rem % mod
        //                         T      rem  mod
        vector<ll> solution = {rem % mod, rem, mod};

        return solution;
    }


    long long getPhiFor(long long n){ // number of numbers are makes coprime with n : between 1 and n.

        long long pPowOfK, relative_primes = 1;

        for(long long number = 2; number * number <= n; number += (1 + (number != 2))){
            pPowOfK = 1;
            while(n % number == 0){ pPowOfK *= number; n /= number; }
            relative_primes *= (pPowOfK / number) * (number - 1);
        }
        if(n) relative_primes *= (n - 1);

        return relative_primes;
    }

    int phi(int n) { // number of numbers are makes coprime with n : between 1 and n.
        int ans = n;
        for (int p = 2; p * p <= n; p++) {
            if (n % p == 0) {
                while (n % p == 0) { n /= p; }
                ans -= ans / p;
            }
        }
        if (n > 1) { ans -= ans / n; }
        return ans;
    }

    void buildPhiForRange(){
        if(isPrime.empty()) buildIsPrime();
        phi.assign(MAXRange + 5, 1);
        for(int i = 2; i <= MAXRange; i++){
            if(isPrime[i]){
                phi[i] = i - 1;
                for(int j = i * 2; j <= MAXRange; j += i){
                    long long pPowOfK = 1, n = j;
                    while(n % i == 0){ pPowOfK *= i; n /= i; }
                    phi[j] *= (pPowOfK / i) * (i - 1);
                }
            }
        }
    }

    long long getPhiForFactorOf(long long n){ // phi(n!) ==> (isprime(n)? (n - 1) : n) * (phi(n - 1))
        long long ans = 1;
        for(long long i = n; i >= 2; i--){
            ans *= (isPrime[i]? i - 1 : i) ;
        } return ans;
    }

    int getMoebiusFor(long long n){
        int mobVal = 1; // the value that the even number of primes takes.
        for(long long i = 2; i * i <= n; i++){
            if(n % i == 0){ if(n % (i * i) == 0) return 0;
                n /= i, mobVal = -mobVal;
            } 
        }
        if(n) mobVal = -mobVal;
        return mobVal;
    }

    void buildMoebiusForRange(){
        if(isPrime.empty()) buildIsPrime();
        moebius.assign(MAXRange + 5, 1);
        for(long long i = 2; i * i <= MAXRange; i++){
            if(isPrime[i]) {
                moebius[i] = -1;
                for(long long j = 2 * i; j <= MAXRange; j += i)
                    moebius[j] = ((j % (i * i) == 0) ? 0 : -moebius[j]);
            }
        }
    }

    long long findIndexOfsquarFree(long long n){
        long long inx = n;
        for(long long i = 2; i * i <= n; i++){
            inx += moebius[i] * (n / (i * i));
        } return inx;
    }




};






void Solve(){

}


int main(){
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    Fast_IO();

    int tc = 1;
    // cin>>tc;
    while (tc--)
    {   
        Solve();
    }
    
    return 0;
}


