// Test for 06_Strings/Aho_Corasick_Intervals.cpp against naive string::compare at every position.
// Every public method is checked: Aho(patterns), Aho() + insert(s, idx) + build(), search.
// Oracle: the set of distinct {start, end} of every pattern occurrence, in the documented order
// (by end, then by decreasing length). Duplicate patterns give one interval.
#include "../../06_Strings/Aho_Corasick_Intervals.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(17);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

string rnd_str(int n, int k) {
    string s(n, 'a');
    for (auto &c : s) c = 'a' + rng() % k;
    return s;
}

int main() {
    for (int it = 0; it < 40000; it++) {
        int k = vector<int>{1, 2, 3, 26}[rng() % 4];
        int m = rng() % 8 + 1, N = rng() % 30;
        vector<string> pats(m);
        for (auto &p : pats) p = rnd_str(rng() % 5 + 1, k);
        if (m > 1 && rng() % 3 == 0) pats[rng() % m] = pats[rng() % m]; // duplicate
        string text = rnd_str(N, k);
        vector<pair<long long, long long>> want;
        for (int e = 0; e < N; e++) {
            set<int> lens;
            for (auto &p : pats)
                if ((int)p.size() <= e + 1 && text.compare(e + 1 - p.size(), p.size(), p) == 0)
                    lens.insert(p.size());
            for (auto it2 = lens.rbegin(); it2 != lens.rend(); ++it2) want.push_back({e - *it2 + 1, e});
        }
        if (it % 2) {
            Aho ac(pats);
            chk("Aho(patterns)+search", ac.search(text, pats) == want);
            chk("search twice", ac.search(text, pats) == want);
        } else {
            Aho ac;
            for (int j = 0; j < m; j++) ac.insert(pats[j], j);
            ac.build();
            chk("insert+build+search", ac.search(text, pats) == want);
        }
    }
    { // empty pattern is never reported, empty text
        Aho ac(vector<string>{"", "a"});
        chk("empty pattern / text", ac.search("aa", {"", "a"}) == vector<pair<long long, long long>>(
                                                                  {{0, 0}, {1, 1}}) &&
                                         ac.search("", {"", "a"}).empty());
    }
    { // timing: 1e5 patterns of length <= 20 (sum 1e6), text 1e6
        auto t0 = clock();
        vector<string> pats(100000);
        for (auto &p : pats) p = rnd_str(rng() % 20 + 1, 2);
        string text = rnd_str(1000000, 2);
        Aho ac(pats);
        size_t cnt = ac.search(text, pats).size();
        printf("1e5 patterns (|p|<=20), 1e6 text: %.3fs (%zu intervals)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, cnt);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
