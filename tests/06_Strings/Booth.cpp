// Test for 06_Strings/Booth.cpp against the minimum over all n rotations built with substr.
// Every public function is checked: least_rotation (empty, n = 1, periodic, random, all-same).
#include "../../06_Strings/Booth.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(11);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

string brute(const string &s) {
    string best = s;
    for (size_t i = 0; i < s.size(); i++) best = min(best, s.substr(i) + s.substr(0, i));
    return best;
}

int main() {
    chk("least_rotation(empty)", least_rotation("") == "");
    for (int it = 0; it < 200000; it++) {
        int k = vector<int>{1, 2, 3, 26}[rng() % 4], n = rng() % 20 + 1;
        string s(n, 'a');
        for (auto &c : s) c = 'a' + rng() % k;
        if (rng() % 4 == 0) { // periodic
            string b = s.substr(0, rng() % n + 1);
            s.clear();
            while ((int)s.size() < n) s += b;
        }
        chk("least_rotation", least_rotation(s) == brute(s));
    }
    // sample: CSES 1110 Minimal Rotation
    chk("samples", least_rotation("acab") == "abac");
    {
        auto t0 = clock();
        string r(1000000, 'a'), a(1000000, 'a');
        for (auto &c : r) c = 'a' + rng() % 2;
        a.back() = 'b';
        size_t acc = least_rotation(r).size() + least_rotation(a).size();
        chk("large(aaa..b)", least_rotation(a) == a && least_rotation(a.back() + a.substr(0, a.size() - 1)) == a);
        printf("n=1e6 random + aaa..b: %.3fs (%zu)\n", double(clock() - t0) / CLOCKS_PER_SEC, acc);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
