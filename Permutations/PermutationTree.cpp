#include<bits/stdc++.h>

struct PermutationTree {
    int n, lgn;
    std::vector<int> p;
    std::vector<int> mn, lst, mod;

    void apply(int node, int x) {
        mn[node] += x;
        mod[node] += x;
    }
    void push(int node) {
        apply(node << 1, mod[node]);
        apply(node << 1 | 1, mod[node]);
        mod[node] = 0;
    }
    void pushall(int node) {
        for (int k = lgn; k > 0; --k) push(node >> k);
    }
    void pull(int node) {
        mn[node] = std::min(mn[node << 1], mn[node << 1 | 1]) + mod[node];
        if (mn[node] == mn[node << 1 | 1] + mod[node]) lst[node] = lst[node << 1 | 1];
        else lst[node] = lst[node << 1];
    }
    void pullall(int node) {
        for (; node > 1; pull(node >>= 1));
    }
    void build() {
        for (int i = 0; i < n; ++i) {
            mn[i + n] = 0, lst[i + n] = i;
        }
        for (int i = n - 1; i > 0; --i) {
            pull(i);
        }
    }
    void update(int l, int r, int x) {
        for (int tl = l += n, tr = r += n; tl <= tr; tl >>= 1, tr >>= 1) {
            if (tl & 1) apply(tl++, x);
            if (~tr & 1) apply(tr--, x);
        }
        pullall(l), pullall(r);
    }
    int get(int l, int r) {
        pushall(l += n), pushall(r += n);
        int ans = -1;
        for (; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) {
                if (mn[l] == 0) ans = std::max(ans, lst[l]);
                ++l;
            }
            if (~r & 1) {
                if (mn[r] == 0) ans = std::max(ans, lst[r]);
                --r;
            }
        }
        return ans;
    }

    std::vector<int> join, tl, tr, ul, ur;
    std::vector<std::vector<int>> adj;
    int rt;

    int make() {
        join.push_back(0);
        tl.push_back(0);
        tr.push_back(0);
        ul.push_back(0);
        ur.push_back(0);
        adj.push_back({});
        return adj.size() - 1;
    }

    PermutationTree() = default;

    PermutationTree(std::vector<int> p) : n(p.size()), lgn(31 - __builtin_clz(n)), p(p), mn(2 * n), lst(2 * n), mod(2 * n), join(n), tl(n), tr(n), ul(n), ur(n), adj(n) {
        std::vector<int> st = {}, mn = {-1}, mx = {-1};
        build();
        for (int i = 0; i < n; ++i) {
            update(0, i - 1, -1);
            while (mx.size() > 1 && p[mx.back()] < p[i]) {
                int k = mx.back(); mx.pop_back();
                update(mx.back() + 1, k, -p[k]);
            }
            update(mx.back() + 1, i, p[i]);
            mx.push_back(i);
            while (mn.size() > 1 && p[mn.back()] > p[i]) {
                int k = mn.back(); mn.pop_back();
                update(mn.back() + 1, k, p[k]);
            }
            update(mn.back() + 1, i, -p[i]);
            mn.push_back(i);
            tl[i] = tr[i] = i;
            ul[i] = ur[i] = p[i];
            int u = i;
            while (st.size()) {
                int p = st.back();
                if (ur[u] + 1 == ul[p] || ur[p] + 1 == ul[u]) {
                    if ((join[p] == 1 && ur[p] < ul[u]) || (join[p] == 2 && ur[u] < ul[p])) {
                        adj[p].push_back(u);
                        ul[p] = std::min(ul[p], ul[u]);
                        ur[p] = std::max(ur[p], ur[u]);
                        tr[p] = i;
                        st.pop_back();
                        u = p;
                    } else {
                        int s = make();
                        adj[s] = {p, u};
                        ul[s] = std::min(ul[u], ul[p]);
                        ur[s] = std::max(ur[u], ur[p]);
                        tl[s] = tl[p], tr[s] = i;
                        if (ur[p] < ul[u]) join[s] = 1;
                        else join[s] = 2;
                        st.pop_back();
                        u = s;
                    }
                } else {
                    int k = get(0, tl[u] - 1);
                    if (k == -1) break;
                    int s = make();
                    ul[s] = ul[u], ur[s] = ur[u];
                    tl[s] = k, tr[s] = i;
                    adj[s] = {u};
                    while (st.size() && k <= tl[st.back()]) {
                        int v = st.back(); st.pop_back();
                        adj[s].push_back(v);
                        ul[s] = std::min(ul[s], ul[v]);
                        ur[s] = std::max(ur[s], ur[v]);
                    }
                    std::reverse(adj[s].begin(), adj[s].end());
                    assert(adj[s].size() != 2);
                    u = s;
                }
            }
            st.push_back(u);
        }
        rt = st[0];
    }

    std::vector<int>& operator[](int i) {
        return adj[i];
    }
};

//solution of https://codeforces.com/contest/526/problem/F
int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);

    int n;
    std::cin >> n;
    std::vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        int r, c;
        std::cin >> r >> c;
        --r, --c;
        p[r] = c;
    }
    PermutationTree pt(p);
    std::cout << [&](this auto&& dfs, int u) -> long long {
        long long ans = 0;
        if (pt.join[u]) ans += 1ll * pt[u].size() * (pt[u].size() - 1) / 2;
        else ++ans;
        for (auto to : pt[u]) ans += dfs(to);
        return ans;
    }(pt.rt);
}