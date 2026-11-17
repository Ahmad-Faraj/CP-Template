// Test for 10_Misc/Big_Int.cpp against __int128 (values that fit) and, for huge values, against an
// independent decimal-string oracle: schoolbook multiplication and residues mod three large primes.
// Every public member is checked: ctor(ll), ctor(string), read, operator=(ll), >>, <<, the six
// comparisons, unary -, += -= + -, * and *= (BigInt and ll), / and /= (BigInt and ll), % (BigInt and
// ll), divmod, abs, zero, cmp_abs, mul_simple, mul_fft (FFT path), gcd, power.
#include "../../10_Misc/Big_Int.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(2026);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

using i128 = __int128;
string to_str(i128 v) {
    if (v == 0) return "0";
    bool neg = v < 0;
    unsigned __int128 u = neg ? (unsigned __int128)0 - (unsigned __int128)v : (unsigned __int128)v;
    string s;
    while (u) s += char('0' + (int)(u % 10)), u /= 10;
    if (neg) s += '-';
    reverse(s.begin(), s.end());
    return s;
}
string show(const BigInt &b) {
    ostringstream os;
    os << b;
    return os.str();
}
bool same(const BigInt &b, i128 v) { return show(b) == to_str(v); }

// ---- independent oracle on signed decimal strings ----
const ll P[3] = {(ll)1e18 + 9, 999999999999999989LL, 1000000000000000003LL};
ll mulmod(ll a, ll b, ll p) { return (ll)((i128)a * b % p); }
ll smod(const string &s, ll p) { // value of the decimal string mod p, in [0, p)
    ll r = 0;
    bool neg = s[0] == '-';
    for (char c : s)
        if (isdigit(c)) r = (ll)(((i128)r * 10 + (c - '0')) % p);
    return neg ? (p - r) % p : r;
}
string absstr(const string &s) { return s[0] == '-' ? s.substr(1) : s; }
bool abs_less(const string &x, const string &y) { // |x| < |y| for canonical decimal strings
    string a = absstr(x), b = absstr(y);
    return a.size() != b.size() ? a.size() < b.size() : a < b;
}
string school_mul(const string &x, const string &y) { // |x| * |y| by base-1e4 schoolbook, unsigned
    auto limbs = [](const string &s) {
        vector<ll> v;
        for (int i = (int)s.size(); i > 0; i -= 4) v.push_back(stoll(s.substr(max(0, i - 4), i - max(0, i - 4))));
        return v;
    };
    vector<ll> a = limbs(absstr(x)), b = limbs(absstr(y)), c(a.size() + b.size() + 1, 0);
    for (size_t i = 0; i < a.size(); i++) {
        ll carry = 0;
        for (size_t j = 0; j < b.size() || carry; j++) {
            ll cur = c[i + j] + carry + (j < b.size() ? a[i] * b[j] : 0);
            c[i + j] = cur % 10000, carry = cur / 10000;
        }
    }
    while (c.size() > 1 && c.back() == 0) c.pop_back();
    string s = to_string(c.back());
    for (int i = (int)c.size() - 2; i >= 0; i--) {
        string t = to_string(c[i]);
        s += string(4 - t.size(), '0') + t;
    }
    bool neg = (x[0] == '-') != (y[0] == '-') && s != "0";
    return (neg ? "-" : "") + s;
}
string rnd_digits(int len, bool allow_neg = true, int style = 0) {
    string s;
    if (style == 1) s = string(len, '9');
    else if (style == 2) s = "1" + string(len - 1, '0');
    else {
        s += char('1' + rng() % 9);
        for (int i = 1; i < len; i++) s += char('0' + rng() % 10);
    }
    if (allow_neg && rng() % 2) s = "-" + s;
    return s;
}
i128 pick(int scale) {
    switch (scale) {
    case 0: return (i128)(ll)(rng() % 41) - 20;
    case 1: return (i128)(ll)(rng() % 4000000001ULL) - 2000000000;
    case 2: return (i128)(ll)rng(); // full 64-bit range, negatives included
    default: { // near the 1e9 limb boundaries
        i128 b = (i128)1;
        for (int k = rng() % 4; k-- > 0;) b *= 1000000000;
        return (rng() % 2 ? -1 : 1) * (b + (i128)(ll)(rng() % 5) - 2);
    }
    }
}

