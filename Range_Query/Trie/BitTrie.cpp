/* Topic: Bit Trie
 * Description: Bit Trie is specifically designed for binary representations of numbers.
 *   Commonly used to find the maximum or minimum XOR of a number with a dynamic set
 *   of elements.
 * Usage: BinaryTrie tr; tr.insert(x); tr.query_max(x); tr.query_min(x); tr.remove(x);
 */
#include "../../core.h"
const int BITS = 30;

struct BinaryTrie struct BinaryTrie {
    struct TrieNode {
        TrieNode* child[2];
        int cnt;

        TrieNode() : cnt(0) { child[0] = child[1] = nullptr; }

        ~TrieNode() {
            if (child[0]) delete child[0];
            if (child[1]) delete child[1];
        }
    };

    TrieNode* root;

    BinaryTrie() { root = new TrieNode(); }

    ~BinaryTrie() { delete root; }

    void insert(int num) {
        // Time Complexity: O(BITS)
        // Space Complexity: O(BITS)
        TrieNode* cur = root;
        for (int i = BITS; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (!cur->child[bit]) { cur->child[bit] = new TrieNode(); }
            cur = cur->child[bit];
            cur->cnt++;
        }
    }

    void remove(int num) {
        // Time Complexity: O(BITS)
        TrieNode* cur = root;
        for (int i = BITS; i >= 0; i--) {
            int bit = (num >> i) & 1;
            TrieNode* next = cur->child[bit];
            if (next) {
                next->cnt--;
                cur = next;
            } else
                break;
        }
    }

    int query_max(int num) {
        // Time Complexity: O(BITS)
        TrieNode* cur = root;
        int ans = 0;
        for (int i = BITS; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int toggled = bit ^ 1;
            if (cur->child[toggled] && cur->child[toggled]->cnt > 0) {
                ans |= (1LL << i);
                cur = cur->child[toggled];
            } else if (cur->child[bit] && cur->child[bit]->cnt > 0) {
                cur = cur->child[bit];
            } else {
                break;
            }
        }
        return ans;
    }

    int query_min(int num) {
        // Time Complexity: O(BITS)
        TrieNode* cur = root;
        int ans = 0;
        for (int i = BITS; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (cur->child[bit] && cur->child[bit]->cnt > 0) {
                cur = cur->child[bit];
            } else if (cur->child[bit ^ 1] && cur->child[bit ^ 1]->cnt > 0) {
                ans |= (1LL << i);
                cur = cur->child[bit ^ 1];
            } else {
                break;
            }
        }
        return ans;
    }
};
