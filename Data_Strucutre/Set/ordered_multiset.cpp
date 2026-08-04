#include "../../core.h"
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

/*
 * Topic: Data Structures - Ordered Set / Ordered Multiset
 * Description: A GCC-specific balanced BST that supports finding the k-th smallest element
 *              and counting the number of elements strictly less than a value in O(log N).
 */

template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

template <typename T>
struct ordered_multiset {
    tree<pair<T, int>, null_type, less<pair<T, int>>, rb_tree_tag, tree_order_statistics_node_update> mst;
    int timer = 0;

    void insert(T x) {
        mst.insert({x, ++timer});
    }

    bool erase_one(T x) {
        auto it = mst.lower_bound({x, 0});
        if (it != mst.end() && it->first == x) {
            mst.erase(it);
            return true;
        }
        return false;
    }

    int erase_all(T x) {
        int count = 0;
        while (erase_one(x)) count++;
        return count;
    }

    // order_of_key(x): Returns the number of elements strictly less than x
    // Equivalent to the 0-indexed position of x if it were to be inserted.
    int order_of_key(T x) {
        return mst.order_of_key({x, 0});
    }

    // find_by_order(k): Returns the k-th smallest element (0-indexed).
    // For example, find_by_order(0) returns the minimum element.
    T find_by_order(int k) {
        auto it = mst.find_by_order(k);
        return it != mst.end() ? it->first : T();
    }

    int count(T x) {
        return mst.order_of_key({x, inf}) - mst.order_of_key({x, 0});
    }

    int lower_bound_index(T x) {
        return mst.order_of_key({x, 0});
    }

    int upper_bound_index(T x) {
        return mst.order_of_key({x, inf});
    }

    int size() {
        return mst.size();
    }

    bool empty() {
        return mst.empty();
    }

    void clear() {
        mst.clear();
        timer = 0;
    }
};
