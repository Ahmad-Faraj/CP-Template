struct Trie {
    struct Node {
        Node* kids[2];
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

    void add(int num) {
        Node *curr = root;
        for (int i = 31; i >= 0; i--) {
            if (!curr -> kids[!!(num & (1 << i))])
                curr -> kids[!!(num & (1 << i))] = new Node();
            curr = curr -> kids[!!(num & (1 << i))];
        }
        curr -> isEnd ++;
    }

    void del(int num) {
        Node *curr = root;
        for (int i = 31; i >= 0; i--) {
            if (!curr -> kids[!!(num & (1 << i))])
                return; // Number not found, So I can't delete
            curr = curr -> kids[!!(num & (1 << i))];
        }
        curr -> isEnd --;
    }

    bool var(int num) {
        Node *curr = root;
        for (int i = 31; i >= 0; i--) {
            if (!curr -> kids[!!(num & (1 << i))])
                return false;
            curr = curr -> kids[!!(num & (1 << i))];
        }
        return curr -> isEnd;
    }
};
