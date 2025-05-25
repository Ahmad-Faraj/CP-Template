template <typename T = int>
struct cord_compression
{

    vector<T> a, b, c;
    int n;

    cord_compression(vector<T> &A)
    {
        a = A;
        n = sz(a);
        b = a;
        sort(all(b));
        b.resize(unique(all(b)) - b.begin());
        c.resize(n);
        for (int i = 0; i < n; i++)
            c[i] = lower_bound(all(b), a[i]) - b.begin();
    }

    vector<T> get_compressed()
    {
        return c;
    }

    vector<T> get_original()
    {
        return b;
    }
};