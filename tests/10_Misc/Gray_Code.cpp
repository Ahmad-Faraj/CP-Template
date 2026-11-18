// Test for 10_Misc/Gray_Code.cpp.
// Oracle: the reflected Gray list built by mirroring (k <= 16), the per-bit definition
// g_i = b_i xor b_(i+1) on all 64 bits, and the one-bit-change property on random 64-bit values.
// Checked: gray, rev_gray. Sample: CSES 2205 (n = 2 -> 00 01 11 10).
#include "../../10_Misc/Gray_Code.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(2205);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    vector<ll> list{0};
    for (int k = 1; k <= 16; k++) { // reflected construction: L, then reversed L with bit k-1 set
        for (int i = (int)list.size() - 1; i >= 0; i--) list.push_back(list[i] | (1LL << (k - 1)));
        bool ok = true, inv = true;
        for (ll i = 0; i < (1LL << k); i++) ok &= gray(i) == list[i], inv &= rev_gray(list[i]) == i;
        chk("gray(reflected list)", ok);
        chk("rev_gray(reflected list)", inv);
    }
    for (int it = 0; it < 1000000; it++) {
        ll x = it % 10 == 0 ? vector<ll>{0, -1, LLONG_MAX, LLONG_MIN, 1}[rng() % 5] : (ll)rng();
        unsigned long long u = x, want = 0;
        for (int i = 0; i < 64; i++) {
            int bi = u >> i & 1, bn = i < 63 ? (u >> (i + 1) & 1) : 0;
            want |= (unsigned long long)(bi ^ bn) << i;
        }
        chk("gray(64-bit per-bit)", (unsigned long long)gray(x) == want);
        chk("rev_gray(64-bit)", rev_gray(gray(x)) == x && gray(rev_gray(x)) == x);
        if (x != LLONG_MAX && x != -1)
            chk("gray(one-bit step)", __builtin_popcountll(gray(x) ^ gray(x + 1)) == 1);
    }
    {   // CSES 2205 sample
        vector<string> v;
        for (ll i = 0; i < 4; i++) {
            string s;
            for (int b = 1; b >= 0; b--) s += char('0' + (gray(i) >> b & 1));
            v.push_back(s);
        }
        chk("samples(CSES 2205)", v == vector<string>{"00", "01", "11", "10"});
    }
    {
        auto t0 = clock();
        ll s = 0;
        for (ll i = 0; i < (1 << 22); i++) s += rev_gray(gray(i * 2654435761LL)) & 1;
        printf("2^22 gray + rev_gray on 64-bit values: %.3fs (%lld)\n",
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
