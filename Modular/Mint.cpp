#include<iostream>

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

using Mint = ModInt<998244353>;

int main() {
    std::cout << Mint(2) / Mint(2);
}