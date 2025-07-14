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
// Implicit Treap
// Balanced Binary Search Tree with random priority and it's height is around 2 * log(n)
// can do like ordered statistics tree and more
// 0 based indexing
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
    struct node{
        DataType val, sum, lazy;
        int pri, size;
        node *ch[2], *par;

        node() : pri(INT_MIN), size(0), ch{this, this}, par(this), sum(0), lazy(0) {}

        node(const DataType &val) : val(val), pri(rnd(eng)), size(1), ch{EMPTY, EMPTY}, par(EMPTY), sum(val), lazy(0) {}

        void upd()
        {
            size = ch[L]->size + 1 + ch[R]->size;
            sum = ch[L]->sum + val + ch[R]->sum;
        }

        void add(DataType v)
        {
            val += v;
            sum += v * size;
            lazy += v;
        }

        void push()
        {
            if (this == EMPTY)
                return;
            if (lazy)
            {
                for (auto i : {L, R})
                    ch[i]->add(lazy);
                lazy = 0;
            }
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
    pair<node *, node *> split(node *root, int frstSz)
    {
        if (root == EMPTY)
            return {EMPTY, EMPTY};
        root->push();
        if (frstSz <= root->ch[L]->size)
        {
            auto [ls, gr] = split(root->ch[L], frstSz);
            link(root, gr, L);
            link(EMPTY, ls, L);
            return {ls, root};
        }
        auto [ls, gr] = split(root->ch[R], frstSz - root->ch[L]->size - 1);
        link(root, ls, R);
        link(EMPTY, gr, R);
        return {root, gr};
    }
    node *merge(node *ls, node *gr)
    {
        if (ls == EMPTY)
            return gr;
        if (gr == EMPTY)
            return ls;
        ls->push();
        gr->push();
        if (ls->pri > gr->pri)
        {
            link(ls, merge(ls->ch[R], gr), R);
            return ls;
        }
        link(gr, merge(ls, gr->ch[L]), L);
        return gr;
    }
    void insert(int val)
    {
        root = merge(root, new node(val));
    }
    void insert(int val, int idx)
    {
        auto [bef, aft] = split(root, idx);
        root = merge(merge(bef, new node(val)), aft);
    }
    
    void eraseindex(int idx){
        auto [bef, aft] = split(root, idx + 1);
        auto [bef2, aft2] = split(bef, idx);
        root = merge(bef2, aft);
    }
    void addRange(node *&root, int s, int e, DataType v)
    {
        node *bef, *aft, *bet;
        tie(root, aft) = split(root, e + 1);
        tie(bef, bet) = split(root, s);
        bet->add(v);
        root = merge(merge(bef, bet), aft);
    }
    void addRange(int s, int e, DataType v)
    {
        addRange(root, s, e, v);
    }
    DataType getSum(node *&root, int s, int e)
    {
        node *bef, *aft, *bet;
        tie(root, aft) = split(root, e + 1);
        tie(bef, bet) = split(root, s);
        DataType res = bet->sum;
        root = merge(merge(bef, bet), aft);
        return res;
    }
    DataType getSum(int s, int e)
    {
        return getSum(root, s, e);
    }

    void swapSegments(int l1, int r1, int l2, int r2) {
        // 1) Split off [0..r2]
        auto [t01, E] = split(root, r2 + 1);
        // 2) Split t01 into [0..l2-1] and [l2..r2]
        auto [t02, D] = split(t01, l2);
        // 3) Split t02 into [0..r1] and [r1+1..l2-1]
        auto [t03, C] = split(t02, r1 + 1);
        // 4) Split t03 into [0..l1-1] and [l1..r1]
        auto [A, B] = split(t03, l1);
    
        // Now A, B, C, D, E are the five pieces.
        // Re‐merge as A + D + C + B + E
        root = merge(A, merge(D, merge(C, merge(B, E))));
    }
    
    void swap(int l1, int r1, int l2, int r2) {
        swapSegments(l1, r1, l2, r2);
    }
    
    void print(node *cur, int d)
    {
        if (cur == EMPTY)
            return;
        print(cur->ch[L], d + 1);
        printf("%s%2d,%2d,%2d,%2d,%2d\n", string(d * 14, ' ').c_str(), cur->val, cur->pri, cur->sz, cur->sum, cur->lazy);
        print(cur->ch[R], d + 1);
    }
    void print()
    {
        puts("----------------------------");
        print(root, 0);
        puts("----------------------------");
    }
    void clear(node *root)
    {
        if (root == EMPTY)
            return;
        clear(root->ch[L]);
        clear(root->ch[R]);
        delete root;
    }
    void clear()
    {
        clear(root);
        root = EMPTY;
    }
};
template <typename DataType>
typename Treap<DataType>::node *Treap<DataType>::EMPTY = new typename Treap<DataType>::node();
// end of treap
// !==========================================================================

void solve(int tc)
{
    int n, q;
    cin >> n >> q;
    Treap<int> t;
    for (int i = 0; i < n; i++)
        t.insert(0);
    while (q--)
    {
        int typ, s, e, v;
        cin >> typ >> s >> e;
        if (typ)
        {
            cout << t.getSum(--s, --e) << nl;
        }
        else
        {
            cin >> v;
            t.addRange(--s, --e, v);
        }
    }
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}