#include<bits/stdc++.h>

using ll = long long;

std::vector<int> max_independent_set(int n, std::vector<std::pair<int, int>> edges) {
    assert(n <= 60);
    std::vector<ll> adj(n);
    for (auto [u, v] : edges) {
        adj[u] |= 1ll << v;
        adj[v] |= 1ll << u;
    }
    auto fst = [&](ll msk) -> int {
        return __builtin_ctzll(msk);
    };
    auto size = [&](ll msk) -> int {
        return __builtin_popcountll(msk);
    };
    auto rec = [&](auto rec, ll msk) -> ll {
        if (msk == 0) return 0;
        int u = -1;
        for (ll cur = msk; cur > 0; ) {
            int v = fst(cur);
            cur ^= 1ll << v;
            if (u == -1 || size(adj[v] & msk) > size(adj[u] & msk)) u = v;
        }
        if (size(adj[u] & msk) <= 2) {
            ll used = 0;
            ll ans = 0;
            auto go = [&](int a, int b) -> void {
                ans |= 1ll << a;
                used |= 1ll << a;
                for (int i = 2; size(adj[b] & msk) >= 2 && !(used >> b & 1); ++i) {
                    used |= 1ll << b;
                    int c = fst(adj[b] & msk);
                    if (c == a) c = fst((adj[b] & msk) ^ (1ll << c));
                    if (i % 2 == 0) ans |= 1ll << c;
                    std::tie(a, b) = {b, c};
                }
                if (used >> b & 1) {
                    ans &= ~(1ll << a);
                } else {
                    used |= 1ll << b;
                }
            };
            for (ll cur = msk; cur > 0; ) {
                int v = fst(cur);
                cur ^= 1ll << v;
                if (used >> v & 1) continue;
                if (size(adj[v] & msk) == 0) {
                    ans |= 1ll << v;
                    used |= 1ll << v;
                }
                if (size(adj[v] & msk) == 1) {
                    go(v, fst(adj[v] & msk));
                }
            }
            for (ll cur = msk; cur > 0; ) {
                int v = fst(cur);
                cur ^= 1ll << v;
                if (used >> v & 1) continue;
                go(v, fst(adj[v] & msk));
            }
            return ans;
        } else {
            ll m1 = rec(rec, msk & ~(1ll << u));
            ll m2 = rec(rec, msk & ~adj[u] & ~(1ll << u)) | (1ll << u);
            if (size(m1) < size(m2)) std::swap(m1, m2);
            return m1;
        }
    };
    auto ret = rec(rec, (1ll << n) - 1);
    std::vector<int> ans;
    for (int i = 0; i < n; ++i) {
        if (ret >> i & 1) ans.push_back(i);
    }
    return ans;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::pair<int, int>> edges(m);
    for (auto& [u, v] : edges) std::cin >> u >> v;
    auto ret = max_independent_set(n, edges);
    std::cout << ret.size() << '\n';
    for (auto x : ret) std::cout << x << ' ';
}