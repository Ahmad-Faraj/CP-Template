#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'

/*
    [1] Definition
    Implicit Treap = Randomized Balanced Binary Search Tree
    where the key is implicit and determined by the subtree size.

    Unlike a normal Treap:
        - There is no explicit key.
        - The position of a node is determined by its left subtree size.

    Supports:
        - Insert at position
        - Erase at position
        - Get / Set
        - Split / Merge
        - Reverse a range
        - Range Sum Query

    [2] Complexity
    Expected:
        Insert           : O(log N)
        Erase            : O(log N)
        Get              : O(log N)
        Set              : O(log N)
        Split            : O(log N)
        Merge            : O(log N)
        Reverse          : O(log N)
        Range Query      : O(log N)

    [3] Notes
    - All indices are 0-based.
    - Priority maintains the heap property.
    - Subtree size determines the implicit position.
    - lazy is used for range reversal.
*/

mt19937 eng(0);
uniform_int_distribution<int> rnd(INT_MIN + 1, INT_MAX);

template <typename T> class ImplicitTreap {
  private:
    struct Node {
        T val;
        T sum;

        int pri;
        int size;

        bool lazy;

        Node *ch[2];

        Node() : pri(INT_MIN), size(0), lazy(false), ch{this, this} {}

        Node(const T &val) : val(val), sum(val), pri(rnd(eng)), size(1), lazy(false), ch{EMPTY, EMPTY} {}

        // customize your query here
        void update() {
            size = ch[0]->size + 1 + ch[1]->size;
            sum = ch[0]->sum + val + ch[1]->sum;
        }

        void push() {
            if (!lazy) return;

            swap(ch[0], ch[1]);

            if (ch[0] != EMPTY) ch[0]->lazy ^= 1;

            if (ch[1] != EMPTY) ch[1]->lazy ^= 1;

            lazy = false;
        }
    };

    static Node *EMPTY;

    Node *root = EMPTY;

    int size(Node *root) {
        return root->size;
    }

    T sum(Node *root) {
        return root->sum;
    }

    void update(Node *root) {
        if (root != EMPTY) root->update();
    }

    void push(Node *root) {
        if (root != EMPTY) root->push();
    }

    Node *merge(Node *a, Node *b) {
        if (a == EMPTY) return b;

        if (b == EMPTY) return a;

        if (a->pri > b->pri) {
            push(a);

            a->ch[1] = merge(a->ch[1], b);

            update(a);

            return a;
        } else {
            push(b);

            b->ch[0] = merge(a, b->ch[0]);

            update(b);

            return b;
        }
    }

    // Split first k elements into a and b.
    void split(Node *root, int k, Node *&a, Node *&b) {
        if (root == EMPTY) {
            a = b = EMPTY;
            return;
        }

        push(root);

        int leftSize = root->ch[0]->size;

        if (k <= leftSize) {
            split(root->ch[0], k, a, root->ch[0]);
            b = root;
            update(b);
        } else {
            split(root->ch[1], k - leftSize - 1, root->ch[1], b);
            a = root;
            update(a);
        }
    }

    T get(Node *root, int pos) {
        push(root);

        int leftSize = root->ch[0]->size;

        if (pos < leftSize) return get(root->ch[0], pos);

        if (pos == leftSize) return root->val;

        return get(root->ch[1], pos - leftSize - 1);
    }

    void set(Node *root, int pos, const T &val) {
        push(root);

        int leftSize = root->ch[0]->size;

        if (pos < leftSize) {
            set(root->ch[0], pos, val);
        } else if (pos == leftSize) {
            root->val = val;
        } else {
            set(root->ch[1], pos - leftSize - 1, val);
        }

        update(root);
    }

    void reverse(Node *root) {
        if (root == EMPTY) return;

        root->lazy ^= 1;
    }

    T query(Node *root, int l, int r) {
        if (root == EMPTY || l > r) return T();

        Node *a;
        Node *b;
        Node *c;

        split(root, l, a, b);
        split(b, r - l + 1, b, c);

        T ans = b->sum;

        root = merge(a, merge(b, c));

        return ans;
    }

    void clear(Node *root) {
        if (root == EMPTY) return;

        clear(root->ch[0]);
        clear(root->ch[1]);

        delete root;
    }

    void print(Node *root) {
        if (root == EMPTY) return;

        push(root);

        print(root->ch[0]);

        cout << root->val << ' ';

        print(root->ch[1]);
    }

  public:
    ImplicitTreap() {
    }

    // split treap to two parts a contains first k element and b contains the remaining elements
    void split(int k, ImplicitTreap &left, ImplicitTreap &right) {
        split(root, k, left.root, right.root);

        root = EMPTY;
    }

    // insert with idx - 0-based indexing
    void insert(int pos, const T &val) {
        Node *a;
        Node *b;

        split(root, pos, a, b);

        root = merge(merge(a, new Node(val)), b);
    }

    // erase by idx - 0-based indexing
    void erase(int pos) {
        Node *a;
        Node *b;
        Node *c;

        split(root, pos, a, b);
        split(b, 1, b, c);

        delete b;

        root = merge(a, c);
    }

    // get element by idx - 0-based indexing
    T get(int pos) {
        return get(root, pos);
    }

    // set element by idx - 0-based indexing
    void set(int pos, const T &val) {
        set(root, pos, val);
    }

    // reverse the range [l, r]
    void reverse(int l, int r) {
        Node *a;
        Node *b;
        Node *c;

        split(root, l, a, b);
        split(b, r - l + 1, b, c);

        reverse(b);

        root = merge(a, merge(b, c));
    }

    // sum query by default and to customize it from node metadata
    T query(int l, int r) {
        return query(root, l, r);
    }

    int size() {
        return root->size;
    }

    bool empty() {
        return root == EMPTY;
    }

    void print() {
        print(root);
        cout << nl;
    }

    void clear() {
        clear(root);
        root = EMPTY;
    }

    ~ImplicitTreap() {
        clear(root);

        if (EMPTY != nullptr) {
            delete EMPTY;
            EMPTY = nullptr;
        }
    }
};

template <typename T> typename ImplicitTreap<T>::Node *ImplicitTreap<T>::EMPTY = new typename ImplicitTreap<T>::Node();

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ImplicitTreap<ll> tp;

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        ll x;
        cin >> x;

        tp.insert(i, x);
    }

    return 0;
}