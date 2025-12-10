#include<bits/stdc++.h>

using ll = long long;

template<int MOD>
struct ModInt {
 public:
    ModInt(int x = 0) : x(norm(x)) {}
    int val() const {
        return x;
    }
    ModInt operator-() {
        return norm(MOD - val());
    }
    ModInt& operator+=(const ModInt rhs) {
        x = norm(x + rhs.val());
        return *this;
    }
    ModInt& operator-=(const ModInt rhs) {
        x = norm(x - rhs.val());
        return *this;
    }
    ModInt& operator*=(const ModInt rhs) {
        x = (ll)x * rhs.val() % MOD;
        return *this;
    }
    friend ModInt operator+(const ModInt lhs, const ModInt rhs) {
        ModInt ret = lhs;
        return ret += rhs;
    }
    friend ModInt operator-(const ModInt lhs, const ModInt rhs) {
        ModInt ret = lhs;
        return ret -= rhs;
    }
    friend ModInt operator*(const ModInt lhs, const ModInt rhs) {
        ModInt ret = lhs;
        return ret *= rhs;
    }
    ModInt power(ll k) {
        ModInt ret = 1;
        for (ModInt x = *this; k; x *= x, k >>= 1) {
            if (k & 1) ret *= x;
        }
        return ret;
    }
    ModInt inv() {
        return power(MOD - 2);
    }
    ModInt& operator/=(ModInt rhs) {
        return *this *= rhs.inv();
    }
    friend ModInt operator/(const ModInt lhs, const ModInt rhs) {
        ModInt ret = lhs;
        return ret /= rhs;
    }
    friend std::istream& operator>>(std::istream& in, ModInt& a) {
        int x;
        in >> x;
        a = ModInt(x);
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, ModInt a) {
        out << a.val();
        return out;
    }

 private:
    int x = 0;
    int norm(int x) {
        if (x < 0) x += MOD;
        if (x >= MOD) x -= MOD;
        return x;
    }
};

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

Comb<998244353> comb;

int main() {
    std::cout << comb.factorial(3) << ' ' << comb.C(3, 2);
}