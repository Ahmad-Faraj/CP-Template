// Trie: a multiset of strings supporting insert, erase, exact search and prefix queries.
// Use when: "how many words start with this prefix", dictionary lookups, autocomplete, prefix-
//           driven DP.
// Usage: Trie<> t; t.insert(s); t.erase(s); t.search(s); t.is_prefix(p); t.count_prefix(p);
// Handles: insert, erase, exact match, prefix test, prefix count. Mode selects lowercase /
//          uppercase / digits.
// Time: every operation O(length of the word)
// Indexing: none
// Note: erase() removes one occurrence and only if the word is present. Set Mode to match your
//       alphabet. count_prefix("") returns 0, not the word count. Nodes are never freed except
//       by erase.

#include <bits/stdc++.h>
using namespace std;

template <int Mode = 0> struct Trie {
    // Mode: 0 = lowercase, 1 = uppercase, 2 = digits
    static constexpr int ALPHA[3] = {26, 26, 10};

    struct Node {
        Node *child[ALPHA[Mode]];
        int words;
        int freq;

        Node() {
            memset(child, 0, sizeof(child));
            words = 0;
            freq = 0;
        }
    };

    Node *root;
    char DEFAULT;

    Trie() {
        root = new Node;
        DEFAULT = "aA0"[Mode];
    }

    void insert(const string &word) {
        Node *curr = root;
        for (char c : word) {
            if (!curr->child[c - DEFAULT]) curr->child[c - DEFAULT] = new Node;
            curr = curr->child[c - DEFAULT];
            curr->freq++;
        }
        curr->words++;
    }

    bool search(const string &word) { // is word stored in the trie
        Node *curr = walk(word);
        return curr && curr->words > 0;
    }

    bool is_prefix(const string &word) { // does any stored word start with word
        return walk(word) != nullptr;
    }

    int count_prefix(const string &word) { // how many stored words start with word
        Node *curr = walk(word);
        return curr ? curr->freq : 0;
    }

    void erase(const string &word) { // remove one occurrence, if present
        if (search(word)) erase(word, 0, root);
    }

  private:
    Node *walk(const string &word) {
        Node *curr = root;
        for (char c : word) {
            if (!curr->child[c - DEFAULT]) return nullptr;
            curr = curr->child[c - DEFAULT];
        }
        return curr;
    }

    void erase(const string &word, int idx, Node *curr) {
        if (idx == (int)word.size()) return void(--curr->words);
        erase(word, idx + 1, curr->child[word[idx] - DEFAULT]);
        if (--curr->child[word[idx] - DEFAULT]->freq == 0) {
            delete curr->child[word[idx] - DEFAULT];
            curr->child[word[idx] - DEFAULT] = nullptr;
        }
    }
};

template <int Mode> constexpr int Trie<Mode>::ALPHA[3];
