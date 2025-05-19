struct Node{
    ll sum, sqsum,cubesum;
    Node(ll x = 0){
        sum = x;
        sqsum = x*x%mod;
        cubesum = x*x%mod*x%mod;
    }
};
 
int sz;
vector < Node > tree;
vector < ll > lazy;
void init(int n){
    sz = 1;
    while(sz < n) sz *= 2;
    tree = vector < Node > (2 * sz);
    lazy = vector < ll > (2 * sz, 0);  // for lazy propagation
}
 
void propagate(int idx, int lx, int rx){
		if(!lazy[idx]) return;
    tree[idx].cubesum += 3 * lazy[idx] * tree[idx].sqsum % mod + 3 * lazy[idx] * lazy[idx] % mod * tree[idx].sum % mod + lazy[idx] * lazy[idx] % mod * lazy[idx] % mod * (rx - lx + 1) % mod;
    tree[idx].cubesum %= mod;
 
    tree[idx].sqsum += 2 * lazy[idx] * tree[idx].sum % mod + lazy[idx] * lazy[idx] % mod * (rx - lx + 1) % mod;
    tree[idx].sqsum %= mod;
 
    tree[idx].sum += lazy[idx] * (rx - lx + 1) % mod;
    tree[idx].sum %= mod;
 
    if(lx != rx){
        lazy[2 * idx] += lazy[idx];
        lazy[2 * idx + 1] += lazy[idx];
    }
    lazy[idx] = 0;
}
 
void merge(Node &res, Node &a,Node &b){
    res.sum = (a.sum + b.sum) % mod;
    res.sqsum = (a.sqsum + b.sqsum) % mod;
    res.cubesum = (a.cubesum + b.cubesum) % mod;
}
 
void build(vector < ll > &v, int idx, int lx, int rx){
    if(lx>(int)v.size()) return;
    if(lx==rx){
        tree[idx] = Node(v[lx]);
        return;
    }
    int mid = (lx + rx) / 2;
    build(v, 2 * idx, lx, mid);
    build(v, 2 * idx + 1, mid + 1, rx);
    merge(tree[idx] , tree[2 * idx], tree[2 * idx + 1]);
}
 
void build(vector < ll > &v){
    build(v, 1, 1, sz);
}
 
void update(int i, ll val, int idx, int lx, int rx){
    if(lx==rx){
        tree[idx] = val;
        return;
    }
    int mid = (lx + rx) / 2;
    if(i<=mid) update(i, val, 2 * idx, lx, mid);
    else update(i, val, 2 * idx + 1, mid + 1, rx);
    merge(tree[idx], tree[2 * idx], tree[2 * idx + 1]);
}
 
void update(int i, ll val){
    update(i, val, 1, 1, sz);
}
 
void update(int l , int r , ll val , int idx , int lx, int rx){ // update range
    propagate(idx , lx , rx);
    if(l > rx || r < lx) return;
    if(l <= lx && rx <= r){
        lazy[idx] = val;
        propagate(idx , lx , rx);
        return;
    }
    int mid = (lx + rx) / 2;
    update(l , r , val , 2*idx , lx , mid);
    update(l , r , val , 2*idx + 1 , mid + 1 , rx);
    merge(tree[idx] ,tree[2*idx] , tree[2*idx + 1]);
}
 
void update(int l , int r , ll val){ // update range
    update(l , r , val , 1 , 1 , sz);
}
 
Node query(int l, int r, int idx, int lx, int rx){
    propagate(idx, lx, rx); // for lazy propagation
    if(lx>r or rx<l) return 0LL;
    if(lx>=l and rx<=r) return tree[idx];
    int mid = (lx + rx) / 2;
    Node left = query(l, r, 2 * idx, lx, mid);
    Node right = query(l, r, 2 * idx + 1, mid + 1, rx);
    Node res;
    merge(res, left, right);
    return res;
}
 
ll query(int l, int r){
    return query(l, r, 1, 1, sz).cubesum;
}
 
 
void solve(){
    int n; cin>>n;
    vector < ll > v(n+1);
    for(int i=1;i<=n;i++) cin>>v[i];
    init(n);
    build(v);
    int q; cin>>q;
    while(q--){
        int type; cin>>type;
        int l,r; cin>>l>>r;
        if(type == 1){
            ll x; cin>>x;
            update(l, r, x);
        }else{
            cout<<query(l, r)<<nl;
        }
    }
 
 
}