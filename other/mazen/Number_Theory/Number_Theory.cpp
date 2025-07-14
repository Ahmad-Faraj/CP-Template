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
struct Number_Theory
{
    // Goldbach’s conjecture: Each even integer n > 2 can be represented as a sum n = a+ b so that both a and b are primes.

    // prime numbers are numbers that are only divisible by 1 and themselves
    /*
        If a number n is not prime, it can be represented as a product a· b, where a ≤ sqrt(n)
        or b ≤ sqrt(n), so it certainly has a factor between 2 and floor(sqrt(n)).
        Using this observation,
        we can both test if a number is prime and find the prime factorization of a number in O(sqrt(n)) time.
    */
    bool is_prime(ll n)
    {
        // O(sqrt(n))
        if (n < 2)
            return false;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
                return false;
        }
        return true;
    }

    // binary exponentiation: a^b = a^(b/2) * a^(b/2) if b is even, a^b = a * a^(b/2) * a^(b/2) if b is odd
    ll binary_exponentiation(ll a, ll b)
    {
        // O(log(b))
        ll ans = 1;
        while (b)
        {
            if (b & 1)
                ans = (ans * a);
            a = (a * a);
            b >>= 1;
        }
        return ans;
    }

    ll binary_exponentiation_mod(ll a, ll b, ll m)
    {
        // O(log(b))
        ll ans = 1;
        while (b)
        {
            if (b & 1)
                ans = (ans * a) % m;
            a = (a * a) % m;
            b >>= 1;
        }
        return ans;
    }

    // ecldiean algorithm: gcd(a, b) = gcd(b, a % b) if b != 0, gcd(a, 0) = a
    ll gcd(ll a, ll b)
    {
        // O(log(min(a, b)))
        return b == 0 ? a : gcd(b, a % b);
    }

    // lcm(a, b) = a * b / gcd(a, b)
    ll lcm(ll a, ll b)
    {
        // O(log(min(a, b)))
        return a / gcd(a, b) * b;
    }

    ll modular_inverse(ll a, ll m)
    {
        // O(log(m))
        return binary_exponentiation_mod(a, m - 2, m);
    }

    // a number can be expressed as a product of prime numbers p1^a1 * p2^a2 * p3^a3 * ... * pk^ak
    // such that p1, p2, p3, ..., pk are prime numbers and a1, a2, a3, ..., ak are their powers
    map<int, int> prime_factorization(ll n)
    {
        map<int, int> mp;
        for (int i = 2; i * i <= n; i++)
        {
            while (n % i == 0)
            {
                mp[i]++;
                n /= i;
            }
        }
        if (n > 1)
            mp[n]++;
        return mp;
    }

    // the number of divisors of a number is equal to (a1 + 1) * (a2 + 1) * (a3 + 1) * ... * (ak + 1)
    ll number_of_divisors(ll n)
    {
        // O(sqrt(n))
        ll ans = 1;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
            {
                int cnt = 0;
                while (n % i == 0)
                {
                    cnt++;
                    n /= i;
                }
                ans *= (cnt + 1);
            }
        }
        if (n > 1)
            ans *= 2;
        return ans;
    }

    // the sum of divisors of a number is equal to (p1^(a1 + 1) - 1) / (p1 - 1) * (p2^(a2 + 1) - 1) / (p2 - 1) * ... * (pk^(ak + 1) - 1) / (pk - 1)
    // the sum of divisors of a number is equal to (1 + p1 + p1^2 + ... + p1^a1) * (1 + p2 + p2^2 + ... + p2^a2) * ... * (1 + pk + pk^2 + ... + pk^ak)
    ll sum_of_divisors(ll n)
    {
        // O(sqrt(n))
        ll ans = 1;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
            {
                ll sum = 1, p = 1;
                while (n % i == 0)
                {
                    p *= i;
                    sum += p;
                    n /= i;
                }
                ans *= sum;
            }
        }
        if (n > 1)
            ans *= (n + 1);
        return ans;
    }

    ll sum_of_divisors_mod(ll n)
    {
        // O(sqrt(n))
        ll ans = 1;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
            {
                ll sum = 1, p = 1;
                while (n % i == 0)
                {
                    p = (p * i) % MOD;
                    sum = (sum + p) % MOD;
                    n /= i;
                }
                ans = (ans * sum) % MOD;
            }
        }
        if (n > 1)
            ans = (ans * (n + 1)) % MOD;
        return ans;
    }

    // the product of divisors of a number is equal to n^(number of divisors / 2)
    // because we can form τ(n)/2 pairs from the factors, each with product n
    ll product_of_divisors(ll n)
    {
        // O(sqrt(n))
        ll ans = number_of_divisors(n);
        return binary_exponentiation(n, ans / 2);
    }

    ll product_of_divisors_mod(ll n, ll m)
    {
        // O(sqrt(n))
        ll ans = number_of_divisors(n);
        return binary_exponentiation_mod(n, ans / 2, m);
    }

    // euler's totient function: φ(n) = n * (1 - 1/p1) * (1 - 1/p2) * (1 - 1/p3) * ... * (1 - 1/pk)
    // where p1, p2, p3, ..., pk are the prime factors of n
    ll euler_totient_function(ll n)
    {
        // O(sqrt(n))
        ll ans = n;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
            {
                while (n % i == 0)
                    n /= i;
                ans = ans / i * (i - 1);
            }
        }
        if (n > 1)
            ans = ans / n * (n - 1);
        return ans;
    }

    // fermat's little theorem: a^(m - 1) ≡ 1 (mod m) if a and m are coprime
    // euler's theorem: a^φ(m) ≡ 1 (mod m) if a and m are coprime because φ(m) = m - 1 if m is prime
    // this also yields x^k mod m = x^(k % (m - 1)) mod m if x and m are coprime
    ll power_big_number_mod(ll b, ll n, ll m)
    {
        // O(log(n))
        ll ans = 1;
        n %= m - 1;
        while (n)
        {
            if (n & 1)
                ans = (ans * b) % m;
            b = (b * b) % m;
            n >>= 1;
        }
        return ans;
    }

    // if a^b^c % m = a^(b^c % (m - 1)) % m
    ll power_tower_mod(ll a, ll b, ll c, ll m)
    {
        // O(log(m))
        return power_big_number_mod(a, power_big_number_mod(b, c, m - 1), m);
    }
};
struct Sieve
{
    // the sieve of Eratosthenes is an algorithm used to find all prime numbers up to a given integer
    // the time complexity of the sieve of Eratosthenes is O(n log log n)
    const int N = 1e6 + 5;
    vector<bool> is_prime(N, true);
    vector<int> prime;
    void sieve()
    {
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i < N; i++)
        {
            if (is_prime[i])
            {
                prime.push_back(i);
                for (int j = i * i; j < N; j += i)
                {
                    is_prime[j] = false;
                }
            }
        }
    }

    //! don't use to get prime numbers as i will not reach prime numbers greater than sqrt(n)
    void sieve()
    {
        // Sieving till root
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i * i < N; i++)
        {
            if (is_prime[i])
            {
                for (int j = i * i; j < N; j += i)
                {
                    is_prime[j] = false;
                }
            }
        }
    }

    void linear_sieve()
    {
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i < N; i++)
        {
            if (is_prime[i])
                prime.push_back(i);
            for (int j = 0; j < prime.size() && i * prime[j] < N; j++)
            {
                is_prime[i * prime[j]] = false;
                if (i % prime[j] == 0)
                    break;
            }
        }
    }

    // segmented sieve:
    // the segmented sieve of Eratosthenes is an algorithm used to find all prime numbers in a range [L, R]
    vector<char> segmentedSieve(long long L, long long R)
    {
        // O(sqrt(R) log log R + (R - L + 1) log log R)
        // generate all primes up to sqrt(R)
        long long lim = sqrt(R);
        vector<char> mark(lim + 1, false);
        vector<long long> primes;
        for (long long i = 2; i <= lim; ++i)
        {
            if (!mark[i])
            {
                primes.emplace_back(i);
                for (long long j = i * i; j <= lim; j += i)
                    mark[j] = true;
            }
        }

        vector<char> isPrime(R - L + 1, true);
        for (long long i : primes)
            for (long long j = max(i * i, (L + i - 1) / i * i); j <= R; j += i)
                isPrime[j - L] = false;
        if (L == 1)
            isPrime[0] = false;
        return isPrime;
    }

    // another implementation of segmented sieve
    vector<char> segmentedSieveNoPreGen(long long L, long long R)
    {
        // O((R - L + 1) log R + sqrt(R))
        vector<char> isPrime(R - L + 1, true);
        long long lim = sqrt(R);
        for (long long i = 2; i <= lim; ++i)
            for (long long j = max(i * i, (L + i - 1) / i * i); j <= R; j += i)
                isPrime[j - L] = false;
        if (L == 1)
            isPrime[0] = false;
        return isPrime;
    }

    // Function to generate primes up to N using a highly optimized sieve
    // O(N / log(N))
    // Number of primes up to 1e9 is 8702706
    // Approximate prime count using the formula pi(N) ~ N / log(N)
    vector<int> sieve(const int N, const int Q = 17, const int L = 1 << 15)
    {
        // Residues mod 30 that are coprime with 30.
        static const int residues[8] = {1, 7, 11, 13, 17, 19, 23, 29};

        // Structure to store prime and initial positions for each residue.
        struct PrimeData
        {
            int prime;
            int pos[8]; // For each residue in 'residues'
            PrimeData(int p) : prime(p) {}
        };

        // Approximate number of primes up to N (for reserving space).
        auto approxPrimeCount = [&](int n) -> int
        {
            return (n > 60184) ? n / (log(n) - 1.1)
                               : max(1.0, n / (log(n) - 1.11)) + 1;
        };

        int lim = sqrt(N), limSqrt = sqrt(lim);
        // Sieve up to lim to mark small primes.
        vector<bool> isPrime(lim + 1, true);
        for (int i = 2; i <= limSqrt; ++i)
            if (isPrime[i])
                for (int j = i * i; j <= lim; j += i)
                    isPrime[j] = false;

        int rsize = approxPrimeCount(N + 30);
        // Start with known small primes.
        vector<int> primes = {2, 3, 5};
        int primeCount = 3;
        primes.resize(rsize);

        vector<PrimeData> primeDataList;
        size_t baseCount = 0; // Count of primes <= Q used for wheel construction
        int wheelProd = 1;    // Product of small primes (<= Q)

        // Process primes from 7 to lim.
        for (int p = 7; p <= lim; ++p)
        {
            if (!isPrime[p])
                continue;
            if (p <= Q)
            {
                wheelProd *= p;
                ++baseCount;
                primes[primeCount++] = p;
            }
            PrimeData pd(p);
            // For each residue class, find the first multiple of p
            // that is congruent to that residue mod 30.
            for (int t = 0; t < 8; ++t)
            {
                int candidate = (p <= Q) ? p : p * p;
                while (candidate % 30 != residues[t])
                    candidate += p << 1; // candidate += 2*p
                pd.pos[t] = candidate / 30;
            }
            primeDataList.push_back(pd);
        }

        // Precompute a base wheel array of size = wheelProd.
        vector<unsigned char> baseWheel(wheelProd, 0xFF);
        for (size_t i = 0; i < baseCount; ++i)
        {
            auto pp = primeDataList[i];
            int p = pp.prime;
            for (int t = 0; t < 8; ++t)
            {
                unsigned char mask = ~(1 << t);
                for (int j = pp.pos[t]; j < wheelProd; j += p)
                    baseWheel[j] &= mask;
            }
        }

        // Process blocks of indices (each index represents a candidate of form 30*k + r)
        int blockSize = ((L + wheelProd - 1) / wheelProd) * wheelProd;
        vector<unsigned char> block(blockSize);
        unsigned char *blockPtr = block.data();
        int totalSlots = (N + 29) / 30; // total slots covering numbers up to N

        // Sieve each block and collect primes.
        for (int start = 0; start < totalSlots; start += blockSize, blockPtr -= blockSize)
        {
            int end = min(totalSlots, start + blockSize);
            // Copy base wheel into current block positions.
            for (int i = start; i < end; i += wheelProd)
                copy(baseWheel.begin(), baseWheel.end(), blockPtr + i);
            // The number 1 is not prime.
            if (start == 0)
                blockPtr[0] &= 0xFE;
            // For primes greater than Q, mark multiples in the block.
            for (size_t i = baseCount; i < primeDataList.size(); ++i)
            {
                auto &pd = primeDataList[i];
                int p = pd.prime;
                for (int t = 0; t < 8; ++t)
                {
                    int pos = pd.pos[t];
                    unsigned char mask = ~(1 << t);
                    for (; pos < end; pos += p)
                        blockPtr[pos] &= mask;
                    pd.pos[t] = pos;
                }
            }
            // Collect primes from the block.
            for (int i = start; i < end; ++i)
            {
                for (int mask = blockPtr[i]; mask > 0; mask &= mask - 1)
                    primes[primeCount++] = i * 30 + residues[__builtin_ctz(mask)];
            }
        }
        while (primeCount > 0 && primes[primeCount - 1] > N)
            --primeCount;
        primes.resize(primeCount);
        return primes;
    }
};
struct SPF
{
    // the smallest prime factor of a number n is the smallest prime number that divides n
    // the time complexity of the SPF algorithm is O(n log log n)
    const int N = 1e6 + 5;
    vector<int> spf(N);
    void SPF()
    {
        iota(spf.begin(), spf.end(), 0);
        for (int i = 2; i < N; i++)
        {
            if (spf[i] == i)
            {

                for (int j = 2 * i; j < N; j += i)
                {
                    if (spf[j] == j)
                    {
                        spf[j] = i;
                    }
                }
            }
        }
    }

