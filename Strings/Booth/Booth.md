# 1. Booth's Algorithm

## 1.0 Overview
Booth's Algorithm uses a modified KMP failure function to find the **lexicographically minimal rotation** (or cyclic shift) of a string in $O(N)$ time and $O(N)$ space.
- **Time Complexity:** $O(N)$
- **Space Complexity:** $O(N)$

---

## 1.1 Minimal & Maximal Rotation Strings
**Description:** Easily extract the actual string objects from the starting indices.

```cpp
string get_min_rotation(string s) {
    if (s.empty()) return s;
    int k = Booth::min_rotation_index(s);
    return s.substr(k) + s.substr(0, k);
}

int max_rotation_index(string s) {
    int original_len = s.size();
    if (original_len == 0) return 0;
    
    s += s;
    vector<int> f(s.size(), -1);
    int k = 0;
    for(int j = 1; j < s.size(); j++) {
        char sj = s[j];
        int i = f[j - k - 1];
        while(i != -1 && sj != s[k + i + 1]) {
            if(sj > s[k + i + 1]) // Flipped from < to >
                k = j - i - 1;
            i = f[i];
        }
        if(sj != s[k + i + 1]) {
            if(sj > s[k]) // Flipped from < to >
                k = j;
            f[j - k] = -1;
        } else {
            f[j - k] = i + 1;
        }
    }
    return k % original_len;
}

string get_max_rotation(string s) {
    if (s.empty()) return s;
    int k = max_rotation_index(s);
    return s.substr(k) + s.substr(0, k);
}
```

## 1.2 Cyclic Equivalence Checking
**Description:** Two strings of the same length are cyclic shifts of each other if and only if their lexicographically minimal rotations are identical. This allows $O(N)$ normalization and comparison without building a double-length string + KMP table for every query pair.

```cpp
bool are_cyclic_shifts(string a, string b) {
    if (a.size() != b.size()) return false;
    return get_min_rotation(a) == get_min_rotation(b);
}
```

## 1.3 Lyndon Factorization (Duval's Algorithm)
**Description:** A string is called a "Lyndon word" if it is strictly lexicographically smaller than all of its non-empty proper suffixes. Duval's algorithm partitions any string into a sequence of Lyndon words $w_1, w_2, \dots, w_k$ such that $w_1 \ge w_2 \ge \dots \ge w_k$.
- **Time Complexity:** $O(N)$
- **Space Complexity:** $O(1)$ extra space!

```cpp
vector<string> duval_factorization(string const& s) {
    int n = s.size();
    int i = 0;
    vector<string> factorization;
    while (i < n) {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j])
                k = i;
            else
                k++;
            j++;
        }
        while (i <= k) {
            factorization.push_back(s.substr(i, j - k));
            i += j - k;
        }
    }
    return factorization;
}

// Minimal Rotation via Duval's Algorithm (Alternative to Booth's)
// Concatenate s + s and run Duval's. The last Lyndon factor boundary found 
// before or at index N gives the start of the lexicographically minimal rotation!
// Many Grandmasters prefer Duval's because it's cleaner to implement and requires O(1) space.
int duval_min_rotation_index(string s) {
    int n = s.size();
    s += s;
    int i = 0, ans = 0;
    while (i < n) {
        ans = i;
        int j = i + 1, k = i;
        while (j < 2 * n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) i += j - k;
    }
    return ans;
}
```

---

## 1.4 Generic Array/Vector Minimal Rotation (Cycle Graph Isomorphism)
**Description:** Booth's and Duval's algorithms aren't limited to strings! When checking if two cyclic graphs (or polygons) are isomorphic, serialize their edge weights or node degrees into a `vector<int>` and compute the minimal rotation of both. If the minimized vectors are equal, the cycles are isomorphic.

```cpp
template<typename T>
int min_rotation_vector(const vector<T>& v) {
    int n = v.size();
    vector<T> s = v;
    s.insert(s.end(), v.begin(), v.end()); // s = v + v
    
    int i = 0, ans = 0;
    while (i < n) {
        ans = i;
        int j = i + 1, k = i;
        while (j < 2 * n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) i += j - k;
    }
    return ans;
}
```

## 1.5 Smallest Suffix of a String in $O(1)$ Extra Space
**Description:** By the Chen-Fox-Lyndon theorem, the very last Lyndon word in the factorization of $S$ is exactly the **strictly lexicographically smallest suffix** of the entire string $S$. Duval's algorithm extracts this in $O(N)$ time without allocating a Suffix Array!

```cpp
int smallest_suffix_index(const string& s) {
    int n = s.size();
    int i = 0, last_start = 0;
    while (i < n) {
        last_start = i; // Track the start of the current Lyndon factor
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) i += j - k;
    }
    return last_start; // Returns the index of the smallest suffix
}
```

