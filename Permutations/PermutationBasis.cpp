#include<bits/stdc++.h>
     
using ll = long long;

const ll base = 1e7;
struct BigInt {
    std::vector<ll> arr;

    ll& operator[](int i) {
        while (i >= arr.size()) arr.push_back(0);
        return arr[i];
    }
    void filter() {
        ll d = 0;
        for (int i = 0; i < arr.size() || d; ++i) {
            if (i >= arr.size()) arr.push_back(0);
            arr[i] += d;
            d = arr[i] / base;
            arr[i] %= base;
        }
    }
    int size() {
        return arr.size();
    }
    BigInt() = default;
    BigInt(ll x) {
        arr = {x};
        filter();
    }
    void print() {
        int i = (int)arr.size() - 1;
        while (i >= 0 && arr[i] == 0) --i;
        if (i < 0) {
            std::cout << "0";
        } else {
            std::string s;
            while (i >= 0) {
                std::string q = std::to_string(arr[i--]);
                while (q.size() < 7) q = "0" + q;
                s += q;
            }
            while (s[0] == '0') s.erase(s.begin());
            std::cout << s;
        }
    }
};
BigInt operator*(BigInt a, BigInt b) {
    BigInt c;
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            c[i + j] += a[i] * b[j];
        }
    }
    c.filter();
    return c;
}
BigInt operator+(BigInt a, BigInt b) {
    BigInt c;
    for (int i = 0; i < a.size(); ++i) c[i] += a[i];
    for (int i = 0; i < b.size(); ++i) c[i] += b[i];
    c.filter();
    return c;
}

using P = std::vector<int>;

P operator*(P a, P b) {
    P c(a.size());
    for (int i = 0; i < a.size(); ++i) {
        c[i] = a[b[i]];
    }
    return c;
}
P inv(P a) {
    P c(a.size());
    for (int i = 0; i < a.size(); ++i) {
        c[a[i]] = i;
    }
    return c;
}
P id(int n) {
    P p(n);
    std::iota(p.begin(), p.end(), 0);
    return p;
}
struct PermutationBasis {
    int n;
    std::vector<std::vector<P>> orbit, S;

    PermutationBasis(int n) : n(n), orbit(n), S(n) {
        for (int i = 0; i < n; ++i) {
            orbit[i].resize(i + 1);
            orbit[i][i] = id(n);
        }
    }
    bool contains(P p, int i) {
        while (i > 0) {
            if (orbit[i][p[i]].empty()) return false;
            p = inv(orbit[i][p[i]]) * p;
            --i;
        }
        return true;
    }
    void insert(P p, int i) {
        if (contains(p, i)) return;
        S[i].push_back(p);
        for (int j = 0; j <= i; ++j) {
            if (!orbit[i][j].empty()) {
                extend_orbit(p * orbit[i][j], i);
            }
        }
    }
    void extend_orbit(P p, int i) {
        if (orbit[i][p[i]].empty()) {
            orbit[i][p[i]] = p;
            for (auto q : S[i]) {
                extend_orbit(q * p, i);
            }
        } else {
            insert(inv(orbit[i][p[i]]) * p, i - 1);
        }
    }
    bool contains(P p) {
        return contains(p, n - 1);
    }
    void insert(P p) {
        insert(p, n - 1);
    }

    BigInt size() {
        BigInt ret(1);
        for (int i = 0; i < n; ++i) {
            int sz = 0;
            for (int j = 0; j <= i; ++j) sz += !orbit[i][j].empty();
            ret = ret * BigInt(sz);
        }
        return ret;
    }
};

//solution of https://codeforces.com/gym/421334/problem/A
int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);

    int n, m;
    std::cin >> n >> m;
    PermutationBasis pb(n);
    for (int i = 0; i < m; ++i){
        P p(n);
        for (auto& j : p) {
            std::cin >> j, --j;
        }
        pb.insert(p);
    }
    pb.size().print();
}