// Test for 04_Graphs/2_SAT.cpp against trying all 2^vars assignments.
// Every public method is checked: addOR, addXOR, add (implication), forcing via addOR(a, f, a, f),
// ok() (also re-run after more clauses), and res[] is verified to satisfy every clause.
#include "../../04_Graphs/2_SAT.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(3);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

struct Clause { int type, a, b; bool af, bf; }; // 0 OR, 1 XOR, 2 implication

bool holds(const Clause &c, const vector<bool> &x) {
    bool A = x[c.a] == c.af, B = x[c.b] == c.bf;
    if (c.type == 0) return A || B;
    if (c.type == 1) return A != B;
    return !A || B;
}

int main() {
    const char *name[3] = {"addOR", "addXOR", "add"};
    for (int it = 0; it < 60000; it++) {
        int vars = rng() % 7, m = rng() % 14;
        Two_SAT s(vars);
        vector<Clause> cl;
        int used = 0;
        auto add_some = [&](int k) {
            for (int i = 0; i < k && vars; i++) {
                int ty = rng() % 3, a = rng() % vars, b = rng() % vars;
                Clause c{ty, a, b, bool(rng() % 2), bool(rng() % 2)};
                if (rng() % 8 == 0) c.type = 0, c.b = c.a, c.bf = c.af; // force x_a = af
                if (c.type == 0) s.addOR(c.a, c.af, c.b, c.bf);
                else if (c.type == 1) s.addXOR(c.a, c.af, c.b, c.bf);
                else s.add(c.a, c.af, c.b, c.bf);
                used |= 1 << c.type;
                cl.push_back(c);
            }
        };
        add_some(m);
        for (int round = 0; round < 2; round++) {
            bool sat = false;
            for (int mask = 0; mask < (1 << vars) && !sat; mask++) {
                vector<bool> x(vars);
                for (int v = 0; v < vars; v++) x[v] = mask >> v & 1;
                bool all = true;
                for (auto &c : cl) all &= holds(c, x);
                sat = all;
            }
            bool got = s.ok();
            string tag = round ? "ok (re-run)" : "ok";
            chk(tag, got == sat);
            if (got) {
                bool all = (int)s.res.size() == vars;
                for (auto &c : cl) all &= holds(c, s.res);
                chk("res satisfies all", all);
                for (int t = 0; t < 3; t++)
                    if (used >> t & 1) chk(string(name[t]) + " (sat)", all);
            } else
                for (int t = 0; t < 3; t++)
                    if (used >> t & 1) chk(string(name[t]) + " (unsat)", !sat);
            add_some(rng() % 4);
        }
    }
    { // forced contradiction and an empty instance
        Two_SAT s(1);
        s.addOR(0, 1, 0, 1), s.addOR(0, 0, 0, 0);
        chk("ok", !s.ok());
        Two_SAT e(0);
        chk("ok", e.ok());
    }
    { // timing: 2e5 vars, 1e6 clauses, plus a 2e5-long implication chain (deep recursion)
        int vars = 200000;
        Two_SAT s(vars);
        vector<bool> plant(vars);
        for (int v = 0; v < vars; v++) plant[v] = rng() % 2;
        for (int v = 0; v + 1 < vars; v++) s.add(v, plant[v], v + 1, plant[v + 1]);
        for (int i = 0; i < 1000000; i++) { // clauses satisfied by the planted assignment
            int a = rng() % vars, b = rng() % vars;
            bool af = rng() % 2, bf = rng() % 2;
            if (plant[a] != af && plant[b] != bf) af = plant[a];
            s.addOR(a, af, b, bf);
        }
        auto t0 = clock();
        bool r = s.ok();
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("vars=2e5 clauses=1.2e6: %.3fs (sat=%d)\n", sec, (int)r);
        chk("ok", r);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-18s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
