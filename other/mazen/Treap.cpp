#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;
// treap
// Balanced Binary Search Tree with random priority and it's height is around 2 * log(n)
// can do like ordered statistics tree and more
// basic treap
// 1. insert
// 2. erase
// 3. kth element
// 4. lower_bound
// 5. upper_bound
// 6. print
//! don't forget to set the seed for random number generator
//! don't forget to set the seed for random number generator
mt19937 eng(0);
uniform_int_distribution<int> rnd{INT_MIN+1,INT_MAX};
// uniform_int_distribution<int> rnd{0, 99}; //! for testing
template <typename DataType>
struct Treap
{
    enum DIR
    {
        L,
        R
    };
    // extern struct node *const EMPTY;
    struct node
    {
        DataType val;
        int pri, size, frq;
        node *ch[2], *par;

        node() : pri(INT_MIN), size(0), ch{this, this}, par(this), frq(0) {}

        node(const DataType &val) : val(val), pri(rnd(eng)), size(1), ch{EMPTY, EMPTY}, par(EMPTY), frq(1) {}

        void upd()
        {
            size = ch[L]->size + frq + ch[R]->size;
        }
    };
    static node *EMPTY;
    node *root = EMPTY;
    Treap() {}
    void link(node *p, node *c, int dir)
    {
        if (c != EMPTY)
            c->par = p;
        if (p != EMPTY)
        {
            p->ch[dir] = c;
            p->upd();
        }
    }

    int getDir(node *p, node *c)
    {
        return p->ch[R] == c;
    }

    void rotate(node *q)
    {
        node *p = q->par;
        int d = getDir(p, q);
        node *gp = p->par;
        int gd = getDir(gp, p);
        node *b = q->ch[!d];
        link(p, b, d);
        link(q, p, !d);
        link(gp, q, gd);
    };
    node *balance(node *root, int d)
    {
        if (root->ch[d]->pri > root->pri)
        {
            node *tmp = root->ch[d];
            rotate(tmp);
            return tmp;
        }
        return root;
    };
    node *insert(node *root, const DataType &val)
    {
        if (root == EMPTY)
            return new node(val);
        if (root->val == val)
        {
            root->frq++;
            root->upd();
            return root;
        }
        int d = val > root->val;
        link(root, insert(root->ch[d], val), d);
        return balance(root, d);
    };
    void insert(const DataType &val)
    {
        root = insert(root, val);
    };

    node *erase(node *root, const DataType &val)
    {
        if (root == EMPTY)
            return root;
        if (root->val == val)
        {
            if (root->frq > 1)
            {
                root->frq--;
                root->upd();
                return root;
            }
            int mxcd = 0;
            for (int d = 0; d < 2; d++)
                if (root->ch[d] == EMPTY)
                {
                    node *tmp = root->ch[!d];
                    tmp->par = EMPTY;
                    delete root;
                    return tmp;
                }
                else if (root->ch[d]->pri > root->ch[mxcd]->pri)
                    mxcd = d;
            node *tmp = root->ch[mxcd];
            rotate(tmp);
            link(tmp, erase(tmp->ch[!mxcd], val), !mxcd);
            return tmp;
        }
        int d = val > root->val;
        link(root, erase(root->ch[d], val), d);
        return root;
    };
    void erase(const DataType &val)
    {
        root = erase(root, val);
    };
    // kth element 0-indexed
    const DataType kth(node *root, int k)
    {
        if (root == EMPTY)
            return -1;
        if (k < root->ch[L]->size)
            return kth(root->ch[L], k);
        if (k < root->ch[L]->size + root->frq)
            return root->val;
        return kth(root->ch[R], k - root->ch[L]->size - root->frq);
    };
    const DataType kth(int k)
    {
        return kth(root, k);
    };
    // lower_bound returns the index of the first element >= val
    int lower_bound(node *root, const DataType &val)
    {
        if (root == EMPTY)
            return 0;
        if (val <= root->val)
            return lower_bound(root->ch[L], val);
        return root->ch[L]->size + root->frq + lower_bound(root->ch[R], val);
    };
    int lower_bound(const DataType &val)
    {
        return lower_bound(root, val);
    };
    // upper_bound returns the index of the first element > val
    int upper_bound(const DataType &val)
    {
        return lower_bound(val + 1);
    };
    // print
    void print(node *root, int d = 0)
    {
        if (root == EMPTY)
            return;
        print(root->ch[L], d + 1);
        cout << root->val << sp << root->pri << sp << root->frq << sp << d << nl;
        print(root->ch[R], d + 1);
    };
    void print()
    {
        print(root);
    };

    void clear(node *root)
    {
        if (root == EMPTY)
            return;
        clear(root->ch[L]);
        clear(root->ch[R]);
        delete root;
    };
    void clear()
    {
        clear(root);
        root = EMPTY;
    };
    ~Treap()
    {
        clear(root);
        if (EMPTY != nullptr)
        {
            delete EMPTY;
            EMPTY = nullptr;
        }
    };
};
template <typename DataType>
typename Treap<DataType>::node *Treap<DataType>::EMPTY = new typename Treap<DataType>::node();
// end of treap
// !==========================================================================
void solve(int tc)
{
    int q;
    cin >> q;
    Treap<int> tp;
    while (q--)
    {
        char op;
        int x;
        cin >> op >> x;
        if (op == 'I')
        {
            int idx = tp.lower_bound(x);
            if (idx == tp.root->size || idx == tp.upper_bound(x))
            {
                tp.insert(x);
            }
        }
        else if (op == 'D')
        {
            tp.erase(x);
        }
        else if (op == 'K')
        {
            if (x > tp.root->size)
            {
                cout << "invalid" << nl;
            }
            else
            {
                cout << tp.kth(x - 1) << nl;
            }
        }
        else
        {
            cout << tp.lower_bound(x) << nl;
        }
    }
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}