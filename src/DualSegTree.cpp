#include <vector>

using namespace std;

template <class T, class U, T mapping(T, U), U composition(U, U), U id()>
struct DualSegmentTree {
    int n;
    int lg;
    vector<T> data;
    vector<U> operate;

    DualSegmentTree(int _n) : DualSegmentTree(vector<T>(_n)) {}
    DualSegmentTree(int _n, T x) : DualSegmentTree(vector<T>(_n, x)) {}
    DualSegmentTree(const vector<T>& v) : lg(1) {
        while ((1 << lg) < (int)v.size()) lg++;
        n = 1 << lg;
        data = vector<T>(n);
        operate = vector<U>(n << 1, id());
        for (int i = 0; i < (int)v.size(); i++) data[i] = v[i];
    }

    void propagate_at(int idx) {
        if (operate[idx] == id()) return;
        operate[idx * 2] = composition(operate[idx * 2], operate[idx]);
        operate[idx * 2 + 1] = composition(operate[idx * 2 + 1], operate[idx]);
        operate[idx] = id();
    }

    void propagate_down(int idx) {
        for (int i = lg; i >= 1; i--) {
            if (idx >> i & 1) {
                for (int j = i; j >= 0; j--) {
                    if ((idx >> j) < n) propagate_at(idx >> j);
                }
                return;
            }
        }
    }

    void apply(int l, int r, U val) {
        assert(0 <= l && l < r && r <= n);
        l += n;
        r += n;

        int lx = l / (l & -l);
        int rx = r / (r & -r) - 1;

        propagate_down(lx);
        propagate_down(rx);

        while (l < r) {
            if (l & 1) {
                operate[l] = composition(operate[l], val);
                l++;
            }
            if (r & 1) {
                r--;
                operate[r] = composition(operate[r], val);
            }
            l >>= 1;
            r >>= 1;
        }
    }

    T get(int idx) {
        propagate_down(idx + n);
        return mapping(data[idx], operate[idx + n]);
    }
};