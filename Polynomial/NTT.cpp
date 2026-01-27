#include<bits/stdc++.h>

using ll = long long;

template<int MOD>
struct ModInt {
 public:
    int x = 0;
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
    int norm(int x) {
        if (x < 0) x += MOD;
        if (x >= MOD) x -= MOD;
        return x;
    }
};

const int MOD = 998244353;
using Mint = ModInt<MOD>;
Mint g = 3;
std::vector<int> rev;
std::vector<Mint> roots{0, 1};

void dft(std::vector<Mint> &a) {
    int n = a.size();

    if (int(rev.size()) != n) {
        int k = __builtin_ctz(n) - 1;
        rev.resize(n);
        for (int i = 0; i < n; i++) {
            rev[i] = rev[i >> 1] >> 1 | (i & 1) << k;
        }
    }

    for (int i = 0; i < n; i++) {
        if (rev[i] < i) {
            std::swap(a[i], a[rev[i]]);
        }
    }
    if (int(roots.size()) < n) {
        int k = __builtin_ctz(roots.size());
        roots.resize(n);
        while ((1 << k) < n) {
            Mint e = g.power((MOD - 1) >> (k + 1));
            for (int i = 1 << (k - 1); i < (1 << k); i++) {
                roots[i << 1] = roots[i];
                roots[i << 1 | 1] = roots[i] * e;
            }
            k++;
        }
    }
    for (int k = 1; k < n; k <<= 1) {
        for (int i = 0; i < n; i += k << 1) {
            for (int j = 0; j < k; j++) {
                Mint u = a[i + j];
                Mint v = a[i + j + k] * roots[k + j];
                a[i + j] = u + v;
                a[i + j + k] = u - v;
            }
        }
    }
}

void idft(std::vector<Mint> &a) {
    int n = a.size();
    std::reverse(a.begin() + 1, a.end());
    dft(a);
    Mint inv = (1 - MOD) / n;
    for (int i = 0; i < n; i++) {
        a[i] *= inv;
    }
}

const int magic = 50;

struct Poly {
 private:
    std::vector<Mint> a;

 public:
    Poly() = default;
    Poly(int n, Mint c = 0) : a(n, c) {}
    Poly(std::vector<Mint> b) : a(b) {}
    Poly(std::vector<Mint> xs, std::vector<Mint> ys) {
        int n = xs.size();
        std::vector<Poly> all(4 * n);
        auto precalc = [&](auto precalc, int node, int l, int r) -> void {
            if (l == r) {
                all[node] = Poly({-xs[l], Mint(1)});
            } else {
                int m = (l + r) / 2;
                precalc(precalc, node << 1, l, m);
                precalc(precalc, node << 1 | 1, m + 1, r);
                all[node] = all[node << 1] * all[node << 1 | 1];
            }
        };
        precalc(precalc, 1, 0, n - 1);
        auto P = all[1].deriv();
        auto denoms = P.eval(xs);
        for (auto& x : denoms) x = x.inv();
        auto go = [&](auto go, int node, int l, int r) -> Poly {
            if (l == r) {
                return Poly({denoms[l] * ys[l]});
            } else {
                int m = (l + r) / 2;
                auto left = go(go, node << 1, l, m);
                auto right = go(go, node << 1 | 1, m + 1, r);
                return std::move(left * all[node << 1 | 1] + right * all[node << 1]);
            }
        };
        *this = go(go, 1, 0, n - 1);
    }

    auto begin() {
        return a.begin();
    }

    auto end() {
        return a.end();
    }

    int size() {
        return a.size();
    }

    void resize(int n, Mint c = 0) {
        a.resize(n, c);
    }

    void normalize() {
        int k = (int)a.size() - 1;
        while (k >= 0 && a[k].x == 0) k--;
        resize(k + 1);
    }

    void reverse() {
        std::reverse(a.begin(), a.end());
    }

    Mint& operator[](int i) {
        return a[i];
    }

