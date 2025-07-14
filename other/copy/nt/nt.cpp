#include <bits/stdc++.h>
using namespace std;

vector<pair<int, int>> factors;
void getFactors(int n)
{
    factors.clear();
    int d = 1;
    for (int i = 2; i * i <= n; i += d, d = 2)
        if (n % i == 0)
        {
            factors.push_back(make_pair(i, 0));
            while (n % i == 0)
            {
                n /= i;
                factors.back().second++;
            }
        }
    if (n != 1)
        factors.push_back(make_pair(n, 1));
}

vector<int> primes;
vector<int> sieve(int n)
{
    vector<bool> is_prime(n + 1, true);
    vector<int> primes;

    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= n; i++)
    {
        if (is_prime[i])
        {
            primes.push_back(i);
            for (int j = i * i; j <= n; j += i)
            {
                is_prime[j] = false;
            }
        }
    }
    return primes;
}
