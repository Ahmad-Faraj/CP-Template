// Driver only. Absent keys read as 0, which is what the judge expects.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    Fast_Map<ll, ll> m;
    string out;
    while (q--) {
        int t;
        ll k;
        cin >> t >> k;
        if (t == 0) {
            ll v;
            cin >> v;
            m[k] = v;
        } else {
            auto it = m.find(k);
            out += (it == m.end() ? string("0") : to_string(it->second)) + "\n";
        }
    }
    cout << out;
}
