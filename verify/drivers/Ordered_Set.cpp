// Driver only.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    string t;
    cin >> n >> q >> t;
    ordered_set<int> s;
    for (int i = 0; i < n; i++)
        if (t[i] == '1') s.insert(i);
    string out;
    while (q--) {
        int c, k;
        cin >> c >> k;
        if (c == 0)
            s.insert(k);
        else if (c == 1)
            s.erase(k);
        else if (c == 2)
            out += (s.find(k) != s.end() ? "1\n" : "0\n");
        else if (c == 3) {
            auto it = s.lower_bound(k);
            out += (it == s.end() ? string("-1") : to_string(*it)) + "\n";
        } else {
            auto it = s.upper_bound(k);
            out += (it == s.begin() ? string("-1") : to_string(*prev(it))) + "\n";
        }
    }
    cout << out;
}
