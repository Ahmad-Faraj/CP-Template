template <typename T = int, int Base = 0>
class MoAlgorithm
{
public:
    struct query
    {
        int l, r, query_idx;
        int64_t ord;

        query(int L = 0, int R = 0, int Query_idx = 0, int HilbertPow = 0)
        {
            l = L - !Base, r = R - !Base, query_idx = Query_idx;
            calcOrder(HilbertPow);
        }

        void calcOrder(int HilbertPow)
        {
            ord = gilbertOrder(l, r, HilbertPow, 0);
        }

        bool operator<(const query &rhs) const
        {
            return ord < rhs.ord;
        }
    };

    MoAlgorithm(int N = 0, int M = 0) : currL(1), currR(0), n(N), m(M), sqrtN(n / sqrt(m) + 1), hilbertPow(calculateHilbertPow()), ans(0), answers(m), queries(m) {}

    void getData(const vector<T> &v = {})
    {
        val = v;

        for (int i = 0, l, r; i < m && cin >> l >> r; i++)
            queries[i] = query(l, r, i, hilbertPow);

        process();
    }

    void process()
    {
        sort(queries.begin(), queries.end());
        currL = queries[0].l, currR = queries[0].l - 1;

        for (auto &q : queries)
        {
            setRange(q);
            answers[q.query_idx] = ans;
        }
    }

    vector<T> getAnswers() const
    {
        return answers;
    }

private:
    int currL, currR, n, m, sqrtN, hilbertPow;
    T ans;
    vector<T> answers, val;
    vector<query> queries;

    static int64_t gilbertOrder(int x, int y, int pow, int rotate)
    {
        if (pow == 0)
            return 0;
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

    int calculateHilbertPow() const
    {
        int pow = 1;
        while ((1 << pow) < n)
            pow++;
        return pow;
    }

    void add(int idx)
    {
        // add idx to the processing range
    }

    void remove(int idx)
    {
        // remove idx from the processing range
    }

    void setRange(const query &q)
    {
        while (currL > q.l)
            currL--, add(currL);
        while (currR < q.r)
            currR++, add(currR);
        while (currL < q.l)
            remove(currL), currL++;
        while (currR > q.r)
            remove(currR), currR--;
    }
};
