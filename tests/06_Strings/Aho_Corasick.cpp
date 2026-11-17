// Test for 06_Strings/Aho_Corasick.cpp against naive string::find of every pattern at every position.
// Every public method is checked: AhoCorasick(patterns), AhoCorasick() + insert(s, idx) + build(), search.
// Nodes are never freed (as in the original): run the sanitizer build with ASAN_OPTIONS=detect_leaks=0.
#include "../../06_Strings/Aho_Corasick.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(41);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

string rnd_str(int n, int k) {
    string s(n, 'a');
    for (auto &c : s) c = 'a' + rng() % k;
    return s;
}

int main() {
    for (int it = 0; it < 30000; it++) {
        int k = vector<int>{1, 2, 3, 26}[rng() % 4];
        int m = rng() % 8 + 1, N = rng() % 25;
        vector<string> pats(m);
        for (auto &p : pats) p = rnd_str(rng() % 4 + 1, k);
        if (m > 1 && rng() % 3 == 0) pats[rng() % m] = pats[rng() % m]; // duplicate pattern
        string text = rnd_str(N, k);
        // oracle: per end position, the sorted list of pattern ids; per pattern, occurrence count via find
        bool manual = it % 2;
        vector<int> id(m);
        for (int j = 0; j < m; j++) id[j] = manual ? 1000 + 7 * j : j;
        vector<vector<int>> want(N);
        vector<int> want_cnt(m, 0);
        for (int j = 0; j < m; j++)
            for (size_t pos = text.find(pats[j]); pos != string::npos; pos = text.find(pats[j], pos + 1))
                want[pos + pats[j].size() - 1].push_back(id[j]), want_cnt[j]++;
        for (auto &w : want) sort(w.begin(), w.end());
        vector<vector<int>> got;
        if (manual) {
            AhoCorasick ac;
            for (int j = 0; j < m; j++) ac.insert(pats[j], id[j]);
            ac.build();
            got = ac.search(text);
            auto again = ac.search(text); // search must not change the automaton
            chk("search(repeat)", again == got);
        } else {
            AhoCorasick ac(pats);
            got = ac.search(text);
        }
        for (auto &g : got) sort(g.begin(), g.end());
        string tag = manual ? "insert+build+search" : "ctor+search";
        chk(tag + "(per position)", got == want);
        bool ok = true; // per pattern: how often its index is reported vs naive find
        for (int j = 0; j < m; j++) {
            int c = 0;
            for (auto &g : got) c += count(g.begin(), g.end(), id[j]);
            ok &= c == want_cnt[j];
        }
        chk(tag + "(occurrence counts)", ok);
    }
    {
        AhoCorasick ac(vector<string>{});
        auto r = ac.search("abc");
        chk("ctor+search(no patterns)", r.size() == 3 && r[0].empty() && r[2].empty());
        AhoCorasick b(vector<string>{"a"});
        chk("ctor+search(empty text)", b.search("").empty());
    }
    {
        vector<string> pats;
        for (int i = 0; i < 100000; i++) pats.push_back(rnd_str(rng() % 13 + 4, 3 + i % 24));
        string text = rnd_str(1000000, 3);
        auto t0 = clock();
        AhoCorasick ac(pats);
        auto r = ac.search(text);
        long long tot = 0;
        for (auto &v : r) tot += v.size();
        printf("1e5 patterns (1e6 chars) + text 1e6: %.3fs (%lld matches)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, tot);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-36s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
