#include<bits/stdc++.h>

using ll = long long;

namespace DirectedMST {
struct DSU {
    std::vector<int> p;
    DSU() = default;
    DSU(int n) : p(n) {
        std::iota(p.begin(), p.end(), 0);
    }
    int find(int u) {
        while (u != p[u]) u = p[u] = p[p[u]];
        return u;
    }
    bool check(int u, int v) {
        return find(u) == find(v);
    }
    void unite(int par, int u) {
        p[find(u)] = find(par);
    }
};
struct Heap {
    std::vector<int> from, to;
    std::vector<ll> w;
    std::vector<int> lc, rc;
    std::vector<ll> mod;
    Heap() = default;
    Heap(int m) : from(m), to(m), w(m), lc(m, -1), rc(m, -1), mod(m, 0) {}

    int ptr = 0;
    int make(int _from, int _to, ll _w) {
        from[ptr] = _from, to[ptr] = _to, w[ptr] = _w;
        return ptr++;
    }
    void apply(int u, ll x) {
        if (u != -1) w[u] += x, mod[u] += x;
    }
    void push(int u) {
        apply(lc[u], mod[u]), apply(rc[u], mod[u]);
        mod[u] = 0;
    }
    int merge(int u, int v) {
        if (u == -1 || (v != -1 && w[u] > w[v])) std::swap(u, v);
        int r = u;
        while (v != -1) {
            int p = u;
            u = rc[p];
            push(p);
            if (u == -1 || (v != -1 && w[u] > w[v])) std::swap(u, v);
            rc[p] = u;
            std::swap(lc[p], rc[p]);
        }
        return r;
    }
    int pop(int u) {
        push(u);
        return merge(lc[u], rc[u]);
    }
};
std::pair<ll, std::vector<int>> solve(int n, int r, std::vector<std::tuple<int, int, ll>> edges) {
    Heap pq(edges.size());
    std::vector<int> e(2 * n, -1);
    for (auto [u, v, w] : edges) {
        e[v] = pq.merge(e[v], pq.make(u, v, w));
    }
    DSU dsu(2 * n), cyc(2 * n);
    std::vector<std::vector<int>> adj(2 * n);
    std::vector<int> idx(2 * n);
    ll ans = 0;
    int sz = n;
    for (int i = 0; i < sz; ++i) {
        if (i == r) continue;
        while (e[i] != -1 && cyc.check(pq.from[e[i]], i)) e[i] = pq.pop(e[i]);
        if (e[i] == -1) return {-1, {}};
        ans += pq.w[e[i]];
        pq.apply(e[i], -pq.w[e[i]]);
        idx[i] = e[i];
        e[i] = pq.pop(e[i]);
        if (dsu.check(i, pq.from[idx[i]])) {
            dsu.unite(sz, i);
            assert(sz < 2 * n);
            for (int u = i; u != sz; u = cyc.find(pq.from[idx[u]])) {
                cyc.unite(sz, u);
                adj[sz].push_back(u);
                e[sz] = pq.merge(e[sz], e[u]);
            }
            assert(adj[sz].size() >= 2);
            ++sz;
        } else {
            dsu.unite(i, pq.from[idx[i]]);
        }
    }
    std::vector<int> tin(sz), tout(sz);
    int timer = 1;
    auto dfs = [&](auto dfs, int u) -> void {
        tin[u] = timer++;
        for (auto v : adj[u]) {
            dfs(dfs, v);
        }
        tout[u] = timer;
    };
    std::vector<int> par(n, -1);
    auto go = [&](auto go, int u, int id) -> void {
        if (u < n) {
            par[u] = pq.from[id];
        } else {
            for (auto w : adj[u]) {
                if (tin[w] <= tin[pq.to[id]] && tin[pq.to[id]] < tout[w]) {
                    go(go, w, id);
                } else {
                    go(go, w, idx[w]);
                }
            }
        }
    };
    for (int i = sz - 1; i >= 0; --i) {
        if (i != r && !tin[i]) {
            dfs(dfs, i);
            go(go, i, idx[i]);
        }
    }
    return {ans, par};
}
} // namespace DirectedMST

int main() {
    int n, m, r;
    std::cin >> n >> m >> r;
    std::vector<std::tuple<int, int, ll>> edges(m);
    for (auto& [u, v, w] : edges) std::cin >> u >> v >> w;
    auto [ans, par] = DirectedMST::solve(n, r, edges);
    std::cout << ans << '\n';
    par[r] = r;
    for (auto x : par) std::cout << x << ' ';
}