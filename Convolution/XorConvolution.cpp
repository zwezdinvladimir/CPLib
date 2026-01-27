#include<bits/stdc++.h>

template<typename T>
void hadamar(std::vector<T>& a) {
    for (int h = a.size() / 2; h >= 1; h >>= 1) {
        for (int s = 0; s < a.size(); s += 2 * h) {
            for (int i = 0; i < h; ++i) {
                T x = a[s + i], y = a[s + i + h];
                a[s + i] = x + y;
                a[s + i + h] = x - y;
            }
        }
    }
}
template<typename T>
void inv_hadamar(std::vector<T>& a) {
    for (int h = 1; h < a.size(); h <<= 1) {
        for (int s = 0; s < a.size(); s += 2 * h) {
            for (int i = 0; i < h; ++i) {
                T x = a[s + i], y = a[s + i + h];
                a[s + i] = x + y;
                a[s + i + h] = x - y;
            }
        }
    }
    for (auto& i : a) i /= a.size();
}
template<typename T>
std::vector<T> xor_conv(std::vector<T> a, std::vector<T> b) {
    hadamar(a), hadamar(b);
    for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
    inv_hadamar(a);
    return a;
}

int main() {
    int k;
    std::cin >> k;
    int n = 1 << k;
    std::vector<int> a(n), b(n);
    for (auto& i : a) std::cin >> i;
    for (auto& i : b) std::cin >> i;
    auto c = xor_conv(a, b);
    for (auto& i : c) std::cout << i << ' ';
}