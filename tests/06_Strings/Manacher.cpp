// Test for 06_Strings/Manacher.cpp against brute-force palindrome expansion and direct reversal checks.
// Every public method is checked: manacher(s) (build + run_manacher), p[], s (padded), getLongest
// (odd and even centres), checkPalindrome (every l <= r).
#include "../../06_Strings/Manacher.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(5);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

bool is_pal(const string &s, int l, int r) {
    string a = s.substr(l, r - l + 1);
    return a == string(a.rbegin(), a.rend());
}
long long brute(const string &s, int c, bool odd) { // longest palindrome at centre c / between c, c+1
    int n = s.size();
    long long best = 0;
    for (int l = 0; l < n; l++)
        for (int r = l; r < n; r++)
            if ((odd ? l + r == 2 * c : l + r == 2 * c + 1) && is_pal(s, l, r)) best = max(best, (long long)r - l + 1);
    return best;
}
vector<long long> enumerate(manacher &m, int n) { // Library Checker enumerate_palindromes output
    vector<long long> L;
    for (int i = 0; i < 2 * n - 1; i++) L.push_back(m.getLongest(i / 2, i % 2 == 0));
    return L;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int k = vector<int>{1, 2, 3, 26}[rng() % 4], n = rng() % 18;
        string s(n, 'a');
        for (auto &c : s) c = 'a' + rng() % k;
        manacher m(s);
        string t = "#";
        for (char c : s) t += c, t += '#';
        chk("s padded", m.s == t && (int)m.p.size() == 2 * n + 1);
        bool ok = true;
        for (int i = 0; i < 2 * n + 1; i++) { // p[i] - 1 = longest palindrome centred at t[i]
            long long want = i % 2 ? brute(s, i / 2, true) : (i == 0 || i == 2 * n ? 0 : brute(s, i / 2 - 1, false));
            ok &= m.p[i] - 1 == want;
        }
        chk("p[]", ok);
        for (int c = 0; c < n; c++) {
            chk("getLongest(odd)", m.getLongest(c, true) == brute(s, c, true));
            chk("getLongest(even)", m.getLongest(c, false) == (c + 1 < n ? brute(s, c, false) : 0));
        }
        for (int l = 0; l < n; l++)
            for (int r = l; r < n; r++) chk("checkPalindrome", m.checkPalindrome(l, r) == is_pal(s, l, r));
    }
    { // samples: Library Checker enumerate_palindromes inputs vs brute, "aaaaa" exact; CSES 1111
        for (string s : {"abcbcba", "mississippi", "ababacaca", "aaaaa"}) {
            manacher m(s);
            vector<long long> want;
            for (int i = 0; i < 2 * (int)s.size() - 1; i++) want.push_back(brute(s, i / 2, i % 2 == 0));
            chk("samples", enumerate(m, s.size()) == want);
        }
        manacher a("aaaaa");
        chk("samples", enumerate(a, 5) == vector<long long>({1, 2, 3, 4, 5, 4, 3, 2, 1}));
        string s = "aybabtu";
        manacher m(s);
        long long mx = 0, idx = 0;
        for (int i = 0; i < (int)m.p.size(); i++)
            if (m.p[i] > mx) mx = m.p[i], idx = i;
        chk("samples", s.substr((idx - mx + 1) / 2, mx - 1) == "bab");
    }
    {
        auto t0 = clock();
        int n = 500000;
        string r(n, 'a'), a(n, 'a');
        for (auto &c : r) c = 'a' + rng() % 2;
        long long acc = 0;
        for (auto *s : {&r, &a}) {
            manacher m(*s);
            for (int q = 0; q < 1000000; q++) {
                int l = rng() % n, rr = l + rng() % min(n - l, 50);
                acc += m.checkPalindrome(l, rr);
            }
        }
        printf("n=5e5 random + all-same build, 2e6 checkPalindrome: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, acc);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
