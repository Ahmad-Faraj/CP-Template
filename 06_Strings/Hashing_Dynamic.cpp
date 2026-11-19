// Dynamic_Hashing: double-mod hash of a string you push and pop at both ends, O(1) per operation.
// Use when: a window that grows/shrinks at both ends, deque palindrome checks (keep a reversed
// copy), building strings char by char and comparing them, hash of a concatenation of two of them.
// Usage: Dynamic_Hashing h("abc"); h.push_front('z'); h.pop_back(); h.get_Hash() == g.get_Hash();
// Handles: push/pop at front and back, merge(a, b) = hash of a's content followed by b's.
// Time: O(1) amortized per operation
// Indexing: none; the deque `hash` holds the current content
// Note: lowercase 'a'..'z' only (value c - 'a' + 1). Other alphabets: map to 1..30 first.
// Pop on an empty hash does nothing. Bases are fixed (31, 313), so anti-hash tests are possible.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Dynamic_Hashing {
    static constexpr ll P[2] = {31, 313}, INV[2] = {129032259, 415335467}; // INV = P^-1 mod M
    static constexpr ll M[2] = {1000000007, 1000000009};

    ll h1 = 0, h2 = 0, len = 0;
    deque<char> hash;

    static ll pw(int k, ll i) {
        static vector<ll> p[2] = {{1}, {1}};
        while ((ll)p[k].size() <= i) p[k].push_back(p[k].back() * P[k] % M[k]);
        return p[k][i];
    }
    static ll get(char c) { return c - 'a' + 1; }

    Dynamic_Hashing() {}
    template <typename T> Dynamic_Hashing(const T &v) { // a string or any char container
        for (char c : v) push_back(c);
    }

    void push_back(char c) {
        len++, hash.push_back(c);
        h1 = (h1 * P[0] + get(c)) % M[0];
        h2 = (h2 * P[1] + get(c)) % M[1];
    }

    void pop_back() {
        if (len == 0) return;
        ll c = get(hash.back());
        h1 = (h1 - c + M[0]) % M[0] * INV[0] % M[0];
        h2 = (h2 - c + M[1]) % M[1] * INV[1] % M[1];
        len--, hash.pop_back();
    }

    void push_front(char c) {
        hash.push_front(c);
        h1 = (h1 + get(c) * pw(0, len)) % M[0];
        h2 = (h2 + get(c) * pw(1, len)) % M[1];
        len++;
    }

    void pop_front() {
        if (len == 0) return;
        len--;
        ll c = get(hash.front());
        h1 = (h1 - c * pw(0, len) % M[0] + M[0]) % M[0];
        h2 = (h2 - c * pw(1, len) % M[1] + M[1]) % M[1];
        hash.pop_front();
    }

    pair<ll, ll> get_Hash() const { return {h1, h2}; }

    static pair<ll, ll> merge(const Dynamic_Hashing &prefix, const Dynamic_Hashing &suffix) {
        return {(prefix.h1 * pw(0, suffix.len) + suffix.h1) % M[0],
                (prefix.h2 * pw(1, suffix.len) + suffix.h2) % M[1]};
    }
};
