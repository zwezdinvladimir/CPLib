#include<bits/stdc++.h>

using ll = long long;

template<typename T>
std::vector<T> lcm_conv(std::vector<T> a, std::vector<T> b) { // 1-indexed
    for (int i = a.size() - 1; i > 0; --i) {
        for (int j = 2 * i; j < a.size(); j += i) a[j] += a[i];
    }
    for (int i = b.size() - 1; i > 0; --i) {
        for (int j = 2 * i; j < b.size(); j += i) b[j] += b[i];
    }
    std::vector<T> c(std::min(a.size(), b.size()));
    for (int i = 1; i < c.size(); ++i) {
        c[i] = a[i] * b[i];
    }
    for (int i = 1; i < c.size(); ++i) {
        for (int j = 2 * i; j < c.size(); j += i) c[j] -= c[i];
    }
    return c;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<ll> a(n + 1), b(n + 1);
    for (int i = 1; i <= n; ++i) std::cin >> a[i];
    for (int i = 1; i <= n; ++i) std::cin >> b[i];
    auto c = lcm_conv(a, b);
    for (int i = 1; i <= n; ++i) std::cout << c[i] << ' ';
}