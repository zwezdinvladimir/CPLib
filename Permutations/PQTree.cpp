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

using Mint = ModInt<998244353>;

struct PQ_Tree {
    int rt;
    std::vector<int> type; // -1 = P, -2 = Q, 0 <= x < n = leaf x
    std::vector<std::vector<int>> adj;
    std::vector<int> used; // 0 = empty, 1 = partially, 2 = full
    std::vector<int> cnt;

    PQ_Tree() = default;
    PQ_Tree(int n) {
        if (n > 1) {
            type.resize(n + 1);
            adj.resize(n + 1);
            used.resize(n + 1);
            cnt.resize(n + 1);
            rt = n;
            adj[n].resize(n);
            std::iota(adj[n].begin(), adj[n].end(), 0);
            std::iota(type.begin(), type.begin() + n, 0);
            type[n] = -1;
        } else {
            rt = 0;
            adj = {{}};
            type = {0};
            used = {0};
            cnt = {0};
        }
    }

    int make(std::vector<int> vrt, int t) {
        if (vrt.size() == 1) {
            return vrt[0];
        } else {
            int u = adj.size();
            type.push_back(t);
            adj.push_back(vrt);
            cnt.push_back(0);
            used.push_back(0);
            bool all = true, none = true;
            for (auto to : vrt) {
                if (used[to] != 2) all = false;
                if (used[to] != 0) none = false;
                cnt[u] += cnt[to];
            }
            if (all) used[u] = 2;
            else if (none) used[u] = 0;
            else used[u] = 1;
            return u;
        }
    }

    bool fail = false;

    bool reduce(std::vector<int> S) {
        if (fail) return false;
        std::fill(cnt.begin(), cnt.end(), 0);
        std::fill(used.begin(), used.end(), 0);
        for (auto x : S) {
            used[x] = 2;
            cnt[x] = 1;
        }
        auto dfs = [&](auto dfs, int u) -> bool {
            if (type[u] >= 0) return cnt[u] == S.size();
            bool done = false;
            bool all = true, none = true;
            for (auto to : adj[u]) {
                if (dfs(dfs, to)) done = true;
                if (fail) return false;
                if (used[to] != 2) all = false;
                if (used[to] != 0) none = false;
                cnt[u] += cnt[to];
            }
            if (all) used[u] = 2;
            else if (none) used[u] = 0;
            else used[u] = 1;
            if (used[u] == 2 || used[u] == 0 || done) { // we are simply done there
                return cnt[u] == S.size();
            } else if (type[u] == -1) {
                std::vector<int> empty, partial, full;
                for (auto to : adj[u]) {
                    if (used[to] == 0) empty.push_back(to);
                    else if (used[to] == 1) partial.push_back(to);
                    else if (used[to] == 2) full.push_back(to);
                }
                if (partial.empty()) {
                    if (cnt[u] == S.size()) { // P2
                        adj[u] = empty;
                        int f = make(full, -1);
                        adj[u].push_back(f);
                    } else { // P3
                        adj[u].clear();
                        adj[u].push_back(make(empty, -1));
                        adj[u].push_back(make(full, -1));
                        type[u] = -2;
                    }
                } else {
                    if (cnt[u] == S.size()) { // P46
                        if (partial.size() > 2) {
                            fail = true;
                            return false;
                        }
                        int v = partial[0];
                        if (used[adj[v].back()] != 2) std::reverse(adj[v].begin(), adj[v].end());
                        if (used[adj[v].back()] != 2) {
                            fail = true;
                            return false;
                        }
                        if (full.size()) adj[v].push_back(make(full, -1));
                        if (partial.size() == 2) {
                            int w = partial[1];
                            if (used[adj[w][0]] != 2) std::reverse(adj[w].begin(), adj[w].end());
                            if (used[adj[w][0]] != 2) {
                                fail = true;
                                return false;
                            }
                            adj[v].insert(adj[v].end(), adj[w].begin(), adj[w].end());
                        }
                        if (empty.empty()) {
                            adj[u] = adj[v];
                            type[u] = type[v];
                        } else {
                            adj[u] = empty;
                            adj[u].push_back(v);
                        }
                    } else { // P5
                        if (partial.size() > 1) {
                            fail = true;
                            return false;
                        }
                        int v = partial[0];
                        if (used[adj[v].back()] != 2) std::reverse(adj[v].begin(), adj[v].end());
                        if (used[adj[v].back()] != 2) {
                            fail = true;
                            return false;
                        }
                        adj[u].clear();
                        if (empty.size()) adj[u].push_back(make(empty, -1));
                        adj[u].insert(adj[u].end(), adj[v].begin(), adj[v].end());
                        if (full.size()) adj[u].push_back(make(full, -1));
                        type[u] = -2;
                    }
                }
            } else if (type[u] == -2) { // Q234
                int L = adj[u].size(), R = 0;
                for (int i = 0; i < adj[u].size(); ++i) {
                    if (used[adj[u][i]]) {
                        L = std::min(L, i);
                        R = std::max(R, i);
                    }
                }
                if (L == R) {
                    if (cnt[u] == S.size()) {
                        return true;
                    } else {
                        if (R == adj[u].size() - 1) {
                            std::reverse(adj[u].begin(), adj[u].end());
                            std::tie(L, R) = std::pair{adj[u].size() - R - 1, adj[u].size() - L - 1};
                        }
                        if (L == 0) {
                            int v = adj[u][L];
                            if (used[v] == 1) {
                                if (used[adj[v][0]] != 2) std::reverse(adj[v].begin(), adj[v].end());
                                if (used[adj[v][0]] != 2) {
                                    fail = true;
                                    return false;
                                }
                                adj[u].erase(adj[u].begin());
                                adj[u].insert(adj[u].begin(), adj[v].begin(), adj[v].end());
                            }
                        } else {
                            fail = true;
                        }
                        return false;
                    }
                }
                bool ok = true;
                for (int i = L + 1; i < R; ++i) {
                    ok &= used[adj[u][i]] == 2;
                }
                std::vector<int> me(adj[u].begin(), adj[u].begin() + L);
                if (used[adj[u][L]] == 1) {
                    int v = adj[u][L];
                    if (used[adj[v].back()] != 2) std::reverse(adj[v].begin(), adj[v].end());
                    ok &= used[adj[v].back()] == 2;
                    me.insert(me.end(), adj[v].begin(), adj[v].end());
                } else {
                    me.push_back(adj[u][L]);
                }
                me.insert(me.end(), adj[u].begin() + L + 1, adj[u].begin() + R);
                if (used[adj[u][R]] == 1) {
                    int v = adj[u][R];
                    if (used[adj[v][0]] != 2) std::reverse(adj[v].begin(), adj[v].end());
                    ok &= used[adj[v][0]] == 2;
                    me.insert(me.end(), adj[v].begin(), adj[v].end());
                } else {
                    me.push_back(adj[u][R]);
                }
                me.insert(me.end(), adj[u].begin() + R + 1, adj[u].end());
                if (!ok) {
                    fail = true;
                    return false;
                }
                adj[u] = me;
            }
            return cnt[u] == S.size();
        };
        dfs(dfs, rt);
        return !fail;
    }

