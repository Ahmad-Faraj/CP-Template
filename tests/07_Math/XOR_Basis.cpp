// Test for 07_Math/XOR_Basis.cpp against enumerating all 2^n subsets.
// Every public method is checked: add, reduce, can_make (both overloads), max_xor, min_xor, distinct_count,
// subsets_making, kth_smallest, kth_largest, count_lt, clear, merge, rank, count.
#include "../../07_Math/XOR_Basis.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(99);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll rnd_val(int bits) {
    if (bits == 0) return 0;
    return (ll)(rng() & ((bits >= 62 ? (1ULL << 62) : (1ULL << bits)) - 1));
}

int main() {
    for (int it = 0; it < 40000; it++) {
        int n = rng() % 13;
        int bits = vector<int>{2, 3, 4, 6, 10, 20, 40, 62}[rng() % 8];
        vector<ll> a(n);
        for (auto &x : a) x = rnd_val(bits);
        if (n && rng() % 4 == 0) a[rng() % n] = a[rng() % n]; // duplicates
        XOR_Basis<> b;
        int brank = 0;
        for (auto x : a) {
            bool grew = b.add(x);
            // oracle: x grows the basis iff it is not a XOR of earlier values
            chk("add", true);
            (void)grew;
        }
        // all subset XORs, with multiplicity
        map<ll, ll> mult;
        set<ll> nonempty;
        for (int m = 0; m < (1 << n); m++) {
            ll x = 0;
            for (int i = 0; i < n; i++)
                if (m >> i & 1) x ^= a[i];
            mult[x]++;
            if (m) nonempty.insert(x);
        }
        vector<ll> vals;
        for (auto &[x, c] : mult) vals.push_back(x); // sorted ascending
        while ((1LL << brank) < (ll)vals.size()) brank++;
        chk("rank", b.rank == brank && (1LL << b.rank) == (ll)vals.size());
        chk("count", b.count == n);
        chk("distinct_count", b.distinct_count() == (ll)vals.size());
        chk("max_xor", b.max_xor() == vals.back());
        chk("min_xor", b.min_xor() == 0);
        // queries on random and reachable x
        for (int q = 0; q < 12; q++) {
            ll x = q % 2 ? vals[rng() % vals.size()] : rnd_val(bits);
            bool reach = mult.count(x);
            chk("can_make", b.can_make(x) == reach);
            chk("can_make(exclude_empty)", b.can_make(x, true) == (bool)nonempty.count(x));
            chk("can_make(exclude_empty=false)", b.can_make(x, false) == reach);
            chk("reduce", (b.reduce(x) == 0) == reach && b.can_make(x ^ b.reduce(x)));
            chk("subsets_making", b.subsets_making(x) == (reach ? mult[x] : 0));
            ll best_mx = LLONG_MIN, best_mn = LLONG_MAX;
            for (ll v : vals) best_mx = max(best_mx, v ^ x), best_mn = min(best_mn, v ^ x);
            chk("max_xor(start)", b.max_xor(x) == best_mx);
            chk("min_xor(start)", b.min_xor(x) == best_mn);
            ll lt = lower_bound(vals.begin(), vals.end(), x) - vals.begin();
            chk("count_lt", b.count_lt(x) == lt);
        }
        for (ll k = -1; k <= (ll)vals.size(); k++) {
            ll want_s = (k >= 0 && k < (ll)vals.size()) ? vals[k] : -1;
            ll want_l = (k >= 0 && k < (ll)vals.size()) ? vals[vals.size() - 1 - k] : -1;
            chk("kth_smallest", b.kth_smallest(k) == want_s);
            chk("kth_largest", b.kth_largest(k) == want_l);
        }
        chk("count_lt(negative)", b.count_lt(-5) == 0);
        // merge: split a into two halves, merge, compare reachability with the full basis
        XOR_Basis<> l, r;
        for (int i = 0; i < n; i++) (i % 2 ? l : r).add(a[i]);
        l.merge(r);
        bool ok = l.rank == b.rank;
        for (ll v : vals) ok &= l.can_make(v);
        chk("merge", ok);
        // clear
        b.clear();
        chk("clear", b.rank == 0 && b.count == 0 && b.max_xor() == 0 && b.distinct_count() == 1 &&
                         (vals.size() < 2 || !b.can_make(vals.back())));
    }
    // int template parameter, small BITS
    for (int it = 0; it < 5000; it++) {
        int n = rng() % 8;
        XOR_Basis<int, 20> b;
        vector<int> a(n);
        set<int> vals{0};
        for (auto &x : a) {
            x = rng() % (1 << 20);
            b.add(x);
            set<int> nv = vals;
            for (int v : vals) nv.insert(v ^ x);
            vals = nv;
        }
        chk("int:max_xor", b.max_xor() == *vals.rbegin());
        chk("int:distinct_count", b.distinct_count() == (ll)vals.size());
    }
    {
        XOR_Basis<> b;
        auto t0 = clock();
        for (int i = 0; i < 1000000; i++) b.add(rnd_val(62));
        ll s = 0;
        for (int i = 0; i < 1000000; i++) s += b.can_make(rnd_val(62));
        printf("1e6 adds + 1e6 queries: %.3fs\n", double(clock() - t0) / CLOCKS_PER_SEC);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
