// Majority Element: the value appearing more than n/2 times, in one pass and no extra memory, by Boyer-Moore.
// Use when: "does one value occupy more than half", stream processing, or the general more-than-n/k version.
// Handles: no majority existing, which the counting pass catches, and the Misra-Gries generalisation to n/k.
// Time: O(n) for the majority, O(n*k) for the n/k version
// Indexing: 0-based
// Note: the first pass only proposes a CANDIDATE. Skip the counting pass and the answer is wrong whenever none exists.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// the value appearing more than n/2 times, or nullopt when there is none
template <typename T> optional<T> majority(const vector<T> &a) {
    if (a.empty()) return nullopt;
    T candidate = a[0];
    int votes = 0;
    for (const T &x : a) { // pass one: the value that survives cancellation
        if (votes == 0) candidate = x, votes = 1;
        else votes += (x == candidate) ? 1 : -1;
    }
    ll count = 0; // pass two: confirm it, because pass one always proposes something
    for (const T &x : a) count += (x == candidate);
    if (count * 2 > (ll)a.size()) return candidate;
    return nullopt;
}

// every value appearing more than n/k times, by Misra-Gries; at most k-1 can qualify
template <typename T> vector<T> majority_k(const vector<T> &a, int k) {
    map<T, ll> counters;
    for (const T &x : a) {
        if (counters.count(x)) counters[x]++;
        else if ((int)counters.size() < k - 1) counters[x] = 1;
        else
            for (auto it = counters.begin(); it != counters.end();) {
                if (--it->second == 0) it = counters.erase(it);
                else ++it;
            }
    }
    vector<T> out;
    for (auto &entry : counters) { // confirm each candidate, same reason as above
        ll count = 0;
        for (const T &x : a) count += (x == entry.first);
        if (count * k > (ll)a.size()) out.push_back(entry.first);
    }
    sort(out.begin(), out.end());
    return out;
}

// Standard problem: n values - print the majority element, or NONE
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    optional<int> m = majority(a);
    cout << (m ? to_string(*m) : "NONE") << '\n';
}
