#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
 
using namespace std;
using namespace __gnu_pbds;

#define ll long long 
#define PI acos(-1)
#define all(x) x.begin(),x.end()
#define clr(v, d) memset(v, d, sizeof(v))
#define endl "\n"
#define sz(x) (int)x.size()
#define vl vector<ll>
#define pb push_back
#define ordered_set tree<ll ,  null_type ,  less_equal<> ,  rb_tree_tag ,  tree_order_statistics_node_update>
vector <int> dx {0, 0, 1, -1}, dy {1, -1, 0, 0};
//vector<int> dx{1, 1, -1, -1, 2, 2, -2, -2}, dy{2, -2, 2, -2, 1, -1, 1, -1};
 
void M7MDAN_FARAG(){
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr); 
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}

const ll Mod = 1e9+7, N = 2e5 + 5, OO = 0x3f3f3f3f, INF = 0x3f3f3f3f3f3f3f3f;

template < typename T = int > istream& operator >> (istream &in, vector < T > &v) {
    for (auto &x : v) in >> x;
    return in;
}

template < typename T = int > ostream& operator << (ostream &out, const vector < T > &v) { 
    for (const T &x : v) out << x << ' '; 
    return out;
}

template < typename T = int, int Base = 0 > struct MO {

    static inline int64_t gilbertOrder(int x, int y, int pow, int rotate) {
        if (pow == 0) return 0;
        int hpow = 1 << (pow - 1);
        int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
        seg = (seg + rotate) & 3;
        const int rotateDelta[4] = {3, 0, 0, 1};
        int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
        int nrot = (rotate + rotateDelta[seg]) & 3;
        int64_t subSquareSize = int64_t(1) << (2 * pow - 2);
        int64_t ordd = seg * subSquareSize;
        int64_t add = gilbertOrder(nx, ny, pow - 1, nrot);
        ordd += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
        return ordd;
    }

    struct query {

        int l, r, query_idx;
        int64_t ord;

        query(int L = 0, int R = 0, int Query_idx = 0){
            l = L - !Base, r = R - !Base, query_idx = Query_idx, calcOrder();
        }

        inline void calcOrder() {
            constexpr int K = 19;
            // K should be minimum such that 2^K >= n
            ord = gilbertOrder(l, r, K, 0);
        }

        bool operator < (const query & rhs) const{
            return ord < rhs.ord;
        }
    };

    T curr_l, curr_r, ans, n, m, Sqrt_N;
    vector < T > answers, nums;
    vector < query > queries;
    multiset<int> number, total;

    MO(int N = 0, int M = 0){
        curr_l = 1, curr_r = 0, ans = 0, n = N, m = M, Sqrt_N = n / sqrt(m) + 1;
        queries = vector < query > (m);
        answers = vector < T > (m);
        nums = vector < T > (n + 5);
    }
    
    void Get_Data(const vector < T > &v){
        // get the array and set the queries
        nums = v;

        for(int i = 0, l, r; i < m && cin >> l >> r; i++)
            queries[i] = query(l, r, i);
        
    }

    // add the idx to the current range
    void add(int idx){
        // do what is increase the answer
         int num = nums[idx];
    if(number.size()==0){
        number.insert(num);
        return;
    }
    auto it = number.lower_bound(num);
    if(it == number.end()){
        int x = *prev(it);
        total.insert(num - x);
    }
    else if(it == number.begin()){
        int x = *it;
        total.insert(x - num);
    }
    else {
        int x = *it;
        int y = *prev(it);
        total.erase(total.find(x - y));
        total.insert(x - num);
        total.insert(num - y);
    }
    number.insert(num);
    ans = *total.begin();
    cout << ans << endl;
    }

    // remove the idx from the current range
    void remove(int idx){
        // do what is decrease the answer
        int num = nums[idx];
    auto it = number.find(num);
    if(it == number.end())
        return;
    number.erase(it);
    if(number.size() == 0)
        return;
    it = number.lower_bound(num);
    if(it == number.end()){
        int x = *prev(it);
        total.erase(total.find(num - x));
    }
    else if(it == number.begin()){
        int x = *it;
        total.erase(total.find(x - num));
    }
    else {
        int x = *it;
        int y = *prev(it);
        total.erase(total.find(x - num));
        total.erase(total.find(num - y));
        total.insert(x - y);
    }
    ans = *total.begin();
    }

    void set_range(const query& q){
        // add the new range and remove the old range
        while (curr_l > q.l) curr_l--, add(curr_l);
        while (curr_r < q.r) curr_r++, add(curr_r);
        while (curr_l < q.l) remove(curr_l), curr_l++;
        while (curr_r > q.r) remove(curr_r), curr_r--;
    }

    void Process(){
        
        sort(all(queries));

        // to start with the first query
        curr_l = queries[0].l, curr_r = queries[0].l - 1;
        
        for(int i = 0; i < m; i++){
            set_range(queries[i]);
            answers[queries[i].query_idx] = ans;
        }
    }

    void Print_queries_answers(){
        for(int i = 0; i < m; i++)
            cout << answers[i] << '\n';
    }

    vector < T > Get_answers(){
        return answers;
    }

};

void Try(){
    ll n, m;
    cin >> n;
    vector < ll > v(n);
    cin >> v;
    cin >> m;
    MO < ll > mo(n, m);
    mo.Get_Data(v);
    mo.Process();
    mo.Print_queries_answers();
}

signed main(){
    M7MDAN_FARAG();
    int t = 1;
    // cin >> t;
    while(t--){
        Try();
    }
}
