// Driver only. SegTree is 0-based with point ASSIGN and inclusive query, so a point add reads back first.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    SegTree st(n);
    st.build(a);
    string out;
    while (q--) {
        int t, x, y;
        cin >> t >> x >> y;
        if (t == 0)
            st.update(x, st.query(x, x) + y);
        else
            out += to_string(st.query(x, y - 1)) + "\n";
    }
    cout << out;
}
