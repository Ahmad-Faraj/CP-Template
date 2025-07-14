
const ll N = 2e5 + 5;
const int inf = 1e8 + 900;
const long long OO = (ll)1e18;

struct Nod{
 int x;
};

int n,m;
int s[N]; 

struct segmentTree
{
    int sz; vector<Nod>seg; Nod ret={1};
    segmentTree(int n)
    {
        sz=1;
        while(sz<n)sz*=2;
        seg.resize(2*sz);
    }
    
    Nod merge(Nod a,Nod b)
    {
        Nod res,mod = {m};
        res.x = a.x * b.x;
        res.x %= m;
        return res;
    }

    void build(int l,int r,int node)
    {
        if(l==r){
          if(l<n){
            seg[node]={s[l]};
          }
          else {
            seg[node]={ret};
          }
            return;
        }
        int mid=(l+r)/2;
        build(l,mid,2*node);
        build(mid+1,r,2*node+1);
        seg[node]=merge(seg[2*node],seg[2*node+1]);
        return;
    }

    void update(int l,int r,int node,int idx,int val)
    {
        if(l==r)
        {
            seg[node]={val};
            return;
        }
        int mid=(l+r)/2;
        if(mid<idx)
        {
            update(mid+1,r,2*node+1,idx,val);
        }
        else
        {
            update(l,mid,2*node,idx,val);
        }
        seg[node]=merge(seg[2*node],seg[2*node+1]);
    }

    Nod query(int lx,int rx,int l=0,int r=n-1,int node=1)
    {
        if(l>rx||r<lx)
        {
            return ret;
        }
        if(l>=lx&&r<=rx)
        {
            return seg[node];
        }
        int mid=(l+r)/2;
        return merge(query(lx,rx,l,mid,2*node),query(lx,rx,mid+1,r,2*node+1));
    }
};

void solve(){
cin>>n>>m;

for(int i=0;i<n;i++)
   cin>>s[i];

segmentTree st(n);
st.build(0,n-1,1);
}
