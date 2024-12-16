template <int mod>
struct ModInt {
    int x;

    ModInt() : x(0) {}

    ModInt(int64_t y) : x(y >= 0 ? y % mod : (mod - (-y) % mod) % mod) {}

    ModInt& operator+=(const ModInt& p) {
        if ((x += p.x) >= mod) x -= mod;
        return *this;
    }

    ModInt& operator-=(const ModInt& p) {
        if ((x += mod - p.x) >= mod) x -= mod;
        return *this;
    }

    ModInt& operator*=(const ModInt& p) {
        x = (int)(1LL * x * p.x % mod);
        return *this;
    }

    ModInt& operator/=(const ModInt& p) {
        *this *= p.inverse();
        return *this;
    }

    ModInt operator-() const { return ModInt(-x); }
    ModInt operator+() const { return ModInt(*this); }

    ModInt operator+(const ModInt& p) const { return ModInt(*this) += p; }

    ModInt operator-(const ModInt& p) const { return ModInt(*this) -= p; }

    ModInt operator*(const ModInt& p) const { return ModInt(*this) *= p; }

    ModInt operator/(const ModInt& p) const { return ModInt(*this) /= p; }

    bool operator==(const ModInt& p) const { return x == p.x; }

    bool operator!=(const ModInt& p) const { return x != p.x; }

    ModInt inverse() const {
        int a = x, b = mod, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            swap(a -= t * b, b);
            swap(u -= t * v, v);
        }
        return ModInt(u);
    }

    ModInt pow(int64_t n) const {
        ModInt ret(1), mul(x);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }

    friend ostream& operator<<(ostream& os, const ModInt& p) {
        return os << p.x;
    }

    friend istream& operator>>(istream& is, ModInt& a) {
        int64_t t;
        is >> t;
        a = ModInt<mod>(t);
        return (is);
    }

    int get() const { return x; }

    static constexpr int get_mod() { return mod; }
};

#include <vector>

template <class Mint>
void ntt998(vector<Mint>& a, bool inv = false) {
    const int MOD = 998244353;
    int n = a.size();
    int h = 0;
    for (int i = 0; (1 << i) < n; i++) h++;
    for (int i = 0; i < n; i++) {
        int j = 0;
        for (int k = 0; k < h; k++) {
            j |= (i >> k & 1) << (h - 1 - k);
        }
        if (i < j) swap(a[i], a[j]);
    }
    for (int b = 1; b < n; b <<= 1) {
        Mint dw = Mint(3).pow((MOD - 1) / (b << 1));
        if (inv) dw = dw.pow(MOD - 2);
        for (int k = 0; k < n; k += b << 1) {
            Mint w = 1;
            for (int j = 0; j < b; j++) {
                Mint s = a[j + k];
                Mint t = a[j + k + b] * w;
                a[j + k] = s + t;
                a[j + k + b] = s - t;
                w = w * dw;
            }
        }
    }
    if (inv) {
        Mint t = Mint(n).pow(MOD - 2);
        for (int i = 0; i < n; i++) a[i] *= t;
    }
}

template <class Mint>
vector<Mint> convolve_Mint(vector<Mint> A, vector<Mint> B) {
    int N = A.size() + B.size() - 1;
    if (N <= 0) {
        return A.size() == 0 ? A : B;
    }
    int t = 1;
    while (t <= N) t <<= 1;
    A.resize(t, 0);
    B.resize(t, 0);
    ntt998(A, false);
    ntt998(B, false);
    for (int i = 0; i < t; i++) {
        A[i] = A[i] * B[i];
    }
    ntt998(A, true);
    A.resize(N);
    return A;
}

using mint = ModInt<998244353>;

template <class T>
vector<T> convolve(const vector<T>& A, const vector<T>& B) {
    vector<mint> cA(A.size()), cB(B.size());
    for (int i = 0; i < (int)A.size(); i++) cA[i] = A[i];
    for (int i = 0; i < (int)B.size(); i++) cB[i] = B[i];
    vector<mint> c_C = convolve_Mint(cA, cB);
    vector<T> res(c_C.size());
    for (int i = 0; i < (int)c_C.size(); i++) res[i] = c_C[i].x;
    return res;
}