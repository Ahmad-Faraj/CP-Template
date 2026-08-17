// CRT: combines congruences x = r_i (mod m_i) into one congruence, or reports that they contradict each other.
// Use when: "x leaves remainder r_i when divided by m_i", recombining results computed under several small moduli.
// Handles: moduli that are NOT coprime (consistency is checked), any count of congruences, large moduli via __int128.
// Time: O(k log m) for k congruences
// Indexing: 0-based parallel vectors of remainders and moduli
// Note: returns {remainder, modulus} and sets ok = false when the system contradicts. Check ok before using it.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll extended_gcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    ll x1, y1;
    ll g = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

// merges x = r1 (mod m1) with x = r2 (mod m2); ok is false when they contradict
pair<ll, ll> crt_pair(ll r1, ll m1, ll r2, ll m2, bool &ok) {
    ll p, q;
    ll g = extended_gcd(m1, m2, p, q); // p is the coefficient of m1, which is what we step by
    ll diff = r2 - r1;
    if (diff % g != 0) {
        ok = false;
        return {0, 0};
    }
    ll lcm = m1 / g * m2;
    // step m1 forward by (diff / g) * p, reduced so the multiplication cannot overflow
    ll mul = (__int128)(diff / g) % (m2 / g) * (p % (m2 / g)) % (m2 / g);
    ll rem = (ll)(((__int128)m1 * mul + r1) % lcm);
    if (rem < 0) rem += lcm;
    ok = true;
    return {rem, lcm};
}

// the whole system; ok is false when any pair contradicts
pair<ll, ll> crt(const vector<ll> &rems, const vector<ll> &mods, bool &ok) {
    ok = true;
    if (rems.empty()) return {0, 1};
    ll r = ((rems[0] % mods[0]) + mods[0]) % mods[0], m = mods[0];
    for (size_t i = 1; i < rems.size(); i++) {
        bool step_ok;
        auto [nr, nm] = crt_pair(r, m, ((rems[i] % mods[i]) + mods[i]) % mods[i], mods[i], step_ok);
        if (!step_ok) {
            ok = false;
            return {0, 0};
        }
        r = nr, m = nm;
    }
    return {r, m};
}

// Standard problem: k congruences - print the smallest non-negative x and its modulus, or -1 if impossible
void solve() {
    int k;
    cin >> k;
    vector<ll> r(k), m(k);
    for (int i = 0; i < k; i++) cin >> r[i] >> m[i];
    bool ok;
    auto [rem, mod] = crt(r, m, ok);
    if (!ok)
        cout << -1 << '\n';
    else
        cout << rem << ' ' << mod << '\n';
}
