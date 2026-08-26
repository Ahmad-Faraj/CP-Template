#include "../../core.h"

/*
 * Topic: Data Structures - Binary Trie (Pointer-based)
 * Description: Advanced Binary Trie with pointers. Supports insert, erase, search,
 *              kth smallest/largest, count in range, max/min XOR/OR in range.
 */

struct BinaryTrie_pointer {
    struct Node {
        int freq;
        Node *child[2];

        Node() : freq(0), child{nullptr, nullptr} {}
    };

    Node *root;
    int LOG, cnt;

    inline bool get_bit(const ll x, const int bit) const { return (x >> bit) & 1; }

    inline ll count_in_range(const ll l, const ll r) {
        if (l > r) return 0;
        ll right_cnt = count(0, r);
        ll left_cnt = (l == 0) ? 0 : count(0, l - 1);
        return right_cnt - left_cnt;
    }

    inline bool check_branch(Node *cur, bool b, ll prefix, int i, ll l, ll r) {
        if (!cur->child[b] or !cur->child[b]->freq) return false;
        ll P_min = prefix | ((ll)(b) << i);
        ll P_max = P_min | ((1LL << i) - 1);
        ll L = max(l, P_min);
        ll R = min(r, P_max);
        if (L > R) return false;
        if (L <= P_min and R >= P_max) return true;
        return count_in_range(L, R) > 0;
    }

    void clear(Node *node) {
        if (!node) return;
        clear(node->child[0]);
        clear(node->child[1]);
        delete node;
    }

    BinaryTrie_pointer(int log = 60) : root(new Node()), LOG(log), cnt(0) {}

    ~BinaryTrie_pointer() { clear(root); }

    // Inserts the value `x` into the Binary Trie.
    inline void insert(const ll x) {
        ++cnt;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            if (!cur->child[bit]) cur->child[bit] = new Node();
            cur = cur->child[bit];
            cur->freq++;
        }
    }

    // Erases one occurrence of the value `x` from the Binary Trie.
    inline void erase(const ll x) {
        if (!search(x)) return;
        --cnt;
        Node *path[65];
        path[LOG + 1] = root;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            cur = cur->child[bit];
            path[i] = cur;
        }
        for (int i = 0; i <= LOG; i++) {
            bool bit = get_bit(x, i);
            Node *current = path[i];
            Node *parent = path[i + 1];
            if (--current->freq == 0) {
                delete current;
                parent->child[bit] = nullptr;
            }
        }
    }

    // Searches for the presence of the value `x` in the Binary Trie.
    inline bool search(const ll x) {
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            if (!cur->child[bit] or !cur->child[bit]->freq) return false;
            cur = cur->child[bit];
        }
        return true;
    }

    // Returns the total number of elements currently stored in the trie.
    inline ll size() { return cnt; }

    // Counts the number of elements `p` in the trie such that (p ^ x) <= k.
    inline ll count(const ll x, const ll k) {
        ll ans = 0;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            if (!cur) break;
            bool val_bit = get_bit(x, i), k_bit = get_bit(k, i);
            if (k_bit) {
                if (cur->child[val_bit]) ans += cur->child[val_bit]->freq;
                cur = cur->child[val_bit ^ 1];
            } else {
                cur = cur->child[val_bit];
            }
        }
        if (cur) ans += cur->freq;
        return ans;
    }

    // Counts the number of elements `p` in the trie such that (p ^ x) > k.
    inline ll count_greater(const ll x, const ll k) { return cnt - count(x, k); }

    // Finds the k-th smallest element (1-based index).
    inline ll kth_min(int k) {
        assert(k > 0 && k <= cnt);
        Node *cur = root;
        ll ans = 0;
        for (int i = LOG; ~i; i--) {
            int left_count = cur->child[0] ? cur->child[0]->freq : 0;
            if (k <= left_count) {
                cur = cur->child[0];
            } else {
                ans |= (1LL << i);
                k -= left_count;
                cur = cur->child[1];
            }
        }
        return ans;
    }

    // Finds the k-th largest element (1-based index).
    inline ll kth_max(int k) { return kth_min(cnt - k + 1); }

    // Finds max XOR of `x` with any element `p` in range [l, r]. Returns -1 if none.
    inline ll max_xor(const ll x, const ll l = 0, const ll r = INF) {
        ll ans = 0, prefix = 0;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            bool target = !bit;

            if (check_branch(cur, target, prefix, i, l, r)) {
                ans |= (1LL << i);
                prefix |= ((ll)(target) << i);
                cur = cur->child[target];
            } else if (check_branch(cur, !target, prefix, i, l, r)) {
                prefix |= ((ll)(!target) << i);
                cur = cur->child[!target];
            } else
                return -1;
        }
        return ans;
    }

    // Finds min XOR of `x` with any element `p` in range [l, r]. Returns -1 if none.
    inline ll min_xor(const ll x, const ll l = 0, const ll r = INF) {
        ll ans = 0, prefix = 0;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool bit = get_bit(x, i);
            bool target = bit;

            if (check_branch(cur, target, prefix, i, l, r)) {
                prefix |= ((ll)(target) << i);
                cur = cur->child[target];
            } else if (check_branch(cur, !target, prefix, i, l, r)) {
                ans |= (1LL << i);
                prefix |= ((ll)(!target) << i);
                cur = cur->child[!target];
            } else
                return -1;
        }
        return ans;
    }

    // Finds max OR of `x` with any element `p` in range [l, r]. Returns -1 if none.
    inline ll max_or(const ll x, const ll l = 0, const ll r = INF) {
        ll ans = 0, prefix = 0;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool xb = get_bit(x, i);

            if (xb == 0) {
                if (check_branch(cur, 1, prefix, i, l, r)) {
                    ans |= (1LL << i);
                    prefix |= (1LL << i);
                    cur = cur->child[1];
                } else if (check_branch(cur, 0, prefix, i, l, r)) {
                    cur = cur->child[0];
                } else
                    return -1;
            } else {
                ans |= (1LL << i);
                if (check_branch(cur, 1, prefix, i, l, r)) {
                    prefix |= (1LL << i);
                    cur = cur->child[1];
                } else if (check_branch(cur, 0, prefix, i, l, r)) {
                    cur = cur->child[0];
                } else
                    return -1;
            }
        }
        return ans;
    }

    // Finds min OR of `x` with any element `p` in range [l, r]. Returns -1 if none.
    inline ll min_or(const ll x, const ll l = 0, const ll r = INF) {
        ll ans = 0, prefix = 0;
        Node *cur = root;
        for (int i = LOG; ~i; i--) {
            bool xb = get_bit(x, i);

            if (xb == 1) {
                ans |= (1LL << i);
                if (check_branch(cur, 0, prefix, i, l, r)) {
                    cur = cur->child[0];
                } else if (check_branch(cur, 1, prefix, i, l, r)) {
                    prefix |= (1LL << i);
                    cur = cur->child[1];
                } else
                    return -1;
            } else {
                if (check_branch(cur, 0, prefix, i, l, r)) {
                    cur = cur->child[0];
                } else if (check_branch(cur, 1, prefix, i, l, r)) {
                    ans |= (1LL << i);
                    prefix |= (1LL << i);
                    cur = cur->child[1];
                } else
                    return -1;
            }
        }
        return ans;
    }
};
