// Test for 06_Strings/KMP.cpp against naive substring comparison.
// Every public method is checked: KMP(p), KMP() + build(), failure, match, count, countInAnother,
// get_prefixes, build_non_overlapping (nol), and that lp survives build_non_overlapping.
#include "../../06_Strings/KMP.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

string rnd_str(int n, int k) {
    string s(n, 'a');
    for (auto &c : s) c = 'a' + rng() % k;
    return s;
}
bool is_border(const string &s, int L, int len) { // prefix of length len is a suffix of s[0..L-1]
    return s.compare(0, len, s, L - len, len) == 0;
}
int occ(const string &t, const string &p) {
    int c = 0;
    for (size_t i = 0; i + p.size() <= t.size(); i++) c += t.compare(i, p.size(), p) == 0;
    return c;
}

int main() {
    for (int it = 0; it < 60000; it++) {
        int k = vector<int>{1, 2, 3, 26}[rng() % 4];
        string p = rnd_str(rng() % 12 + 1, k), t = rnd_str(rng() % 30, k);
        if (rng() % 4 == 0) { // periodic pattern
            string b = rnd_str(rng() % 3 + 1, k);
            p.clear();
            while ((int)p.size() < 1 + (int)(rng() % 12)) p += b;
        }
        int m = p.size();
        KMP K;
        if (it % 2) K = KMP(p);
        else K.pattern = p, K.build();
        bool ok = (int)K.lp.size() == m;
        for (int i = 0; i < m && ok; i++) {
            int b = 0;
            for (int len = 1; len <= i; len++)
                if (is_border(p, i + 1, len)) b = len;
            ok &= K.lp[i] == b;
        }
        chk(it % 2 ? "KMP(p) lp" : "build() lp", ok);
        // failure(idx, c): longest prefix of p that is a suffix of p[0..idx-1] + c, idx in [0, m]
        for (int q = 0; q < 4; q++) {
            int idx = rng() % (m + 1);
            char c = 'a' + rng() % k;
            string w = p.substr(0, idx) + c;
            int want = 0;
            for (int len = 1; len <= min(m, (int)w.size()); len++)
                if (w.compare(w.size() - len, len, p, 0, len) == 0) want = len;
            chk("failure", K.failure(idx, c) == want);
        }
        vector<int> mt;
        for (int i = 0; i + m <= (int)t.size(); i++)
            if (t.compare(i, m, p) == 0) mt.push_back(i + 1);
        chk("match", K.match(t) == mt);
        // text = pattern repeated (many overlapping matches)
        string tt = p + p + p.substr(0, m / 2) + p;
        vector<int> mt2;
        for (int i = 0; i + m <= (int)tt.size(); i++)
            if (tt.compare(i, m, p) == 0) mt2.push_back(i + 1);
        chk("match(overlapping)", K.match(tt) == mt2);
        auto cnt = K.count();
        ok = (int)cnt.size() == m + 1;
        for (int len = 1; len <= m && ok; len++) ok &= cnt[len] == occ(p, p.substr(0, len));
        chk("count", ok);
        auto ca = K.countInAnother(t);
        ok = (int)ca.size() == m;
        for (int len = 1; len <= m && ok; len++) ok &= ca[len - 1] == occ(t, p.substr(0, len));
        chk("countInAnother", ok);
        vector<int> br;
        for (int len = 1; len < m; len++)
            if (is_border(p, m, len)) br.push_back(len);
        br.push_back(m);
        chk("get_prefixes", K.get_prefixes() == br);
        vector<int> lp0 = K.lp;
        K.build_non_overlapping();
        ok = (int)K.nol.size() == m;
        for (int i = 0; i < m && ok; i++) {
            int b = 0;
            for (int len = 1; 2 * len <= i + 1; len++)
                if (is_border(p, i + 1, len)) b = len;
            ok &= K.nol[i] == b;
        }
        chk("build_non_overlapping", ok);
        chk("lp kept after build_non_overlapping", K.lp == lp0 && K.match(t) == mt);
    }
    { // documented example
        KMP K("ababab");
        K.build_non_overlapping();
        chk("nol example ababab", K.nol == vector<int>({0, 0, 1, 2, 1, 2}));
    }
    { // samples: CSES 1753 String Matching, CSES 1732 Finding Borders, CF 432D Prefixes and Suffixes
        chk("samples", KMP("pp").match("saippuakauppias").size() == 2);
        auto b = KMP("abcababcab").get_prefixes();
        b.pop_back();
        chk("samples", b == vector<int>({2, 5}));
        KMP K("ABACABA");
        auto c = K.count();
        vector<pair<int, int>> got;
        for (int x : K.get_prefixes()) got.push_back({x, c[x]});
        chk("samples", got == vector<pair<int, int>>({{1, 4}, {3, 2}, {7, 1}}));
    }
    {
        auto t0 = clock();
        string text = rnd_str(1000000, 2), a(1000000, 'a');
        KMP K(rnd_str(10, 2)), A(a);
        long long s = K.match(text).size() + K.countInAnother(text)[0];
        A.build_non_overlapping();
        s += A.count()[1] + A.nol.back() + A.get_prefixes().size();
        printf("1e6 text match + countInAnother, 1e6 'a' pattern count/nol/borders: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-38s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
