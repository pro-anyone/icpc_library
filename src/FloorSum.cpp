// calculate sum_{i=0}^{n-1} floor((a*i + b) / m)
using i64 = long long;
i64 floor_sum(i64 n, i64 m, i64 a, i64 b) {
    i64 res = 0;
    if (a >= m) {
        res += n * (n - 1) / 2 * (a / m);
        a %= m;
    }
    if (b >= m) {
        res += n * (b / m);
        b %= m;
    }

    i64 last = a * n + b;
    if (last >= m) {
        res += floor_sum(last / m, a, m, last % m);
    }

    return res;
}