    friend Poly operator*(Poly a, Poly b) {
        if (a.size() == 0 || b.size() == 0) return Poly(0);
        int m = a.size() + b.size() - 1;
        if (std::max(a.size(), b.size()) <= magic) {
            std::vector<Mint> ret(m);
            for (int i = 0; i < a.size(); ++i) {
                for (int j = 0; j < b.size(); ++j) {
                    ret[i + j] += a[i] * b[j];
                }
            }
            return ret;
        }
        int n = 1;
        while (n < m) n <<= 1;
        a.a.resize(n, 0);
        b.a.resize(n, 0);
        dft(a.a), dft(b.a);
        for (int i = 0; i < a.size(); ++i) {
            a[i] *= b[i];
        }
        idft(a.a);
        a.resize(m);
        return a;
    }

    Poly& operator*=(Poly y) {
        return *this = *this * y;
    }

    Poly& operator*=(Mint c) {
        for (auto& i : a) i *= c;
        return *this;
    }

    friend Poly operator*(Poly x, Mint c) {
        return x *= c;
    }

    Poly& operator+=(Poly b) {
        int n = std::max(size(), b.size());
        a.resize(n);
        for (int i = 0; i < b.size(); ++i) {
            a[i] += b[i];
        }
        return *this;
    }

    friend Poly operator+(Poly x, Poly y) {
        return x += y;
    }

    Poly& operator-=(Poly b) {
        int n = std::max(size(), b.size());
        a.resize(n);
        for (int i = 0; i < b.size(); ++i) {
            a[i] -= b[i];
        }
        return *this;
    }

    friend Poly operator-(Poly x, Poly y) {
        return x -= y;
    }

    Poly modxn(int n) {
        n = std::min(n, size());
        return Poly(std::vector(a.begin(), a.begin() + n));
    }

    Poly divxn(int n) {
        if (n > size()) return Poly(0);
        return Poly(std::vector(a.begin() + n, a.end()));
    }

    Poly mulxn(int n) {
        std::vector<Mint> b(n, 0);
        b.reserve(n + size());
        for (auto x : a) b.push_back(x);
        return Poly(b);
    }

    Poly deriv() {
        Poly ret(size() - 1);
        for (int i = 1; i < size(); ++i) {
            ret[i - 1] = i * a[i];
        }
        return ret;
    }

    Poly integr() {
        Poly ret(size() + 1);
        for (int i = size() - 1; i > 0; --i) {
            ret[i] = a[i - 1] / i;
        }
        return ret;
    }

    Poly inv(int m) {
        Poly B(1);
        B[0] = a[0].inv();
        int sz = 1;
        while (sz < m) {
            sz *= 2;
            auto A = this->modxn(sz);
            int n = 2 * sz;
            B.resize(n, 0), A.resize(n, 0);
            dft(B.a), dft(A.a);
            for (int i = 0; i < n; ++i) {
                B[i] = B[i] * Mint(2) - B[i] * B[i] * A[i];
            }
            idft(B.a);
            B.resize(sz);
        }
        B = B.modxn(m);
        return B;
    }

    friend Poly operator/(Poly a, Poly b) {
        std::reverse(b.a.begin(), b.a.end());
        std::reverse(a.a.begin(), a.a.end());
        int n = a.size(), m = b.size();
        int deg = n - m + 1;
        if (deg <= 0) return Poly(0);
        a = (a * b.inv(deg)).modxn(deg);
        std::reverse(a.a.begin(), a.a.end());
        return a;
    }

    Poly& operator/=(Poly b) {
        return *this = *this / b;
    }

    friend Poly operator%(Poly a, Poly b) {
        Poly ret = (a - b * (a / b)).modxn(b.size() - 1);
        ret.normalize();
        return ret;
    }

    Poly& operator%=(Poly b) {
        return *this = *this % b;
    }

    Poly log(int m) {
        return (deriv() * inv(m)).integr().modxn(m);
    }

    Poly exp(int m) {
        Poly ret(1, 1);
        int sz = 1;
        while (sz < m) {
            sz *= 2;
            ret = (ret * (Poly(1, 1) + modxn(sz) - ret.log(sz))).modxn(sz);
        }
        return ret.modxn(m);
    }

    Mint eval(Mint x) {
        Mint ret = 0, pow = 1;
        for (int i = 0; i < size(); i++, pow *= x) {
            ret += pow * a[i];
        }
        return ret;
    }

