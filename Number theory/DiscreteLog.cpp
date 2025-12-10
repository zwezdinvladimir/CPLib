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
    u32 power(u32 x, u64 y) {
        u32 ret = one;
        for (; y; y >>= 1, x = mul(x, x)) {
            if (y & 1) ret = mul(ret, x);
        }
        return ret;
    }
    u32 inv(u32 x) {
        return power(x, mod - 2);
    }
    u32 div(u32 x, u32 y) {
        return mul(x, inv(y));
    }
};

std::array<int, 3> gcd_ex(int a, int b) {
    int x1 = 1, y1 = 0, x2 = 0, y2 = 1;
    while (b != 0) {
        const int k = a / b;
        std::swap(b, a -= b * k);
        std::swap(x2, x1 -= x2 * k);
        std::swap(y2, y1 -= y2 * k);
    }
    return {a, x1, y1};
}

int solve_linear_modulo(int x, int y, int m) { // s * x = y mod m
    auto [g, s, k] = gcd_ex(x, m);
    if (y % g != 0) return -1;
    else return (1ll * s * (y / g) % m + m) % m;
}

int primitive_root(int p) {
    if (p == 2) return 1;
    if (p == 3) return 2;
    std::vector<int> divs;
    for (int i = 2; i * i <= p - 1; ++i) {
        if ((p - 1) % i == 0) {
            divs.push_back(i);
            divs.push_back((p - 1) / i);
        }
    }
    Montgomery mt(p);
    for (int g = mt.one; ; g = mt.add(g, mt.one)) {
        bool ok = true;
        for (auto d : divs) {
            if (mt.power(g, d) == mt.one) {
                ok = false;
                break;
            }
        }
        if (ok) return mt.reduce(g);
    }
}


std::unordered_map<int, int> who;
int discrete_log_single(int a, int b, int p) { // a^x = y mod p
    if (p == 2) return 0;
    Montgomery mt(p);
    a = mt.transform(a), b = mt.transform(b);
    const int K = 5000;
    who.clear();
    for (int i = 0, x = mt.one; i < K; ++i, x = mt.mul(x, a)) {
        who[x] = i;
    }
    int ia = mt.inv(mt.power(a, K));
    for (int i = 0, x = b; i * K < p; ++i, x = mt.mul(x, ia)) {
        if (who.find(x) != who.end()) {
            return (i * K + who[x]) % (p - 1);
        }
    }
    return -1;
}
std::vector<int> discrete_log_multi(std::vector<int> a, std::vector<int> b, int p) {
    if (p == 2) return std::vector(a.size(), 0);
    int g = primitive_root(p);
    Montgomery mt(p);
    for (auto& x : a) x = mt.transform(x);
    for (auto& x : b) x = mt.transform(x);
    g = mt.transform(g);
    const int K = 1e6;
    who.clear();
    for (int i = 0, x = mt.one; i < K; ++i, x = mt.mul(x, g)) {
        who[x] = i;
    }
    int ig = mt.inv(mt.power(g, K));
    auto get = [&](int s) -> int {
        for (int i = 0, x = s; i * K < p; ++i, x = mt.mul(x, ig)) {
            if (who.find(x) != who.end()) {
                return (i * K + who[x]) % (p - 1);
            }
        }
        assert(false);
    };
    std::vector<int> ret(a.size());
    for (int i = 0; i < a.size(); ++i) {
        int x = get(b[i]), y = get(a[i]);
        ret[i] = solve_linear_modulo(y, x, p - 1);
    }
    return ret;
}

//slow solution of https://judge.yosupo.jp/problem/kth_root_mod
int main() {
    int q;
    std::cin >> q;
    for (int i = 0; i < q; ++i) {
        int k, y, p;
        std::cin >> k >> y >> p;
        if (y == 0) {
            if (k == 0) {
                std::cout << "-1\n";
            } else {
                std::cout << "0\n";
            }
            continue;
        }
        k %= p - 1;
        if (p == 2) {
            std::cout << "1\n";
            continue;
        }
        int g = primitive_root(p);
        int s = discrete_log_single(g, y, p);
        if (s == -1) std::cout << "-1\n";
        else {
            int d = solve_linear_modulo(k, s, p - 1);
            if (d == -1) std::cout << "-1\n";
            else {
                Montgomery mt(p);
                std::cout << mt.reduce(mt.power(mt.transform(g), d)) << '\n';
            }
        }
    }
}