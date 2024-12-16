#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

template <class R>
struct Matrix {
    int H, W;
    std::vector<std::vector<R>> A;

    Matrix(int h, int w) : H(h), W(w), A(std::vector(h, std::vector<R>(w))) {}
    Matrix(int n) : Matrix(n, n) {}
    Matrix(const std::vector<std::vector<R>>& a)
        : H(a.size()), W(a[0].size()), A(a) {}

    inline const std::vector<R>& operator[](int i) const { return A[i]; }

    inline std::vector<R>& operator[](int i) { return A[i]; }

    void swap_row(int i, int j) { std::swap(A[i], A[j]); }

    void swap_column(int i, int j) {
        for (int k = 0; k < H; k++) {
            std::swap(A[k][i], A[k][j]);
        }
    }

    Matrix& operator+=(const Matrix& B) {
        assert(H == B.H && W == B.W);
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                A[i][j] += B[i][j];
            }
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& B) {
        assert(H == B.H && W == B.W);
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                A[i][j] += B[i][j];
            }
        }
        return *this;
    }

    Matrix& operator*=(const Matrix& B) {
        assert(W == B.H);
        std::vector C(H, std::vector<R>(B.W, R(0)));
        for (int i = 0; i < H; i++) {
            for (int k = 0; k < W; k++) {
                for (int j = 0; j < B.W; j++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        A.swap(C);
        W = B.W;
        return (*this);
    }

    Matrix operator+(const Matrix& B) const { return Matrix(*this) += B; }

    Matrix operator-(const Matrix& B) const { return Matrix(*this) -= B; }

    Matrix operator*(const Matrix& B) const { return Matrix(*this) *= B; }

    bool operator==(const Matrix& B) const {
        if (H != B.H || W != B.W) return false;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (A[i][j] != B[i][j]) return false;
            }
        }
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& B) {
        for (int i = 0; i < B.H; i++) {
            for (int j = 0; j < B.W; j++) {
                os << B[i][j] << (j == B.W - 1 ? "\n" : " ");
            }
        }
        return os;
    }

    // return {rank, det}
    static std::pair<int, R> GaussElimination(Matrix& a, int pivot_end = -1,
                                              bool diagonalize = false) {
        int h = a.H, w = a.W, rank = 0;
        if (pivot_end == -1) pivot_end = w;
        R det = 1;
        for (int j = 0; j < pivot_end; j++) {
            int idx = -1;
            for (int i = rank; i < h; i++) {
                if (a[i][j] != R(0)) {
                    idx = i;
                    break;
                }
            }
            if (idx == -1) {
                det = 0;
                continue;
            }
            if (rank != idx) {
                det = -det;
                a.swap_row(rank, idx);
            }
            det *= a[rank][j];
            if (diagonalize && a[rank][j] != R(1)) {
                R cr = R(1) / a[rank][j];
                for (int k = j; k < w; k++) a[rank][k] *= cr;
            }
            int is = diagonalize ? 0 : rank + 1;
            for (int i = is; i < h; i++) {
                if (i == rank) continue;
                if (a[i][j] != R(0)) {
                    R cr = a[i][j] / a[rank][j];
                    for (int k = j; k < w; k++) a[i][k] -= a[rank][k] * cr;
                }
            }
            rank++;
        }
        return std::make_pair(rank, det);
    }

    std::pair<std::vector<R>, std::vector<std::vector<R>>> LinearEquation(
        std::vector<R> b) {
        assert(H == (int)b.size());
        Matrix<R> M = Matrix(*this);
        for (int i = 0; i < H; i++) M[i].push_back(b[i]);
        M.W++;
        auto [rank, _] = Matrix<R>::GaussElimination(M, W, true);
        for (int i = rank; i < H; i++) {
            if (M[i][W] != R(0))
                return std::make_pair(std::vector<R>(),
                                      std::vector<std::vector<R>>());
        }
        std::vector<R> sol(W, 0);
        std::vector<std::vector<R>> basis;
        std::vector<int> pivot(W, -1);
        for (int i = 0, j = 0; i < rank; i++) {
            while (M[i][j] == R(0)) j++;
            sol[j] = M[i][W], pivot[j] = i;
        }
        for (int j = 0; j < W; j++) {
            if (pivot[j] == -1) {
                std::vector<R> x(W);
                x[j] = 1;
                for (int k = 0; k < j; k++) {
                    if (pivot[k] != -1) x[k] = -M[pivot[k]][j];
                }
                basis.emplace_back(x);
            }
        }
        return std::make_pair(sol, basis);
    }
};

template <class R>
struct SquareMatrix : Matrix<R> {
    int N;

    SquareMatrix(int n_) : Matrix<R>(n_, n_), N(n_) {}

    SquareMatrix<R> inverse() const {
        Matrix<R> m(N, 2 * N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                m[i][j] = this->A[i][j];
            }
            m[i][N + i] = R(1);
        }
        auto [rank, det] = Matrix<R>::GaussElimination(m, N, true);
        if (rank != N) return SquareMatrix<R>(0);

        SquareMatrix<R> res(N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                res[i][j] = m[i][N + j];
            }
        }
        return res;
    }

    R determinant() {
        Matrix M = Matrix(*this);
        auto [rank, det] = Matrix<R>::GaussElimination(M);
        return det;
    }

    static SquareMatrix<R> I(int n) {
        SquareMatrix<R> res(n);
        for (int i = 0; i < n; i++) res[i][i] = R(1);
        return res;
    }

    SquareMatrix<R> pow(unsigned long long x) {
        SquareMatrix<R> res = SquareMatrix<R>::I(N);
        auto a = SquareMatrix(*this);
        while (x > 0) {
            if (x & 1) res *= a;
            a *= a;
            x >>= 1;
        }
        return res;
    }

    R cofactor(int x, int y) {
        SquareMatrix<R> a(N - 1);
        for (int i = 0; i < N; i++) {
            if (i == x) continue;
            for (int j = 0; j < N; j++) {
                if (j == y) continue;
                a[i - (i > x)][j - (j > y)] = this->A[i][j];
            }
        }
        R res = a.determinant();
        if ((x + y) & 1) res = -res;
        return res;
    }
};
