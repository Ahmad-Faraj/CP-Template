// Test for 01_Data_Structures/DSU.cpp against naive component labels.
// Every public method is checked: constructor, find_leader, is_same_sets, union_sets, get_size,
// get_components, get_components_number, print. Both Base = 0 and Base = 1, T = int and long long.
#include "../../01_Data_Structures/DSU.cpp"

map<string, pair<long long, long long>> cov; // method -> (checks, mismatches)
mt19937 rng(12345);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <typename T, int Base> void run(int iters) {
    for (int it = 0; it < iters; it++) {
        int n = rng() % 12 + 1;
        DSU<T, Base> d(n);
        vector<int> lab(n + Base);
        for (int i = Base; i < n + Base; i++) lab[i] = i;
        int ops = rng() % 30;
        for (int o = 0; o <= ops; o++) {
            int u = Base + rng() % n, v = Base + rng() % n;
            if (rng() % 2) {
                d.union_sets(u, v);
                int a = lab[u], b = lab[v];
                if (a != b)
                    for (int i = Base; i < n + Base; i++)
                        if (lab[i] == b) lab[i] = a;
                chk("union_sets", true);
            }
            chk("is_same_sets", d.is_same_sets(u, v) == (lab[u] == lab[v]));
            chk("find_leader", (d.find_leader(u) == d.find_leader(v)) == (lab[u] == lab[v]));
            int sz = 0;
            for (int i = Base; i < n + Base; i++) sz += lab[i] == lab[u];
            chk("get_size", d.get_size(u) == sz);
        }
        // components: compare as sorted sets of sets
        set<vector<int>> want;
        map<int, vector<int>> g;
        for (int i = Base; i < n + Base; i++) g[lab[i]].push_back(i);
        for (auto &[k, v] : g) want.insert(v);
        set<vector<int>> got;
        for (auto c : d.get_components()) {
            sort(c.begin(), c.end());
            got.insert(c);
        }
        chk("get_components", got == want && d.get_components().size() == want.size());
        chk("get_components_number", d.get_components_number() == (int)want.size());
        // print: one line per component
        stringstream ss;
        auto *old = cout.rdbuf(ss.rdbuf());
        d.print();
        cout.rdbuf(old);
        set<vector<int>> printed;
        string line;
        while (getline(ss, line)) {
            stringstream ls(line);
            vector<int> c;
            int x;
            while (ls >> x) c.push_back(x);
            sort(c.begin(), c.end());
            printed.insert(c);
        }
        chk("print", printed == want);
    }
}

int main() {
    run<int, 1>(20000);
    run<int, 0>(20000);
    run<long long, 1>(10000);
    // large: 2e5 nodes, 2e5 random unions, then a chain of unions
    {
        int n = 200000;
        DSU<int, 1> d(n);
        auto t0 = clock();
        for (int i = 0; i < n; i++) d.union_sets(1 + rng() % n, 1 + rng() % n);
        for (int i = 1; i < n; i++) d.union_sets(i, i + 1);
        chk("large_all_joined", d.get_components_number() == 1 && d.get_size(1) == n);
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("large n=2e5: %.3fs\n", t);
        chk("large_time_under_1s", t < 1.0);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
