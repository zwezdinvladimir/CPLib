#pragma once

#include "Mint.h"

template<int MOD>
struct Comb {
    using Mint = ModInt<MOD>;
 public:
    Comb(int n = 1) {
        calc(n);
    }
    Mint factorial(int n) {
        if (_fact.size() <= n) {
            int m = std::max<int>(1, _fact.size());
            while (m <= n) m *= 2;
            calc(m);
        }
        return _fact[n];
    }
    Mint inv_factorial(int n) {
        if (_fact.size() <= n) {
            int m = std::max<int>(1, _fact.size());
            while (m <= n) m *= 2;
            calc(m);
        }
        return _ifact[n];
    }
    Mint C(int n, int k) {
        if (n < 0 || k < 0 || n < k) return 0;
        return factorial(n) * inv_factorial(k) * inv_factorial(n - k);
    }

 private:
    std::vector<Mint> _fact = {1}, _ifact = {1};
    void calc(int n) {
        _fact.resize(n);
        _ifact.resize(n);
        _fact[0] = 1;
        for (int i = 1; i < n; ++i) {
            _fact[i] = _fact[i - 1] * i;
        }
        _ifact.back() = _fact.back().inv();
        for (int i = n - 1; i; --i) {
            _ifact[i - 1] = _ifact[i] * i;
        }
    }
};