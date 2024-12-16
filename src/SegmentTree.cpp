#include <cassert>
#include <vector>

template<class T, T (*op) (T, T), T (*e)()>
struct SegmentTree {
    private:
        std::vector<T> data;
        int _n, size;

        void update(int k) {
            data[k] = op(data[k << 1], data[k << 1 | 1]);
        }

    public:
        SegmentTree(int n) : SegmentTree(std::vector<T>(n, e())) {}
        SegmentTree(const std::vector<T>& v) : size(int(v.size())) {
            for (_n = 1; _n < size; _n *= 2);
            data.resize(2 * _n, e());
            for (int i = 0; i < size; i++) data[i + _n] = v[i];
            for (int i = _n - 1; i > 0; i--) update(i);
        }

        void set(int i, const T x) {
            assert(0 <= i && i < size);
            i += _n;
            data[i] = x;
            while (i > 1) {
                i >>= 1;
                update(i);
            }
        }

        void add(int i, const T x) {
            assert(0 <= i && i < size);
            i += _n;
            data[i] += x;
            while (i > 1) {
                i >>= 1;
                update(i);
            }
        }

        T get(int i) {
            assert(0 <= i && i < size);
            return data[i + _n];
        }

        T prod(int l, int r) {
            assert(0 <= l && l <= r && r <= size);
            T lf = e(), rf = e();
            l += _n;
            r += _n;
            while(l < r) {
                if (l & 1) lf = op(lf, data[l++]);
                if (r & 1) rf = op(data[--r], rf);
                l >>= 1;
                r >>= 1;
            }
            return op(lf, rf);
        }

        T all_prod() const { return data[1]; }

        template<bool (*f) (T, T)>
        int min_left(int l, int r, T target) {
            assert(0 <= l && l <= r && r <= size);
            if (!f(prod(l, r), target)) return r;
            l += _n;
            r += _n;

            T now = e();
            while (l < r) {
                if (f(op(now, data[l]), target)) {
                    if (l >= _n) return l - _n;
                    l <<= 1;
                }
                else {
                    now = op(now, data[l]);
                    if (l & 1) l = (l >> 1) + 1;
                    else l++;
                }
            }
        }

        template<bool (*f) (T, T)>
        int max_right(int l, int r, T target) {
            assert(0 <= l && l <= r && r <= size);
            if (!f(prod(l, r), target)) return l - 1;

            l += _n;
            r += _n;
            r--;

            T now = e();
            while (l < r) {
                if (f(op(data[r], now), target)) {
                    if (r >= _n) return r - _n;
                    r = (r << 1) + 1;
                }
                else {
                    now = op(data[r], now);
                    if (r & 1) r--;
                    else r = (r >> 1) - 1;
                }
            }
        }
};