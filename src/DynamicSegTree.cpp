#include <algorithm>
#include <utility>
#include <vector>

const int MAX_SIZE = 1010101;

template <class T, T op(T, T), T e()>
struct DynamicSegTree {
    using u64 = unsigned long long;

    struct node {
        int left, right;
        T prod;
        T val;
        u64 pos;
        node() : node(-1, e()) {}
        node(int p, T x) : left(0), right(0), prod(x), val(x), pos(p) {}
    };

    inline u64 index(int x) { return pool[x].pos; }

    static inline int counter = 1;
    u64 n;
    int root;
    static inline node pool[MAX_SIZE];
    DynamicSegTree(u64 _n) : n(_n), root(0) {}

    void update(int t) {
        pool[t].prod = op(op(pool[pool[t].left].prod, pool[t].val),
                          pool[pool[t].right].prod);
    }

    void _set(int& t, u64 p, T& x, u64 l, u64 r) {
        if (!t) {
            t = counter++;
            pool[t] = node(p, x);
            return;
        }
        if (index(t) == p) {
            pool[t].val = x;
        } else {
            u64 mid = (l + r) >> 1;
            if (p < mid) {
                if (index(t) < p) {
                    std::swap(pool[t].pos, p);
                    std::swap(pool[t].val, x);
                }
                _set(pool[t].left, p, x, l, mid);
            } else {
                if (index(t) > p) {
                    std::swap(pool[t].pos, p);
                    std::swap(pool[t].val, x);
                }
                _set(pool[t].right, p, x, mid, r);
            }
        }
        update(t);
        return;
    }

    T _get(int& t, u64 p, u64 l, u64 r) {
        if (!t) return e();
        if (index(t) == p) return pool[t].val;
        u64 mid = (l + r) >> 1;
        if (p < mid)
            return _get(pool[t].left, p, l, mid);
        else
            return _get(pool[t].right, p, mid, r);
    }

    T _prod(int& t, u64 L, u64 R, u64 l, u64 r) {
        if (!t || R <= l || r <= L) return e();
        if (L <= l && r <= R) return pool[t].prod;
        u64 mid = (l + r) >> 1;
        T res = _prod(pool[t].left, L, R, l, mid);
        if (L <= index(t) && index(t) < R) res = op(res, pool[t].val);
        return op(res, _prod(pool[t].right, L, R, mid, r));
    }

    void set(u64 p, T x) { _set(root, p, x, 0, n); }

    T get(u64 p) { return _get(root, p, 0, n); }

    T prod(u64 l, u64 r) { return _prod(root, l, r, 0, n); }
};