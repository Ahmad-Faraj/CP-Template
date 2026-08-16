/* Topic: Hash Lazy Segment Tree
 * Description: Hash Lazy Segment Tree maintains the polynomial rolling hash of a string
 *   dynamically using a Lazy Segment Tree. Allows for range updates (e.g., setting a 
 *   substring to a specific character) and O(log N) string equivalence checks.
 * Usage: Hash_LazySegmentTree hs(n, s); hs.update(l, r, val); hs.query(l, r);
 */
#include "../../core.h"

// Note: Requires precomputing p_pow (powers of bases) and p_inv (modular inverses) arrays globally.
const long long p_1inv = 233333335, p_2inv = 972222231;
const long long mod1 = 1e9 + 7, mod2 = 1e9 + 9, p1 = 31, p2 = 37;
long long p_pow[2][1000005], inv[2][1000005];
 
long long mul(long long a, long long b, long long mod) { return (a * b) % mod; }
long long add(long long a, long long b, long long mod) { return (a + b) % mod; }
 
void precompute(){
  p_pow[0][0] = p_pow[1][0] = inv[0][0] = inv[1][0] = 1;
  for(int i = 1; i < 1000005; i++){
    p_pow[0][i] = mul(p_pow[0][i - 1], p1, mod1), p_pow[1][i] = mul(p_pow[1][i - 1], p2, mod2);
    inv[0][i] = mul(inv[0][i - 1], p_1inv, mod1), inv[1][i] = mul(inv[1][i - 1], p_2inv, mod2);
  }
}
 
template < typename T = int, bool Base = 0 > struct Hash_LazySegmentTree {
 
  string s;
  int n, size;
  vector < T > lazy;
  vector < pair < T, T > > tree;
 
  #define LEFT (idx << 1)
  #define RIGHT ((idx << 1) | 1)
 
  struct Item{
    T len;
    pair < T, T > ans;
    Item() : ans({0, 0}), len(0) {}
    Item(pair < T, T > ans, T len) : ans(ans) , len(len) {}
  };
 
  explicit Hash_LazySegmentTree(const int N, const string &s) : s(s) , n(N) {
    size = 1;
    while(size < n) size *= 2;
    lazy = vector < T > (2 * size, -1);
    tree = vector < pair < T, T > > (2 * size, {0, 0});
    build(1, 1, n);
  }
 
  inline pair < T, T > merge(const pair < T, T >& a, const pair < T, T >& b, T len){
    long long h1 = add(a.first, mul(b.first, p_pow[0][len], mod1), mod1);
    long long h2 = add(a.second, mul(b.second, p_pow[1][len], mod2), mod2);
    return {h1 , h2};
  }  
 
  inline pair < T, T > lazy_process(T val, T len){
    T h1 = mul(val, (p_pow[0][len] - 1 + mod1) % mod1, mod1);
    T h2 = mul(val, (p_pow[1][len] - 1 + mod2) % mod2, mod2);
    return {mul(h1, p_1inv, mod1), mul(h2, p_2inv, mod2)};
  }  
 
  inline void propagate(int idx, int l, int r){
    if(lazy[idx] == -1) return;
    tree[idx] = lazy_process(lazy[idx], (r - l + 1));
    if(l != r) lazy[LEFT] = lazy[RIGHT] = lazy[idx];
    lazy[idx] = -1;
  }
  
  inline void build(int idx, int l, int r){
    if(l == r) 
      return void(tree[idx] = {s[l - !Base] - 'a' + 1, s[l - !Base] - 'a' + 1});
    int m = (l + r) / 2;
    build(LEFT, l, m);
    build(RIGHT, m + 1, r);
    tree[idx] = merge(tree[LEFT], tree[RIGHT], m - l + 1);
  }
 
  inline void update(int idx, int l, int r, int L, int R, T val){
    propagate(idx, l, r);
    if(l > R or r < L) return;
    if(l >= L and r <= R){
      lazy[idx] = val;
      propagate(idx, l, r);
      return;
    }
    int m = (l + r) / 2;
    update(LEFT, l, m, L, R, val);
    update(RIGHT, m + 1, r, L, R, val);
    tree[idx] = merge(tree[LEFT], tree[RIGHT], m - l + 1);
  }
 
  inline Item query(int idx, int l, int r, int L, int R){
    propagate(idx, l, r);
    if(l > R or r < L) return Item();
    if(l >= L and r <= R) return Item(tree[idx], r - l + 1);
    int m = (l + r) / 2;
    Item left = query(LEFT, l, m, L, R);
    Item right = query(RIGHT, m + 1, r, L, R);
    return Item(merge(left.ans, right.ans, left.len), left.len + right.len);
  }
 
  inline void update(int l, int r, T val){
    update(1, 1, n, l, r, val);
  }
 
  inline void update(int idx, T val){
    update(idx, idx, val);
  }
 
  inline pair < T, T > query(int l, int r){
    return query(1, 1, n, l, r).ans;
  }
  
  #undef LEFT
  #undef RIGHT
 
};
