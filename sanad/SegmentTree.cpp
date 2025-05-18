template <typename T = int> struct SegTree {
    struct Node {
        T val;
        Node(T VAL = 0) {val = VAL;}
        Node operator = (const T other) {
            val = other;
            return *this;
        }
    };
    int SZ;
    Node ORIGINAL;
    vector < Node > tree;
private:
    Node Opr(const Node &A, const Node &B) {                /** @remark Change Operation */
        return A.val + B.val;
    }
public:
    SegTree(vector<T> &TheVec) {                            /** @remark Change Original */
        ORIGINAL = 0;
        int n = TheVec.size();
        while (n & (n - 1)) n++, TheVec.push_back(0);
        SZ = n;
        tree.assign(2 * SZ, ORIGINAL);
        for (int i = 0; i < SZ; i++) tree[i + n] = TheVec[i];
        for (int i = n - 1; i >= 1; i--)tree[i] = Opr(tree[i * 2], tree[i * 2 + 1]);
    }
private:
    void update(int NodeId, int myL, int myR, const int &VALUE, const int &low, const int &high) {
        if (low <= myL && myR <= high)  return void(tree[NodeId] = VALUE);
        if (myR < low || high < myL)  return;
        int mid = (myL + myR) / 2;
        update(NodeId * 2, myL, mid, VALUE, low, high), update(NodeId * 2 + 1, mid + 1, myR, VALUE, low, high);
        tree[NodeId] = Opr(tree[NodeId * 2], tree[NodeId * 2 + 1]);
    }
    Node query(int NodeId, int myL, int myR, const int &low, const int &high) {
        if (low <= myL && myR <= high)  return tree[NodeId];
        if (myR < low || high < myL)  return ORIGINAL;
        int mid = (myL + myR) / 2;
        return Opr(query(NodeId * 2, myL, mid, low, high), query(NodeId * 2 + 1, mid + 1, myR, low, high));
    }
public:
    void update(const int theLeft, const int theRight, const T theValue) {
        update(1, 0, SZ - 1, theValue, theLeft, theRight);
    }

    void update(const int the_idx, const T the_value) {
        update(the_idx, the_idx, the_value);
    }

    Node query_Node(const int LOW, const int HIGH) {
        return query(1, 0, SZ - 1, LOW, HIGH);
    }

    T query(const int LOW, const int HIGH) {
        return query_Node(LOW, HIGH).val;
    }

    T AT(const int IDX) {
        return query_Node(IDX, IDX).val;
    }

    friend ostream& operator << (ostream &out, const Node &node) {
        out << node.val << ' ';
        return out;
    }
};
