template <typename Tabletype = int, typename numsType = int, int Base = 0>
class Sparse_Table
{
private:
    int n, LOG;
    vector<vector<Tabletype>> table;
    vector<int> Bin_Log;
    function<Tabletype(const Tabletype &, const Tabletype &)> operation;
    Tabletype DEFAULT;

    void Build_Table()
    {
        for (int log = 1; log < LOG; log++)
            for (int i = 1; i + (1 << log) - 1 <= n; i++)
                table[i][log] = operation(table[i][log - 1], table[i + (1 << (log - 1))][log - 1]);
    }

    Tabletype query_1(int L, int R)
    {
        int log = Bin_Log[R - L + 1];
        return operation(table[L][log], table[R - (1 << log) + 1][log]);
    }

    Tabletype query_log_n(int L, int R)
    {
        Tabletype answer = DEFAULT;
        for (int log = LOG; log >= 0; log--)
        {
            if (L + (1 << log) - 1 <= R)
            {
                answer = operation(answer, table[L][log]);
                L += 1 << log;
            }
        }
        return answer;
    }

public:
    Sparse_Table(
        int N = 0,
        const vector<numsType> &vec = vector<numsType>(),
        function<Tabletype(const Tabletype &, const Tabletype &)> op = [](const Tabletype &a, const Tabletype &b)
        { return min(a, b); },
        Tabletype def = numeric_limits<Tabletype>::max()) : n(N), LOG(__lg(n) + 1), operation(op), DEFAULT(def)
    {
        table = vector<vector<Tabletype>>(n + 10, vector<Tabletype>(LOG, DEFAULT));
        Bin_Log = vector<int>(n + 10);
        for (int i = 2; i <= n; i++)
            Bin_Log[i] = Bin_Log[i >> 1] + 1;
        for (int i = 1; i <= N; i++)
            table[i][0] = Tabletype(vec[i - !Base]);
        Build_Table();
    }

    Tabletype query(int L, int R, bool is_overlap = false)
    {
        return (!is_overlap ? query_1(L, R) : query_log_n(L, R));
    }
};
