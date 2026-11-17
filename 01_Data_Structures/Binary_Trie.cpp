// Binary Trie: a multiset of non-negative integers answering max/min XOR and counting by XOR value.
// Use when: "max XOR of a pair", "max XOR subarray" (insert prefix XORs), "count v: x ^ v <= k".
// Usage: Binary_Trie<ll> t(40); t.insert(0); ... t.max_xor(x); t.count(x, k); t.erase(v);
// Handles: duplicates, insert, erase, search, max/min XOR, count <= k / > k, size.
// Time: every operation O(LOG)
// Indexing: none; bits LOG..0 are stored, so values must be in [0, 2^(LOG+1))
// Note: max_xor / min_xor need a non-empty trie. erase() asserts the value is present.
// Note: for subarray-XOR problems insert(0) first (the empty prefix). Recursion depth is only LOG.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = int> struct Binary_Trie {
    struct Node {
        int freq = 0; // stored values passing through here
        Node *child[2] = {nullptr, nullptr};
    };

    Node *root;
    int LOG, cnt = 0;

    Binary_Trie(int log = 31) : root(new Node()), LOG(log) {}

    bool get_bit(T x, int bit) const { return (x >> bit) & 1; }

    void insert(T x) { // add one copy of x
        ++cnt;
        Node *cur = root;
        for (int i = LOG; i >= 0; i--) {
            bool b = get_bit(x, i);
            if (!cur->child[b]) cur->child[b] = new Node();
            cur = cur->child[b];
            cur->freq++;
        }
    }

    void erase(T x) { // remove one copy of x; it must be present
        assert(search(x));
        erase(x, LOG, root);
    }

    bool search(T x) const { // is x stored
        Node *cur = root;
        for (int i = LOG; i >= 0; i--) {
            cur = cur->child[get_bit(x, i)];
            if (!cur) return false;
        }
        return true;
    }

    T max_xor(T x) const { // max of x ^ v over stored v
        T ans = 0;
        Node *cur = root;
        for (int i = LOG; i >= 0; i--) {
            bool b = get_bit(x, i);
            if (cur->child[!b]) ans |= T(1) << i, cur = cur->child[!b];
            else cur = cur->child[b];
        }
        return ans;
    }

    T min_xor(T x) const { // min of x ^ v over stored v
        T ans = 0;
        Node *cur = root;
        for (int i = LOG; i >= 0; i--) {
            bool b = get_bit(x, i);
            if (cur->child[b]) cur = cur->child[b];
            else ans |= T(1) << i, cur = cur->child[!b];
        }
        return ans;
    }

    int count(T x, T k) const { // how many stored v have (x ^ v) <= k
        if (k < 0) return 0;
        if (LOG + 1 < 8 * (int)sizeof(T) && (k >> (LOG + 1))) return cnt; // k above every value
        int ans = 0;
        Node *cur = root;
        for (int i = LOG; i >= 0 && cur; i--) {
            bool xb = get_bit(x, i), kb = get_bit(k, i);
            if (kb) {
                if (cur->child[xb]) ans += cur->child[xb]->freq; // this bit equal: x ^ v < k
                cur = cur->child[!xb];
            } else cur = cur->child[xb];
        }
        if (cur) ans += cur->freq; // x ^ v == k
        return ans;
    }

    int count_greater(T x, T k) const { return cnt - count(x, k); } // stored v with (x ^ v) > k
    int size() const { return cnt; }

  private:
    void erase(T x, int bit, Node *cur) { // nodes whose count drops to 0 are freed
        if (bit < 0) return void(--cnt);
        bool b = get_bit(x, bit);
        erase(x, bit - 1, cur->child[b]);
        if (--cur->child[b]->freq == 0) {
            delete cur->child[b];
            cur->child[b] = nullptr;
        }
    }
};
