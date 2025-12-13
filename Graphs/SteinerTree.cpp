#include<bits/stdc++.h>

using ll = long long;

namespace SteinerTree {
const ll inf = 1e18;
const int K = 12, N = 100, M = 1000;
int n, m, k;
ll dp[1 << K][N];
int U[M], V[M], W[M];
std::vector<int> adj[N];

ll solve(int _n, std::vector<std::tuple<int, int, ll>> edges, std::vector<int> key) {
    n = _n, m = edges.size(), k = key.size();
    for (int i = 0; i < edges.size(); ++i) {
        std::tie(U[i], V[i], W[i]) = edges[i];
        adj[U[i]].push_back(i);
        adj[V[i]].push_back(i);
    }
    for (int i = 0; i < 1 << k; ++i) {
        std::fill(dp[i], dp[i] + n, inf);
    }
    for (int i = 0; i < k; ++i) {
        dp[1 << i][key[i]] = 0;
    }
    for (int mask = 0; mask < 1 << k; ++mask) {
        for (int sub = (mask - 1) & mask; sub > (mask ^ sub); sub = (sub - 1) & mask) {
            for (int i = 0; i < n; ++i) {
                dp[mask][i] = std::min(dp[mask][i], dp[sub][i] + dp[mask ^ sub][i]);
            }
        }
        std::priority_queue<std::pair<ll, int>, std::vector<std::pair<ll, int>>, std::greater<>> pq;
        for (int i = 0; i < n; ++i) {
            pq.emplace(dp[mask][i], i);
        }
        while (pq.size()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (dp[mask][u] != d) continue;
            for (auto id : adj[u]) {
                int to = U[id] ^ V[id] ^ u, w = W[id];
                if (dp[mask][to] > d + w) {
                    dp[mask][to] = d + w;
                    pq.emplace(dp[mask][to], to);
                }
            }
        }
    }
    return *std::min_element(dp[(1 << k) - 1], dp[(1 << k) - 1] + n);
}
std::vector<int> recover() {
    std::vector<int> ret;
    auto gen = [&](auto gen, int msk, int u) -> void {
        if (dp[msk][u] == 0) return;
        for (auto id : adj[u]) {
            int to = U[id] ^ V[id] ^ u;
            if (dp[msk][u] == dp[msk][to] + W[id]) {
                ret.push_back(id);
                gen(gen, msk, to);
                return;
            }
        }
        for (int sub = (msk - 1) & msk; sub > 0; sub = (sub - 1) & msk) {
            if (dp[sub][u] + dp[msk ^ sub][u] == dp[msk][u]) {
                gen(gen, sub, u);
                gen(gen, msk ^ sub, u);
                return;   
            }
        }
    };
    int r = std::min_element(dp[(1 << k) - 1], dp[(1 << k) - 1] + n) - dp[(1 << k) - 1];
    gen(gen, (1 << k) - 1, r);
    return ret;
}
} // namespace SteinerTree

int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
  
    int n, m;
    std::cin >> n >> m;
    std::vector<std::tuple<int, int, ll>> edges(m);
    for (auto& [u, v, w] : edges) std::cin >> u >> v >> w;
    int k;
    std::cin >> k;
    std::vector<int> key(k);
    for (auto& u : key) std::cin >> u;
    std::cout << SteinerTree::solve(n, edges, key) << ' ';
    auto ret = SteinerTree::recover();
    std::cout << ret.size() << '\n';
    for (auto x : ret) std::cout << x << ' ';
}