template<class T>
struct FenwickTree{
    int n;
    vector<T> data;

    FenwickTree(int _n) : n(_n), data(_n, 0) {}

    void add(int x, const T& w){
        assert(0 <= x && x < n);
        for (int i = x + 1; i <= n; i += i & -i) data[i - 1] += w;
    }

    T presum(int x){ // [0, x)
        T res = 0;
        for (int i = x; i > 0; i -= i & -i) res += data[i - 1];
        return res;
    }

    T sum(int l, int r){
        assert(0 <= l && l <= r && r <= n);
        return presum(r) - presum(l);
        }

    T get(int x){
        assert(0 <= x && x < n);
        return presum(x + 1) - presum(x);
        }
};