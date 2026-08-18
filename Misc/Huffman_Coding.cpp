// Huffman Coding: the shortest prefix-free binary code for a set of weighted symbols.
// Use when: "minimum total length after encoding", merging piles at the cost of their sum, optimal merge order.
// Handles: a single symbol, equal weights, and returns the codes themselves as well as the total cost.
// Time: O(n log n)
// Indexing: symbols are whatever you hand in; codes come back as strings of 0 and 1
// Note: total_cost is the sum of every merge, which equals sum(weight * code length). One symbol costs 0, not 1.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Huffman_Coding {
    struct Node {
        ll weight;
        int symbol; // -1 for an internal node
        int left = -1, right = -1;
    };

    vector<Node> nodes;
    vector<ll> weights;
    map<int, string> codes; // symbol -> its bit string
    ll total_cost = 0;
    int root = -1;

    // weights[i] is the weight of symbol i
    Huffman_Coding(const vector<ll> &w) : weights(w) {
        int n = (int)w.size();
        if (n == 0) return;
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> heap;
        for (int i = 0; i < n; i++) {
            nodes.push_back({w[i], i});
            heap.push({w[i], (int)nodes.size() - 1});
        }
        while (heap.size() > 1) { // always merge the two lightest, and pay their sum
            auto [w1, a] = heap.top();
            heap.pop();
            auto [w2, b] = heap.top();
            heap.pop();
            nodes.push_back({w1 + w2, -1, a, b});
            total_cost += w1 + w2;
            heap.push({w1 + w2, (int)nodes.size() - 1});
        }
        root = heap.top().second;
        assign(root, "");
        if (n == 1) codes[0] = "0"; // a lone symbol still needs one bit to write down
    }

    void assign(int at, const string &prefix) {
        if (nodes[at].symbol >= 0) {
            codes[nodes[at].symbol] = prefix;
            return;
        }
        assign(nodes[at].left, prefix + "0");
        assign(nodes[at].right, prefix + "1");
    }

    ll encoded_length() { // sum of weight times code length; equals total_cost for n > 1
        ll total = 0;
        for (auto &[symbol, code] : codes) total += weights[symbol] * (ll)code.size();
        return total;
    }

    int max_code_length() {
        size_t longest = 0;
        for (auto &[symbol, code] : codes) longest = max(longest, code.size());
        return (int)longest;
    }
};

// Standard problem: n weights - print the minimum total encoded length, then each symbol's code
void solve() {
    int n;
    cin >> n;
    vector<ll> w(n);
    for (ll &x : w) cin >> x;
    Huffman_Coding h(w);
    cout << h.encoded_length() << '\n';
    for (int i = 0; i < n; i++) cout << i << ' ' << h.codes[i] << '\n';
}
