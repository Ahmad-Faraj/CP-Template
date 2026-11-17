template <int Mode = 0> struct Trie {
    // Mode [lowercase, uppercase, digits]
    static constexpr int sz[4] = {26, 26, 10};

    struct Node {

        Node *child[sz[Mode]];
        bool is_word;
        int freq;

        Node() {
            memset(child, 0, sizeof(child));
            is_word = false;
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
        for (auto &c : word) {
            if (!curr->child[c - DEFAULT]) curr->child[c - DEFAULT] = new Node;
            curr = curr->child[c - DEFAULT];
            curr->freq++;
        }
        curr->is_word = true;
    }

    void erase(const string &word, int idx, Node *curr) {
        if (idx == sz(word)) return void(curr->is_word = curr->freq > 1);
        erase(word, idx + 1, curr->child[word[idx] - DEFAULT]);
        if (--curr->child[word[idx] - DEFAULT]->freq == 0) {
            delete curr->child[word[idx] - DEFAULT];
            curr->child[word[idx] - DEFAULT] = nullptr;
        }
    }

    bool search(const string &word) {
        Node *curr = root;
        for (auto &c : word) {
            if (!curr->child[c - DEFAULT]) return false;
            curr = curr->child[c - DEFAULT];
        }
        return curr->is_word;
    }

    void erase(const string &word) {
        if (search(word)) erase(word, 0, root);
    }

    bool is_prefix(const string &word) {
        Node *curr = root;
        for (auto &c : word) {
            if (!curr->child[c - DEFAULT]) return false;
            curr = curr->child[c - DEFAULT];
        }
        return true;
    }
};
