#include<bits/stdc++.h>

using ll = long long;

std::vector<int> chromatic_number(int n, std::vector<std::pair<int, int>> edges) {
    assert(n <= 60);
    std::vector<ll> adj(n);
    for (auto [u, v] : edges) {
        adj[u] |= 1ll << v;
        adj[v] |= 1ll << u;
    }
    int ans = n + 1;
    std::vector<int> best(n), cur(n);
    auto rec = [&](auto rec, int c) -> void {
        if (c >= ans) return;
        int u = -1;
        ll q = 0;
        for (int i = 0; i < n; ++i) {
            if (cur[i]) continue;
            ll k = 0;
            for (ll msk = adj[i]; msk > 0; ) {
                int j = __builtin_ctzll(msk);
                if (cur[j]) k |= 1 << cur[j];
                msk ^= 1ll << j;
            }
            if (u == -1 || __builtin_popcountll(k) > __builtin_popcountll(q)) {
                u = i, q = k;
            }
        }
        if (u == -1) {
            if (c < ans) {
                ans = c;
                best = cur;
            }
        } else {
            for (cur[u] = 1; cur[u] <= c + 1; ++cur[u]) {
                if (q >> cur[u] & 1) continue;
                rec(rec, std::max(c, cur[u]));
            }
            cur[u] = 0;
        }
    };
    rec(rec, 0);
    return best;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::pair<int, int>> edges(m);
    for (auto& [u, v] : edges) std::cin >> u >> v;
    auto ret = chromatic_number(n, edges);
    std::cout << *std::max_element(ret.begin(), ret.end());
}