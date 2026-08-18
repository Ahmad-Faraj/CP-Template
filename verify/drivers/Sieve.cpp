// Driver only.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, a, b;
    cin >> n >> a >> b;
    vector<int> p = primes_up_to(n);
    string picked;
    int count = 0;
    for (size_t i = b; i < p.size(); i += a) {
        picked += (count++ ? " " : "") + to_string(p[i]);
    }
    cout << p.size() << " " << count << "\n" << picked << "\n";
}
