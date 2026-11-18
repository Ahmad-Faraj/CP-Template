// Test for 07_Math/Mod_Int.cpp against __int128 arithmetic and an extended-Euclid inverse.
// Every public member is checked: constructor (default, negative, huge), += -= *= /=, binary + - * /
// with ModInt or a plain integer on either side, unary -, prefix/postfix ++ --, == !=, power
// (positive, zero, negative exponents), inverse, for MOD = 1e9+7, 998244353, 7 and 2^31 - 1.
#include "../../07_Math/Mod_Int.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(3);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll norm(__int128 x, ll p) { return (ll)(((x % p) + p) % p); }
ll inv_euclid(ll a, ll p) { // a * x = 1 mod p via extended Euclid (independent of Fermat)
    ll g = p, x = 0, x1 = 1, a1 = a;
    while (a1) {
        ll q = g / a1;
        tie(g, a1) = make_pair(a1, g - q * a1);
        tie(x, x1) = make_pair(x1, x - q * x1);
    }
    return norm(x, p);
}
ll slow_pow(ll a, ll e, ll p) { // repeated multiplication for small e
    __int128 r = 1;
    for (ll i = 0; i < e; i++) r = r * a % p;
    return (ll)r;
}

ll rnd_ll() {
    switch (rng() % 6) {
    case 0: return (ll)rng();                       // any 64-bit value, often negative
    case 1: return (ll)(rng() % 21) - 10;           // small, incl. 0 and negatives
    case 2: return rng() % 2 ? LLONG_MAX : LLONG_MIN;
    case 3: return -(ll)(rng() % 4000000000ULL);
    default: return (ll)(rng() % 4000000000ULL);
    }
}

template <ll P> void run(const string &tag, int iters) {
    using M = ModInt<P>;
    chk(tag + "default ctor", M().val == 0);
    for (int it = 0; it < iters; it++) {
        ll x = rnd_ll(), y = rnd_ll();
        ll xn = norm(x, P), yn = norm(y, P);
        M a = x, b = y;
        chk(tag + "ctor normalises", a.val == xn && b.val == yn && a.val >= 0 && a.val < P);
        chk(tag + "a+b", (a + b).val == norm((__int128)xn + yn, P));
        chk(tag + "a-b", (a - b).val == norm((__int128)xn - yn, P));
        chk(tag + "a*b", (a * b).val == norm((__int128)xn * yn, P));
        chk(tag + "a+int, int+a", (a + y).val == norm((__int128)xn + yn, P) &&
                                       (x + b).val == norm((__int128)xn + yn, P));
        chk(tag + "a-int, int-a", (a - y).val == norm((__int128)xn - yn, P) &&
                                       (x - b).val == norm((__int128)xn - yn, P));
        chk(tag + "a*int, int*a", (a * y).val == norm((__int128)xn * yn, P) &&
                                       (x * b).val == norm((__int128)xn * yn, P));
        chk(tag + "unary -", (-a).val == norm(-(__int128)xn, P));
        M c = a;
        c += b, chk(tag + "+=", c.val == norm((__int128)xn + yn, P));
        c = a, c -= b, chk(tag + "-=", c.val == norm((__int128)xn - yn, P));
        c = a, c *= b, chk(tag + "*=", c.val == norm((__int128)xn * yn, P));
        c = a, c += y, chk(tag + "+=int", c.val == norm((__int128)xn + yn, P));
        if (yn) {
            ll iy = inv_euclid(yn, P);
            chk(tag + "a/b", (a / b).val == norm((__int128)xn * iy, P));
            chk(tag + "a/int, int/a", (a / y).val == norm((__int128)xn * iy, P) &&
                                           (x / b).val == norm((__int128)xn * iy, P));
            c = a, c /= b, chk(tag + "/=", c.val == norm((__int128)xn * iy, P));
            chk(tag + "inverse", b.inverse().val == iy && (b * b.inverse()).val == 1);
            ll e = rng() % 50;
            chk(tag + "power(-e)", b.power(-e).val == slow_pow(iy, e, P));
        } else {
            chk(tag + "inverse(0)", b.inverse().val == 0);
        }
        ll e = rng() % 60;
        chk(tag + "power(small e)", a.power(e).val == slow_pow(xn, e, P));
        chk(tag + "power(0)", a.power(0).val == 1);
        if (xn) { // Fermat: a^(P-1) = 1, and a^(k(P-1)+r) = a^r for big exponents
            ll r = rng() % 30, k = rng() % 1000;
            chk(tag + "power(big e)", a.power(P - 1).val == 1 &&
                                          a.power(k * (P - 1) + r).val == slow_pow(xn, r, P));
        }
        c = a;
        M old = c++;
        chk(tag + "postfix ++", old.val == xn && c.val == norm((__int128)xn + 1, P));
        old = c--;
        chk(tag + "postfix --", old.val == norm((__int128)xn + 1, P) && c.val == xn);
        chk(tag + "prefix ++", (++c).val == norm((__int128)xn + 1, P) && c.val == norm(xn + 1, P));
        chk(tag + "prefix --", (--c).val == xn);
        chk(tag + "==, !=", (a == b) == (xn == yn) && (a != b) == (xn != yn) && (a == x) &&
                                (x == a) && !(a != x));
        chk(tag + "== after wrap", M(xn) == M(xn - P) && (M(xn) == xn - P));
    }
}

int main() {
    run<1000000007>("1e9+7: ", 300000);
    run<998244353>("998244353: ", 100000);
    run<7>("7: ", 50000);
    run<2147483647>("2^31-1: ", 100000);
    {   // default template argument and the mint alias
        mint z = -1;
        chk("mint alias", z.val == 1000000006 && ModInt<>(-1) == z);
    }
    {
        auto t0 = clock();
        mint s = 1, x = 123456789;
        for (int i = 0; i < 20000000; i++) s = s * x + i;
        mint inv = 0;
        for (int i = 1; i <= 1000000; i++) inv += mint(i).inverse();
        printf("2e7 mul-add + 1e6 inverses: %.3fs (%lld %lld)\n", double(clock() - t0) / CLOCKS_PER_SEC,
               s.val, inv.val);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
