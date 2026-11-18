// Test for 10_Misc/All_Permutations.cpp.
// all_permutations: compared with a brute enumeration of all n! index orders put into a std::set
// (distinct sequences, sorted). applyPermutation / permute: compared with k naive steps, and for
// huge k with per-element cycle walking (k mod cycle length). Sample: CSES 1622 Creating Strings.
#include "../../10_Misc/All_Permutations.cpp"
using ll = long long;

map<string, pair<long long, long long>> cov;
mt19937_64 rng(1622);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <class C> vector<C> brute_perms(const C &a) {
    int n = a.size();
    set<C> s;
    vector<int> idx(n);
    // all n! index orders via Heap-free recursion
    function<void(int, vector<bool> &, C &)> rec = [&](int d, vector<bool> &used, C &cur) {
        if (d == n) {
            s.insert(cur);
            return;
        }
        for (int i = 0; i < n; i++)
            if (!used[i]) {
                used[i] = true, cur[d] = a[i];
                rec(d + 1, used, cur);
                used[i] = false;
            }
    };
    vector<bool> used(n);
    C cur = a;
    rec(0, used, cur);
    return vector<C>(s.begin(), s.end());
}

vector<int> rand_perm(int n) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    shuffle(p.begin(), p.end(), rng);
    return p;
}

int main() {
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 7;
        vector<int> a(n);
        int vr = vector<int>{2, 3, 100}[rng() % 3];
        for (auto &x : a) x = (int)(rng() % vr) - vr / 2;
        vector<vector<int>> got;
        all_permutations(a, [&](const vector<int> &p) { got.push_back(p); });
        chk("all_permutations(vector)", got == brute_perms(a));
        string s(n, 'a');
        for (auto &c : s) c = 'a' + rng() % 3;
        vector<string> gs;
        all_permutations(s, [&](const string &p) { gs.push_back(p); });
        chk("all_permutations(string)", gs == brute_perms(s));
    }
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 9 + 1;
        vector<int> seq(n), p = rand_perm(n);
        for (auto &x : seq) x = (int)(rng() % 2000000001) - 1000000000;
        vector<int> naive(n);
        for (int i = 0; i < n; i++) naive[i] = seq[p[i]];
        chk("applyPermutation", applyPermutation(seq, p) == naive);
        ll k = rng() % 40;
        vector<int> cur = seq;
        for (ll t = 0; t < k; t++) {
            vector<int> nx(n);
            for (int i = 0; i < n; i++) nx[i] = cur[p[i]];
            cur = nx;
        }
        chk("permute(small k)", permute(seq, p, k) == cur);
        ll K = rng() % 4 ? (ll)(rng() >> 1) : LLONG_MAX - (ll)(rng() % 5);
        vector<int> want(n);
        for (int i = 0; i < n; i++) { // p^K(i): walk K mod cycle length steps
            int len = 1;
            for (int j = p[i]; j != i; j = p[j]) len++;
            int j = i;
            for (ll t = 0; t < K % len; t++) j = p[j];
            want[i] = seq[j];
        }
        chk("permute(huge k)", permute(seq, p, K) == want);
    }
    chk("edge:k=0", permute({5, 6, 7}, {2, 0, 1}, 0) == vector<int>({5, 6, 7}));
    chk("edge:empty", permute({}, {}, 7).empty() && applyPermutation({}, {}).empty());
    {
        int cnt = 0;
        all_permutations(vector<int>{}, [&](const vector<int> &) { cnt++; });
        chk("edge:empty all_permutations", cnt == 1);
    }
    {   // CSES 1622 sample: "aabac" has 20 distinct orderings, listed in sorted order
        vector<string> v;
        all_permutations(string("aabac"), [&](const string &p) { v.push_back(p); });
        chk("samples(CSES 1622)", v.size() == 20 && v[0] == "aaabc" && v[1] == "aaacb" &&
                                      v.back() == "cbaaa");
    }
    {
        int n = 200000;
        vector<int> seq(n), p = rand_perm(n);
        iota(seq.begin(), seq.end(), 0);
        auto t0 = clock();
        auto r = permute(seq, p, (ll)1e18);
        ll cnt = 0;
        all_permutations(vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                         [&](const vector<int> &q) { cnt += q[0]; });
        printf("permute n=2e5 k=1e18 + all 10! permutations: %.3fs (%lld %d)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, cnt, r[0]);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
