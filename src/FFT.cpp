#include <complex>
#include <vector>

using C = complex<double>;
using namespace std;
constexpr double pi = acos(-1);

void fft(vector<C>& a, bool inv = false) {
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
        for (int k = 0; k < n; k += b << 1) {
            C w = 1;
            C dw = polar(1.0, (2 * pi) / (2 * b) * (inv ? 1 : -1));
            for (int j = 0; j < b; j++) {
                C s = a[j + k];
                C t = a[j + k + b] * w;
                a[j + k] = s + t;
                a[j + k + b] = s - t;
                assert(j + k + b <= n);
                w *= dw;
            }
        }
    }
    if (inv) {
        for (int i = 0; i < n; i++) a[i] /= n;
    }
}

vector<C> convolve_complex(vector<C> A, vector<C> B) {
    int N = A.size() + B.size() - 1;
    if (N <= 0) {
        return A.size() == 0 ? A : B;
    }
    int t = 1;
    while (t <= N) t <<= 1;
    A.resize(t, 0.0);
    B.resize(t, 0.0);
    fft(A, false);
    fft(B, false);
    for (int i = 0; i < t; i++) {
        A[i] *= B[i];
    }
    fft(A, true);
    A.resize(N);
    return A;
}

template <class T>
vector<T> convolve(vector<T> A, vector<T> B) {
    vector<C> cA(A.size()), cB(B.size());
    for (int i = 0; i < A.size(); i++) cA[i] = C(double(A[i]), 0.0);
    for (int i = 0; i < B.size(); i++) cB[i] = C(double(B[i]), 0.0);
    vector<C> c_C = convolve_complex(cA, cB);
    vector<T> res(c_C.size());
    for (int i = 0; i < c_C.size(); i++) res[i] = (c_C[i].real() + 0.5);
    return res;
}