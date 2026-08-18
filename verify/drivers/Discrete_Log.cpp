// Driver only. The judge counts K = 0 as valid (it defines 0^0 = 1), which is what allow_zero enables.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    string out;
    while (t--) {
        ll x, y, m;
        cin >> x >> y >> m;
        out += to_string(discrete_log(x, y, m, true)) + "\n";
    }
    cout << out;
}
