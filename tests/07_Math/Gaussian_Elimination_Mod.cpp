// Test for 07_Math/Gaussian_Elimination_Mod.cpp. Oracle, no elimination anywhere: for tiny primes
// (2, 3, 5, 7) and n, m <= 4 enumerate all p^m vectors x to get the exact solution set of A x = b
// and the null space (rank = m - log_p |null space|); determinant by permutation expansion;
// inverse checked by A * inv = I and singular iff that determinant is 0. Large primes
// (998244353, 1e9+7): det vs permutation expansion, solve checked by substitution, rank against
// product-built low-rank matrices and rank(A) == rank(A^T). Checked: power, rank, det,
// inverse, solve (x and basis; rref through all of them), out-of-range entries, empty matrices.
#include "../../07_Math/Gaussian_Elimination_Mod.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(57);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

using Mat = vector<vector<ll>>;

Mat rnd_mat(int n, int m, ll p, int zero_pct) {
    Mat a(n, vector<ll>(m));
    for (auto &r : a)
        for (auto &v : r) v = (int)(rng() % 100) < zero_pct ? 0 : rng() % p;
    return a;
}

ll leibniz(const Mat &a, ll p) {
    int n = a.size();
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    ll s = 0;
    do {
        int inv = 0;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++) inv += perm[i] > perm[j];
        ll t = 1;
        for (int i = 0; i < n; i++) t = t * (((a[i][perm[i]] % p) + p) % p) % p;
        s = (s + (inv % 2 ? p - t : t)) % p;
    } while (next_permutation(perm.begin(), perm.end()));
    return s;
}

vector<ll> mul(const Mat &a, const vector<ll> &x, ll p) {
    vector<ll> r(a.size(), 0);
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < x.size(); j++) r[i] = ((r[i] + a[i][j] % p * x[j]) % p + p) % p;
    return r;
}

Mat mulm(const Mat &a, const Mat &b, ll p) {
    int n = a.size(), k = b.size(), m = k ? b[0].size() : 0;
    Mat c(n, vector<ll>(m, 0));
    for (int i = 0; i < n; i++)
        for (int t = 0; t < k; t++)
            for (int j = 0; j < m; j++) c[i][j] = (c[i][j] + a[i][t] * b[t][j]) % p;
    return c;
}

Mat transpose(const Mat &a, int m) {
    Mat t(m, vector<ll>(a.size()));
    for (size_t i = 0; i < a.size(); i++)
        for (int j = 0; j < m; j++) t[j][i] = a[i][j];
    return t;
}

bool ident(const Mat &a) {
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < a.size(); j++)
            if (a[i][j] != (i == j)) return false;
    return true;
}

