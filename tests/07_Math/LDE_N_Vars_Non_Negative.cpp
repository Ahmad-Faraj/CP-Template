// Test for 07_Math/LDE_N_Vars_Non_Negative.cpp against an unbounded-knapsack reachability table.
// Every public method is checked: the constructor (vector<int> and vector<ll>), can(k) and count(l, r),
// for k up to the table size directly, and for k up to 1e18 by periodicity: once k > T = min(a) * max(a),
// k is reachable iff k - min(a) is (adding the smallest coin never hurts and every residue's cheapest
// total is below T), so a huge k maps to the residue-equivalent k' in (T, T + min(a)].
#include "../../07_Math/LDE_N_Vars_Non_Negative.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(97);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }

int main() {
    for (int it = 0; it < 4000; it++) {
        int n = rnd(1, 5);
        vector<ll> a(n);
        int top = vector<int>{3, 10, 40, 200}[rng() % 4];
        for (auto &v : a) v = rnd(1, top);
        if (rng() % 4 == 0) // shared factor, so some residues are never reachable
            for (auto &v : a) v *= 2;
        ll x = *min_element(a.begin(), a.end()), mx = *max_element(a.begin(), a.end());
        ll T = x * mx, L = T + x;
        vector<char> reach(L + 1, 0); // unbounded knapsack
        reach[0] = 1;
        for (ll k = 1; k <= L; k++)
            for (ll v : a)
                if (v <= k && reach[k - v]) reach[k] = 1;
        vector<ll> pre(L + 2, 0); // pre[k] = reachable totals in [0, k)
        for (ll k = 0; k <= L; k++) pre[k + 1] = pre[k] + reach[k];
        auto R = [&](ll k) -> bool { // oracle for any k
            if (k < 0) return false;
            if (k <= L) return reach[k];
            return reach[T + 1 + ((k - T - 1) % x)];
        };
        auto upto = [&](ll k) -> __int128 { // reachable totals in [0, k], any k
            if (k < 0) return 0;
            if (k <= L) return pre[k + 1];
            __int128 total = pre[T + 1]; // [0, T]
            for (ll r = T + 1; r <= T + x; r++) // each residue class beyond T is all-or-nothing
                if (reach[r] && r <= k) total += (k - r) / x + 1;
            return total;
        };
        LDE t;
        if (rng() % 2) {
            vector<int> ai(a.begin(), a.end());
            t = LDE(ai);
            chk("LDE(vector<int>)", t.x == x);
        } else {
            t = LDE(a);
            chk("LDE(vector<ll>)", t.x == x);
        }
        for (ll k = -3; k <= min(L, 3000LL); k++) chk("can", t.can(k) == R(k));
        for (int q = 0; q < 30; q++) {
            ll k = q < 3 ? (ll)1e18 + q : rnd(0, 1e18);
            chk("can(huge)", t.can(k) == R(k));
        }
        for (int q = 0; q < 60; q++) {
            ll l = rnd(-5, L), r = rnd(l, L + 5);
            chk("count", (__int128)t.count(l, r) == upto(r) - upto(l - 1));
            l = rnd(0, 1e18), r = rnd(l, 1e18);
            if (q == 0) l = 0, r = 1e18;
            if (q == 1) l = r = 1e18;
            chk("count(huge)", (__int128)t.count(l, r) == upto(r) - upto(l - 1));
        }
    }
    {
        vector<int> a = {2}; // main's inf = 1e18 made can(1e18 + 1) true and count(1e18, 1e18) 2
        LDE t(a);
        chk("can(huge)", !t.can((ll)1e18 + 1) && t.can((ll)1e18) && t.count((ll)1e18, (ll)1e18) == 1);
        chk("can(negative)", !t.can(-1) && !t.can(-4));
    }
    {
        vector<int> a = {1000000, 999983, 1234567, 7777777, 31415926};
        auto t0 = clock();
        LDE t(a);
        ll s = 0;
        for (int i = 0; i < 1000000; i++) s += t.can(rnd(0, 1e18));
        s += t.count(0, 1e18);
        printf("build with min(a) = 1e6, n = 5, + 1e6 can + 1 count: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
