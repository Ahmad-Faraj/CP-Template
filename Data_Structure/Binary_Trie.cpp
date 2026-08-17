// Binary Trie: a multiset of integers answering max/min XOR against a query, and counting by XOR value.
// Use when: "maximum XOR of a pair", "count i with (x ^ a[i]) <= k", XOR queries over a set that changes.
// Handles: insert, erase, search, max/min XOR, count <= k and count > k. Values must fit in LOG + 1 bits.
// Time: every operation O(LOG)
// Indexing: none; LOG defaults to 31 - raise it for values past 2^31
// Note: erase() asserts the value is present. Set LOG to your value range or the high bits are silently dropped.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = int> class Binary_Trie {
  public:
    Binary_Trie(int log = 31) : root(new Node()), LOG(log), cnt(0) {}

    void insert(const T x) { // add one copy of x
        ++cnt;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            if (!cur->child[bit]) cur->child[bit] = new Node();
            cur = cur->child[bit];
            cur->freq++;
        }
    }

    void erase(const T x) { // remove one copy of x; it must be present
        assert(search(x));
        erase(x, LOG, root);
    }

    bool search(const T x) { // is x in the trie
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            if (!cur->child[bit] or !cur->child[bit]->freq) return false;
            cur = cur->child[bit];
        }
        return true;
    }

    T max_xor(const T x) { // maximum of x ^ v over every v in the trie
        T ans = 0;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            if (cur->child[!bit] and cur->child[!bit]->freq)
                ans |= (T(1) << i), cur = cur->child[!bit];
            else
                cur = cur->child[bit];
        }
        return ans;
    }

    T min_xor(const T x) { // minimum of x ^ v over every v in the trie
        T ans = 0;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            if (cur->child[bit] and cur->child[bit]->freq)
                cur = cur->child[bit];
            else
                ans |= (T(1) << i), cur = cur->child[!bit];
        }
        return ans;
    }

    T count(const T x, const T k) { return count_less_equal(x, k); }            // how many v have x ^ v <= k
    T count_greater(const T x, const T k) { return cnt - count_less_equal(x, k); } // how many v have x ^ v > k
    int size() const { return cnt; }

  private:
    struct Node {
        int freq;
        Node *child[2];
        Node() : freq(0), child{nullptr, nullptr} {}
    };

    Node *root;
    int LOG, cnt;

    bool get_bit(const T x, const int bit) const { return (x >> bit) & 1; }

    void erase(const T x, const int bit, Node *curr) {
        if (bit < 0) return void(--cnt);
        bool bit_val = get_bit(x, bit);
        erase(x, bit - 1, curr->child[bit_val]);
        if (--curr->child[bit_val]->freq == 0) {
            delete curr->child[bit_val];
            curr->child[bit_val] = nullptr;
        }
    }

    T count_less_equal(const T x, const T k) {
        T ans = 0;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            if (!cur) break;
            bool val_bit = get_bit(x, i), k_bit = get_bit(k, i);
            if (k_bit) {
                if (cur->child[val_bit]) ans += cur->child[val_bit]->freq;
                cur = cur->child[val_bit ^ 1];
            } else
                cur = cur->child[val_bit];
        }
        if (cur) ans += cur->freq;
        return ans;
    }
};

// Standard problem: maximum XOR of any two elements of the array
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    Binary_Trie<int> trie;
    int best = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        if (i) best = max(best, trie.max_xor(a[i]));
        trie.insert(a[i]);
    }
    cout << best << '\n';
}
