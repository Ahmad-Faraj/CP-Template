template <typename T = int, int Base = 0>
struct Sqrt_Decomp
{

    int n, len;
    vector<T> a;
    vector<vector<T>> b;
    T U_Default, Q_Default;

    // calculate the length of each block
    T calc_sq(int N)
    {
        int sq = sqrt(N);
        return sq * sq == N ? sq : sq + 1;
    }

    Sqrt_Decomp(int N = 0, const vector<T> &vec = vector<T>())
    {
        n = N, len = calc_sq(n), U_Default = 0, Q_Default = 0;
        a = (vec.empty() ? vector<T>(n + 5, U_Default) : vec);
        b = vector<vector<T>>(len + 5);
        if (!vec.empty())
            build();
    }

    // build each block
    void build()
    {
        for (int i = 1; i <= n; i++)
            b[i / len].push_back(a[i - !Base]);
        for (int i = 0; i <= len; i++)
            sort(all(b[i]));
    }

    // just update this index in the block O(1)
    void update(int idx, T val)
    {
        int idx2 = lower_bound(all(b[idx / len]), a[idx - !Base]) - b[idx / len].begin();
        b[idx / len][idx2] = a[idx - !Base] = val;
        sort(all(b[idx / len]));
    }

    // re-calculate the block again O(sqrt(n))
    void update_range(int idx, T val)
    {
        a[idx - !Base] = val;
        b[idx / len].clear();
        for (int i = idx / len * len; i < min(n, (idx / len + 1) * len); i++)
            b[idx / len].push_back(a[i - !Base]);
        sort(all(b[idx / len]));
    }

    // query on the range from L to R
    T query(int l, int r, T x)
    {
        T res = Q_Default;
        while (l < r && l % len != 0)
            res += a[l++ - !Base] >= x;
        while (l + len <= r)
        {
            res += sz(b[l / len]) - (lower_bound(all(b[l / len]), x) - b[l / len].begin());
            l += len;
        }
        while (l <= r)
            res += a[l++ - !Base] >= x;
        return res;
    }
};
