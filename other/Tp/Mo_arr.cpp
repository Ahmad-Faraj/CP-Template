
inline int64_t gilbertOrder(int x, int y, int pow, int rotate) {
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

struct Query {
    int l, r, query_idx;
    int64_t ord;

    Query(int L = 0, int R = 0, int Query_idx = 0) {
        l = L - 1, r = R - 1, query_idx = Query_idx, calcOrder();
    }

    inline void calcOrder() {
        constexpr int K = 20;
        ord = gilbertOrder(l, r, K, 0);
    }

    bool operator < (const Query &rhs) const {
        return ord < rhs.ord;
    }
};

const int MAXN = 1e6 + 5, N = 1e6 + 5;

vector<Query> queries;

int curr_l, curr_r, ans, n, m, Sqrt_N;

int answers[N], cnt[MAXN], freq[MAXN], nums[MAXN];

void Get_Data() {
    for(int i = 0; i < n; i++)
        cin >> nums[i];

    for (int i = 0, l, r; i < m && cin >> l >> r; i++)
        queries[i] = Query(l, r, i);

    sort(all(queries));
}

void add(int idx) {

}

void remove(int idx) {

}

void set_range(int l, int r) {
    while (curr_l > l) curr_l--, add(curr_l);
    while (curr_r < r) curr_r++, add(curr_r);
    while (curr_l < l) remove(curr_l), curr_l++;
    while (curr_r > r) remove(curr_r), curr_r--;
}

void Process() {
    curr_l = queries[0].l, curr_r = queries[0].l - 1;

    for (int i = 0; i < m; i++) {
        set_range(queries[i].l, queries[i].r);
        answers[queries[i].query_idx] = ans;
    }
    for (int i = queries.back().l; i <= queries.back().r; i++) remove(i);
}


signed main() {
    while (t--) {
        cin >> n >> m;
        queries = vector<Query>(m);
        Get_Data();
        Process();
        for (int i = 0; i < m; i++)
            cout << answers[i] << '\n';
        }
}