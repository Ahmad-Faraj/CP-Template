#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

using ll = long long;

#define ordered_set tree<ll, null_type, less_equal<ll>, rb_tree_tag, tree_order_statistics_node_update>

int main() {
	ordered_set os;

	os.insert(5);
	os.insert(3);
	os.insert(5);

	cout << "Ascending multiset order_of_key(5): " << os.order_of_key(5) << "\n";
	cout << "Ascending multiset 2nd element: " << *os.find_by_order(1) << "\n";

	return 0;
}
