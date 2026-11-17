// Test for 06_Strings/Suffix_Array.cpp against sorting every suffix as a std::string and brute-force
// substring enumeration. Every public method is checked: constructor (p, c, lcp, n), longest_common_prefix,
// compare(i, j, l), compare(i, pattern), find, lower, upper, count, distinct_substrings,
// longest_repeated_substring, longest_common_substring, kth_substring, min_common_substring.
#include "../../06_Strings/Suffix_Array.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

string rnd_str(int n, const string &alpha) {
    string s(n, 'a');
    for (auto &c : s) c = alpha[rng() % alpha.size()];
    return s;
}
int sgn(int x) { return (x > 0) - (x < 0); }
int occ(const string &s, const string &t) { // overlapping occurrences of t in s
    int c = 0;
    for (int i = 0; i + (int)t.size() <= (int)s.size(); i++) c += s.compare(i, t.size(), t) == 0;
    return c;
}

int main() {
    vector<string> alphas = {"a", "ab", "abc", "!aZ~", "abcdefghijklmnopqrstuvwxyz"};
    for (int it = 0; it < 12000; it++) {
        const string &al = alphas[rng() % alphas.size()];
        int N = rng() % 13;
        string s = rnd_str(N, al);
        SuffixArray sa(s);
        int n = N + 1;
        vector<int> want_p(n);
        iota(want_p.begin(), want_p.end(), 0);
        sort(want_p.begin(), want_p.end(), [&](int a, int b) { return s.substr(a) < s.substr(b); });
        chk("n", sa.n == n && sa.s == s);
        chk("p", sa.p == want_p);
        bool okc = true, okl = sa.lcp[0] == 0;
        auto nlcp = [&](int a, int b) {
            int L = 0;
            while (a + L < N && b + L < N && s[a + L] == s[b + L]) L++;
            return L;
        };
        for (int r = 0; r < n; r++) okc &= sa.c[want_p[r]] == r;
        for (int r = 1; r < n; r++) okl &= sa.lcp[r] == nlcp(want_p[r - 1], want_p[r]);
        chk("c(rank)", okc);
        chk("lcp", okl);
        bool oklcp = true, okcmp = true;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                oklcp &= sa.longest_common_prefix(i, j) == (i == j ? N - i : nlcp(i, j));
                for (int l = 0; l <= n; l++)
                    okcmp &= sa.compare(i, j, l) == sgn(s.substr(i, l).compare(s.substr(j, l)));
            }
        chk("longest_common_prefix", oklcp);
        chk("compare(i,j,l)", okcmp);
        for (int q = 0; q < 6; q++) {
            string pat = rnd_str(rng() % 4 + 1, al);
            if (N && q % 2) {
                int l = rng() % N;
                pat = s.substr(l, rng() % (N - l) + 1);
            }
            int c = occ(s, pat), lo = 0, le = 0;
            for (int i = 0; i < n; i++) lo += s.compare(i, pat.size(), pat) < 0, le += s.compare(i, pat.size(), pat) <= 0;
            chk("find", sa.find(pat) == (c > 0));
            chk("count", sa.count(pat) == c);
            chk("lower", sa.lower(pat) == lo);
            chk("upper", sa.upper(pat) == le - 1);
            int i = rng() % n;
            chk("compare(i,pattern)", sgn(sa.compare(i, pat)) == sgn(s.compare(i, pat.size(), pat)));
        }
        // all substrings
        vector<string> all;
        map<string, int> cnt;
        for (int i = 0; i < N; i++)
            for (int j = i; j < N; j++) all.push_back(s.substr(i, j - i + 1)), cnt[all.back()]++;
        sort(all.begin(), all.end());
        chk("distinct_substrings", sa.distinct_substrings() == (long long)cnt.size());
        int best = 0;
        for (auto &[t, k] : cnt) if (k >= 2) best = max(best, (int)t.size());
        string lrs = sa.longest_repeated_substring();
        chk("longest_repeated_substring", (int)lrs.size() == best && (best == 0 || cnt[lrs] >= 2));
        for (int k = 1; k <= (int)all.size(); k++)
            chk("kth_substring", SuffixArray::kth_substring(k, sa.s, sa.p) == all[k - 1]);
        // two-string functions
        string t = rnd_str(rng() % 13, al);
        map<string, int> ct;
        for (int i = 0; i < (int)t.size(); i++)
            for (int j = i; j < (int)t.size(); j++) ct[t.substr(i, j - i + 1)]++;
        int lcs = 0, mcs = INT_MAX;
        for (auto &[u, k] : cnt)
            if (ct.count(u)) {
                lcs = max(lcs, (int)u.size());
                if (k == 1 && ct[u] == 1) mcs = min(mcs, (int)u.size());
            }
        string got = SuffixArray::longest_common_substring(s, t);
        chk("longest_common_substring", (int)got.size() == lcs && (lcs == 0 || (cnt.count(got) && ct.count(got))));
        chk("min_common_substring", SuffixArray::min_common_substring(s, t) == (mcs == INT_MAX ? -1 : mcs));
    }
    { // the case the original longest_common_substring missed (last adjacent pair never checked)
        chk("longest_common_substring", SuffixArray::longest_common_substring("z", "z") == "z");
    }
    {
        int N = 500000;
        string s = rnd_str(N, "ab");
        auto t0 = clock();
        SuffixArray sa(s);
        long long d = sa.distinct_substrings(), sum = 0;
        for (int q = 0; q < 200000; q++) sum += sa.longest_common_prefix(rng() % N, rng() % N);
        for (int q = 0; q < 100000; q++) sum += sa.count(rnd_str(12, "ab"));
        string lcs = SuffixArray::longest_common_substring(s.substr(0, 250000), s.substr(250000));
        printf("n=5e5 build + 2e5 lcp + 1e5 count + lcs(2.5e5, 2.5e5): %.3fs (%lld, %zu)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, d, lcs.size());
        string a(200000, 'a');
        SuffixArray sb(a);
        chk("large:all-a", sb.distinct_substrings() == 200000 && sb.longest_repeated_substring().size() == 199999);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