#ifdef __SANITIZE_ADDRESS__
const int SCALE = 3; // the sanitizer build runs a third of the random cases
#else
const int SCALE = 1;
#endif

int main() {
    // ---------------- small and 64-bit values against __int128 ----------------
    for (int it = 0; it < 200000 / SCALE; it++) {
        i128 x = pick(rng() % 4), y = pick(rng() % 4);
        while (x > LLONG_MAX || x < LLONG_MIN) x /= 1000;
        while (y > LLONG_MAX || y < LLONG_MIN) y /= 1000;
        ll xl = (ll)x, yl = (ll)y;
        BigInt a(xl), b(to_str(y)), c;
        c = xl;
        chk("ctor(ll)", same(a, x));
        chk("ctor(string)", same(b, y));
        chk("operator=(ll)", same(c, x));
        chk("operator<", (a < b) == (x < y));
        chk("operator>", (a > b) == (x > y));
        chk("operator<=", (a <= b) == (x <= y));
        chk("operator>=", (a >= b) == (x >= y));
        chk("operator==", (a == b) == (x == y) && a == a);
        chk("operator!=", (a != b) == (x != y));
        i128 ax = x < 0 ? -x : x, ay = y < 0 ? -y : y;
        chk("cmp_abs", cmp_abs(a, b) == (ax < ay ? -1 : ax > ay ? 1 : 0));
        chk("unary -", same(-a, -x));
        chk("abs", same(a.abs(), ax));
        chk("zero", a.zero() == (x == 0));
        chk("operator+", same(a + b, x + y));
        chk("operator-", same(a - b, x - y));
        chk("operator*(BigInt)", same(a * b, x * y));
        chk("operator*(ll)", same(a * yl, x * y));
        chk("mul_simple", same(a.mul_simple(b), x * y));
        BigInt t = a;
        t += b;
        chk("operator+=", same(t, x + y));
        t = a, t -= b;
        chk("operator-=", same(t, x - y));
        t = a, t *= b;
        chk("operator*=(BigInt)", same(t, x * y));
        t = a, t *= yl;
        chk("operator*=(ll)", same(t, x * y));
        if (y != 0) {
            chk("operator/(BigInt)", same(a / b, x / y));
            chk("operator%(BigInt)", same(a % b, x % y));
            auto [q, r] = divmod(a, b);
            chk("divmod", same(q, x / y) && same(r, x % y));
            chk("operator/(ll)", same(a / yl, x / y));
            t = a, t /= b;
            chk("operator/=(BigInt)", same(t, x / y));
            t = a, t /= yl;
            chk("operator/=(ll)", same(t, x / y));
            if (y >= -9000000000LL && y <= 9000000000LL) chk("operator%(ll)", (i128)(a % yl) == x % y);
        }
        // gcd against Euclid on __int128
        i128 g1 = ax, g2 = ay;
        while (g2) g1 %= g2, swap(g1, g2);
        chk("gcd", same(gcd(a, b), g1));
        // power, while it fits
        if (ax <= 1000) {
            int k = rng() % 12;
            i128 pw = 1;
            for (int i = 0; i < k; i++) pw *= x;
            chk("power", same(power(a, k), pw));
        }
        // stream I/O
        stringstream ss;
        ss << a << ' ' << b;
        BigInt r1, r2;
        ss >> r1 >> r2;
        chk("operator>>", r1 == a && r2 == b && same(r1, x));
        chk("operator<<", show(a) == to_str(x));
    }
    // ---------------- up to ~1e37 (two-limb-plus) via strings, still inside __int128 ----------------
    for (int it = 0; it < 100000 / SCALE; it++) {
        int lx = rng() % 37 + 1, ly = rng() % 37 + 1;
        string sx = rnd_digits(lx, true, rng() % 8 == 0 ? 1 + rng() % 2 : 0);
        string sy = rnd_digits(ly, true, rng() % 8 == 0 ? 1 + rng() % 2 : 0);
        i128 x = 0, y = 0;
        for (char ch : absstr(sx)) x = x * 10 + (ch - '0');
        for (char ch : absstr(sy)) y = y * 10 + (ch - '0');
        if (sx[0] == '-') x = -x;
        if (sy[0] == '-') y = -y;
        BigInt a(sx), b;
        b.read(sy);
        chk("read", same(b, y));
        chk("ctor(string)", same(a, x));
        chk("operator+", same(a + b, x + y));
        chk("operator-", same(a - b, x - y));
        chk("operator<", (a < b) == (x < y));
        chk("operator==", (a == b) == (x == y));
        if (lx + ly <= 37) chk("operator*(BigInt)", same(a * b, x * y));
        chk("operator/(BigInt)", same(a / b, x / y));
        chk("operator%(BigInt)", same(a % b, x % y));
        ll v = (ll)(rng() % 9000000000ULL) + 1;
        if (rng() % 2) v = -v;
        chk("operator%(ll)", (i128)(a % v) == x % v);
        chk("operator/(ll)", same(a / v, x / v));
        if (lx <= 18 && lx + ly <= 37) chk("operator*(ll)", same(b * (ll)x, x * y));
        i128 g1 = x < 0 ? -x : x, g2 = y < 0 ? -y : y;
        while (g2) g1 %= g2, swap(g1, g2);
        chk("gcd", same(gcd(a, b), g1));
    }
    // ---------------- zero, signs and parsing edge cases ----------------
    {
        BigInt z = BigInt(5) + BigInt(-5), z2 = BigInt(-3), z3 = BigInt(-7) * 0, z4("-0"), z5("+-000");
        z2 += BigInt(3);
        BigInt z6 = BigInt(-6) % BigInt(3), z7 = BigInt(-1) / 2, z8 = BigInt(-4) - BigInt(-4);
        for (BigInt *p : {&z, &z2, &z3, &z4, &z5, &z6, &z7, &z8})
            chk("zero sign", p->zero() && *p == BigInt(0) && !(*p < 0) && !(BigInt(0) < *p) &&
                                 show(*p) == "0" && show(-*p) == "0");
        chk("ctor(string)", show(BigInt("000123")) == "123" && show(BigInt("-0001")) == "-1" &&
                                show(BigInt("+42")) == "42" && show(BigInt("--7")) == "7");
        chk("ctor(ll)", show(BigInt(LLONG_MIN)) == "-9223372036854775808" &&
                            show(BigInt(LLONG_MAX)) == "9223372036854775807");
        chk("operator%(BigInt)", show(BigInt(-7) % BigInt(-2)) == "-1" && show(BigInt(7) % BigInt(-2)) == "1");
        chk("gcd", show(gcd(BigInt(-4), BigInt(-6))) == "2" && show(gcd(BigInt(0), BigInt(-9))) == "9" &&
                       show(gcd(BigInt(0), BigInt(0))) == "0");
        chk("operator*(ll)", show(BigInt(1) * 10000000000LL) == "10000000000" &&
                                 show(BigInt(3) * LLONG_MIN) == "-27670116110564327424");
        chk("operator/(ll)", show(BigInt("100000000000000000000") / 10000000000LL) == "10000000000");
        chk("power", show(power(BigInt(2), 100)) == "1267650600228229401496703205376" &&
                         show(power(BigInt(-3), 0)) == "1" && show(power(BigInt(0), 0)) == "1");
    }
    // ---------------- huge values: string oracle, identities ----------------
    for (int it = 0; it < 1500 / SCALE; it++) {
        int la = rng() % 400 + 1, lb = rng() % 400 + 1;
        if (it % 10 == 0) la = rng() % 3000 + 1, lb = rng() % 3000 + 1;
        string sa = rnd_digits(la, true, rng() % 6 == 0 ? 1 + rng() % 2 : 0);
        string sb = rnd_digits(lb, true, rng() % 6 == 0 ? 1 + rng() % 2 : 0);
        BigInt a(sa), b(sb);
        chk("huge:<<", show(a) == sa && show(b) == sb);
        string prod = school_mul(sa, sb);
        BigInt ab = a * b;
        chk("huge:operator*", show(ab) == prod);
        chk("huge:mul_simple", show(a.mul_simple(b)) == prod);
        chk("huge:mul_fft", show(a.mul_fft(b)) == prod);
        string sum = show(a + b), dif = show(a - b);
        bool ok = true;
        for (ll p : P)
            ok &= smod(sum, p) == (smod(sa, p) + smod(sb, p)) % p &&
                  smod(dif, p) == (smod(sa, p) - smod(sb, p) + p) % p;
        chk("huge:operator+ operator-", ok);
        chk("huge:(a+b)-b==a", (a + b) - b == a);
        chk("huge:(a*b)/b==a", ab / b == a && ab / a == b && (ab % b).zero());
        auto [q, r] = divmod(a, b);
        string sq = show(q), sr = show(r);
        ok = abs_less(sr, sb) && (sr == "0" || (sr[0] == '-') == (sa[0] == '-'));
        ok &= sq == "0" || (sq[0] == '-') == ((sa[0] == '-') != (sb[0] == '-'));
        for (ll p : P) ok &= smod(sa, p) == (mulmod(smod(sq, p), smod(sb, p), p) + smod(sr, p)) % p;
        chk("huge:divmod (oracle)", ok);
        chk("huge:(a/b)*b+a%b==a", (a / b) * b + a % b == a);
        ll v = (ll)(rng() % 9000000000ULL) + 1;
        if (rng() % 2) v = -v;
        ll rm = a % v;
        BigInt qv = a / v;
        ok = (rm == 0 || (rm < 0) == (sa[0] == '-')) && llabs(rm) < llabs(v);
        ok &= qv * v + rm == a;
        for (ll p : P) {
            ll vp = ((v % p) + p) % p, rp = ((rm % p) + p) % p;
            ok &= smod(sa, p) == (mulmod(smod(show(qv), p), vp, p) + rp) % p;
        }
        chk("huge:/ and % by ll", ok);
        // comparisons against string order
        bool lt;
        bool na = sa[0] == '-', nb = sb[0] == '-';
        if (na != nb) lt = na;
        else lt = na ? abs_less(sb, sa) : abs_less(sa, sb);
        chk("huge:operator<", (a < b) == lt && (b < a) == (!lt && sa != sb) && (a == b) == (sa == sb));
        // gcd(c*u, c*v) == c * gcd(u, v)
        ll u = (ll)(rng() % (ll)1e18) + 1, w = (ll)(rng() % (ll)1e18) + 1;
        if (rng() % 3 == 0) w = u / ((ll)(rng() % 1000) + 1) + 1;
        BigInt cpos = a.abs();
        ll g = __gcd(u, w);
        chk("huge:gcd", show(gcd(cpos * u, -(cpos * w))) == school_mul(absstr(sa), to_string(g)));
        // power residues
        if (la <= 50) {
            int k = rng() % 30;
            string pw = show(power(a, k));
            ok = true;
            for (ll p : P) {
                ll e = 1, base = smod(sa, p);
                for (int i = 0; i < k; i++) e = mulmod(e, base, p);
                ok &= smod(pw, p) == e;
            }
            chk("huge:power", ok);
        }
    }
    // ---------------- FFT path: sizes past the 1e6 limb-product threshold, adversarial carries ----------------
    {
        vector<pair<int, int>> sizes = {{16384, 16388}, {16384, 16384}, {16380, 16388}, {9010, 9010},
                                        {9001, 9020}, {12288, 20481}, {32768, 32769}};
        for (int it = 0; it < 6; it++) sizes.push_back({int(rng() % 20000) + 9000, int(rng() % 20000) + 9000});
        for (int s = 0; s < (int)sizes.size(); s++)
            for (int style : {0, 1, 2}) {
                string sa = rnd_digits(sizes[s].first, true, style), sb = rnd_digits(sizes[s].second, true, style);
                BigInt a(sa), b(sb);
                string want = school_mul(sa, sb);
                chk("fft:mul_fft", show(a.mul_fft(b)) == want);
                chk("fft:operator*", show(a * b) == want);
                if (s < 3) chk("fft:(a*b)/b==a", (a * b) / b == a);
            }
    }
    // ---------------- timing ----------------
    {
        auto t0 = clock();
        BigInt a(rnd_digits(200000, false)), b(rnd_digits(200000, false));
        BigInt c = a * b;
        double t1 = double(clock() - t0) / CLOCKS_PER_SEC;
        BigInt d(rnd_digits(20000, false)), e(rnd_digits(10000, false));
        auto t2 = clock();
        BigInt q = d / e;
        double t3 = double(clock() - t2) / CLOCKS_PER_SEC;
        auto t4 = clock();
        BigInt f = 1;
        for (int i = 1; i <= 5000; i++) f *= i;
        double t5 = double(clock() - t4) / CLOCKS_PER_SEC;
        printf("2e5x2e5-digit multiply (FFT): %.3fs | 2e4/1e4-digit divide: %.3fs | 5000!: %.3fs  [%d]\n", t1,
               t3, t5, (int)(c.a.size() + q.a.size() + f.a.size()) % 2);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
