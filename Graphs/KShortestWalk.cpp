#include<bits/stdc++.h>

using ll = long long;

namespace KShortestWalk {
    const ll inf = 1e18;
    struct node {
        node *l = nullptr, *r = nullptr;
        int size = 0;
        ll cost;
        int u;

        node() = default;
        node(ll cost, int u) : cost(cost), u(u), size(1) {}
    };
    int size(node* t) {
        return t ? t->size : 0;
    }
    node* insert(node* t, ll cost, int u) {
        if (!t) {
            t = new node(cost, u);
        } else {
            t = new node(*t);
            if (t->cost > cost) {
                std::swap(cost, t->cost);
                std::swap(u, t->u);
            }
            if (size(t->l) < size(t->r)) {
                t->l = insert(t->l, cost, u);
            } else {
                t->r = insert(t->r, cost, u);
            }
            t->size++;
        }
        return t;
    }

    std::vector<ll> solve(int n, int k, int s, int t, std::vector<std::tuple<int, int, ll>> edges, bool directed = true) {
        std::vector<std::vector<int>> g(n), gr(n);
        for (int i = 0; i < edges.size(); ++i) {
            auto [u, v, w] = edges[i];
            g[u].push_back(i);
            gr[v].push_back(i);
            if (!directed) {
                g[v].push_back(i);
                gr[u].push_back(i);
            }
        }
        std::vector<ll> d(n, inf);
        std::vector<int> p(n, -1);
        std::vector<node*> heap(n);
        {
            std::priority_queue<std::pair<ll, int>, std::vector<std::pair<ll, int>>, std::greater<>> pq;
            pq.emplace(0, t);
            d[t] = 0;
            while (pq.size()) {
                auto [di, u] = pq.top();
                pq.pop();
                if (d[u] != di) continue;
                for (auto id : gr[u]) {
                    auto [_u, _v, w] = edges[id];
                    int to = _u ^ _v ^ u;
                    if (d[to] > d[u] + w) {
                        p[to] = id;
                        d[to] = d[u] + w;
                        heap[to] = heap[u];
                        pq.emplace(d[to], to);
                    }
                }
            }
        }
        {
            std::queue<int> que;
            que.push(t);
            while (que.size()) {
                int u = que.front();
                que.pop();
                for (auto id : g[u]) {
                    if (id != p[u]) {
                        auto [_u, _v, w] = edges[id];
                        int to = _u ^ _v ^ u;
                        heap[u] = insert(heap[u], d[to] - d[u] + w, to);
                    }
                }
                for (auto id : gr[u]) {
                    auto [_u, _v, w] = edges[id];
                    int to = _u ^ _v ^ u;
                    if (p[to] == id) {
                        heap[to] = heap[u];
                        que.push(to);
                    }
                }
            }
        }
        std::priority_queue<std::pair<ll, node*>, std::vector<std::pair<ll, node*>>, std::greater<>> pq;
        pq.emplace(d[s], new node(0, s));
        std::vector<ll> ans(k);
        for (int c = 0; c < k; ++c) {
            if (pq.empty() || pq.top().first >= inf) {
                ans[c] = -1;
            } else {
                auto [d, h] = pq.top();
                pq.pop();
                ans[c] = d;
                int v = h->u;
                if (heap[v]) {
                    pq.emplace(d + heap[v]->cost, heap[v]);
                }
                if (h->l) {
                    pq.emplace(d - h->cost + h->l->cost, h->l);
                }
                if (h->r) {
                    pq.emplace(d - h->cost + h->r->cost, h->r);
                }
            }
        }
        return ans;
    }

} // namespace KShortestWalk

int main() {
    int n, m, s, t, k;
    std::cin >> n >> m >> s >> t >> k;
    std::vector<std::tuple<int, int, ll>> edges(m);
    for (auto& [u, v, w] : edges) {
        std::cin >> u >> v >> w;
    }
    auto ret = KShortestWalk::solve(n, k, s, t, edges);
    for (auto x : ret) std::cout << x << ' ';
}