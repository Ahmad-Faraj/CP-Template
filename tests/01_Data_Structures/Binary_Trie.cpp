// Test for 01_Data_Structures/Binary_Trie.cpp against a std::multiset with brute-force scans.
// Every public method is checked: insert, erase, search, max_xor, min_xor, count, count_greater,
// size, get_bit, for T = int (LOG 31) and T = ll (LOG 40, 62), and small LOG for heavy duplicates.
#include "../../01_Data_Structures/Binary_Trie.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(2026);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <typename T> void run(const string &tag, int LOG, int iters) {
    // largest storable value
    long long cap = LOG + 1 >= 63 ? LLONG_MAX : (1LL << (LOG + 1)) - 1;
    T hi = (T)min<long long>(numeric_limits<T>::max(), cap);
    int hib = 0;
    while (hib < 63 && (hi >> hib)) hib++; // values have at most hib bits
    for (int it = 0; it < iters; it++) {
        int vbits = (int)(rng() % (hib + 1)); // values below 2^vbits, so duplicates appear often
        auto rv = [&]() -> T {
            if (rng() % 8 == 0) return hi;
            return (T)(rng() % (1ULL << vbits));
        };
        Binary_Trie<T> t(LOG);
        multiset<T> ms;
        int ops = rng() % 40;
        for (int o = 0; o < ops; o++) {
            int typ = rng() % 3;
            if (typ < 2 || ms.empty()) {
                T x = rv();
                t.insert(x), ms.insert(x);
                chk(tag + "insert", t.size() == (int)ms.size());
            } else {
                auto itv = next(ms.begin(), rng() % ms.size());
                T x = *itv;
                t.erase(x), ms.erase(itv);
                chk(tag + "erase", t.size() == (int)ms.size() && t.search(x) == (bool)ms.count(x));
            }
            chk(tag + "size", t.size() == (int)ms.size());
            for (int q = 0; q < 4; q++) {
                T x = (q == 0 && !ms.empty()) ? *next(ms.begin(), rng() % ms.size()) : rv();
                chk(tag + "search", t.search(x) == (bool)ms.count(x));
                if (!ms.empty()) {
                    T mx = 0, mn = hi;
                    for (T v : ms) mx = max(mx, (T)(x ^ v)), mn = min(mn, (T)(x ^ v));
                    chk(tag + "max_xor", t.max_xor(x) == mx);
                    chk(tag + "min_xor", t.min_xor(x) == mn);
                }
                T k = q == 3 ? (T)(rng() % 3) - 1 : rv(); // includes negative k
                if (rng() % 10 == 0 && hi < numeric_limits<T>::max() - 5)
                    k = hi + 1 + (T)(rng() % 5); // k above every storable value
                int le = 0;
                for (T v : ms) le += k >= 0 && (T)(x ^ v) <= k;
                chk(tag + "count", t.count(x, k) == le);
                chk(tag + "count_greater", t.count_greater(x, k) == (int)ms.size() - le);
                int bit = rng() % (LOG + 1);
                chk(tag + "get_bit", t.get_bit(x, bit) == (bool)((x >> bit) & 1));
            }
        }
    }
}

int main() {
    run<int>("int,LOG31:", 31, 4000);
    run<int>("int,LOG3:", 3, 8000);
    run<ll>("ll,LOG40:", 40, 4000);
    run<ll>("ll,LOG62:", 62, 3000);
    run<ll>("ll,LOG4:", 4, 8000);
    { // max XOR of a subarray, the classic use, vs O(n^2)
        for (int it = 0; it < 2000; it++) {
            int n = 1 + rng() % 30;
            vector<int> a(n);
            for (auto &x : a) x = rng() % 64;
            Binary_Trie<int> t(6);
            t.insert(0);
            int pre = 0, best = 0, want = 0;
            for (int i = 0; i < n; i++) {
                pre ^= a[i], best = max(best, t.max_xor(pre)), t.insert(pre);
                int s = 0;
                for (int j = i; j >= 0; j--) s ^= a[j], want = max(want, s);
            }
            chk("usage:max_xor_subarray", best == want);
        }
    }
    {
        Binary_Trie<int> t(29);
        auto t0 = clock();
        long long s = 0;
        vector<int> vals(500000);
        for (auto &v : vals) v = rng() % (1 << 30), t.insert(v);
        for (int i = 0; i < 500000; i++) {
            int x = rng() % (1 << 30), k = rng() % (1 << 30);
            s += t.max_xor(x) + t.min_xor(x) + t.count(x, k);
            if (i % 2) t.erase(vals[i]);
        }
        printf("5e5 inserts, 5e5 x (max_xor, min_xor, count), 2.5e5 erases: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, s % 7);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-30s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
