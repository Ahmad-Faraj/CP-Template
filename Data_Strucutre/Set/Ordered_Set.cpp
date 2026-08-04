#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
#define ordered_set tree<int, null_type, less_equal<>, rb_tree_tag, tree_order_statistics_node_update>

int main() {
    ordered_set st;
    st.insert(1);
    st.insert(2);
    cout << *st.find_by_order(0) << endl; /// k th smallest element
    cout << st.order_of_key(2) << endl;   /// number of elements less than k
}