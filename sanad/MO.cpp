template < typename T = int, int Q_BASE = 0 > struct MO {
    struct query {
        int l, r, query_idx, block_idx;
        query(int L = 0, int R = 0, int Query_idx = 0){l = L - Q_BASE, r = R - Q_BASE, query_idx = Query_idx;}
        bool operator < (const query& other) const {
            return (block_idx < other.block_idx) || (block_idx == other.block_idx && r < other.r);
        }
    };

    T curr_l, curr_r, ans, n, m, Sqrt_N;
    vector < T > Ans, nums;
    vector < query > queries;

    MO(int N = 0, int M = 0) {
        curr_l = 1, curr_r = 0, ans = 0, n = N, m = M, Sqrt_N = n / sqrt(m) + 1;
        queries.resize(m), Ans.resize(m), nums.resize(n + 1);
    }
    
    void cinQ(vector < T > &v) {    // cin the queries from the user
        nums = v;
        for(int i = 0, l, r; i < m && cin >> l >> r; i++)   queries[i] = query(l, r, i);
        /* set the block id for each query */
        for(int i = 0; i < m; i++)  queries[i].block_idx = queries[i].l / Sqrt_N;
    }

    void ADD(int idx){

    }

    void REMOVE(int idx){

    }

    void Process(){
        sort(queries.begin(), queries.end());
        curr_l = queries[0].l, curr_r = queries[0].l - 1;
        for(int i = 0; i < m; i++){
            while(curr_l > queries[i].l)        ADD(--curr_l);
            while(curr_r < queries[i].r)        ADD(++curr_r);
            while(curr_l < queries[i].l)        REMOVE(curr_l++);
            while(curr_r > queries[i].r)        REMOVE(curr_r--);
            Ans[queries[i].query_idx] = ans;
        }
    }

    void PrintAnsToTheQueries(){
        for(int i = 0; i < m; i++)
            cout << Ans[i] << '\n';
    }

    vector < T > Get_Ans() {return Ans;}
};
