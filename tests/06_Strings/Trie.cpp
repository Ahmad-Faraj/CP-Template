// Test for 06_Strings/Trie.cpp against std::multiset<string>.
// Every public method is checked, for all three modes: insert, erase, search, is_prefix, count_prefix.
#include "../../06_Strings/Trie.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(5);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <int Mode> void run(char base, int alpha) {
    string tag = "mode" + to_string(Mode) + ":";
    for (int it = 0; it < 3000; it++) {
        Trie<Mode> t;
        multiset<string> ms;
        int sigma = rng() % alpha + 1, maxlen = rng() % 5 + 1;
        auto word = [&]() {
            string w(rng() % (maxlen + 1), base);
            for (auto &c : w) c = base + rng() % sigma;
            return w;
        };
        for (int o = 0; o < 80; o++) {
            string w = word();
            int type = rng() % 5;
            if (type <= 1) t.insert(w), ms.insert(w);
            else if (type == 2) {
                t.erase(w);
                auto p = ms.find(w);
                if (p != ms.end()) ms.erase(p);
            }
            int cnt = 0;
            for (auto &s : ms) cnt += s.compare(0, w.size(), w) == 0;
            chk(tag + "search", t.search(w) == (bool)ms.count(w));
            chk(tag + "is_prefix", t.is_prefix(w) == (cnt > 0 || w.empty()));
            if (!w.empty()) chk(tag + "count_prefix", t.count_prefix(w) == cnt);
        }
    }
}

int main() {
    run<0>('a', 26);
    run<1>('A', 26);
    run<2>('0', 10);
    {
        Trie<> t;
        auto t0 = clock();
        string w(20, 'a');
        long long s = 0;
        for (int i = 0; i < 200000; i++) {
            for (auto &c : w) c = 'a' + rng() % 26;
            t.insert(w), s += t.count_prefix(w.substr(0, 3));
        }
        printf("2e5 inserts of length 20: %.3fs (checksum %lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
