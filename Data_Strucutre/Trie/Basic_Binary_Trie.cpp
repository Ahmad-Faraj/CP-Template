#include "../../core.h"

/*
 * Topic: Data Structures - Binary Trie
 * Description: A Trie for binary representations of numbers. Supports fast XOR 
 *              operations, finding max/min XOR, and bitwise manipulations.
 */

struct BinaryTrie {
    const int LOG = 60;

    struct Node {
        Node *child[2];
        int freq;
        Node() {
            memset(child, 0, sizeof(child));
            freq = 0;
        }
    };

    Node *root;
    BinaryTrie() {
        root = new Node;
        insert(0);
    }

    void insert(const ll n) {
        Node *curr = root;
        for (int i = LOG; i >= 0; i--) {
            int bit = (n >> i) & 1;
            if (!curr->child[bit]) curr->child[bit] = new Node;
            curr = curr->child[bit];
            curr->freq++;
        }
    }

    void erase(const ll n, int idx, Node *curr) {
        if (idx == -1) {
            return;
        }
        int bit = (n >> idx) & 1;
        erase(n, idx - 1, curr->child[bit]);
        curr->child[bit]->freq--;
        if (curr->child[bit]->freq == 0) {
            delete curr->child[bit];
            curr->child[bit] = 0;
        }
    }
    void erase(const ll n) { erase(n, LOG, root); }
    ll max_xor(const ll n) {
        Node *curr = root;
        ll ans = 0;
        for (int i = LOG; i >= 0; i--) {
            int bit = (n >> i) & 1;
            if (curr->child[!bit]) {
                ans |= (1LL << i);
                curr = curr->child[!bit];
            } else {
                curr = curr->child[bit];
            }
        }
        return ans;
    }
    ll min_xor(const ll n) {
        Node *curr = root;
        ll ans = 0;
        for (int i = LOG; i >= 0; i--) {
            int bit = (n >> i) & 1;
            if (curr->child[bit]) {
                curr = curr->child[bit];
            } else {
                ans |= (1LL << i);
                curr = curr->child[!bit];
            }
        }
        return ans;
    }

    inline bool search(const ll x) {
        Node* cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = (x >> i) & 1;
            if (!cur->child[bit] or !cur->child[bit]->freq) return false;
            cur = cur->child[bit];
        }
        return true;
    }
};
