// Test for 07_Math/Mod_Arithmetic.cpp against exact __int128 arithmetic reduced into [0, m),
// repeated multiplication, Fermat inverses checked by multiplication, and stoll / a splitting
// identity for Big_Mod. Every public function is checked (with and without the default MOD):
// add_mod, sub_mod, mul_mod, Big_Mod, mod_pow, mod_inv.
#include "../../07_Math/Mod_Arithmetic.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(3);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll md(__int128 v, ll m) { v %= m; return (ll)(v < 0 ? v + m : v); }
ll rnd(ll lo, ll hi) { // uniform in [lo, hi], unsigned so the span cannot overflow
    unsigned long long span = (unsigned long long)hi - (unsigned long long)lo + 1;
    return (ll)((unsigned long long)lo + (span ? rng() % span : rng()));
}

int main() {
    vector<ll> mods = {1, 2, 7, 1000000007, 998244353, (1LL << 61) - 1, LLONG_MAX, 1000000000000LL};
    for (int it = 0; it < 300000; it++) {
        ll m = it % 3 ? mods[rng() % mods.size()] : rnd(1, LLONG_MAX);
        auto val = [&]() {
            int t = rng() % 4;
            return t == 0 ? rnd(LLONG_MIN, LLONG_MAX) : t == 1 ? rnd(-20, 20) : t == 2 ? m - 1 - rng() % 3 : -m + 1;
        };
        ll a = val(), b = val();
        chk("add_mod", add_mod(a, b, m) == md((__int128)a + b, m));
        chk("sub_mod", sub_mod(a, b, m) == md((__int128)a - b, m));
        chk("mul_mod", mul_mod(a, b, m) == md((__int128)md(a, m) * md(b, m), m));
        chk("add_mod(MOD)", add_mod(a, b) == md((__int128)a + b, MOD));
        chk("sub_mod(MOD)", sub_mod(a, b) == md((__int128)a - b, MOD));
        chk("mul_mod(MOD)", mul_mod(a, b) == md((__int128)md(a, MOD) * md(b, MOD), MOD));
        ll e = rng() % 50, r = md(1, m);
        for (ll i = 0; i < e; i++) r = md((__int128)r * md(a, m), m);
        chk("mod_pow", mod_pow(a, e, m) == r);
    }
    for (int it = 0; it < 5000; it++) { // huge exponent: a^(x+y) = a^x * a^y, and Fermat
        ll a = rnd(LLONG_MIN, LLONG_MAX), x = rnd(0, (ll)4e18), y = rnd(0, (ll)4e18);
        chk("mod_pow(huge e)", mod_pow(a, x + y) == md((__int128)mod_pow(a, x) * mod_pow(a, y), MOD));
        if (md(a, MOD)) chk("mod_pow(fermat)", mod_pow(a, MOD - 1) == 1);
    }
    for (ll p : {2LL, 13LL, 1000000007LL, 998244353LL, (1LL << 61) - 1})
        for (int it = 0; it < 2000; it++) {
            ll a = rnd(LLONG_MIN, LLONG_MAX);
            if (md(a, p) == 0) continue;
            ll iv = mod_inv(a, p);
            chk("mod_inv", iv >= 0 && iv < p && md((__int128)md(a, p) * iv, p) == 1);
            if (p == MOD) chk("mod_inv(MOD)", mod_inv(a) == iv);
        }
    chk("mod_pow(edges)", mod_pow(5, 0, 1) == 0 && mod_pow(0, 0) == 1 && mod_pow(-1, 3) == MOD - 1);
    for (int it = 0; it < 20000; it++) {
        ll m = it % 2 ? rnd(1, (ll)9e17) : mods[rng() % 6];
        int len = rng() % 19;
        string s = len ? to_string(rng() % 9 + 1) : "";
        for (int i = 1; i < len; i++) s += char('0' + rng() % 10);
        if (rng() % 5 == 0) s = string(rng() % 4, '0') + s; // leading zeros
        chk("Big_Mod(<=18 digits)", Big_Mod(s, m) == (s.empty() ? 0 : md(stoll(s), m)));
        // long strings: value(s + t) = value(s) * 10^|t| + value(t)
        string t;
        int lt = rng() % 30;
        for (int i = 0; i < lt; i++) t += char('0' + rng() % 10);
        ll p10 = md(1, m);
        for (int i = 0; i < lt; i++) p10 = md((__int128)p10 * 10, m);
        chk("Big_Mod(long)", Big_Mod(s + t, m) == md((__int128)Big_Mod(s, m) * p10 + Big_Mod(t, m), m));
    }
    {
        string big(1000000, '7');
        auto t0 = clock();
        ll s = Big_Mod(big, MOD);
        for (int i = 0; i < 2000000; i++) s = add_mod(mul_mod(s, rng()), sub_mod(rng(), s));
        for (int i = 0; i < 1000000; i++) s ^= mod_inv(rng() % (MOD - 1) + 1);
        printf("Big_Mod 1e6 digits + 2e6 add/mul/sub + 1e6 mod_inv: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, s % 10);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
