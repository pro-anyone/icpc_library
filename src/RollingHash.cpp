#include <algorithm>
#include <string>
#include <vector>

using u64 = unsigned long long;

struct RollingHash {
    std::vector<u64> table, basepow;
    static u64 base;
    static const u64 MOD = (1ull << 61) - 1;
    static const u64 mask30 = (1ull << 30) - 1;
    static const u64 P = MOD * 4;
    static const u64 mask31 = (1ull << 31) - 1;

    u64 CalcMod(u64 x) {
        u64 res = (x >> 61) + (x & MOD);
        if (res >= MOD) res -= MOD;
        return res;
    }

    u64 Mul(u64 a, u64 b) {
        u64 au = a >> 31;
        u64 ad = a & mask31;
        u64 bu = b >> 31;
        u64 bd = b & mask31;
        u64 mid = ad * bu + au * bd;
        u64 midu = mid >> 30;
        u64 midd = mid & mask30;
        return au * bu * 2 + midu + (midd << 31) + ad * bd;
    }

    RollingHash(std::string& s) {
        int n = s.size();
        table = std::vector<u64>(n + 1);
        basepow = std::vector<u64>(n + 1, 1);
        for (int i = 0; i < n; i++) {
            basepow[i + 1] = CalcMod(Mul(basepow[i], base));
            table[i + 1] = CalcMod(Mul(table[i], base) + s[i]);
        }
    }

    u64 slice(int l, int r) {
        return CalcMod(table[r] + P - Mul(table[l], basepow[r - l]));
    }

    int lcp(int l, int r) {
        int hi = table.size() - std::max(l, r), lw = 0;
        while (hi - lw > 1) {
            int mid = (hi + lw) / 2;
            if (slice(l, l + mid) == slice(r, r + mid))
                lw = mid;
            else
                hi = mid;
        }
        return lw;
    }
};

u64 RollingHash::base = 10007;
