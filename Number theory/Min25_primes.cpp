#include<bits/stdc++.h>

using ll = long long;
template<typename R>
R min25_primes(ll n, std::function<R(ll)> f, std::function<R(ll)> pref) {
    std::vector<ll> v;
    int sq = std::sqrt(n);
    v.reserve(2 * sq + 3);
    ll k = 1;
    for (; k * k <= n; ++k) v.push_back(k);
    for (--k, k -= k * k == n; k > 0; --k) v.push_back(n / k);
    std::reverse(v.begin(), v.end());
    v.resize(std::unique(v.begin(), v.end()) - v.begin());
    std::vector<R> _dp(v.size());
    auto dp = [&](ll x) -> R& {
        if (x <= sq) return _dp[x - 1];
        else return _dp[v.size() - n / x];
    };
    for (auto x : v) dp(x) = pref(x) - 1;
    for (ll p = 2; p * p <= n; ++p) {
        if (_dp[p - 2] != _dp[p - 1]) {
            R vp = _dp[p - 2], fp = f(p);
            double ip = 1.0 / p;
            for (int i = 0; i < v.size() && v[i] >= p * p; ++i) {
                dp(v[i]) -= (dp(v[i] * ip + 1e-6) - vp) * fp;
            }
        }
    }
    return dp(n);
}

int main() {
    ll n;
    std::cin >> n;
    std::cout << min25_primes(n, std::function<ll(ll)>([&](ll x) {return 1;}), std::function<ll(ll)>([&](ll x) {return x;}));
}