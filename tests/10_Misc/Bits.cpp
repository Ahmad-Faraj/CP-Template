// Test for 10_Misc/Bits.cpp against std::bitset (64 and 32 wide) and per-bit naive loops.
// Checked for int, long long, unsigned long long and negative values, bits 0..31 / 0..63:
// set_bit, clear_bit, toggle_bit, check_bit, count_bits, count_zero_bits.
#include "../../10_Misc/Bits.cpp"
using ll = long long;
using ull = unsigned long long;

map<string, pair<long long, long long>> cov;
mt19937_64 rng(579);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <class T, int W> void test_type(const string &tag, T x) {
    using U = make_unsigned_t<T>;
    bitset<W> b((U)x);
    int naive = 0;
    for (U y = (U)x; y; y >>= 1) naive += y & 1;
    chk(tag + "count_bits", count_bits(x) == naive && naive == (int)b.count());
    chk(tag + "count_zero_bits", count_zero_bits(x) == W - naive);
    int i = rng() % W;
    bitset<W> s = b, c = b, t = b;
    s.set(i), c.reset(i), t.flip(i);
    chk(tag + "check_bit", check_bit(x, i) == b.test(i));
    chk(tag + "set_bit", bitset<W>((U)set_bit(x, i)) == s);
    chk(tag + "clear_bit", bitset<W>((U)clear_bit(x, i)) == c);
    chk(tag + "toggle_bit", bitset<W>((U)toggle_bit(x, i)) == t);
}

int main() {
    vector<ll> edge{0, 1, -1, LLONG_MAX, LLONG_MIN, INT_MAX, INT_MIN, 1LL << 31, 1LL << 62};
    for (int it = 0; it < 400000; it++) {
        ll x = it % 5 == 0 ? edge[rng() % edge.size()] : (ll)rng();
        if (it % 7 == 0) x &= 0xFF;
        test_type<ll, 64>("ll:", x);
        test_type<ull, 64>("ull:", (ull)x);
        test_type<int, 32>("int:", (int)x);
    }
    chk("edge:bit 63", set_bit(0LL, 63) == LLONG_MIN && check_bit(LLONG_MIN, 63) &&
                           clear_bit(-1LL, 63) == LLONG_MAX && count_bits(-1LL) == 64);
    chk("edge:int -1", count_bits(-1) == 32 && count_zero_bits(-1) == 0 && check_bit(-1, 31));
    chk("edge:bit 40", set_bit(0LL, 40) == (1LL << 40) && count_zero_bits(0LL) == 64);
    // Codeforces 579A Raising Bacteria: answer = popcount(x); samples 5 -> 2, 8 -> 1
    chk("samples(CF 579A)", count_bits(5) == 2 && count_bits(8) == 1);
    {
        auto t0 = clock();
        ll s = 0, m = 0;
        for (int i = 0; i < 10000000; i++) {
            m = toggle_bit(m, i & 63);
            s += count_bits(m) + check_bit(m, (i * 7) & 63);
        }
        printf("1e7 toggles + popcounts: %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
