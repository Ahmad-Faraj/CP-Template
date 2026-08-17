// Hashed Deque: a double-mod hash of a string you can push and pop at either end, O(1) per operation.
// Use when: a window that grows and shrinks at both ends, or checking a two-ended buffer for a palindrome.
// Handles: push/pop at front and back, double mod, concatenating two deques' hashes. No random access.
// Time: init O(N) once | every deque operation O(1)
// Indexing: none; the deque holds the current content
// Note: call init() once before use. Hashes compare equal only at equal lengths.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 1e5 + 5;
const ll base1 = 31, base2 = 37, mod1 = 1e9 + 9, mod2 = 1e9 + 7;
ll pw1[N], pw2[N], inv_pw1[N], inv_pw2[N];

ll powmod(ll b, ll e, ll m) {
    ll r = 1;
    while (e > 0) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

void init() { // fills the power and inverse-power tables; call once
    pw1[0] = pw2[0] = inv_pw1[0] = inv_pw2[0] = 1;
    ll i1 = powmod(base1, mod1 - 2, mod1), i2 = powmod(base2, mod2 - 2, mod2);
    for (int i = 1; i < N; i++) {
        pw1[i] = pw1[i - 1] * base1 % mod1;
        pw2[i] = pw2[i - 1] * base2 % mod2;
        inv_pw1[i] = inv_pw1[i - 1] * i1 % mod1;
        inv_pw2[i] = inv_pw2[i - 1] * i2 % mod2;
    }
}

struct HashDeque {
    deque<char> d;
    ll h1 = 0, h2 = 0;
    int len = 0;

    HashDeque() = default;

    template <typename T> HashDeque(const T &v) { // build from a string or any char container
        for (char c : v) push_back(c);
    }

    void push_back(char c) {
        d.push_back(c);
        h1 = (h1 * base1 + (c - 'a' + 1)) % mod1;
        h2 = (h2 * base2 + (c - 'a' + 1)) % mod2;
        len++;
    }

    void pop_back() {
        if (!len) return;
        char c = d.back();
        h1 = (h1 - (c - 'a' + 1) % mod1 + mod1) % mod1 * inv_pw1[1] % mod1;
        h2 = (h2 - (c - 'a' + 1) % mod2 + mod2) % mod2 * inv_pw2[1] % mod2;
        d.pop_back();
        len--;
    }

    void push_front(char c) {
        d.push_front(c);
        h1 = (h1 + (ll)(c - 'a' + 1) * pw1[len]) % mod1;
        h2 = (h2 + (ll)(c - 'a' + 1) * pw2[len]) % mod2;
        len++;
    }

    void pop_front() {
        if (!len) return;
        char c = d.front();
        h1 = ((h1 - (ll)(c - 'a' + 1) * pw1[len - 1]) % mod1 + mod1) % mod1;
        h2 = ((h2 - (ll)(c - 'a' + 1) * pw2[len - 1]) % mod2 + mod2) % mod2;
        d.pop_front();
        len--;
    }

    pair<ll, ll> get_hash() const { return {h1, h2}; } // hash of the current content

    static pair<ll, ll> concat(const HashDeque &a, const HashDeque &b) { // hash of a's content then b's
        return {(a.h1 * pw1[b.len] + b.h1) % mod1, (a.h2 * pw2[b.len] + b.h2) % mod2};
    }
};

// Standard problem: push/pop characters at either end, reporting after each whether the content is a palindrome
void solve() {
    init();
    int q;
    cin >> q;
    HashDeque fwd, rev; // rev holds the same content reversed
    while (q--) {
        string op;
        cin >> op;
        if (op == "push_back") {
            char c;
            cin >> c;
            fwd.push_back(c), rev.push_front(c);
        } else if (op == "push_front") {
            char c;
            cin >> c;
            fwd.push_front(c), rev.push_back(c);
        } else if (op == "pop_back") {
            fwd.pop_back(), rev.pop_front();
        } else {
            fwd.pop_front(), rev.pop_back();
        }
        cout << (fwd.get_hash() == rev.get_hash() ? "YES" : "NO") << '\n';
    }
}
