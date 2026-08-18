// Driver only. SegTree is 1-based with point ASSIGN, so a point add reads the value back first.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    SegTree st;
    st.init(n);
    st.build(a);
    string out;
    while (q--) {
        int t, x, y;
        cin >> t >> x >> y;
        if (t == 0)
            st.update(x + 1, st.query(x + 1, x + 1) + y);
        else
            out += to_string(st.query(x + 1, y)) + "\n";
    }
    cout << out;
}
