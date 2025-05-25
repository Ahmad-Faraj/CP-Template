template <typename T = int, const int Base = 0>
struct Lazy_Propagation
{

    struct Node
    {
        T val, update;
        bool is_lazy;
        Node(T V = 0) : val(V), update(0), is_lazy(false) {}
        Node operator=(const T &other)
        {
            val = other;
            return *this;
        }
    };

    int size;
    T lazy_default, tree_default;
    vector<Node> tree;
    vector<T> lazy;

    // initial lazy and tree
    void intial(int n)
    {
        size = 1;
        tree_default = LONG_LONG_MAX, lazy_default = 0;
        while (size <= n)
            size *= 2;
        tree = vector<Node>(size * 2, tree_default);
        lazy = vector<T>(size * 2, lazy_default);
    }

    Lazy_Propagation(int n, const vector<T> &v)
    {
        intial(n);
        build(v);
    }

    // the function that will be used to update the tree
    T lazy_operation(T a, T b)
    {
        return (a + b);
    }

    // the function that will be used to query on the tree
    Node Tree_operation(const Node &a, const Node &b)
    {
        return Node(min(a.val, b.val));
    }

    // push lazy value to children in lazy
    void propagate(int idx, int lx, int rx)
    {
        if (lazy[idx] == lazy_default)
            return;
        tree[idx] = lazy_operation(tree[idx].val, lazy[idx]);
        if (lx != rx)
        {
            lazy[2 * idx] = lazy_operation(lazy[2 * idx], lazy[idx]);
            lazy[2 * idx + 1] = lazy_operation(lazy[2 * idx + 1], lazy[idx]);
        }
        lazy[idx] = lazy_default;
    }

    // push value to children into lazy
    void propagate(int idx, int lx, int rx, T v)
    {
        tree[idx] = lazy_operation(tree[idx].val, v);
        if (lx != rx)
        {
            lazy[2 * idx] = lazy_operation(lazy[2 * idx], v);
            lazy[2 * idx + 1] = lazy_operation(lazy[2 * idx + 1], v);
        }
    }

    // build the tree with given vector
    void build(const vector<T> &nums, int idx, int lx, int rx)
    {
        propagate(idx, lx, rx);
        if (Base ? lx >= sz(nums) : lx > sz(nums))
            return;
        if (rx == lx)
            tree[idx] = nums[lx - !Base];
        else
        {
            int m = (rx + lx) / 2;
            build(nums, 2 * idx, lx, m);
            build(nums, 2 * idx + 1, m + 1, rx);
            tree[idx] = Tree_operation(tree[2 * idx], tree[2 * idx + 1]);
        }
    }

    // build the tree with initial value
    void build(T initial_value, int idx, int lx, int rx)
    {
        propagate(idx, lx, rx);
        if (rx == lx)
            tree[idx] = initial_value;
        else
        {
            int m = (rx + lx) / 2;
            build(initial_value, 2 * idx, lx, m);
            build(initial_value, 2 * idx + 1, m + 1, rx);
            tree[idx] = tree_operation(tree[2 * idx], tree[2 * idx + 1]);
        }
    }

    // the vector should be 1-based also the tree is 1-based
    void build(const vector<T> &nums)
    {
        build(nums, 1, 1, size);
    }

    // update the value of the tree in range [l, r] with value v
    void update(int l, int r, T v, int idx, int lx, int rx)
    {
        propagate(idx, lx, rx);
        if (lx >= l && rx <= r)
            return propagate(idx, lx, rx, v);
        if (lx > r || rx < l)
            return;
        int m = (lx + rx) / 2;
        update(l, r, v, 2 * idx, lx, m), update(l, r, v, 2 * idx + 1, m + 1, rx);
        tree[idx] = Tree_operation(tree[2 * idx], tree[2 * idx + 1]);
    }

    // update the value in one index
    void update(int i, T v)
    {
        update(i, i, v, 1, 1, size);
    }

    // update the value of the tree in range [l, r] with value v
    void update(int l, int r, T v)
    {
        update(l, r, v, 1, 1, size);
    }

    // query the value of the tree in range [l, r]
    Node query(int l, int r, int idx, int lx, int rx)
    {
        propagate(idx, lx, rx);
        if (lx >= l && rx <= r)
            return tree[idx];
        if (lx > r || rx < l)
            return tree_default;
        int m = (rx + lx) / 2;
        return Tree_operation(query(l, r, 2 * idx, lx, m), query(l, r, 2 * idx + 1, m + 1, rx));
    }

    // query the value of the tree in range [l, r]
    T query(int l, int r)
    {
        return query(l, r, 1, 1, size).val;
    }
};