// Test for 01_Data_Structures/Ordered_Set.cpp against std::set / std::multiset with linear scans.
// Every public piece is checked: ordered_set (insert, erase, find, order_of_key, find_by_order,
// lower_bound, upper_bound, size), ordered_multiset (insert, order_of_key, find_by_order, size) and
// multiset_erase (present / missing), plus a long long and a pair key.
#include "../../01_Data_Structures/Ordered_Set.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <class C, class T> long long naive_less(const C &c, const T &x) {
    long long r = 0;
    for (auto &v : c) r += v < x;
    return r;
}

int main() {
    for (int it = 0; it < 3000; it++) {
        int R = vector<int>{1, 3, 10, 1000}[rng() % 4];
        ordered_set<int> s;
        set<int> os;
        ordered_multiset<int> m;
        multiset<int> om;
        for (int op = 0; op < 60; op++) {
            int x = (int)(rng() % (2 * R + 1)) - R;
            int t = rng() % 4;
            if (t <= 1) {
                s.insert(x), os.insert(x), m.insert(x), om.insert(x);
                chk("insert", s.size() == os.size() && m.size() == om.size());
            } else if (t == 2) {
                chk("erase", s.erase(x) == os.erase(x));
                auto oit = om.find(x);
                bool had = oit != om.end();
                if (had) om.erase(oit);
                chk("multiset_erase", multiset_erase(m, x) == had && m.size() == om.size());
            }
            chk("find", (s.find(x) != s.end()) == os.count(x) && (s.find(x) == s.end() || *s.find(x) == x));
            chk("order_of_key", (long long)s.order_of_key(x) == naive_less(os, x));
            chk("multi:order_of_key", (long long)m.order_of_key(x) == naive_less(om, x));
            auto lb = s.lower_bound(x), ub = s.upper_bound(x);
            auto olb = os.lower_bound(x), oub = os.upper_bound(x);
            chk("lower_bound", (lb == s.end()) == (olb == os.end()) && (lb == s.end() || *lb == *olb));
            chk("upper_bound", (ub == s.end()) == (oub == os.end()) && (ub == s.end() || *ub == *oub));
        }
        // full scan of find_by_order, including out of range
        vector<int> v(os.begin(), os.end()), mv(om.begin(), om.end());
        bool ok = s.find_by_order(v.size()) == s.end();
        for (size_t k = 0; k < v.size(); k++) ok &= *s.find_by_order(k) == v[k];
        chk("find_by_order", ok);
        ok = m.find_by_order(mv.size()) == m.end();
        for (size_t k = 0; k < mv.size(); k++) ok &= *m.find_by_order(k) == mv[k];
        chk("multi:find_by_order", ok);
        // in-order iteration
        chk("iterate", vector<int>(s.begin(), s.end()) == v && vector<int>(m.begin(), m.end()) == mv);
    }
    // long long and pair keys; empty containers
    {
        ordered_multiset<long long> m;
        multiset<long long> om;
        for (int i = 0; i < 20000; i++) {
            long long x = (long long)(rng() % 50) * 1000000000000LL - 25000000000000LL;
            if (rng() % 3) m.insert(x), om.insert(x);
            else {
                auto oit = om.find(x);
                bool had = oit != om.end();
                if (had) om.erase(oit);
                chk("ll:multiset_erase", multiset_erase(m, x) == had);
            }
            chk("ll:order_of_key", (long long)m.order_of_key(x) == naive_less(om, x));
        }
        ordered_set<pair<int, int>> p;
        set<pair<int, int>> op;
        for (int i = 0; i < 5000; i++) {
            pair<int, int> x{(int)(rng() % 10), (int)(rng() % 10)};
            p.insert(x), op.insert(x);
            chk("pair:order_of_key", (long long)p.order_of_key(x) == naive_less(op, x));
        }
        ordered_set<int> e;
        ordered_multiset<int> em;
        chk("empty", e.order_of_key(5) == 0 && e.find_by_order(0) == e.end() &&
                         !multiset_erase(em, 3) && em.find_by_order(0) == em.end());
    }
    {
        auto t0 = clock();
        ordered_multiset<int> m;
        long long sum = 0;
        for (int i = 0; i < 1000000; i++) m.insert(rng() % 1000000);
        for (int i = 0; i < 1000000; i++) {
            int x = rng() % 1000000;
            sum += m.order_of_key(x);
            if (i % 2) multiset_erase(m, x);
        }
        printf("1e6 inserts + 1e6 order_of_key + 5e5 erases: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, sum % 7);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
