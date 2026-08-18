// Driver only.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    string out;
    while (q--) {
        unsigned long long n;
        cin >> n;
        out += is_prime(n) ? "Yes\n" : "No\n";
    }
    cout << out;
}
