// Driver only. SparseTable is 0-based and query(l, r) is inclusive; the judge asks for [l, r).
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    SparseTable<ll> st(a);
    string out;
    while (q--) {
        int l, r;
        cin >> l >> r;
        out += to_string(st.query(l, r - 1)) + "\n";
    }
    cout << out;
}
