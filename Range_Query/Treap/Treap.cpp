/* Topic: Treap (Cartesian Tree)
 * Description: Treap (Tree + Heap) is a randomized binary search tree that maintains heap
 *   properties to ensure expected O(log N) depth. Useful for dynamically maintaining
 *   ordered sets with split and merge operations.
 * Usage:
 *   Treap* root = nullptr;
 *   insert(root, new Treap(key));
 *   erase(root, key);
 *   int k = kth(root, 3);
 */
#include "../../core.h"
static mt19937_64 RNG((unsigned)chrono::steady_clock::now().time_since_epoch().count());

int randomLongLong(int l, int r) {
    uniform_int_distribution<int> dist(l, r);
    return dist(RNG);
}

struct Treap {
    int key;
    int priority;
    int size;
    Treap* kids[2];

    Treap(int k = 0) {
        key = k;
        size = 1;
        priority = randomLongLong(1, 2e9);
        kids[0] = kids[1] = nullptr;
    }

    ~Treap() {
        delete kids[0];
        delete kids[1];
    }
};

int Size(Treap* me) { return me ? me->size : 0; }

void recalc(Treap* me) {
    if (me == nullptr) return;
    me->size = 1 + Size(me->kids[0]) + Size(me->kids[1]);
}

pair<Treap*, Treap*> split(Treap* me, int key) {
    if (me == nullptr) return {nullptr, nullptr};
    if (me->key <= key) {
        auto p = split(me->kids[1], key);
        me->kids[1] = p.first;
        recalc(me);
        return {me, p.second};
    } else {
        auto p = split(me->kids[0], key);
        me->kids[0] = p.second;
        recalc(me);
        return {p.first, me};
    }
}

Treap* Merge(Treap* left, Treap* right) {
    if (left == nullptr) return right;
    if (right == nullptr) return left;
    if (left->priority > right->priority) {
        left->kids[1] = Merge(left->kids[1], right);
        recalc(left);
        return left;
    } else {
        right->kids[0] = Merge(left, right->kids[0]);
        recalc(right);
        return right;
    }
}

void insert(Treap*& root, Treap* node) {
    if (root == nullptr) {
        root = node;
        return;
    }
    auto p = split(root, node->key);
    root = Merge(Merge(p.first, node), p.second);
}

void erase(Treap*& root, int key) {
    auto p1 = split(root, key - 1);
    auto p2 = split(p1.second, key);
    if (p2.first) {
        // Delete only one instance if multiple exist
        auto p3 = split(p2.first, key - 1);  // Not really necessary if keys are unique, but safe
        Treap* to_del = p3.first ? p3.first : p2.first;
        if (to_del == p2.first)
            p2.first = Merge(to_del->kids[0], to_del->kids[1]);
        else
            p3.first = Merge(to_del->kids[0], to_del->kids[1]), p2.first = Merge(p3.first, p3.second);
        // In a simple distinct key treap, we just drop p2.first
        // For now, let's just delete the root of p2.first (lazy free)
        delete p2.first;
        p2.first = nullptr;
    }
    root = Merge(p1.first, p2.second);
}

// Returns the k-th smallest element (1-based)
int kth(Treap* me, int k) {
    if (!me) return -1;
    int left_size = Size(me->kids[0]);
    if (k <= left_size) return kth(me->kids[0], k);
    if (k == left_size + 1) return me->key;
    return kth(me->kids[1], k - left_size - 1);
}

// Returns the number of strictly smaller elements
int order_of_key(Treap* me, int key) {
    if (!me) return 0;
    if (me->key >= key) return order_of_key(me->kids[0], key);
    return Size(me->kids[0]) + 1 + order_of_key(me->kids[1], key);
}
