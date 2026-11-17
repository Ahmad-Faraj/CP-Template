// Test for 06_Strings/Z_Function.cpp against naive O(n^2) prefix matching and naive search.
// Every public method is checked: build, operator[], z, match, minString.
#include "../../06_Strings/Z_Function.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

string rnd_str(int n, int k) {
    string s(n, 'a');
    for (auto &c : s) c = 'a' + rng() % k;
    return s;
}

vector<int> naive_z(const string &s) {
    int n = s.size();
    vector<int> z(n, 0);
    for (int i = 1; i < n; i++)
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
    return z;
}

string naive_min(const string &a, const string &b) {
    if (b.find(a) != string::npos) return b;
    for (int k = min(a.size(), b.size()); k >= 0; k--)
        if (b.compare(b.size() - k, k, a, 0, k) == 0) return b + a.substr(k);
    return b + a;
}

int main() {
    for (int it = 0; it < 60000; it++) {
        int n = rng() % 15, k = rng() % 3 + 1;
        string s = rnd_str(n, k);
        Zfunctions z;
        z.build(s);
        vector<int> nz = naive_z(s);
        chk("build", z.z == nz);
        bool ok = true;
        for (int i = 0; i < n; i++) ok &= z[i] == nz[i];
        chk("operator[]", ok);
        string p = rnd_str(rng() % 5 + 1, k);
        if (n && rng() % 3 == 0) { // pattern taken from the text
            int l = rng() % n, len = rng() % (n - l) + 1;
            p = s.substr(l, len);
        }
        vector<int> want;
        for (int i = 0; i + (int)p.size() <= n; i++)
            if (s.compare(i, p.size(), p) == 0) want.push_back(i);
        chk("match", z.match(s, p) == want);
        string a = rnd_str(rng() % 7, k), b = rnd_str(rng() % 9, k);
        chk("minString", z.minString(a, b) == naive_min(a, b));
    }
    { // the case that threw out_of_range in the original (a inside b, but z > |a| there)
        Zfunctions z;
        chk("minString", z.minString("ab", "aba") == "aba");
        chk("minString", z.minString("", "") == "" && z.minString("abc", "") == "abc");
        chk("match", z.match("", "a").empty() && z.match("aaaa", "aa") == vector<int>({0, 1, 2}));
    }
    {
        string s = rnd_str(1000000, 2), p = rnd_str(10, 2);
        Zfunctions z;
        auto t0 = clock();
        z.build(s);
        auto m = z.match(s, p);
        string mn = z.minString(s, s.substr(500000));
        printf("n=1e6 build + match + minString: %.3fs (%zu matches)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, m.size());
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
