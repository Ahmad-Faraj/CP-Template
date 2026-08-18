// Brute-force test for:
//   Math/Combinatorics/Binomial_Coefficients.cpp
//   Math/Combinatorics/Catalan.cpp
//   Math/Combinatorics/Permutations.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace BI {
#include "../Math/Combinatorics/Binomial_Coefficients.cpp"
}
namespace CA {
#include "../Math/Combinatorics/Catalan.cpp"
}
namespace PM {
#include "../Math/Combinatorics/Permutations.cpp"
}

int main() {
    mt19937_64 gen(4242);
    const ll P = 1000000007;
    int ncrFail = 0, nprFail = 0, catFail = 0, sbFail = 0, sbpFail = 0, mnFail = 0, derFail = 0, lucasFail = 0;
    int catRecFail = 0, catTabFail = 0, catOneFail = 0, catModFail = 0;
    int applyFail = 0, powFail = 0, decFail = 0, resFail = 0, powCycFail = 0, ordFail = 0, ordModFail = 0, sgnFail = 0, enumFail = 0;

    // ---------- binomials against Pascal's triangle ----------
    {
        const int N = 60;
        vector<vector<ll>> pas(N + 1, vector<ll>(N + 1, 0));
        for (int i = 0; i <= N; i++) {
            pas[i][0] = 1 % P;
            for (int j = 1; j <= i; j++) pas[i][j] = (pas[i - 1][j - 1] + pas[i - 1][j]) % P;
        }
        BI::Binomial b(200, P);
        for (int n = 0; n <= N; n++)
            for (int r = -2; r <= N + 2; r++) {
                ll want = (r < 0 || r > n) ? 0 : pas[n][r];
                if (b.nCr(n, r) != want) ncrFail++;
                // nPr = nCr * r!
                ll wantP = (r < 0 || r > n) ? 0 : want * b.fact[r] % P;
                if (b.nPr(n, r) != wantP) nprFail++;
            }
        // catalan against the recurrence
        vector<ll> cr = CA::catalan_recurrence(40, P);
        for (int n = 0; n <= 40; n++)
            if (b.catalan(n) != cr[n]) catFail++;
        // stars and bars against a direct count of compositions
        for (int n = 0; n <= 8; n++)
            for (int k = 1; k <= 5; k++) {
                // count solutions to x1+..+xk = n with xi >= 0
                vector<int> x(k, 0);
                ll cnt = 0, cntPos = 0;
                function<void(int, int)> rec = [&](int idx, int left) {
                    if (idx == k) {
                        if (left == 0) {
                            cnt++;
                            bool allPos = true;
                            for (int v : x)
                                if (v == 0) allPos = false;
                            if (allPos) cntPos++;
                        }
                        return;
                    }
                    for (int v = 0; v <= left; v++) {
                        x[idx] = v;
                        rec(idx + 1, left - v);
                    }
                };
                rec(0, n);
                if (b.stars_and_bars(n, k) != cnt % P) sbFail++;
                if (b.stars_and_bars_positive(n, k) != cntPos % P) sbpFail++;
            }
        // multinomial against factorial arithmetic
        for (int it = 0; it < 2000; it++) {
            int k = 1 + (int)(gen() % 4);
            vector<int> parts(k);
            int total = 0;
            for (int &v : parts) v = (int)(gen() % 8), total += v;
            ll want = b.fact[total];
            for (int v : parts) want = want * b.inv_fact[v] % P;
            if (b.multinomial(parts) != want) mnFail++;
        }
        // derangements against inclusion-exclusion via brute force for small n
        for (int n = 0; n <= 8; n++) {
            vector<int> perm(n);
            iota(perm.begin(), perm.end(), 0);
            ll cnt = 0;
            if (n == 0) cnt = 1;
            else {
                do {
                    bool fixedPoint = false;
                    for (int i = 0; i < n; i++)
                        if (perm[i] == i) fixedPoint = true;
                    if (!fixedPoint) cnt++;
                } while (next_permutation(perm.begin(), perm.end()));
            }
            if (b.derangements(n) != cnt % P) derFail++;
        }
        // n = 0 must not crash and must give sane answers
        BI::Binomial tiny(0, P);
        if (tiny.nCr(0, 0) != 1) ncrFail++;
    }

    // ---------- Lucas against Pascal's triangle mod p ----------
    // Pascal is only ADDITION, so it is valid for any modulus and for n >= p, where a
    // factorial table is not: p! == 0 mod p, so the inverse factorials do not exist.
    for (ll p : {(ll)5, (ll)7, (ll)13}) {
        const int N = 80;
        vector<vector<ll>> pas(N + 1, vector<ll>(N + 1, 0));
        for (int i = 0; i <= N; i++) {
            pas[i][0] = 1 % p;
            for (int j = 1; j <= i; j++) pas[i][j] = (pas[i - 1][j - 1] + pas[i - 1][j]) % p;
        }
        for (ll n = 0; n <= N; n++)
            for (ll r = 0; r <= n; r++)
                if (BI::lucas_nCr(n, r, p) != pas[n][r]) lucasFail++;
    }

    // ---------- Catalan three ways ----------
    {
        vector<ll> rec = CA::catalan_recurrence(200, P);
        vector<ll> tab = CA::catalan_table(200, P);
        for (int n = 0; n <= 200; n++) {
            if (rec[n] != tab[n]) catTabFail++;
            if (CA::catalan_single(n, P) != rec[n]) catOneFail++;
        }
        // known values
        vector<ll> known{1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862};
        for (int i = 0; i < (int)known.size(); i++)
            if (rec[i] != known[i]) catRecFail++;
        // the recurrence must work under a COMPOSITE modulus too
        ll comp = 1000000;
        vector<ll> rc = CA::catalan_recurrence(30, comp);
        vector<ll> rp = CA::catalan_recurrence(30, P);
        for (int n = 0; n <= 9; n++)
            if (rc[n] != known[n] % comp) catModFail++;
        (void)rp;
    }

    // ---------- permutations ----------
    for (int it = 0; it < 5000; it++) {
        int n = 1 + (int)(gen() % 9);
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), gen);
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = (int)(gen() % 100);

        // permute_times against repeated application
        ll k = (ll)(gen() % 12);
        vector<int> want = a;
        for (ll t = 0; t < k; t++) want = PM::apply_permutation(want, p);
        if (PM::permute_times(a, p, k) != want) powFail++;
        if (PM::permute_times(a, p, 0) != a) applyFail++;

        // decompose: every element in exactly one cycle, and each cycle closes under p
        vector<vector<int>> cyc = PM::decompose(p);
        vector<int> count(n, 0);
        for (auto &c : cyc) {
            for (size_t i = 0; i < c.size(); i++) {
                count[c[i]]++;
                if (p[c[i]] != c[(i + 1) % c.size()]) decFail++;
            }
        }
        for (int i = 0; i < n; i++)
            if (count[i] != 1) decFail++;
        // restore must invert decompose
        if (PM::restore(n, cyc) != p) resFail++;

        // order: p^order is the identity, and no smaller k is
        ll ord = PM::order(p);
        vector<int> id(n);
        iota(id.begin(), id.end(), 0);
        if (PM::permute_times(id, p, ord) != id) ordFail++;
        for (ll t = 1; t < min(ord, (ll)40); t++)
            if (PM::permute_times(id, p, t) == id) ordFail++;
        if (PM::order_mod(p, P) != ord % P) ordModFail++;

        // power_cycles must describe p^k
        for (ll kk = 1; kk <= 4; kk++) {
            vector<int> pk = PM::permute_times(id, p, kk);
            vector<vector<int>> got = PM::power_cycles(p, kk);
            vector<vector<int>> want2 = PM::decompose(pk);
            // compare as sets of cycles, normalised to start at the smallest element
            auto norm = [](vector<vector<int>> cs) {
                for (auto &c : cs) {
                    auto it = min_element(c.begin(), c.end());
                    rotate(c.begin(), it, c.end());
                }
                sort(cs.begin(), cs.end());
                return cs;
            };
            if (norm(got) != norm(want2)) powCycFail++;
        }

        // sign against counting inversions of the permutation
        int inv = 0;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (p[i] > p[j]) inv++;
        if (PM::sign(p) != ((inv & 1) ? -1 : 1)) sgnFail++;
    }
    // enumeration visits n! permutations
    for (int n = 1; n <= 6; n++) {
        vector<int> items(n);
        iota(items.begin(), items.end(), 0);
        ll seen = 0, factorial = 1;
        for (int i = 2; i <= n; i++) factorial *= i;
        set<vector<int>> uniq;
        PM::for_each_permutation(items, [&](const vector<int> &v) { seen++; uniq.insert(v); });
        if (seen != factorial || (ll)uniq.size() != factorial) enumFail++;
    }

    cout << "Combinatorics\n";
    cout << "  nCr vs Pascal's triangle       : " << ncrFail << " failures\n";
    cout << "  nPr = nCr * r!                 : " << nprFail << " failures\n";
    cout << "  Binomial::catalan vs recurrence: " << catFail << " failures\n";
    cout << "  stars_and_bars                 : " << sbFail << " failures\n";
    cout << "  stars_and_bars_positive        : " << sbpFail << " failures\n";
    cout << "  multinomial                    : " << mnFail << " failures\n";
    cout << "  derangements vs brute force    : " << derFail << " failures\n";
    cout << "  Lucas vs direct table          : " << lucasFail << " failures\n";
    cout << "  Catalan known values           : " << catRecFail << " failures\n";
    cout << "  Catalan recurrence == table    : " << catTabFail << " failures\n";
    cout << "  catalan_single agrees          : " << catOneFail << " failures\n";
    cout << "  Catalan under composite modulus: " << catModFail << " failures\n";
    cout << "  apply / permute_times          : " << applyFail + powFail << " failures\n";
    cout << "  decompose is a partition       : " << decFail << " failures\n";
    cout << "  restore inverts decompose      : " << resFail << " failures\n";
    cout << "  power_cycles describes p^k     : " << powCycFail << " failures\n";
    cout << "  order is the smallest exponent : " << ordFail << " failures\n";
    cout << "  order_mod agrees               : " << ordModFail << " failures\n";
    cout << "  sign vs inversion parity       : " << sgnFail << " failures\n";
    cout << "  enumeration visits n!          : " << enumFail << " failures\n";
}
