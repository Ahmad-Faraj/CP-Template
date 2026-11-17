// Test for 07_Math/FFT.cpp against naive O(n*m) convolution in __int128 and schoolbook decimal
// multiplication. Every public function is checked: fft (forward/inverse round trip and against a
// naive DFT), multiply, poly_pow, mul_big_int. Precision is probed at the documented limit.
#include "../../07_Math/FFT.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(41);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }

vector<__int128> naive(const vector<ll> &a, const vector<ll> &b) {
    if (a.empty() || b.empty()) return {};
    vector<__int128> c(a.size() + b.size() - 1, 0);
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < b.size(); j++) c[i + j] += (__int128)a[i] * b[j];
    return c;
}
bool same(const vector<ll> &x, const vector<__int128> &y) {
    if (x.size() != y.size()) return false;
    for (size_t i = 0; i < x.size(); i++)
        if ((__int128)x[i] != y[i]) return false;
    return true;
}
string school(const string &a, const string &b) { // schoolbook decimal multiplication
    vector<int> r(a.size() + b.size(), 0);
    for (int i = a.size() - 1; i >= 0; i--)
        for (int j = b.size() - 1; j >= 0; j--) {
            int k = i + j + 1;
            r[k] += (a[i] - '0') * (b[j] - '0');
            r[k - 1] += r[k] / 10, r[k] %= 10;
        }
    string s;
    for (int d : r)
        if (!s.empty() || d) s.push_back('0' + d);
    return s.empty() ? "0" : s;
}

int main() {
    // multiply: small random, negatives, empty, n = 1
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 40, m = rng() % 40;
        ll lim = vector<ll>{1, 9, 1000, 1000000}[rng() % 4];
        bool neg = rng() % 2;
        vector<ll> a(n), b(m);
        for (auto &x : a) x = rnd(neg ? -lim : 0, lim);
        for (auto &x : b) x = rnd(neg ? -lim : 0, lim);
        chk(n && m ? "multiply" : "multiply(empty)", same(multiply(a, b), naive(a, b)));
    }
    // fft: forward matches a naive DFT (sign +), inverse undoes it
    for (int it = 0; it < 300; it++) {
        int n = 1 << (rng() % 8);
        vector<cplx> a(n);
        for (auto &x : a) x = cplx(rnd(-100, 100), rnd(-100, 100));
        vector<cplx> f = a;
        fft(f, false);
        long double err = 0;
        for (int k = 0; k < n; k++) {
            cplx s = 0;
            for (int j = 0; j < n; j++) s += a[j] * polar(1.0L, 2 * acosl(-1) * j * k / n);
            err = max(err, abs(s - f[k]));
        }
        chk("fft(forward)", err < 1e-9);
        fft(f, true);
        err = 0;
        for (int j = 0; j < n; j++) err = max(err, abs(f[j] - a[j]));
        chk("fft(inverse)", err < 1e-9);
    }
    // poly_pow against repeated naive multiplication
    for (int it = 0; it < 2000; it++) {
        int n = rng() % 5;
        ll e = rng() % 7;
        vector<ll> a(n);
        for (auto &x : a) x = rnd(-3, 3);
        vector<__int128> want{1};
        for (int k = 0; k < e; k++) {
            vector<ll> w(want.begin(), want.end());
            want = naive(w, a);
        }
        chk("poly_pow", same(poly_pow(a, e), want));
    }
    // mul_big_int against schoolbook, with leading zeros and zero operands
    for (int it = 0; it < 3000; it++) {
        auto gen = [&]() {
            int len = rng() % 3 == 0 ? rng() % 4 + 1 : rng() % 300 + 1;
            string s;
            for (int i = 0; i < len; i++) s.push_back('0' + rng() % 10);
            if (rng() % 10 == 0) s = string(rng() % 3 + 1, '0');
            return s;
        };
        string a = gen(), b = gen();
        chk("mul_big_int", mul_big_int(a, b) == school(a, b));
    }
    chk("mul_big_int", mul_big_int("123", "456") == "56088" && mul_big_int("0", "0") == "0");
    {
        string a(20000, '9'); // 99..9^2 = 99..9800..01
        string want = string(19999, '9') + "8" + string(19999, '0') + "1";
        chk("mul_big_int(20000 digits)", mul_big_int(a, a) == want);
    }
    // precision near the limit: sampled indices against exact __int128 dot products
    auto probe = [&](int lg, ll mx, bool expect_exact) {
#ifdef __SANITIZE_ADDRESS__
        lg = min(lg, 16);
#endif
        int n = 1 << lg;
        vector<ll> a(n), b(n);
        for (auto &x : a) x = rnd(mx / 2, mx);
        for (auto &x : b) x = rnd(mx / 2, mx);
        auto c = multiply(a, b);
        int bad = 0;
        for (int s = 0; s < 120; s++) {
            int k = s == 0 ? n - 1 : rng() % c.size();
            __int128 v = 0;
            for (int i = max<int>(0, k - n + 1); i <= min<int>(k, n - 1); i++) v += (__int128)a[i] * b[k - i];
            bad += (__int128)c[k] != v;
        }
        printf("precision: 2^%d x 2^%d terms in [%lld, %lld]: max |c| ~ %.1e, wrong %d / 120\n", lg, lg,
               mx / 2, mx, (double)mx * mx * n, bad);
        if (expect_exact) chk("multiply(precision 1e17)", bad == 0);
    };
    probe(19, 1000000, true);   // |c| up to ~5e17
    probe(17, 1000000, true);
    probe(15, 3000000, true);   // |c| up to ~3e17
    probe(17, 10000000, false); // ~1e19: past the limit (and past ll), only reported
    {
        int n = 1 << 19;
        vector<ll> a(n), b(n);
        for (auto &x : a) x = rnd(0, 1000);
        for (auto &x : b) x = rnd(0, 1000);
        auto t0 = clock();
        auto c = multiply(a, b);
        printf("multiply 2^19 x 2^19 (padded 2^20): %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC,
               c[n]);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
