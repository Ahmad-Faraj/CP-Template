// Trie: a multiset of strings supporting insert, erase, exact search and prefix queries.
// Use when: "how many words start with this prefix", dictionary lookups, autocomplete, prefix-driven DP.
// Handles: insert, erase, exact match, prefix test, prefix count. Mode selects lowercase / uppercase / digits.
// Time: every operation O(length of the word)
// Indexing: none
// Note: erase() removes one occurrence and only if the word is present. Set Mode to match your alphabet.

#include <bits/stdc++.h>
using namespace std;

template <int Mode = 0> struct Trie {
    // Mode: 0 = lowercase, 1 = uppercase, 2 = digits
    static constexpr int ALPHA[3] = {26, 26, 10};

    struct Node {
        Node *child[ALPHA[Mode]];
        int words; // stored words ending exactly here
        int freq;  // stored words passing through here

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

// Standard problem: n words, then q queries - 1 insert, 2 erase, 3 exact search, 4 count words with that prefix
void solve() {
    int n, q;
    cin >> n >> q;
    Trie<> trie;
    for (int i = 0; i < n; i++) {
        string w;
        cin >> w;
        trie.insert(w);
    }
    while (q--) {
        int type;
        string w;
        cin >> type >> w;
        if (type == 1)
            trie.insert(w);
        else if (type == 2)
            trie.erase(w);
        else if (type == 3)
            cout << (trie.search(w) ? "YES" : "NO") << '\n';
        else
            cout << trie.count_prefix(w) << '\n';
    }
}
