// Test for 06_Strings/Hashing.cpp against direct string / vector comparison and the hash definition
// sum v[i] * p^(r - i) mod m computed by naive loops. Every public method is checked: Hash(string),
// Hash(vector), Hash() default, sub, merge_hash, at, equal, both Base values. Collisions are counted
// (any collision is a mismatch in "equal").
#include "../../06_Strings/Hashing.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(123);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

using ll = long long;
template <class H, class V> pair<ll, ll> naive(const V &v, int l, int r) { // 0-based [l, r]
    ll a = 0, b = 0;
    for (int i = l; i <= r; i++) {
        ll x = (ll)v[i];
        a = ((__int128)a * H::p1 + ((x % H::m1) + H::m1) % H::m1) % H::m1;
        b = ((__int128)b * H::p2 + ((x % H::m2) + H::m2) % H::m2) % H::m2;
    }
    return {a, b};
}

int main() {
    Hash<> dflt; // default ctor only draws the parameters
    chk("Hash() params", Hash<>::p1 > 0 && Hash<>::p2 > 0 && Hash<>::m1 != Hash<>::m2);
    long long collisions = 0;
    for (int it = 0; it < 6000; it++) {
        int k = vector<int>{1, 2, 2, 3, 26}[rng() % 5], n = rng() % 30 + 1;
        string s(n, 'a');
        for (auto &c : s) c = 'a' + rng() % k;
        if (it % 50 == 0) s[rng() % n] = char(200 + rng() % 50); // bytes > 127 (negative char)
        Hash<> h(s);
        Hash<ll, 1> h1(" " + s);
        vector<unsigned char> us(s.begin(), s.end());
        for (int l = 1; l <= n; l++)
            for (int r = l; r <= n; r++) {
                chk("sub", h.sub(l, r) == naive<Hash<>>(us, l - 1, r - 1));
                chk("sub(Base=1)", h1.sub(l, r) == naive<Hash<ll, 1>>(us, l - 1, r - 1));
                for (int q = 0; q < 3; q++) { // compare against random other substrings
                    int l2 = rng() % n + 1, r2 = l2 + rng() % (n - l2 + 1);
                    if (q == 0 && r - l + l2 <= n) r2 = l2 + r - l; // same length, more interesting
                    bool same = s.compare(l - 1, r - l + 1, s, l2 - 1, r2 - l2 + 1) == 0;
                    bool e = h.equal(l, r, l2, r2);
                    collisions += e != same;
                    chk("equal", e == same);
                    chk("equal(Base=1)", h1.equal(l, r, l2, r2) == same);
                    string cat = s.substr(l - 1, r - l + 1) + s.substr(l2 - 1, r2 - l2 + 1);
                    vector<unsigned char> uc(cat.begin(), cat.end());
                    chk("merge_hash", h.merge_hash(l, r, l2, r2) == naive<Hash<>>(uc, 0, cat.size() - 1));
                }
            }
        for (int i = 1; i <= n; i++) chk("at", h.at(i) == naive<Hash<>>(us, i - 1, i - 1));
        // vectors: negatives, zeros, huge values
        vector<ll> v(n);
        for (auto &x : v) {
            int t = rng() % 4;
            ll big = (ll)(((unsigned long long)rng() << 31) * 1000003ULL >> 1);
            x = t == 0 ? 0 : t == 1 ? -(ll)(rng() % 3) : t == 2 ? (ll)(rng() % 2) : big;
            if (rng() % 10 == 0) x = rng() % 2 ? LLONG_MAX : LLONG_MIN;
        }
        Hash<> hv(v);
        vector<ll> v1 = v;
        v1.insert(v1.begin(), 777);
        Hash<ll, 1> hv1(v1);
        for (int q = 0; q < 60; q++) {
            int l = rng() % n + 1, r = l + rng() % (n - l + 1);
            int l2 = rng() % n + 1, r2 = l2 + rng() % (n - l2 + 1);
            bool same = vector<ll>(v.begin() + l - 1, v.begin() + r) == vector<ll>(v.begin() + l2 - 1, v.begin() + r2);
            collisions += hv.equal(l, r, l2, r2) != same;
            chk("vector:equal", hv.equal(l, r, l2, r2) == same);
            chk("vector:equal(Base=1)", hv1.equal(l, r, l2, r2) == same);
            chk("vector:sub", hv.sub(l, r) == naive<Hash<>>(v, l - 1, r - 1));
            chk("vector:sub(Base=1)", hv1.sub(l, r) == naive<Hash<ll, 1>>(v, l - 1, r - 1));
        }
    }
    { // birthday test: every length-24 window of a random 3e5 binary string; equal hash => equal text
        int n = 300000, L = 24;
        string s(n, 'a');
        for (auto &c : s) c = 'a' + rng() % 2;
        Hash<> h(s);
        map<pair<ll, ll>, int> seen;
        for (int i = 1; i + L - 1 <= n; i++) {
            auto [itr, fresh] = seen.insert({h.sub(i, i + L - 1), i});
            bool bad = !fresh && s.compare(i - 1, L, s, itr->second - 1, L) != 0;
            collisions += bad;
            chk("equal(birthday, 3e5 windows)", !bad);
        }
    }
    printf("collisions on small alphabets: %lld\n", collisions);
    { // samples: CSES 1753 String Matching (saippuakauppias / pp -> 2), CSES 1732 (abcababcab -> 2 5)
        string s = "saippuakauppias", p = "pp", t = p + s;
        Hash<> h(t);
        int c = 0;
        for (int i = 3; i + 1 <= (int)t.size(); i++) c += h.equal(1, 2, i, i + 1);
        chk("samples", c == 2);
        string b = "abcababcab";
        Hash<> hb(b);
        vector<int> br;
        for (int len = 1; len < (int)b.size(); len++)
            if (hb.equal(1, len, b.size() - len + 1, b.size())) br.push_back(len);
        chk("samples", br == vector<int>({2, 5}));
    }
    {
        auto t0 = clock();
        int n = 1000000;
        string s(n, 'a');
        for (auto &c : s) c = 'a' + rng() % 2;
        Hash<> h(s);
        long long acc = 0;
        for (int q = 0; q < 2000000; q++) {
            int len = rng() % 1000 + 1, a = rng() % (n - len + 1) + 1, b = rng() % (n - len + 1) + 1;
            acc += h.equal(a, a + len - 1, b, b + len - 1);
        }
        printf("1e6 build + 2e6 equal queries: %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, acc);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
