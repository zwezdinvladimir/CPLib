#include<bits/stdc++.h>

using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;

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
    u32 power(u32 x, u64 y) {
        u32 ret = one;
        for (; y; y >>= 1, x = mul(x, x)) {
            if (y & 1) ret = mul(ret, x);
        }
    }
    u32 inv(u32 x) {
        return power(x, mod - 2);
    }
    u32 div(u32 x, u32 y) {
        return mul(x, inv(y));
    }
};
struct Montgomery64 {
    u64 mod, mod_inv, one;

    Montgomery64(u64 mod) : mod(mod), mod_inv(1) {
        for (int i = 0; i < 6; ++i) {
            mod_inv *= 2 - mod * mod_inv;
        }
        one = (u128(1) << 64) % mod;
    }
    u64 norm(u64 x) const {
        return x < x - mod ? x : x - mod;
    }
    u64 transform(u64 x) const {
        return ((u128)x << 64) % mod;
    }
    u64 reduce(u128 x) const {
        u64 m = (u64(x) * mod_inv * u128(mod)) >> 64;
        return norm((x >> 64) + mod - m);
    }
    u64 add(u64 x, u64 y) const {
        return x + y >= mod ? x + y - mod : x + y;
    }
    u64 sub(u64 x, u64 y) const {
        return x >= y ? x - y : x + mod - y;
    }
    u64 mul(u64 x, u64 y) const {
        return reduce((u128)x * y);
    }
    u64 power(u64 x, u64 y) {
        u64 ret = one;
        for (; y; y >>= 1, x = mul(x, x)) {
            if (y & 1) ret = mul(ret, x);
        }
        return ret;
    }
    u64 inv(u64 x) {
        return power(x, mod - 2);
    }
    u64 div(u64 x, u64 y) {
        return mul(x, inv(y));
    }
};

const int MOD = 1e9 + 7;
Montgomery mt(MOD);
Montgomery64 mt2(MOD);

int main() {
    std::cout << mt.reduce(mt.add(mt.transform(5), mt.mul(mt.transform(2), mt.transform(3)))) << '\n';
    std::cout << mt2.reduce(mt2.add(mt2.transform(5), mt2.mul(mt2.transform(2), mt2.transform(3))));
}