    // the prime factorization of a number n can be found in O(log(n)) time
    map<int, int> prime_factorization(int n)
    {
        map<int, int> mp;
        while (n > 1)
        {
            mp[spf[n]]++;
            n /= spf[n];
        }
        return mp;
    }

    // the number of divisors of a number n can be found in O(log(n)) time
    int number_of_divisors(int n)
    {
        int ans = 1;
        while (n > 1)
        {
            int cnt = 1;
            int p = spf[n];
            while (n % p == 0)
            {
                cnt++;
                n /= p;
            }
            ans *= cnt;
        }
        return ans;
    }

    // the sum of divisors of a number n can be found in O(log(n)) time
    int sum_of_divisors(int n)
    {
        int ans = 1;
        while (n > 1)
        {
            int cnt = 1;
            int p = spf[n];
            int sum = 1;
            int pp = 1;
            while (n % p == 0)
            {
                cnt++;
                pp *= p;
                sum += pp;
                n /= p;
            }
            ans *= sum;
        }
        return ans;
    }

    // the product of divisors of a number n can be found in O(log(n)) time
    int product_of_divisors(int n)
    {
        ll ans = number_of_divisors(n);
        return binary_exponentiation(n, ans / 2);
    }

    // // the number of coprime numbers of a number n can be found in O(log(n)) time
    // int number_of_coprime(int n){
    //     int ans = n;
    //     while(n > 1){
    //         int p = spf[n];
    //         while(n % p == 0){
    //             n /= p;
    //         }
    //         ans = ans / p * (p - 1);
    //     }
    //     return ans;
    // }

    // // the sum of coprime numbers of a number n can be found in O(log(n)) time
    // int sum_of_coprime(int n){
    //     int ans = n;
    //     while(n > 1){
    //         int p = spf[n];
    //         while(n % p == 0){
    //             n /= p;
    //         }
    //         ans = ans / p * (p - 1);
    //     }
    //     return ans / 2;
    // }

    // // the product of coprime numbers of a number n can be found in O(log(n)) time
    // int product_of_coprime(int n){
    //     int ans = n;
    //     while(n > 1){
    //         int p = spf[n];
    //         while(n % p == 0){
    //             n /= p;
    //         }
    //         ans = ans / p * (p - 1);
    //     }
    //     return ans / 2;
    // }
};

void solve(int tc)
{
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