    void show() {
        auto dfs = [&](auto dfs, int u) -> void {
            if (type[u] >= 0) return;
            std::cout << u << ' ';
            if (type[u] == -1) std::cout << "P";
            else if (type[u] == -2) std::cout << "Q";
            std::cout << std::endl;
            for (auto v : adj[u]) std::cout << v << ' ';
            std::cout << std::endl;
            for (auto v : adj[u]) dfs(dfs, v);
        };
        dfs(dfs, rt);
        std::cout << std::endl;
    }
    
    std::vector<int> gen() {
        if (fail) return {};
        std::vector<int> p;
        auto dfs = [&](auto dfs, int u) -> void {
            if (type[u] >= 0) {
                p.push_back(type[u]);
            } else {
                for (auto to : adj[u]) {
                    dfs(dfs, to);
                }
            }
        };
        dfs(dfs, rt);
        return p;
    }

    Mint count() {
        if (fail) return 0;
        Mint ans = 1;
        auto dfs = [&](auto dfs, int u) -> void {
            if (type[u] >= 0) return;
            assert(adj[u].size() >= 2);
            if (type[u] == -1) {
                for (int i = 1; i <= adj[u].size(); ++i) ans *= i;
            } else {
                ans *= 2;
            }
            for (auto to : adj[u]) {
                dfs(dfs, to);
            }
        };
        dfs(dfs, rt);
        return ans;
    }
};

//https://codeforces.com/contest/1552/submission/353423256
int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);

    int n, q;
    std::cin >> n >> q;
    PQ_Tree pq(n);
    for (int i = 0; i < q; ++i) {
        int k;
        std::cin >> k;
        std::vector<int> p(k);
        for (auto& i : p) {
            std::cin >> i, --i;
        }
        pq.reduce(p);
    }
    std::cout << pq.count();
}