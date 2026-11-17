#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

/** @return a random integer between 0 and INT64_MAX */
long long rng() {
	static std::mt19937 gen(
	    std::chrono::steady_clock::now().time_since_epoch().count());
	return std::uniform_int_distribution<long long>(0, INT64_MAX)(gen);
}

int main() {
	int len;
	std::cin >> len;

	std::map<int, long long> hash_vals;
	vector<int> a(len);
	for (int &i : a) {
		std::cin >> i;
		// assign a hash value to each unique number in the array
		if (!hash_vals.count(i)) { hash_vals[i] = rng(); }
	}
	vector<int> b(len);
	for (int &i : b) {
		std::cin >> i;
		if (!hash_vals.count(i)) { hash_vals[i] = rng(); }
	}

	std::set<int> seen;
	vector<long long> a_xor(len);
	for (int i = 0; i < len; i++) {
		// only add to prefix xor if not encountered before
		if (!seen.count(a[i])) {
			a_xor[i] = hash_vals[a[i]];
			seen.insert(a[i]);
		}
		if (i > 0) { a_xor[i] ^= a_xor[i - 1]; }
	}

	seen.clear();
	// do the same thing for b
	vector<long long> b_xor(len);
	for (int i = 0; i < len; i++) {
		if (!seen.count(b[i])) {
			b_xor[i] = hash_vals[b[i]];
			seen.insert(b[i]);
		}
		if (i > 0) { b_xor[i] ^= b_xor[i - 1]; }
	}

	int query_num;
	std::cin >> query_num;
	for (int q = 0; q < query_num; q++) {
		int a_set, b_set;
		std::cin >> a_set >> b_set;
		// check if the prefix xors are equal
		cout << (a_xor[--a_set] == b_xor[--b_set] ? "Yes" : "No") << '\n';
	}
}
