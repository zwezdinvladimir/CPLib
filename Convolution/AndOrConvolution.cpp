#include<bits/stdc++.h>

template<typename T>
void SOS(std::vector<T>& a) {
    for (int h = 1; h < a.size(); h <<= 1) {
        for (int i = 0; i < a.size(); ++i) {
            if (i & h) {
                a[i] += a[i ^ h];
            }
        }
    }
}
template<typename T>
void inv_SOS(std::vector<T>& a) {
    for (int h = 1; h < a.size(); h <<= 1) {
        for (int i = 0; i < a.size(); ++i) {
            if (i & h) {
                a[i] -= a[i ^ h];
            }
        }
    }
}
template<typename T>
std::vector<T> or_conv(std::vector<T> a, std::vector<T> b) {
    SOS(a), SOS(b);
    for (int i = 0; i < a.size(); ++i) a[i] *= b[i];
    inv_SOS(a);
    return a;
}
template<typename T>
std::vector<T> and_conv(std::vector<T> a, std::vector<T> b) {
    int n = a.size();
    auto flip = [&](std::vector<T>& c) -> void {
        for (int i = 0; i < n; ++i) {
            if (i < ((n - 1) ^ i)) {
                std::swap(c[i], c[(n - 1) ^ i]);
            }
        }
    };
    flip(a);
    flip(b);
    a = or_conv(a, b);
    flip(a);
    return a;
}
 
signed main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);

    
}
