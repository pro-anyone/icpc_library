#include <cassert>
#include <utility>
#include <vector>

using i64 = long long;

i64 CalcMod(i64 a, i64 b) { return (a % b + b) % b; }

// solve ax + by = d where d = gcd(a, b)
i64 ExtendedGCD(i64 a, i64 b, i64& x, i64& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    i64 d = ExtendedGCD(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

// return (x, y) where x = r_i mod m_i, y = lcm(m_i)
// if no solutions, return (0, -1)
std::pair<i64, i64> CRT(std::vector<i64> r, std::vector<i64> m) {
    assert(r.size() == m.size());
    i64 R = 0, M = 1;
    for (int i = 0; i < (int)r.size(); i++) {
        i64 x, y;
        i64 d = ExtendedGCD(M, m[i], x, y);
        if ((r[i] - R) % d != 0) return std::make_pair(0, -1);
        i64 tmp = (r[i] - R) / d * x % (m[i] / d);
        R += M * tmp;
        M *= m[i] / d;
    }
    return std::make_pair(CalcMod(R, M), M);
}