int main() {
    // power
    for (ll p : {2LL, 3LL, 7LL, 998244353LL, 1000000007LL}) {
        Gauss_Mod g(p);
        for (int it = 0; it < 200; it++) {
            ll b = rng() % (p * 3), e = rng() % 50, want = 1 % p;
            for (int i = 0; i < e; i++) want = want * (b % p) % p;
            chk("power", g.power(b, e) == want);
        }
    }
    // exhaustive over tiny primes
    for (int it = 0; it < 30000; it++) {
        ll p = vector<ll>{2, 3, 5, 7}[rng() % 4];
        int n = rng() % 5, m = rng() % 5;
        if (p == 7 && m == 4 && rng() % 3) m = 3; // keep it fast
        Gauss_Mod g(p);
        Mat a = rnd_mat(n, m, p, vector<int>{0, 30, 60, 90}[rng() % 4]);
        if (n >= 2 && rng() % 3 == 0) a[rng() % n] = a[rng() % n]; // dependent rows
        vector<ll> b(n);
        for (auto &v : b) v = rng() % 4 == 0 ? 0 : rng() % p;
        if (rng() % 4 == 0) { // make it solvable
            vector<ll> x0(m);
            for (auto &v : x0) v = rng() % p;
            b = mul(a, x0, p);
        }
        // all vectors of Z_p^m
        ll tot = 1;
        for (int i = 0; i < m; i++) tot *= p;
        set<vector<ll>> sols, nul;
        for (ll code = 0; code < tot; code++) {
            vector<ll> x(m);
            ll c = code;
            for (int i = 0; i < m; i++) x[i] = c % p, c /= p;
            vector<ll> ax = mul(a, x, p);
            if (ax == b) sols.insert(x);
            if (count(ax.begin(), ax.end(), 0) == n) nul.insert(x);
        }
        int dim = 0;
        for (ll s = 1; s < (ll)nul.size(); s *= p) dim++;
        // rank, with entries shifted by multiples of p (also negative)
        Mat sh = a;
        for (auto &r : sh)
            for (auto &v : r) v += p * ((ll)(rng() % 7) - 3);
        chk("rank", g.rank(a) == m - dim);
        chk("rank(out-of-range entries)", g.rank(sh) == m - dim);
        // solve
        vector<ll> x;
        Mat basis;
        bool ok = g.solve(a, b, x, basis);
        if (m == 0) { // no unknowns: solvable iff b == 0
            chk("solve(m=0)", ok == (count(b.begin(), b.end(), 0) == n) && x.empty());
        } else if (n > 0) { // n = 0 has no rows to read m from (documented)
            chk("solve(exists)", ok == !sols.empty());
            if (ok) {
                chk("solve(x)", sols.count(x) == 1);
                chk("solve(basis size)", (int)basis.size() == dim);
                // x + span(basis) must equal the solution set exactly
                set<vector<ll>> span;
                ll k = 1;
                for (size_t i = 0; i < basis.size(); i++) k *= p;
                for (ll code = 0; code < k; code++) {
                    vector<ll> v = x;
                    ll c = code;
                    for (auto &bv : basis) {
                        ll coef = c % p;
                        c /= p;
                        for (int j = 0; j < m; j++) v[j] = (v[j] + coef * bv[j]) % p;
                    }
                    span.insert(v);
                }
                chk("solve(basis spans all solutions)", span == sols);
            }
        }
        // det + inverse on square matrices
        if (n == m) {
            ll want = leibniz(a, p);
            chk("det", g.det(a) == want);
            chk("det(out-of-range entries)", g.det(sh) == want);
            Mat inv;
            bool has = g.inverse(a, inv);
            chk("inverse(singular iff det 0)", has == (want != 0));
            if (has)
                chk("inverse(A*inv=I, inv*A=I)", ident(mulm(a, inv, p)) && ident(mulm(inv, a, p)));
        }
    }
    // large primes, small sizes
    for (int it = 0; it < 20000; it++) {
        ll p = rng() % 2 ? 998244353 : 1000000007;
        Gauss_Mod g(p);
        int n = 1 + rng() % 6, m = 1 + rng() % 6, k = 1 + rng() % 6;
        Mat a;
        bool prod = rng() % 2 == 0;
        if (!prod) a = rnd_mat(n, m, p, vector<int>{0, 50, 90}[rng() % 3]);
        else a = mulm(rnd_mat(n, k, p, 0), rnd_mat(k, m, p, 0), p); // rank min(n, m, k) w.h.p.
        int r = g.rank(a);
        chk("rank(transpose)", r == g.rank(transpose(a, m)));
        if (prod) chk("rank(product)", r == min({n, m, k}));
        vector<ll> x0(m);
        for (auto &v : x0) v = rng() % p;
        vector<ll> b = mul(a, x0, p), x;
        Mat basis;
        bool ok = g.solve(a, b, x, basis);
        bool good = ok && mul(a, x, p) == b && (int)basis.size() == m - r;
        for (auto &v : basis)
            good &= count(v.begin(), v.end(), 0) < m && mul(a, v, p) == vector<ll>(n, 0);
        // basis must be independent: its rank equals its size
        good &= basis.empty() || g.rank(basis) == (int)basis.size();
        chk("solve(large p, solvable)", good);
        if (r < n) { // perturb b off the column space: random b is unsolvable w.h.p.
            vector<ll> b2(n);
            for (auto &v : b2) v = rng() % p;
            Mat aug = a;
            for (int i = 0; i < n; i++) aug[i].push_back(b2[i]);
            bool want = g.rank(aug) == r;
            chk("solve(large p, rouche-capelli)", g.solve(a, b2, x, basis) == want);
        }
        if (n == m) {
            ll want = leibniz(a, p);
            chk("det(large p)", g.det(a) == want);
            Mat inv;
            bool has = g.inverse(a, inv);
            chk("inverse(large p)", has == (want != 0) && (!has || ident(mulm(a, inv, p))));
        }
    }
    // empty matrices
    {
        Gauss_Mod g;
        Mat e, inv;
        chk("empty", g.rank(e) == 0 && g.det(e) == 1 && g.inverse(e, inv) && inv.empty());
        chk("empty", g.rank(Mat(3, vector<ll>())) == 0);
    }
    // timing: n = m = 500
    {
        Gauss_Mod g;
        int n = 500;
        Mat a = rnd_mat(n, n, 998244353, 0), inv;
        auto t0 = clock();
        ll d = g.det(a);
        double t1 = double(clock() - t0) / CLOCKS_PER_SEC;
        t0 = clock();
        bool has = g.inverse(a, inv);
        double t2 = double(clock() - t0) / CLOCKS_PER_SEC;
        t0 = clock();
        vector<ll> b(n, 1), x;
        Mat basis;
        g.solve(a, b, x, basis);
        double t3 = double(clock() - t0) / CLOCKS_PER_SEC;
        t0 = clock();
        int r1 = g.rank(Mat(250000, vector<ll>(1, 5)));
        int r2 = g.rank(rnd_mat(1, 250000, 998244353, 0));
        double t4 = double(clock() - t0) / CLOCKS_PER_SEC;
        chk("large", d != 0 && has && ident(mulm(a, inv, 998244353)) &&
                         mul(a, x, 998244353) == b && r1 == 1 && r2 == 1);
        printf("500x500: det %.3fs inverse %.3fs solve %.3fs; rank 250000x1 + 1x250000 %.3fs\n", t1,
               t2, t3, t4);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-36s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
