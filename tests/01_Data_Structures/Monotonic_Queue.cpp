// Test for 01_Data_Structures/Monotonic_Queue.cpp against a std::deque scanned with max_element.
// Every public method is checked: push, pop (incl. pop on empty), top, empty; T = int and long long.
#include "../../01_Data_Structures/Monotonic_Queue.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(5);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

// values: either from a tiny range [-r, r] (many duplicates) or the full range of T
template <typename T> void run(const string &tag, int r) {
    for (int it = 0; it < 20000; it++) {
        Monotonic_Queue<T> q;
        deque<T> d;
        int ops = rng() % 40;
        int rr = rng() % 2 ? r : (int)(rng() % 4);
        for (int o = 0; o < ops; o++) {
            if (rng() % 3) {
                T x = rr ? (T)((int)(rng() % (2 * rr + 1)) - rr) : (T)rng(); // rr = 0: full range
                if (rng() % 15 == 0)
                    x = rng() % 2 ? numeric_limits<T>::min() : numeric_limits<T>::max();
                q.push(x), d.push_back(x);
                chk(tag + "push", true);
            } else {
                q.pop();
                if (!d.empty()) d.pop_front();
                chk(tag + "pop", q.empty() == d.empty());
            }
            chk(tag + "empty", q.empty() == d.empty());
            if (!d.empty()) chk(tag + "top", q.top() == *max_element(d.begin(), d.end()));
        }
    }
}

int main() {
    run<int>("int:", 3);
    run<int>("int-wide:", 1000);
    run<long long>("ll:", 3);
    { // empty queue: pop is a no-op, then the queue still works
        Monotonic_Queue<int> q;
        q.pop();
        chk("pop(empty)", q.empty());
        q.push(4), q.pop(), q.pop(), q.push(2);
        chk("pop(empty)", !q.empty() && q.top() == 2);
    }
    { // samples: sliding window max of 1 3 -1 -3 5 3 6 7 with k = 3
        vector<int> a = {1, 3, -1, -3, 5, 3, 6, 7}, got, want = {3, 3, 5, 5, 6, 7};
        Monotonic_Queue<int> q;
        for (int i = 0; i < (int)a.size(); i++) {
            q.push(a[i]);
            if (i >= 3) q.pop();
            if (i >= 2) got.push_back(q.top());
        }
        chk("samples", got == want);
    }
    { // timing: sliding window max, n = 1e7, k = 1000
        int n = 10000000, k = 1000;
        Monotonic_Queue<int> q;
        long long s = 0;
        auto t0 = clock();
        for (int i = 0; i < n; i++) {
            q.push((int)(rng() % 1000000000));
            if (i >= k) q.pop();
            if (i >= k - 1) s += q.top();
        }
        printf("sliding window max n=1e7 k=1e3: %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC,
               s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
