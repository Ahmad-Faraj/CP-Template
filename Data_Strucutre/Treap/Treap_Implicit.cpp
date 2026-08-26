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

        // Query information
        T sum;
        T mn;
        T mx;

        // Lazy information
        T add;
        T assign;

        int pri;
        int size;

        bool rev;
        bool assign_flag;

        Node *ch[2];

        Node()
            : val(T()), sum(T()), mn(T()), mx(T()), add(T()), assign(T()), pri(INT_MIN), size(0), rev(false),
              assign_flag(false), ch{this, this} {}

        Node(const T &val)
            : val(val), sum(val), mn(val), mx(val), add(T()), assign(T()), pri(rnd(eng)), size(1), rev(false),
              assign_flag(false), ch{EMPTY, EMPTY} {}

        // customize your query here
        void update() {
            size = ch[0]->size + 1 + ch[1]->size;

            sum = ch[0]->sum + val + ch[1]->sum;

            mn = min({ch[0]->mn, val, ch[1]->mn});
            mx = max({ch[0]->mx, val, ch[1]->mx});
        }

        void push() {
            if (assign_flag) {
                apply_assign(ch[0], assign);
                apply_assign(ch[1], assign);

                val = assign;

                assign_flag = false;
            }

            if (add != T()) {
                apply_add(ch[0], add);
                apply_add(ch[1], add);

                val += add;

                add = T();
            }

            if (rev) {
                swap(ch[0], ch[1]);

                if (ch[0] != EMPTY) ch[0]->rev ^= 1;

                if (ch[1] != EMPTY) ch[1]->rev ^= 1;

                rev = false;
            }
        }
    };

    static Node *EMPTY;

    Node *root = EMPTY;

    int size(Node *root) { return root->size; }

    T sum(Node *root) { return root->sum; }

    T mn(Node *root) { return root->mn; }

    T mx(Node *root) { return root->mx; }

    // Apply assignment to a whole subtree
    void apply_assign(Node *root, const T &val) {
        if (root == EMPTY) return;

        root->val = val;
        root->sum = val * root->size;
        root->mn = val;
        root->mx = val;

        root->assign = val;
        root->assign_flag = true;

        root->add = T();
    }

    // Add value to a whole subtree
    void apply_add(Node *root, const T &val) {
        if (root == EMPTY) return;

        root->val += val;
        root->sum += val * root->size;
        root->mn += val;
        root->mx += val;

        root->add += val;
    }

    // Reverse a whole subtree
    void apply_reverse(Node *root) {
        if (root == EMPTY) return;

        root->rev ^= 1;
    }

    void update(Node *root) {
        if (root != EMPTY) root->update();
    }

    void push(Node *root) {
        if (root != EMPTY) root->push();
    }

    // Merge two treaps
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

    // Split first k elements into a and b
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

    // Range query helper
    T query(Node *root, int l, int r, int type) {
        if (root == EMPTY || l > r) return T();

        Node *a;
        Node *b;
        Node *c;

        split(root, l, a, b);
        split(b, r - l + 1, b, c);

        T ans;

        if (type == 0)
            ans = b->sum;
        else if (type == 1)
            ans = b->mn;
        else
            ans = b->mx;

        root = merge(a, merge(b, c));

        return ans;
    }

    // Delete whole subtree
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
    ImplicitTreap() = default;

    // Split first k elements into left
    void split(int k, ImplicitTreap &left, ImplicitTreap &right) {
        split(root, k, left.root, right.root);

        root = EMPTY;
    }

    // Merge two treaps
    void merge(ImplicitTreap &left, ImplicitTreap &right) {
        root = merge(left.root, right.root);

        left.root = EMPTY;
        right.root = EMPTY;
    }

    // Insert val at index pos
    void insert(int pos, const T &val) {
        Node *a;
        Node *b;

        split(root, pos, a, b);

        root = merge(merge(a, new Node(val)), b);
    }

    // Erase element at index pos
    void erase(int pos) {
        Node *a;
        Node *b;
        Node *c;

        split(root, pos, a, b);
        split(b, 1, b, c);

        clear(b);

        root = merge(a, c);
    }

    // Get element at index pos
    T get(int pos) { return get(root, pos); }

    // Set element at index pos
    void set(int pos, const T &val) { set(root, pos, val); }

    // Reverse range [l, r]
    void reverse(int l, int r) {
        Node *a;
        Node *b;
        Node *c;

        split(root, l, a, b);
        split(b, r - l + 1, b, c);

        apply_reverse(b);

        root = merge(a, merge(b, c));
    }

    // Add val to every element in [l, r]
    void add(int l, int r, const T &val) {
        Node *a;
        Node *b;
        Node *c;

        split(root, l, a, b);
        split(b, r - l + 1, b, c);

        apply_add(b, val);

        root = merge(a, merge(b, c));
    }

    // Set every element in [l, r] to val
    void assign(int l, int r, const T &val) {
        Node *a;
        Node *b;
        Node *c;

        split(root, l, a, b);
        split(b, r - l + 1, b, c);

        apply_assign(b, val);

        root = merge(a, merge(b, c));
    }

    // Range sum query
    T query(int l, int r) { return query(root, l, r, 0); }

    // Range minimum query
    T query_min(int l, int r) { return query(root, l, r, 1); }

    // Range maximum query
    T query_max(int l, int r) { return query(root, l, r, 2); }

    // Right cyclic shift
    void cyclic_shift(int l, int r, int k) {
        if (l >= r) return;

        int len = r - l + 1;

        k %= len;

        if (k < 0) k += len;

        if (k == 0) return;

        Node *a;
        Node *b;
        Node *c;
        Node *x;
        Node *y;

        split(root, l, a, b);
        split(b, len, b, c);

        split(b, len - k, x, y);

        b = merge(y, x);

        root = merge(a, merge(b, c));
    }

    // Left cyclic shift
    void cyclic_shift_left(int l, int r, int k) {
        if (l >= r) return;

        int len = r - l + 1;

        k %= len;

        if (k < 0) k += len;

        if (k == 0) return;

        cyclic_shift(l, r, len - k);
    }

    // Get number of elements
    int size() { return root->size; }

    // Check if treap is empty
    bool empty() { return root == EMPTY; }

    // Print elements in order
    void print() {
        print(root);
        cout << nl;
    }

    // Clear treap
    void clear() {
        clear(root);

        root = EMPTY;
    }

    ~ImplicitTreap() { clear(root); }
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

    tp.print();

    return 0;
}