#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

using ll = long long;

#define ordered_multiset_asc tree<ll, null_type, less_equal<ll>, rb_tree_tag, tree_order_statistics_node_update>
#define ordered_multiset_desc tree<ll, null_type, greater_equal<ll>, rb_tree_tag, tree_order_statistics_node_update>

struct cmp {
	bool operator()(const ll &a, const ll &b) const {
		if (abs(a) != abs(b)) return abs(a) < abs(b);
		return a < b;
	}
};

#define ordered_multiset_abs tree<ll, null_type, cmp, rb_tree_tag, tree_order_statistics_node_update>

int main() {
	ordered_multiset_asc os_asc;
	os_asc.insert(5);
	os_asc.insert(3);
	os_asc.insert(5);

	cout << "Ascending multiset order_of_key(5): " << os_asc.order_of_key(5) << "\n";
	cout << "Ascending multiset 2nd element: " << *os_asc.find_by_order(1) << "\n";

	ordered_multiset_desc os_desc;
	os_desc.insert(5);
	os_desc.insert(3);
	os_desc.insert(5);

	cout << "Descending multiset order_of_key(4): " << os_desc.order_of_key(4) << "\n";
	cout << "Descending multiset largest element: " << *os_desc.find_by_order(0) << "\n";

	ordered_multiset_abs os_abs;
	os_abs.insert(-10);
	os_abs.insert(5);
	os_abs.insert(-5);
	os_abs.insert(3);

	cout << "Absolute value multiset elements: ";
	for (int i = 0; i < (int)os_abs.size(); i++) {
		cout << *os_abs.find_by_order(i) << " ";
	}
	cout << "\n";

	return 0;
}