    Poly mulT(Poly b) {
        if (b.size() == 0) {
            return Poly();
        }
        int n = b.size();
        std::reverse(b.a.begin(), b.a.end());
        return ((*this) * b).divxn(n - 1);
    }

    std::vector<Mint> eval(std::vector<Mint> x) {
        if (size() == 0) {
            return std::vector<Mint>(x.size(), 0);
        }
        const int n = std::max(int(x.size()), size());
        std::vector<Poly> q(4 * n);
        std::vector<Mint> ans(x.size());
        x.resize(n);
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                q[p] = Poly({Mint(1), -x[l]});
            } else {
                int m = (l + r) >> 1;
                build(p << 1, l, m);
                build(p << 1 | 1, m, r);
                q[p] = q[p << 1] * q[p << 1 | 1];
            }
        };
        build(1, 0, n);
        std::function<void(int, int, int, Poly)> work = [&](int p, int l, int r, Poly num) {
            if (r - l == 1) {
                if (l < int(ans.size())) {
                    ans[l] = num[0];
                }
            } else {
                int m = (l + r) >> 1;
                work(p << 1, l, m, num.mulT(q[p << 1 | 1]).modxn(m - l));
                work(p << 1 | 1, m, r, num.mulT(q[p << 1]).modxn(r - m));
            }
        };
        work(1, 0, n, mulT(q[1].inv(n)));
        return ans;
    }

    std::vector<Mint> to_vec() {
        return a;
    }
};

std::vector<Mint> chirp_z(Poly p, int n, Mint a, Mint r) {
    if (n == 0) return {};
    if (r.x == 0) {
        std::vector<Mint> ret(n, p[0]);
        ret[0] = p.eval(a);
        return ret;
    }
    for (int i = 0; i < p.size(); ++i) {
        p[i] *= a.power(i) / r.power(1ll * i * (i - 1) / 2);
    }
    p.reverse();
    Poly q(n + p.size());
    for (int i = 0; i < q.size(); ++i) {
        q[i] = r.power(1ll * i * (i - 1) / 2);
    }
    p = (p * q).divxn(p.size() - 1).modxn(n);
    for (int i = 0; i < n; ++i) {
        p[i] /= r.power(1ll * i * (i - 1) / 2);
    }
    return p.to_vec();
}

Poly inv_chirp_z(std::vector<Mint> y, Mint a, Mint r) {
    int n = y.size();
    std::vector<Mint> s(n + 1);
    s[0] = 1;
    {
        Mint p = 1;
        for (int i = 1; i <= n; ++i) {
            p *= r;
            s[i] = s[i - 1] * (1 - p);
        }
    }
    for (int i = 0; i < n; ++i) {
        Mint denom = r.power(1ll * i * (i - 1) / 2) * r.power(1ll * i * (n - i - 1)) * s[i] * s[n - i - 1];
        if (i % 2 == 1) denom *= -1;
        y[i] /= denom;
    }
    Poly p(n);
    for (int i = 0; i < n; ++i) {
        p[i] = (i == 0 ? Mint(1) : s[n].x / s[n - i] / s[i]) * r.power(1ll * i * (i - 1) / 2);
        if (i % 2 == 1) p[i] *= -1;
    }
    Poly q = chirp_z(y, n, 1, r);
    p = (p * q).modxn(n);
    p.reverse();
    for (int i = 0; i < n; ++i) {
        p[i] /= a.power(i);
    }
    return p;
}

template<typename T>
using mat = std::array<std::array<T, 2>, 2>;

template<typename T>
mat<T> operator*(mat<T> x, mat<T> y) {
    mat<T> ret;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            ret[i][j] = x[i][0] * y[0][j] + x[i][1] * y[1][j];
        }
    }
    return ret;
}