## 1.6 Largest Suffix of a String in $O(1)$ Extra Space
**Description:** By simply reversing the comparison operators in Duval's algorithm, we partition the string into words $w_1 \le w_2 \le \dots \le w_k$. The last word is the **strictly lexicographically largest suffix**.

```cpp
int largest_suffix_index(const string& s) {
    int n = s.size();
    int i = 0, last_start = 0;
    while (i < n) {
        last_start = i;
        int j = i + 1, k = i;
        while (j < n && s[k] >= s[j]) { // Flipped from <= to >=
            if (s[k] > s[j]) k = i;     // Flipped from < to >
            else k++;
            j++;
        }
        while (i <= k) i += j - k;
    }
    return last_start;
}
```

## 1.7 All Starting Indices of the Minimal Rotation
**Description:** If a string is periodic (e.g., `abcabc`), the minimal rotation appears multiple times. This returns an array of all valid starting indices in $O(N)$.

```cpp
vector<int> all_min_rotations(string s) {
    int n = s.size();
    if (n == 0) return {};
    
    int k = Booth::min_rotation_index(s);
    string min_rot = s.substr(k) + s.substr(0, k);
    
    // Compute KMP pi array for the minimal rotation
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && min_rot[i] != min_rot[j]) j = pi[j - 1];
        if (min_rot[i] == min_rot[j]) j++;
        pi[i] = j;
    }
    
    int period = n - pi[n - 1];
    if (n % period != 0) period = n; // Not perfectly periodic
    
    vector<int> starts;
    for (int i = 0; i < n; i += period) {
        starts.push_back((k + i) % n);
    }
    sort(starts.begin(), starts.end());
    return starts;
}
```

---

## 1.8 Check if a String is a Lyndon Word
**Description:** A string is a Lyndon word if it is strictly smaller than all of its proper suffixes. Using Duval's factorization, $S$ is a Lyndon word if and only if its factorization consists of exactly one factor of length $|S|$.

```cpp
bool is_lyndon_word(const string& s) {
    if (s.empty()) return false;
    int n = s.size(), i = 0, factors = 0, last_len = 0;
    while (i < n) {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) {
            last_len = j - k;
            i += last_len;
            factors++;
        }
    }
    return factors == 1 && last_len == n;
}
```

## 1.9 Generate All Lyndon Words (FKM Algorithm)
**Description:** The Fredricksen-Kessler-Maiorana (FKM) algorithm generates all Lyndon words whose length divides $N$ over an alphabet of size $K$, in pure lexicographical order. 
- **Time Complexity:** $O(1)$ amortized time per generated word!

```cpp
// k_alphabet = 2 for binary ('a', 'b'), 26 for lowercase English
vector<string> generate_lyndon_words(int n, int k_alphabet) {
    vector<string> res;
    vector<int> a(n + 1, 0); // 1-indexed state array
    int i = 1;
    while (i != 0) {
        if (n % i == 0) {
            string s = "";
            for (int j = 1; j <= i; j++) s += (char)('a' + a[j]);
            res.push_back(s);
        }
        i = n;
        while (i > 0 && a[i] == k_alphabet - 1) i--;
        if (i > 0) {
            a[i]++;
            for (int j = i + 1; j <= n; j++) {
                a[j] = a[j - i];
            }
        }
    }
    return res;
}
```

## 1.10 Count Exact Lyndon Words of Length $N$ (Möbius Inversion)
**Description:** Combines combinatorics and Number Theory to mathematically calculate the total number of distinct Lyndon words of exact length $N$ over an alphabet of size $K$. 
- **Formula:** $L(N, K) = \frac{1}{N} \sum_{d | N} \mu(d) K^{N/d}$
- **Time Complexity:** $O(\sqrt{N})$

```cpp
long long power(long long base, long long exp) {
    long long res = 1;
    while (exp > 0) {
        if (exp % 2 == 1) res *= base;
        base *= base;
        exp /= 2;
    }
    return res;
}

long long count_lyndon_words(int n, int k_alphabet) {
    // Computes the Mobius function mu(x)
    auto get_mu = [](int x) {
        int res = 1;
        for (int i = 2; i * i <= x; i++) {
            if (x % i == 0) {
                res = -res;
                x /= i;
                if (x % i == 0) return 0;
            }
        }
        if (x > 1) res = -res;
        return res;
    };
    
    long long total = 0;
    for (int d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            total += get_mu(d) * power(k_alphabet, n / d);
            if (d * d != n) { // If d is not the square root
                total += get_mu(n / d) * power(k_alphabet, d);
            }
        }
    }
    return total / n;
}
```
