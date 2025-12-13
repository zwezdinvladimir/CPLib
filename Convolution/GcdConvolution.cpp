#include<bits/stdc++.h>

using ll = long long;

template<typename T>
std::vector<T> gcd_conv(std::vector<T> a, std::vector<T> b) { // 1-indexed
    std::vector<T> c(std::min(a.size(), b.size()));
    for (int i = 1; i < c.size(); ++i) {
        T sa = 0, sb = 0;
        for (int j = i; j < a.size(); j += i) sa += a[j];
        for (int j = i; j < b.size(); j += i) sb += b[j];
        c[i] = sa * sb;
    }
    for (int i = c.size() - 1; i > 0; --i) {
        for (int j = 2 * i; j < c.size(); j += i) c[i] -= c[j];
    }
    return c;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<ll> a(n + 1), b(n + 1);
    for (int i = 1; i <= n; ++i) std::cin >> a[i];
    for (int i = 1; i <= n; ++i) std::cin >> b[i];
    auto c = gcd_conv(a, b);
    for (int i = 1; i <= n; ++i) std::cout << c[i] << ' ';
}