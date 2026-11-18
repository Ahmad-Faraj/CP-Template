// Test for 07_Math/Matrix_Power.cpp against naive __int128 products, naive repeated multiplication,
// a naive linear recurrence, and (for k up to 1e18) the period of A^k over a tiny field.
// Every public member is checked: Matrix(n, m), Matrix(vector) (negatives), identity, + - *,
// ^ (k = 0, small, huge), vec_mul, Matrix_Jumper (build + jump), for several values of Matrix::mod.
#include "../../07_Math/Matrix_Power.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(11);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
using VV = vector<vector<ll>>;

VV naive_mul(const VV &a, const VV &b, ll p) {
    int n = a.size(), m = b.size(), q = b[0].size();
    VV c(n, vector<ll>(q));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < q; j++) {
            __int128 s = 0;
            for (int k = 0; k < m; k++) s += (__int128)a[i][k] * b[k][j];
            c[i][j] = (ll)(((s % p) + p) % p);
        }
    return c;
}
VV rnd_vv(int n, int m, ll lo, ll hi) {
    VV a(n, vector<ll>(m));
    for (auto &r : a)
        for (auto &x : r) x = lo + (ll)(rng() % (unsigned long long)(hi - lo + 1));
    return a;
}
VV norm(VV a, ll p) {
    for (auto &r : a)
        for (auto &x : r) x = ((x % p) + p) % p;
    return a;
}

int main() {
    for (ll p : {1000000007LL, 998244353LL, 7LL, 2147483647LL, 2LL}) {
        Matrix::mod = p;
        string tag = "mod=" + to_string(p) + ": ";
        for (int it = 0; it < 3000; it++) {
            int n = rng() % 5 + 1, m = rng() % 5 + 1, q = rng() % 5 + 1;
            ll big = rng() % 2 ? 5 : (ll)4e18;
            VV a = rnd_vv(n, m, -big, big), b = rnd_vv(m, q, -big, big), c = rnd_vv(n, m, -big, big);
            Matrix A(a), B(b), C(c);
            chk(tag + "Matrix(vector) normalises", A.mat == norm(a, p) && A.n == n && A.m == m);
            Matrix Z(n, m);
            chk(tag + "Matrix(n, m) zero", Z.mat == VV(n, vector<ll>(m, 0)));
            chk(tag + "*", (A * B).mat == naive_mul(norm(a, p), norm(b, p), p));
            VV s = norm(a, p), d = norm(a, p);
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++) {
                    s[i][j] = (s[i][j] + ((c[i][j] % p) + p) % p) % p;
                    d[i][j] = ((d[i][j] - ((c[i][j] % p) + p) % p) % p + p) % p;
                }
            chk(tag + "+", (A + C).mat == s);
            chk(tag + "-", (A - C).mat == d);
            // ^ against repeated multiplication
            VV sq = rnd_vv(n, n, -big, big);
            Matrix S(sq);
            VV id(n, vector<ll>(n, 0));
            for (int i = 0; i < n; i++) id[i][i] = 1 % p;
            chk(tag + "identity", Matrix::identity(n).mat == norm(id, p));
            VV cur = norm(id, p);
            int K = rng() % 12;
            for (int k = 0; k <= K; k++) {
                chk(tag + (k ? "^ small k" : "^ 0"), (S ^ k).mat == cur);
                cur = naive_mul(cur, norm(sq, p), p);
            }
            // vec_mul and the jumper against repeated vector products
            vector<ll> v(n);
            for (auto &x : v) x = (ll)(rng() % 2000) - 1000;
            vector<ll> vn(n);
            for (int i = 0; i < n; i++) vn[i] = ((v[i] % p) + p) % p;
            chk(tag + "vec_mul", vec_mul(vn, S) == naive_mul(VV{vn}, norm(sq, p), p)[0]);
            Matrix_Jumper J(S, 8);
            vector<ll> w = vn;
            int k2 = rng() % 256;
            for (int k = 0; k < k2; k++) w = naive_mul(VV{w}, norm(sq, p), p)[0];
            chk(tag + "jump (small k, raw v)", J.jump(v, k2) == w);
        }
    }
    // huge k: over mod 7 with 2x2, A^k is eventually periodic; find it naively and compare
    Matrix::mod = 7;
    for (int it = 0; it < 300; it++) {
        VV a = rnd_vv(2, 2, 0, 6);
        map<VV, ll> seen;
        vector<VV> seq;
        VV cur = {{1, 0}, {0, 1}};
        while (!seen.count(cur)) seen[cur] = seq.size(), seq.push_back(cur), cur = naive_mul(cur, a, 7);
        ll mu = seen[cur], lam = (ll)seq.size() - mu;
        Matrix A(a);
        Matrix_Jumper J(A, 60);
        for (int q = 0; q < 20; q++) {
            ll k = (ll)(rng() >> 4);
            VV want = seq[k < mu ? k : mu + (k - mu) % lam];
            chk("^ huge k (period)", (A ^ k).mat == want);
            vector<ll> v = {(ll)(rng() % 7), (ll)(rng() % 7)};
            chk("jump huge k (period)", J.jump(v, k) == naive_mul(VV{v}, want, 7)[0]);
        }
    }
    // linear recurrence f(i) = sum_{j=1..M} c_j f(i-j): state row (f(i-M+1..i)), A[from][to]
    Matrix::mod = 1000000007;
    for (int it = 0; it < 300; it++) {
        int M = rng() % 6 + 1, N = rng() % 300 + M;
        vector<ll> cf(M + 1), f(N + 1);
        for (int j = 1; j <= M; j++) cf[j] = rng() % 1000000007;
        for (int i = 0; i < M; i++) f[i] = rng() % 1000000007;
        for (int i = M; i <= N; i++) {
            __int128 s = 0;
            for (int j = 1; j <= M; j++) s += (__int128)cf[j] * f[i - j];
            f[i] = (ll)(s % 1000000007);
        }
        Matrix A(M, M); // new[M-1] = sum cf[j] * old[M-j]; new[t] = old[t+1]
        for (int t = 0; t + 1 < M; t++) A.mat[t + 1][t] = 1;
        for (int j = 1; j <= M; j++) A.mat[M - j][M - 1] = cf[j];
        Matrix_Jumper J(A, 10);
        vector<ll> st(f.begin(), f.begin() + M);
        int k = N - (M - 1);
        chk("recurrence via jump", J.jump(st, k).back() == f[N]);
        chk("recurrence via ^", vec_mul(st, A ^ k).back() == f[N]);
    }
    {   // ACPC 2024 F size: M = 100, LOG = 60, 200 jumps
        int M = 100;
        Matrix A(M, M);
        for (int t = 0; t + 1 < M; t++) A.mat[t + 1][t] = 1;
        for (int j = 0; j < M; j++) A.mat[j][M - 1] = 1;
        auto t0 = clock();
        Matrix_Jumper J(A, 60);
        vector<ll> st(M, 1);
        for (int q = 0; q < 200; q++) st = J.jump(st, (ll)(rng() >> 4));
        Matrix P = A ^ (ll)1e18;
        printf("M=100: build 60 powers + 200 jumps + one A^1e18: %.3fs (%lld %lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, st[0], P.mat[0][0]);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
