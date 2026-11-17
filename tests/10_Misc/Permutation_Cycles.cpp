// Test for 10_Misc/Permutation_Cycles.cpp against repeated application of the permutation.
// Every public function is checked: decompose (each cycle follows p, cycles partition 0..n-1),
// restore (round trip), order (smallest t with p^t = id, found by stepping), and power for
// k in [-2n, 2n] against stepping, and for huge / negative k against k mod order.
#include "../../10_Misc/Permutation_Cycles.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(1024);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

vector<int> compose_pow(const vector<int> &p, ll k) { // oracle: apply p (or its inverse) |k| times
    int n = p.size();
    vector<int> q(n), inv(n);
    for (int i = 0; i < n; i++) q[i] = i, inv[p[i]] = i;
    const vector<int> &s = k >= 0 ? p : inv;
    for (ll t = 0; t < (k >= 0 ? k : -k); t++)
        for (int i = 0; i < n; i++) q[i] = s[q[i]];
    return q;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 13;
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), rng);
        auto cyc = decompose(p);
        vector<int> seen(n, 0);
        bool ok = true;
        for (auto &v : cyc) {
            ok &= !v.empty();
            for (size_t i = 0; i < v.size(); i++) {
                ok &= p[v[i]] == v[(i + 1) % v.size()];
                seen[v[i]]++;
            }
        }
        for (int x : seen) ok &= x == 1;
        chk("decompose", ok);
        chk("restore", restore(n, cyc) == p);
        // order by stepping
        ll t = 1;
        vector<int> id(n);
        iota(id.begin(), id.end(), 0);
        while (compose_pow(p, t) != id) t++;
        ll ord = order(p);
        chk("order", ord == t);
        for (ll k = -2 * n - 1; k <= 2 * n + 1; k++)
            chk("power(small k)", restore(n, power(p, k)) == compose_pow(p, k));
        for (int q = 0; q < 6; q++) {
            ll k = (ll)(rng() >> 1) * (q % 2 ? -1 : 1);
            if (q == 4) k = LLONG_MAX;
            if (q == 5) k = LLONG_MIN + 1;
            ll r = k % ord;
            chk("power(huge k)", restore(n, power(p, k)) == compose_pow(p, r));
        }
    }
    { // order beyond int: cycles of lengths 23..47 (primes), lcm = 63392725189 (original gave junk)
        vector<int> ls{23, 29, 31, 37, 41, 43, 47}, p;
        for (int L : ls) {
            int s = p.size();
            for (int i = 0; i < L; i++) p.push_back(s + (i + 1) % L);
        }
        chk("order (> int)", order(p) == 63392725189LL);
        // power(order) and power(order + 1) on this permutation
        chk("power(order) = id", restore(p.size(), power(p, 63392725189LL)) ==
                                     compose_pow(p, 0));
        chk("power(order+1) = p", restore(p.size(), power(p, 63392725190LL)) == p);
    }
    {
        int n = 300000;
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), rng);
        auto t0 = clock();
        auto c = decompose(p);
        auto q = restore(n, power(p, 123456789012345LL));
        auto back = restore(n, c);
        ll o = order(vector<int>{1, 0, 2}); // tiny, just to touch
        printf("n=3e5 decompose+power+restore: %.3fs\n", double(clock() - t0) / CLOCKS_PER_SEC);
        chk("large restore", back == p && o == 2 && (int)q.size() == n);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