mat<Poly> half_gcd(Poly A, Poly B) {
    A.normalize(), B.normalize();
    if (B.size() <= 3) {
        mat<Poly> M1;
        M1[0][0] = M1[1][1] = Poly(1, 1);
        M1[0][1] = M1[1][0] = Poly(0);
        if (B.size() <= 0) return M1;
        auto Q = A / B;
        M1[0][0] = Poly(0);
        M1[0][1] = M1[1][0] = Poly(1, 1);
        M1[1][1] = Poly(0) - Q;
        return M1;
    }
    int d = A.size() / 4;
    auto M1 = half_gcd(A.divxn(2 * d), B.divxn(2 * d));
    auto C = A * M1[0][0] + B * M1[1][0];
    auto D = A * M1[0][1] + B * M1[1][1];
    C.normalize();
    D.normalize();
    if (D.size() == 0) return M1;
    auto Q = C / D;
    C = (C - D * Q);
    std::swap(C, D);
    C.normalize();
    D.normalize();
    mat<Poly> M2;
    M2[0][0] = M1[0][1];
    M2[0][1] = M1[0][0] - Q * M1[0][1];
    M2[1][0] = M1[1][1];
    M2[1][1] = M1[1][0] - Q * M1[1][1];
    if (D.size() == 0) return M2;
    auto M3 = half_gcd(C.divxn(d), D.divxn(d));
    return M2 * M3;
}

Poly gcd(Poly a, Poly b) {
    a.normalize(), b.normalize();
    while (b.size() > 0) {
        if (b.size() > magic) {
            auto M = half_gcd(a, b);
            auto c = a * M[0][0] + b * M[1][0];
            auto d = a * M[0][1] + b * M[1][1];
            a = c, b = d;
            a.normalize(), b.normalize();
        }
        if (!b.size()) break;
        {
            auto Q = a / b;
            mat<Poly> M;
            M[0][0] = Poly(0);
            M[0][1] = M[1][0] = Poly(1, 1);
            M[1][1] = Poly(0) - Q;
            a = (a - b * Q);
            a.normalize();
            std::swap(a, b);
        }
    }
    return a;
}

std::pair<Poly, Poly> exgcd(Poly a, Poly b) {
    std::vector<mat<Poly>> mats;
    a.normalize(), b.normalize();
    bool swap = false;
    if (a.size() < b.size()) {
        std::swap(a, b);
        swap = true;
    }
    while (b.size() > 0) {
        if (b.size() > magic) {
            auto M = half_gcd(a, b);
            mats.push_back(M);
            auto c = a * M[0][0] + b * M[1][0];
            auto d = a * M[0][1] + b * M[1][1];
            a = c, b = d;
            a.normalize(), b.normalize();
        }
        if (!b.size()) break;
        {
            auto Q = a / b;
            mat<Poly> M;
            M[0][0] = Poly(0);
            M[0][1] = M[1][0] = Poly(1, 1);
            M[1][1] = Poly(0) - Q;
            mats.push_back(M);
            a = (a - b * Q);
            a.normalize();
            std::swap(a, b);
        }
    }
    mat<Poly> ret;
    ret[0][0] = ret[1][1] = Poly(1, 1);
    ret[0][1] = ret[1][0] = Poly(0);
    for (int i = (int)mats.size() - 1; i >= 0; --i) {
        ret = mats[i] * ret;
    }
    ret[swap][0].normalize();
    return {a, ret[swap][0]};
}

Poly power(Poly a, int n, Poly b) {
    Poly ret(1, 1);
    for (; n; n >>= 1, a = a * a % b) {
        if (n & 1) {
            ret = ret * a % b;
        }
    }
    return ret;
}

std::random_device rd;
std::mt19937 rng(rd());
std::vector<Mint> _solve(Poly p) {
    if (p.size() < 2) {
        return {};
    }
    if (p.size() == 2) {
        return {-p[0] / p[1]};
    }
    Mint a = rng() % MOD;
    Poly q = power(Poly(std::vector{Mint(a), Mint(1)}), (MOD - 1) / 2, p);
    q -= Poly(1, 1);
    Poly f = gcd(p, q);
    auto lft = _solve(f), rgt = _solve(p / f);
    for (auto x : rgt) lft.push_back(x);
    return lft;
}
std::vector<Mint> find_roots(Poly f) {
    Poly q = power(std::vector<Mint>{0, 1}, MOD, f);
    q -= std::vector<Mint>{0, 1};
    return _solve(gcd(f, q));
}

int main() {
    
}