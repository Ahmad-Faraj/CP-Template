// Driver only. The judge gives l d r u, which is exactly the half-open box add() takes.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    Rectangle_Union ru;
    vector<array<ll, 4>> rs(n);
    for (auto &r : rs) cin >> r[0] >> r[1] >> r[2] >> r[3];
    for (auto &r : rs) ru.add(r[0], r[1], r[2], r[3]);
    cout << ru.area() << "\n";
}
