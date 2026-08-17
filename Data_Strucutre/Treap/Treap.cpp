#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'

/*
    [1] Definition
    Treap = Randomized Balanced Binary Search Tree.

    Each node has:
        - BST property by value.
        - Heap property by random priority.

    Supports:
        - Insert
        - Erase
        - K-th smallest
        - Lower bound - count of numbers < k
        - Upper bound - count of numbers <= k
        - Frequency of duplicate values

    [2] Complexity
    Expected:
        Build / Insert / Erase : O(log N)
        K-th                  : O(log N)
        Lower Bound           : O(log N)
        Upper Bound           : O(log N)

    [3] Notes
    - Duplicates are stored using frq.
    - kth() is 0-indexed internally.
    - lower_bound() returns the number of elements < x.
    - upper_bound() returns the number of elements <= x.
*/

mt19937 eng(0);
uniform_int_distribution<int> rnd(INT_MIN + 1, INT_MAX);
template <typename T> class Treap {
  private:
    enum DIR { L, R };

    struct Node {
        T val;
        int pri, size, frq;

        Node *ch[2];
        Node *par;

        Node() : pri(INT_MIN), size(0), frq(0), ch{this, this}, par(this) {
        }

        Node(const T &val) : val(val), pri(rnd(eng)), size(1), frq(1), ch{EMPTY, EMPTY}, par(EMPTY) {
        }

        void update() {
            size = ch[L]->size + frq + ch[R]->size;
        }
    };

    static Node *EMPTY;

    Node *root = EMPTY;

    void link(Node *p, Node *c, int dir) {
        if (c != EMPTY) c->par = p;

        if (p != EMPTY) {
            p->ch[dir] = c;
            p->update();
        }
    }

    int getDir(Node *p, Node *c) {
        return p->ch[R] == c;
    }

    void rotate(Node *q) {
        Node *p = q->par;
        int d = getDir(p, q);

        Node *gp = p->par;
        int gd = getDir(gp, p);

        Node *b = q->ch[!d];

        link(p, b, d);
        link(q, p, !d);
        link(gp, q, gd);
    }

    Node *balance(Node *root, int dir) {
        if (root->ch[dir]->pri > root->pri) {
            Node *tmp = root->ch[dir];
            rotate(tmp);
            return tmp;
        }

        return root;
    }

    Node *insert(Node *root, const T &val) {
        if (root == EMPTY) return new Node(val);

        if (root->val == val) {
            root->frq++;
            root->update();
            return root;
        }

        int dir = val > root->val;

        link(root, insert(root->ch[dir], val), dir);

        return balance(root, dir);
    }

    Node *erase(Node *root, const T &val) {
        if (root == EMPTY) return EMPTY;

        if (root->val == val) {
            if (root->frq > 1) {
                root->frq--;
                root->update();
                return root;
            }

            for (int dir = 0; dir < 2; dir++) {
                if (root->ch[dir] == EMPTY) {
                    Node *child = root->ch[!dir];

                    child->par = EMPTY;

                    delete root;

                    return child;
                }
            }

            // Both children exist.
            // Rotate the child with the larger priority up.
            int dir = 0;

            if (root->ch[R]->pri > root->ch[L]->pri) dir = R;

            Node *child = root->ch[dir];

            rotate(child);

            link(child, erase(child->ch[!dir], val), !dir);

            return child;
        }

        int dir = val > root->val;

        link(root, erase(root->ch[dir], val), dir);

        return root;
    }

    // 0-indexed.
    T kth(Node *root, int k) {
        if (root == EMPTY) return T();

        int leftSize = root->ch[L]->size;

        if (k < leftSize) return kth(root->ch[L], k);

        if (k < leftSize + root->frq) return root->val;

        return kth(root->ch[R], k - leftSize - root->frq);
    }

    void clear(Node *root) {
        if (root == EMPTY) return;

        clear(root->ch[L]);
        clear(root->ch[R]);

        delete root;
    }

    void print(Node *root, int depth = 0) {
        if (root == EMPTY) return;

        print(root->ch[L], depth + 1);

        cout << root->val << ' ' << root->pri << ' ' << root->frq << ' ' << depth << nl;

        print(root->ch[R], depth + 1);
    }

    int lower_bound(Node *root, const T &val) {
        if (root == EMPTY) return 0;

        if (val <= root->val) return lower_bound(root->ch[L], val);

        return root->ch[L]->size + root->frq + lower_bound(root->ch[R], val);
    }

    int upper_bound(Node *root, const T &val) {
        if (root == EMPTY) return 0;

        if (val < root->val) return upper_bound(root->ch[L], val);

        return root->ch[L]->size + root->frq + upper_bound(root->ch[R], val);
    }

  public:
    void insert(const T &val) {
        root = insert(root, val);
        root->par = EMPTY;
    }

    void erase(const T &val) {
        root = erase(root, val);
        root->par = EMPTY;
    }

    // 0-based indexing kth smallest
    T kth(int k) {
        return kth(root, k);
    }

    // count of numbers < val
    int lower_bound(const T &val) {
        return lower_bound(root, val);
    }

    // count of numbers <= val
    int upper_bound(const T &val) {
        return upper_bound(root, val);
    }

    bool isExist(int x) {
        return lower_bound(x) != upper_bound(x);
    }

    int size() {
        return root->size;
    }

    bool empty() {
        return root == EMPTY;
    }

    // testing
    void print() {
        print(root);
    }

    void clear() {
        clear(root);
        root = EMPTY;
    }

    ~Treap() {
        clear(root);

        if (EMPTY != nullptr) {
            delete EMPTY;
            EMPTY = nullptr;
        }
    }
};

template <typename T> typename Treap<T>::Node *Treap<T>::EMPTY = new typename Treap<T>::Node();

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;

    Treap<int> tp;

    while (q--) {
        char op;
        int x;

        cin >> op >> x;

        if (op == 'I') {
            // Insert only if x doesn't already exist.
            if (!tp.isExist(x)) tp.insert(x);
        } else if (op == 'D') {
            tp.erase(x);
        } else if (op == 'K') {
            // K-th smallest is 1-indexed in the query.
            if (x <= 0 || x > tp.size())
                cout << "invalid" << nl;
            else
                cout << tp.kth(x - 1) << nl;
        } else if (op == 'C') {
            // Count elements strictly smaller than x.
            cout << tp.lower_bound(x) << nl;
        }
    }
}