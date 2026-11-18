// Test for 10_Misc/Bases.cpp.
// Oracle: strtoull / strtoll from libc (bases 2..36, either case, sign) and printf %llo %lld %llX;
// round trips over random 64-bit values including 0, +-1, LLONG_MIN, LLONG_MAX.
// Checked: decimal_to_any_base, any_base_to_decimal. Sample: Codeforces 61C (base a -> base b).
#include "../../10_Misc/Bases.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(61);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

bool canonical(const string &s, int b) { // digits valid for base b, upper case, no leading zero
    size_t i = s[0] == '-';
    if (i >= s.size() || (s[i] == '0' && s.size() > i + 1) || (i && s == "-0")) return false;
    for (; i < s.size(); i++) {
        int d = isdigit(s[i]) ? s[i] - '0' : isupper(s[i]) ? s[i] - 'A' + 10 : 99;
        if (d >= b) return false;
    }
    return true;
}

int main() {
    vector<ll> edge{0, 1, -1, 2, -2, 35, 36, LLONG_MAX, LLONG_MIN, LLONG_MIN + 1, LLONG_MAX - 1};
    for (int it = 0; it < 300000; it++) {
        ll x;
        int r = rng() % 4;
        if (r == 0) x = edge[rng() % edge.size()];
        else if (r == 1) x = (ll)(rng() % 2000) - 1000;
        else x = (ll)rng();
        int b = rng() % 35 + 2;
        string s = decimal_to_any_base(x, b);
        errno = 0;
        ll back = strtoll(s.c_str(), nullptr, b);
        chk("decimal_to_any_base", canonical(s, b) && errno == 0 && back == x);
        chk("any_base_to_decimal", any_base_to_decimal(s, b) == x);
        string low = s;
        for (auto &c : low) c = tolower(c);
        chk("any_base_to_decimal(lower)", any_base_to_decimal(low, b) == x);
        char buf[80];
        snprintf(buf, 80, "%lld", x);
        chk("decimal_to_any_base(b=10)", decimal_to_any_base(x, 10) == buf);
        if (x >= 0) {
            snprintf(buf, 80, "%llo", (unsigned long long)x);
            chk("decimal_to_any_base(b=8)", decimal_to_any_base(x, 8) == buf);
            snprintf(buf, 80, "%llX", (unsigned long long)x);
            chk("decimal_to_any_base(b=16)", decimal_to_any_base(x, 16) == buf);
        }
        // random digit strings, possibly with leading zeros, read back by strtoll
        string t;
        int len = rng() % 12 + 1;
        for (int i = 0; i < len; i++) t += "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[rng() % b];
        if (rng() % 3 == 0) t = "-" + t;
        errno = 0;
        ll want = strtoll(t.c_str(), nullptr, b);
        if (errno == 0) chk("any_base_to_decimal(random digits)", any_base_to_decimal(t, b) == want);
    }
    chk("edge:zero", decimal_to_any_base(0, 2) == "0" && any_base_to_decimal("0", 7) == 0);
    chk("edge:LLONG_MIN b2", decimal_to_any_base(LLONG_MIN, 2) == "-1" + string(63, '0'));
    chk("edge:max hex", any_base_to_decimal("7FFFFFFFFFFFFFFF", 16) == LLONG_MAX);
    // Codeforces 61C sample 1: "10 2 / 1" -> 1 ; sample 2: "16 R / 5" -> V (roman, not this file)
    chk("samples(CF 61C 1)", decimal_to_any_base(any_base_to_decimal("1", 10), 2) == "1");
    chk("samples(CF 61C a=16,b=10)", decimal_to_any_base(any_base_to_decimal("A", 16), 10) == "10");
    {
        auto t0 = clock();
        ll s = 0;
        for (int i = 0; i < 1000000; i++) {
            ll x = (ll)rng();
            int b = i % 35 + 2;
            s += any_base_to_decimal(decimal_to_any_base(x, b), b) == x;
        }
        printf("1e6 round trips: %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
