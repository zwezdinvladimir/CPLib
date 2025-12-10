#pragma once

#include<cstdint>
#include<random>
#include<chrono>
#include<cassert>

using u32 = uint32_t;
using u64 = uint64_t;

struct Montgomery {
    u32 mod, mod_inv;

    Montgomery(u32 mod) : mod(mod), mod_inv(1) {
        for (int i = 0; i < 5; ++i) {
            mod_inv *= 2 - mod * mod_inv;
        }
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

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
int sqrt_mod(int x, int p) {
    if (x == 0) return 0;
    if (p == 2) return 1;
    Montgomery mt(p);
    int one = mt.transform(1);
    int a, b;
    while (true) {
        a = rng() % (p - 1) + 1;
        b = mt.sub(mt.mul(a, a), x);
        int k = (p - 1) / 2, ret = one;
        for (int x = b; k; k >>= 1, x = mt.mul(x, x)) {
            if (k & 1) ret = mt.mul(ret, x);
        }
        if (ret != one) break;
    }
    auto mul = [&](std::pair<int, int> x, std::pair<int, int> y) -> std::pair<int, int> {
        return {mt.add(mt.mul(x.first, y.first), mt.mul(b, mt.mul(x.second, y.second))), mt.add(mt.mul(x.second, y.first), mt.mul(x.first, y.second))};
    };
    int k = (p + 1) / 2;
    std::pair<int, int> ret = {1, 0};
    for (std::pair<int, int> x = {a, 1}; k; k >>= 1, x = mul(x, x)) {
        if (k & 1) ret = mul(ret, x);
    }
    assert(ret.second == 0);
    return mt.reduce(ret.first);
}