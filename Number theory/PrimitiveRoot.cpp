#include<bits/stdc++.h>

using u64 = uint64_t;
using u128 = __uint128_t;
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
std::random_device rd;
std::mt19937_64 rng(rd());
bool is_prime(u64 n) {
    if (n == 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    Montgomery64 mt(n);
    int s = 0;
    u64 d = n - 1;
    while (~d & 1) ++s, d >>= 1;
    const int TRY = 5;
    for (int iter = 0; iter < TRY; ++iter) {
        u64 a = rng() % (n - 1) + 1;
        a = mt.power(a, d);
        if (a == mt.one) continue;
        for (int i = 0; i < s && a != mt.sub(0, mt.one); ++i, a = mt.mul(a, a));
        if (a != mt.sub(0, mt.one)) return false;
    }
    return true;
}
std::vector<std::pair<u64, int>> factorize(u64 n) {
    if (n == 1) return {};
    std::map<u64, int> mp;
    auto gen = [&](auto gen, u64 n) -> void {
        if (is_prime(n)) {
            ++mp[n];
        } else if (n % 2 == 0) {
            gen(gen, n / 2);
            ++mp[2];
        } else {
            Montgomery64 mt(n);
            while (true) {
                u64 a = rng() % n;
                auto f = [&](u64 x) -> u64 {
                    return mt.add(mt.mul(x, x), a);
                };
                u64 x = rng() % n;
                for (int i = 1; ; i <<= 1) {
                    u64 y = x, q = mt.one;
                    for (int j = 0; j < i; ++j) {
                        x = f(x);
                        q = mt.mul(q, mt.sub(y, x));
                    }
                    u64 g = std::gcd(q, n);
                    if (g == 1) continue;
                    if (g == n) break;
                    gen(gen, g);
                    gen(gen, n / g);
                    return;
                }
            }
        }
    };
    gen(gen, n);
    return std::vector<std::pair<u64, int>>(mp.begin(), mp.end());
}
u64 primitive_root(u64 p) {
    if (p == 2) return 1;
    auto fact = factorize(p - 1);
    Montgomery64 mt(p);
    while (true) {
        u64 g = rng() % (p - 1) + 1;
        bool ok = true;
        for (auto [x, y] : fact) {
            if (mt.power(g, (p - 1) / x) == mt.one) {
                ok = false;
                break;
            }
        }
        if (ok) return mt.reduce(g);
    }
}

//solution of https://judge.yosupo.jp/problem/primitive_root
int main() {
    int tests;
    std::cin >> tests;
    while (tests--) {
        long long p;
        std::cin >> p;
        std::cout << primitive_root(p) << '\n';
    }
}