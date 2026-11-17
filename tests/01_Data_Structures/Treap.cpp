// Test for 01_Data_Structures/Treap.cpp against std::multiset.
// Every public method is checked: insert, erase, kth, lower_bound, upper_bound, size, count, print, clear,
// and two treaps of the same type alive at once (they share the EMPTY sentinel).
#include "../../01_Data_Structures/Treap.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(11);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <typename V> void run(const string &tag, int iters, int ops, V range) {
    for (int it = 0; it < iters; it++) {
        Treap<V> t, other;
        multiset<V> ms;
        for (int o = 0; o < ops; o++) {
            V x = (V)(rng() % (2 * range + 1)) - range;
            int type = rng() % 8;
            if (type <= 2) t.insert(x), ms.insert(x), other.insert(-x);
            else if (type == 3) {
                t.erase(x), other.erase(-x);
                auto p = ms.find(x);
                if (p != ms.end()) ms.erase(p);
            } else if (type == 4 && !ms.empty()) {
                int k = rng() % ms.size();
                chk(tag + "kth", t.kth(k) == *next(ms.begin(), k));
            } else if (type == 5) {
                chk(tag + "lower_bound", t.lower_bound(x) == (int)distance(ms.begin(), ms.lower_bound(x)));
                chk(tag + "upper_bound", t.upper_bound(x) == (int)distance(ms.begin(), ms.upper_bound(x)));
            } else if (type == 6) {
                chk(tag + "count", t.count(x) == (int)ms.count(x));
            }
            chk(tag + "size", t.size() == (int)ms.size());
        }
        // print: in-order "value count depth", values strictly increasing and counts matching
        stringstream ss;
        auto *old = cout.rdbuf(ss.rdbuf());
        t.print();
        cout.rdbuf(old);
        map<V, int> want;
        for (V x : ms) want[x]++;
        map<V, int> got;
        V v;
        int c, d;
        bool ok = true;
        vector<V> order;
        while (ss >> v >> c >> d) got[v] = c, order.push_back(v), ok &= d >= 0;
        ok &= is_sorted(order.begin(), order.end()) && got == want;
        chk(tag + "print", ok);
        // other holds the negations, so it must mirror ms exactly
        multiset<V> neg;
        for (V x : ms) neg.insert(-x);
        bool same = other.size() == (int)ms.size();
        for (int i = 0; same && i < other.size(); i++) same = other.kth(i) == *next(neg.begin(), i);
        chk(tag + "independent", same);
        t.clear();
        chk(tag + "clear", t.size() == 0 && t.lower_bound(0) == 0);
        t.insert(5);
        chk(tag + "reuse_after_clear", t.size() == 1 && t.kth(0) == 5);
    }
}

int main() {
    run<int>("int:", 3000, 60, 6);
    run<int>("int_wide:", 500, 300, 1000000);
    run<long long>("ll:", 1000, 80, 20);
    {
        // independence of two treaps: same values inserted into one only
        Treap<int> a, b;
        for (int i = 0; i < 100; i++) a.insert(i);
        chk("independent", a.size() == 100 && b.size() == 0 && b.count(5) == 0);
    }
    {
        Treap<int> t;
        int n = 500000;
        auto t0 = clock();
        for (int i = 0; i < n; i++) t.insert(rng() % 1000000000);
        long long s = 0;
        for (int i = 0; i < n; i++) s += t.kth(rng() % t.size()) + t.lower_bound(rng() % 1000000000);
        for (int i = 0; i < n; i++) t.erase(rng() % 1000000000);
        printf("5e5 insert + kth/rank + erase: %.3fs (checksum %lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
