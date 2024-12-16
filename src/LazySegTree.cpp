#include <vector>

template <class T, T (*op)(T, T), T (*e)(), class U, T (*mapping)(T, U),
          U (*composition)(U, U), U (*id)()>
struct LazySegTree {
   private:
    int n;
    int log = 0;
    vector<T> data;
    vector<U> lazy;

    inline T eval_at(int x) { return mapping(data[x], lazy[x]); }

    inline void propagate_at(int x) {
        if (lazy[x] == id()) return;
        data[x] = eval_at(x);
        if (x < n) {
            lazy[x << 1] = composition(lazy[x << 1], lazy[x]);
            lazy[(x << 1) | 1] = composition(lazy[(x << 1) | 1], lazy[x]);
        }
        lazy[x] = id();
        return;
    }

    void propagate_down(int x) {
        for (int i = log; i >= 1; i--) {
            if (x >> i & 1) {
                for (int j = i; j >= 0; j--) {
                    propagate_at(x >> j);
                }
                return;
            }
        }
    }

    inline void merge_up(int x) {
        while (x > 1) {
            x >>= 1;
            data[x] = op(eval_at(x << 1), eval_at((x << 1) | 1));
        }
    }

   public:
    LazySegTree(int _n) : LazySegTree(vector<T>(_n, e())) {}
    LazySegTree(vector<T> v) {
        while (1 << log < int(v.size())) {
            log++;
        }
        n = 1 << log;
        data.resize(n << 1, e());
        lazy.resize(n << 1, id());
        for (int i = 0; i < int(v.size()); i++) {
            data[i + n] = v[i];
        }
        for (int i = n - 1; i >= 1; i--) {
            data[i] = op(data[i << 1], data[(i << 1) | 1]);
        }
    }

    void set(int x, T value) {
        x += n;
        propagate_down(x);
        data[x] = value;
        lazy[x] = id();
        merge_up(x);
    }

    T get(int x) {
        propagate_down(x + n);
        return eval_at(x + n);
    }

    void apply(int l, int r, U value) {
        l += n;
        r += n;

        int lx = l / (l & -l);
        int rx = r / (r & -r) - 1;

        propagate_down(lx);
        propagate_down(rx);

        while (l < r) {
            if (l & 1) {
                lazy[l] = composition(lazy[l], value);
                l++;
            }
            if (r & 1) {
                r--;
                lazy[r] = composition(lazy[r], value);
            }
            l >>= 1;
            r >>= 1;
        }

        merge_up(lx);
        merge_up(rx);
    }

    T prod(int l, int r) {
        l += n;
        r += n;

        propagate_down(l / (l & -l));
        propagate_down(r / (r & -r) - 1);

        T lx = e(), rx = e();

        while (l < r) {
            if (l & 1) {
                lx = op(lx, eval_at(l));
                l++;
            }
            if (r & 1) {
                r--;
                rx = op(eval_at(r), rx);
            }
            l >>= 1;
            r >>= 1;
        }

        return op(lx, rx);
    }

    T all_prod() { return eval_at(1); }

    template <bool (*f)(T, T)>
    int min_left(int l, int r, T target) {
        if (!f(prod(l, r), target)) return r;
        l += n;
        r += n;
        propagate_down(l / (l & -l));
        propagate_down(r / (r & -r) - 1);

        while ((l & 1) == 0) l >>= 1;

        T now = e();
        while (l < r) {
            propagate_at(l >> 1);
            if (f(op(now, eval_at(l)), target)) {
                if (l >= n) return l - n;
                l <<= 1;
            } else {
                now = op(now, eval_at(l));
                if (l & 1)
                    l = (l >> 1) + 1;
                else
                    l++;
            }
        }
        return r;
    }

    template <bool f(T, T)>
    int max_right(int l, int r, T target) {
        if (!f(prod(l, r), target)) return l - 1;
        l += n;
        r += n;
        propagate_down(l / (l & -l));
        propagate_down(r / (r & -r) - 1);
        r--;

        while (r & 1) r >>= 1;

        T now = e();
        while (l < r) {
            propagate_at(r >> 1);
            if (f(op(eval_at(r), now), target)) {
                if (r >= n) return r - n;
                r = (r << 1) + 1;
            } else {
                now = op(eval_at(r), now);
                if (r & 1)
                    r--;
                else
                    r = (r >> 1) - 1;
            }
        }
        return l - 1;
    }
};