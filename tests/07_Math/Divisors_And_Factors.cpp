// Test for 07_Math/Divisors_And_Factors.cpp against sieve tables (divisor lists, smallest prime factor)
// for every n <= 2e5 plus random n <= 1e6, and against numbers built from known primes up to ~1e18.
// Every public function is checked: prime_factorization, get_factors, get_divisors, count_divisors,
// get_sum_div.
#include "../../07_Math/Divisors_And_Factors.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(31);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

bool naive_prime(ll x) {
    if (x < 2) return false;
    for (ll d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

int main() {
    const int N = 1000000;
    vector<int> spf(N + 1, 0);
    for (int i = 2; i <= N; i++)
        if (!spf[i])
            for (int j = i; j <= N; j += i)
                if (!spf[j]) spf[j] = i;
    const int D = 200000; // full divisor lists up to D
    vector<vector<ll>> divs(D + 1);
    for (int d = 1; d <= D; d++)
        for (int m = d; m <= D; m += d) divs[m].push_back(d);
    vector<__int128> pref_sigma(N + 1, 0); // sum_{i <= n} sigma(i), sigma by a divisor sieve
    {
        vector<ll> sigma(N + 1, 0);
        for (int d = 1; d <= N; d++)
            for (int m = d; m <= N; m += d) sigma[m] += d;
        for (int i = 1; i <= N; i++) pref_sigma[i] = pref_sigma[i - 1] + sigma[i];
    }
    auto oracle_pf = [&](ll n) { // n <= N
        vector<ll> v;
        while (n > 1) v.push_back(spf[n]), n /= spf[n];
        return v;
    };
    auto grouped = [](const vector<ll> &pf) {
        vector<pair<ll, int>> g;
        for (ll p : pf)
            if (!g.empty() && g.back().first == p) g.back().second++;
            else g.push_back({p, 1});
        return g;
    };
    auto check_small = [&](ll n) {
        auto pf = oracle_pf(n);
        chk("prime_factorization", prime_factorization(n) == pf);
        chk("get_factors", get_factors(n) == grouped(pf));
        vector<ll> dv;
        if (n <= D) dv = divs[n];
        else
            for (ll d = 1; d <= n; d++)
                if (n % d == 0) dv.push_back(d);
        chk("get_divisors", get_divisors(n) == dv);
        chk("count_divisors", count_divisors(n) == (ll)dv.size());
        chk("get_sum_div", (__int128)get_sum_div(n) == pref_sigma[n]);
    };
    for (ll n = 1; n <= D; n++) check_small(n);
    for (int it = 0; it < 300; it++) check_small(rng() % N + 1);
    for (ll n : {999983LL, 1000000LL, 998001LL /* 999^2 */, 994009LL /* 997^2 */}) check_small(n);

    // big numbers with a known factorization: products of random primes
    vector<ll> small_pr, mid_pr; // < 1000, around 1e6..1e7
    for (ll p = 2; p < 1000; p++)
        if (naive_prime(p)) small_pr.push_back(p);
    for (ll p = 1000003; mid_pr.size() < 200; p += 2)
        if (naive_prime(p)) mid_pr.push_back(p);
    vector<ll> big_pr = {999999937LL, 1000000007LL, 2147483647LL, 4294967311LL, 999999999989LL};
    for (ll p : big_pr) chk("oracle:prime", naive_prime(p));
    auto check_big = [&](vector<ll> pf, bool divisor_checks) {
        sort(pf.begin(), pf.end());
        __int128 n128 = 1;
        for (ll p : pf) n128 *= p;
        if (n128 > (__int128)4e18) return;
        ll n = (ll)n128;
        chk("prime_factorization(big)", prime_factorization(n) == pf);
        auto g = grouped(pf);
        chk("get_factors(big)", get_factors(n) == g);
        if (!divisor_checks) return;
        vector<ll> dv{1}; // all divisors from the factorization
        for (auto [p, e] : g) {
            int sz = dv.size();
            ll pw = 1;
            for (int k = 1; k <= e; k++) {
                pw *= p;
                for (int i = 0; i < sz; i++) dv.push_back(dv[i] * pw);
            }
        }
        sort(dv.begin(), dv.end());
        chk("get_divisors(big)", get_divisors(n) == dv);
        chk("count_divisors(big)", count_divisors(n) == (ll)dv.size());
    };
    for (int it = 0; it < 400; it++) { // n up to ~1e12-1e18, all factors small or mid
        vector<ll> pf;
        int k = rng() % 8 + 1;
        for (int i = 0; i < k; i++) pf.push_back(small_pr[rng() % small_pr.size()]);
        if (rng() % 2) pf.push_back(mid_pr[rng() % mid_pr.size()]);
        if (rng() % 3 == 0) pf.push_back(mid_pr[rng() % mid_pr.size()]);
        __int128 n = 1;
        for (ll p : pf) n *= p;
        check_big(pf, n <= (__int128)1e13);
    }
    // the cases main's version got wrong: a prime factor past 2^31, n near INT_MAX, prime squares
    check_big({2, 4294967311LL}, true);
    check_big({2147483647LL}, true);
    check_big({3, 3, 999999999989LL}, false);
    check_big({1000003, 1000003}, true);
    check_big({999999937LL, 999999937LL}, false); // ~1e18, sqrt loop runs to 1e9
    check_big({1000000007LL}, true);

    // get_sum_div for x > 1e6: sum_{k <= x} T(x / k), T(m) = m (m + 1) / 2, a different identity
    for (ll x : {1000001LL, 123456789LL, 1000000000LL, 2000000000LL, 3000000000LL}) {
        __int128 want = 0;
        for (ll k = 1, r; k <= x; k = r + 1) {
            ll q = x / k;
            r = x / q;
            want += (__int128)(r - k + 1) * q * (q + 1) / 2;
        }
        chk("get_sum_div(big)", (__int128)get_sum_div(x) == want);
    }
    {
        auto t0 = clock();
        ll s = 0;
        for (int i = 0; i < 200; i++) s += count_divisors(1000000000000LL - i); // 1e6 steps each
        s += get_factors(999999999989LL).size() + get_divisors(963761198400LL).size();
        printf("200 count_divisors(~1e12) + factor/divisors of ~1e12: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
