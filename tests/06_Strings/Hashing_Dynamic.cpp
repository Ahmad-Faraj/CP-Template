// Test for 06_Strings/Hashing_Dynamic.cpp against a std::string mirror and a naive polynomial hash.
// Every public method is checked: both constructors, push_back, pop_back, push_front, pop_front
// (including on empty), get_Hash, merge. Oracle: H(s) = sum (s[i]-'a'+1) * P^(len-1-i) mod M, with
// P^k by repeated multiplication. Collisions: all strings over {a,b,c} up to length 9 must differ.
#include "../../06_Strings/Hashing_Dynamic.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(1234);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

pair<ll, ll> naive(const string &s) {
    const ll P[2] = {31, 313}, M[2] = {1000000007, 1000000009};
    ll r[2] = {0, 0};
    for (int k = 0; k < 2; k++)
        for (size_t i = 0; i < s.size(); i++) {
            ll t = s[i] - 'a' + 1;
            for (size_t e = 0; e + 1 + i < s.size(); e++) t = t * P[k] % M[k];
            r[k] = (r[k] + t) % M[k];
        }
    return {r[0], r[1]};
}

bool same(const Dynamic_Hashing &h, const string &s) {
    return h.get_Hash() == naive(s) && h.len == (ll)s.size() &&
           string(h.hash.begin(), h.hash.end()) == s;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int alpha = vector<int>{1, 2, 3, 26}[rng() % 4];
        string init;
        for (int i = rng() % 6; i > 0; i--) init += char('a' + rng() % alpha);
        Dynamic_Hashing h(init);
        string s = init;
        chk("ctor(string)", same(h, s));
        Dynamic_Hashing hv(vector<char>(s.begin(), s.end()));
        chk("ctor(vector<char>)", same(hv, s));
        int ops = rng() % 40;
        for (int q = 0; q < ops; q++) {
            int t = rng() % 4;
            char c = 'a' + rng() % alpha;
            if (t == 0) h.push_back(c), s.push_back(c), chk("push_back", same(h, s));
            if (t == 1) h.push_front(c), s.insert(s.begin(), c), chk("push_front", same(h, s));
            if (t == 2) {
                bool e = s.empty();
                h.pop_back();
                if (!e) s.pop_back();
                chk(e ? "pop_back(empty)" : "pop_back", same(h, s));
            }
            if (t == 3) {
                bool e = s.empty();
                h.pop_front();
                if (!e) s.erase(s.begin());
                chk(e ? "pop_front(empty)" : "pop_front", same(h, s));
            }
            chk("get_Hash", h.get_Hash() == naive(s));
        }
        // merge against the hash of the explicitly concatenated string
        string t;
        for (int i = rng() % 8; i > 0; i--) t += char('a' + rng() % alpha);
        Dynamic_Hashing g(t);
        chk("merge", Dynamic_Hashing::merge(h, g) == naive(s + t));
        chk("merge(empty)", Dynamic_Hashing::merge(h, Dynamic_Hashing()) == naive(s) &&
                                Dynamic_Hashing::merge(Dynamic_Hashing(), g) == naive(t));
        // hash equality must match string equality, whatever op sequence built each side
        Dynamic_Hashing a;
        string as;
        for (int i = rng() % 6; i > 0; i--) {
            char c = 'a' + rng() % alpha;
            if (rng() % 2) a.push_back(c), as += c;
            else a.push_front(c), as = c + as;
        }
        chk("equal<->string equal", (a.get_Hash() == h.get_Hash()) == (as == s));
    }
    // long window: powers grow on demand past any fixed table, front/back pops stay consistent
    {
        Dynamic_Hashing h;
        string s;
        for (int i = 0; i < 300000; i++) {
            char c = 'a' + rng() % 26;
            if (i % 3) h.push_front(c), s.insert(s.begin(), c);
            else h.push_back(c), s.push_back(c);
        }
        Dynamic_Hashing b(s);
        bool ok = h.get_Hash() == b.get_Hash();
        for (int i = 0; i < 1000; i++)
            h.pop_front(), s.erase(s.begin()), h.pop_back(), s.pop_back();
        ok &= h.get_Hash() == Dynamic_Hashing(s).get_Hash();
        chk("long(3e5)", ok);
    }
    // collisions: every string over {a,b,c} of length 0..9 (29524 strings), all distinct hashes
    {
        map<pair<ll, ll>, string> seen;
        long long coll = 0, tot = 0;
        vector<string> cur{""};
        for (int L = 0; L <= 9; L++) {
            vector<string> nxt;
            for (auto &x : cur) {
                Dynamic_Hashing h(x);
                auto it = seen.find(h.get_Hash());
                if (it != seen.end() && it->second != x) coll++;
                seen[h.get_Hash()] = x;
                tot++;
                for (char c : {'a', 'b', 'c'}) nxt.push_back(x + c);
            }
            cur = nxt;
        }
        printf("collision scan: %lld strings, %lld collisions\n", tot, coll);
        chk("no_collisions", coll == 0);
    }
    {
        auto t0 = clock();
        Dynamic_Hashing h, r;
        ll pal = 0;
        for (int i = 0; i < 1000000; i++) {
            char c = 'a' + rng() % 2;
            int t = rng() % 4;
            if (t < 2) h.push_back(c), r.push_front(c);
            else if (t == 2) h.push_front(c), r.push_back(c);
            else h.pop_front(), r.pop_back();
            pal += h.get_Hash() == r.get_Hash();
        }
        printf("1e6 deque ops + palindrome checks: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, pal);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
