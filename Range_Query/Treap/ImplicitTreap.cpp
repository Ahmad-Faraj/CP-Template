/* Topic: Implicit Treap
 * Description: Implicit Treap keys elements by their positions (indices) rather than their
 *   values. Functions as a highly dynamic array, supporting insertions, deletions,
 *   and interval reversals in O(log N) time.
 * hashing. Usage: ImplicitTreap* root = nullptr; insert(root, pos, new ImplicitTreap(val)); erase(root, pos);
 *   reverse_range(root, l, r);
 */
#include "../../core.h"
static mt19937_64 RNG((unsigned)chrono::steady_clock::now().time_since_epoch().count());

int randomLongLong(int l, int r) {
    uniform_int_distribution<int> dist(l, r);
    return dist(RNG);
}

struct Info {
    int val, sum;
    int size;
    bool rev;

    Info(int k = 0) {
        val = sum = k;
        size = 1;
        rev = false;
    }
};

struct ImplicitTreap {
    int priority;
    ImplicitTreap* kids[2];
    ImplicitTreap* parent;
    Info info;

    ImplicitTreap(int k = 0) {
        info = Info(k);
        priority = randomLongLong(1, 2e9);
        kids[0] = kids[1] = nullptr;
        parent = this;
    }

    ~ImplicitTreap() {
        delete kids[0];
        delete kids[1];
    }
};

int Size(ImplicitTreap* me) { return me ? me->info.size : 0; }

void prop(ImplicitTreap* me) {
    if (!me || !me->info.rev) return;
    swap(me->kids[0], me->kids[1]);
    if (me->kids[0]) me->kids[0]->info.rev ^= 1;
    if (me->kids[1]) me->kids[1]->info.rev ^= 1;
    me->info.rev = 0;
}

void recalc(ImplicitTreap* me) {
    if (!me) return;
    me->info.size = 1;
    me->info.sum = me->info.val;
    for (int i = 0; i < 2; i++) {
        if (me->kids[i]) {
            me->info.size += me->kids[i]->info.size;
            me->info.sum += me->kids[i]->info.sum;
            me->kids[i]->parent = me;
        }
    }
}

pair<ImplicitTreap*, ImplicitTreap*> split(ImplicitTreap* me, int toLeft) {
    if (!me) return {nullptr, nullptr};
    prop(me);
    if (Size(me->kids[0]) >= toLeft) {
        auto p = split(me->kids[0], toLeft);
        me->kids[0] = p.second;
        recalc(me);
        if (p.first) p.first->parent = p.first;
        me->parent = me;
        return {p.first, me};
    } else {
        auto p = split(me->kids[1], toLeft - Size(me->kids[0]) - 1);
        me->kids[1] = p.first;
        recalc(me);
        if (p.second) p.second->parent = p.second;
        me->parent = me;
        return {me, p.second};
    }
}

ImplicitTreap* Merge(ImplicitTreap* left, ImplicitTreap* right) {
    if (!left) return right;
    if (!right) return left;
    prop(left);
    prop(right);
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

void insert(ImplicitTreap*& root, int pos, ImplicitTreap* node) {
    auto p = split(root, pos);
    root = Merge(Merge(p.first, node), p.second);
}

void erase(ImplicitTreap*& root, int pos) {
    auto p1 = split(root, pos);
    auto p2 = split(p1.second, 1);
    delete p2.first;
    root = Merge(p1.first, p2.second);
}

void reverse_range(ImplicitTreap*& root, int l, int r) {
    auto p1 = split(root, l);
    auto p2 = split(p1.second, r - l + 1);
    if (p2.first) p2.first->info.rev ^= 1;
    root = Merge(p1.first, Merge(p2.first, p2.second));
}

int query_sum(ImplicitTreap*& root, int l, int r) {
    auto p1 = split(root, l);
    auto p2 = split(p1.second, r - l + 1);
    int ans = p2.first ? p2.first->info.sum : 0;
    root = Merge(p1.first, Merge(p2.first, p2.second));
    return ans;
}
