// Test for 06_Strings/Aho_Corasick_Automaton.cpp against naive matching on explicit prefixes.
// Every public member is checked: ctor(patterns), ctor() + insert(s, idx) (return value, node_of)
// + build(), size, go / nxt (full goto function), link, cnt, order, total_matches, occurrences,
// and a DP over the automaton (strings of length L avoiding all patterns) vs brute enumeration.
#include "../../06_Strings/Aho_Corasick_Automaton.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(71);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

string rnd_str(int n, int k) {
    string s(n, 'a');
    for (auto &c : s) c = 'a' + rng() % k;
    return s;
}
bool is_suffix(const string &suf, const string &s) {
    return suf.size() <= s.size() && s.compare(s.size() - suf.size(), suf.size(), suf) == 0;
}
ll naive_count(const string &p, const string &t) {
    ll c = 0;
    for (size_t i = 0; i + p.size() <= t.size(); i++) c += t.compare(i, p.size(), p) == 0;
    return c;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int k = vector<int>{1, 2, 3, 26}[rng() % 4];
        int m = rng() % 7, N = rng() % 30;
        vector<string> pats(m);
        for (auto &p : pats) p = rnd_str(rng() % 5 + 1, k);
        if (m > 1 && rng() % 3 == 0) pats[rng() % m] = pats[rng() % m]; // duplicate pattern
        string text = rnd_str(N, k);
        bool manual = it % 2;
        vector<int> id(m);
        for (int j = 0; j < m; j++) id[j] = manual ? 3 * j + 1 : j;
        AhoCorasickAutomaton ac;
        if (manual) {
            bool ok = true;
            for (int j = 0; j < m; j++) {
                int s = ac.insert(pats[j], id[j]);
                ok &= ac.node_of[id[j]] == s;
            }
            ac.build();
            for (int j = 0; j < m; j++) { // terminal state = walk of the pattern through go
                int s = 0;
                for (char c : pats[j]) s = ac.go(s, c);
                ok &= ac.node_of[id[j]] == s;
            }
            chk("insert(return, node_of)", ok);
        } else ac = AhoCorasickAutomaton(pats);
        string t = manual ? "manual:" : "ctor:";

        // explicit prefix set, each prefix located by walking go from the root
        set<string> pre{""};
        for (auto &p : pats)
            for (size_t l = 1; l <= p.size(); l++) pre.insert(p.substr(0, l));
        vector<string> str(ac.size(), "?");
        bool ok = (int)pre.size() == ac.size();
        map<string, int> st;
        for (auto &p : pre) {
            int s = 0;
            for (char c : p) s = ac.go(s, c);
            ok &= str[s] == "?", str[s] = p, st[p] = s;
        }
        chk(t + "size", ok);
        auto longest = [&](const string &s) { // state of the longest suffix of s in the prefix set
            for (size_t l = s.size();; l--)
                if (pre.count(s.substr(s.size() - l))) return st[s.substr(s.size() - l)];
        };
        bool okg = true, okl = true, okc = true;
        for (int s = 0; s < ac.size(); s++) {
            for (int c = 0; c < k; c++) okg &= ac.go(s, 'a' + c) == longest(str[s] + char('a' + c));
            if (s) okl &= ac.link[s] == longest(str[s].substr(1));
            int want = 0;
            for (auto &p : pats) want += is_suffix(p, str[s]);
            okc &= ac.cnt[s] == want;
        }
        chk(t + "go", okg), chk(t + "link", okl), chk(t + "cnt", okc);
        vector<int> pos(ac.size(), -1);
        bool oko = (int)ac.order.size() == ac.size() && ac.order[0] == 0;
        for (int i = 0; oko && i < (int)ac.order.size(); i++) {
            oko &= pos[ac.order[i]] == -1, pos[ac.order[i]] = i;
            if (i) oko &= pos[ac.link[ac.order[i]]] != -1 && pos[ac.link[ac.order[i]]] < i;
        }
        chk(t + "order", oko);

        ll total = 0;
        for (auto &p : pats) total += naive_count(p, text);
        chk(t + "total_matches", ac.total_matches(text) == total);
        auto occ = ac.occurrences(text);
        bool okq = (int)occ.size() == (m ? id[m - 1] + 1 : 0);
        for (int j = 0; j < m && okq; j++) okq &= occ[id[j]] == naive_count(pats[j], text);
        if (manual)
            for (int x = 0; x < (int)occ.size(); x++)
                if (x % 3 != 1) okq &= occ[x] == 0; // indices never inserted
        chk(t + "occurrences", okq);

        if (k <= 3) { // DP: strings of length L over k letters containing no pattern
            int L = rng() % 7;
            vector<ll> dp(ac.size(), 0);
            dp[0] = 1;
            for (int step = 0; step < L; step++) {
                vector<ll> nd(ac.size(), 0);
                for (int s = 0; s < ac.size(); s++)
                    if (dp[s])
                        for (int c = 0; c < k; c++) {
                            int u = ac.go(s, 'a' + c);
                            if (!ac.cnt[u]) nd[u] += dp[s];
                        }
                dp = nd;
            }
            ll want = 0, got = accumulate(dp.begin(), dp.end(), 0LL), tot = 1;
            for (int i = 0; i < L; i++) tot *= k;
            for (ll code = 0; code < tot; code++) {
                string s;
                for (ll c = code, i = 0; i < L; i++, c /= k) s += char('a' + c % k);
                bool bad = false;
                for (auto &p : pats) bad |= s.find(p) != string::npos;
                want += !bad;
            }
            chk("dp avoid-all (count strings)", got == want);
        }
    }
    { // empty pattern set
        AhoCorasickAutomaton ac(vector<string>{});
        chk("empty set", ac.size() == 1 && ac.go(0, 'q') == 0 && ac.total_matches("abc") == 0 &&
                             ac.occurrences("abc").empty());
    }
    { // large: Library Checker sized, sum |p| = 1e6 nodes (worst memory), text 1e6
        int S = 1000000;
#ifdef __SANITIZE_ADDRESS__
        S = 100000;
#endif
        vector<string> pats;
        for (int tot = 0; tot < S;) {
            int l = min(S - tot, (int)(rng() % 20 + 1));
            pats.push_back(rnd_str(l, 26)), tot += l;
        }
        string text = rnd_str(S, 2);
        auto t0 = clock();
        AhoCorasickAutomaton ac(pats);
        ll tm = ac.total_matches(text);
        auto occ = ac.occurrences(text);
        printf("build sum|p|=%d (%d states) + scan |text|=%d: %.3fs (matches=%lld)\n", S, ac.size(),
               S,
               double(clock() - t0) / CLOCKS_PER_SEC, tm);
        ll sum = 0;
        for (int i = 0; i < (int)pats.size(); i++) sum += occ[i];
        chk("large total == sum occurrences", sum == tm);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-30s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
