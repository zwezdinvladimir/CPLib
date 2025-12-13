#include<bits/stdc++.h>

using u32 = uint32_t;
using u64 = uint64_t;

struct Montgomery {
    u32 mod, mod_inv, one;

    Montgomery(u32 mod) : mod(mod), mod_inv(1) {
        for (int i = 0; i < 5; ++i) {
            mod_inv *= 2 - mod * mod_inv;
        }
        one = (u64(1) << 32) % mod;
    }
    u32 norm(u32 x) const {
        return x < x - mod ? x : x - mod;
    }
    u32 transform(u32 x) const {
        return ((u64)x << 32) % mod;
    }
    u32 reduce(u64 x) const {
        u32 m = (u32(x) * mod_inv * u64(mod)) >> 32;
        return norm((x >> 32) + mod - m);
    }
    u32 add(u32 x, u32 y) const {
        return x + y >= mod ? x + y - mod : x + y;
    }
    u32 sub(u32 x, u32 y) const {
        return x >= y ? x - y : x + mod - y;
    }
    u32 mul(u32 x, u32 y) const {
        return reduce((u64)x * y);
    }
};

std::random_device rd;
std::mt19937 rng(rd());

int sqrt_mod(int x, int p) {
    if (x == 0) return 0;
    if (p == 2) return 1;
    Montgomery mt(p);
    auto legendre = [&](int y) -> int {
        int ret = mt.one;
        for (int k = (p - 1) / 2; k; k >>= 1, y = mt.mul(y, y)) {
            if (k & 1) ret = mt.mul(ret, y);
        }
        return ret;
    };
    x = mt.transform(x);
    if (legendre(x) != mt.one) return -1;
    int a, b;
    do {
        a = rng() % (p - 1) + 1;
        b = mt.sub(mt.mul(a, a), x);
    } while (legendre(b) == mt.one);
    if (b == 0) return mt.reduce(a);
    auto mul = [&](std::pair<int, int> x, std::pair<int, int> y) -> std::pair<int, int> {
        return {mt.add(mt.mul(x.first, y.first), mt.mul(b, mt.mul(x.second, y.second))), mt.add(mt.mul(x.second, y.first), mt.mul(x.first, y.second))};
    };
    int k = (p + 1) / 2;
    std::pair<int, int> ret = {mt.one, 0};
    for (std::pair<int, int> x = {a, mt.one}; k; k >>= 1, x = mul(x, x)) {
        if (k & 1) ret = mul(ret, x);
    }
    assert(ret.second == 0);
    return mt.reduce(ret.first);
}

int main() {
    int tests;
    std::cin >> tests;
    while (tests--) {
        int y, p;
        std::cin >> y >> p;
        std::cout << sqrt_mod(y, p) << '\n';
    }
}