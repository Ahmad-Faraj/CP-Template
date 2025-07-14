struct Trie {
    struct Node {
        Node* kids[26];
        int isEnd;
        Node() {
            isEnd = 0;
            memset(kids, 0, sizeof(kids));
        }
        #if !defined(ONLINE_JUDGE)
            ~Node() {
                for (auto child : kids)
                    if (child)
                        delete child;
            }
        #endif
    };

    Node* root;

    Trie() {root = new Node();}
    #if !defined(ONLINE_JUDGE)
        ~Trie() {delete root;}
    #endif

    int get_idx(char ch) {return ch - 'a';}

    void add(string word) {
        Node* curr = root;
        for (auto &ch : word) {
            if (!curr -> kids[get_idx(ch)])
                curr -> kids[get_idx(ch)] = new Node();
            curr = curr -> kids[get_idx(ch)];
        }
        curr->isEnd++;
    }

    void del(string word) {
        Node *curr = root;
        for (auto &ch : word) {
            if (!curr -> kids[get_idx(ch)])
                return;
            curr = curr -> kids[get_idx(ch)];
        }
        curr->isEnd--;
    }

    bool var(string word) {
        Node *curr = root;
        for (auto &ch : word) {
            if (!curr -> kids[get_idx(ch)])
                return false;
            curr = curr -> kids[get_idx(ch)];
        }
        return curr -> isEnd;
    }
};
