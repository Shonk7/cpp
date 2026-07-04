#pragma region

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <numeric>

#define int long long
#define uint unsigned long long

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define ii int, int
#define vec vector
#define uset unordered_set
#define umap unordered_map
#define mii map<ii>
#define umii umap<ii>
#define pq priority_queue
#define pii pair<ii>
#define vi vec<int>
#define vii vec<pii>
#define vvi vec<vec<int>>
#define vvii vec<vec<pii>>

#define all(c) c.begin(), c.end()
#define sign(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))

#define __NARG__(...) __NARG_I_(__VA_ARGS__, __RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define __RSEQ_N() 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__))(__VA_ARGS__)

#define range(...) VFUNC(range, __VA_ARGS__)
#define rangei(...) VFUNC(rangei, __VA_ARGS__)
#define rrange(...) VFUNC(rrange, __VA_ARGS__)
#define get(...) VFUNC(get, __VA_ARGS__)

#define range3(i, x, n) for (int i = x; i < n; i++)
#define range4(t, i, x, n) for (t i = x; i < n; i++)
#define rangei3(i, x, n) for (int i = x; i <= n; i++)
#define rangei4(t, i, x, n) for (t i = x; i <= n; i++)
#define rrange3(i, x, n) for (int i = x; i >= n; i--)
#define rrange4(t, i, x, n) for (t i = x; i >= n; i--)
#define get1(i) \
    int i;      \
    cin >> i;
#define get2(t, i) \
    t i;           \
    cin >> i;

#define space << " "
#define newline << "\n"

#define bcount __builtin_popcountll
#define bset0(bs, k) ((bs) & ~(1 << (k)))
#define bset1(bs, k) ((bs) | (1 << (k)))
#define bset(bs, k, n) (n == 0 ? bset0(bs, k) : bset1(bs, k))
#define btog(bs, k) ((bs) ^ (1 << k))
#define bget(bs, k) ((bs) & (1 << k))

#define fastio        \
    cin.tie(nullptr); \
    cin.sync_with_stdio(false);

using namespace std;

template <typename A, typename B>
ostream& operator<<(ostream& os, const pair<A, B>& p) {
    return os << '(' << p.first << ", " << p.second << ')';
}
template <
    typename T_container,
    typename T = typename enable_if<
        !is_same<T_container, string>::value,
        typename T_container::value_type>::type>
ostream& operator<<(ostream& os, const T_container& v) {
    os << '{';
    string sep;
    for (const T& x : v) os << sep << x, sep = ", ";
    return os << '}';
}

#pragma endregion


i32 main() {
    fastio;
    // n workers, m bugs, s total budget
    get1(n); get1(m); get1(s);

    vi bugDiff(m), sk(n), cost(n);
    range(i, 0, m) cin >> bugDiff[i];
    range(i, 0, n) cin >> sk[i];
    range(i, 0, n) cin >> cost[i];

    // bugs sorted hardest-first so we greedily assign cheapest-capable worker
    vii bugs(m);
    range(i, 0, m) bugs[i] = {bugDiff[i], i};
    sort(all(bugs), greater<pii>());

    // ord: worker indices sorted by skill descending
    vi ord(n);
    iota(all(ord), 0);
    sort(all(ord), [&](int a, int b) { return sk[a] > sk[b]; });

    // check(d): can all bugs be fixed in d days within budget s?
    // With d days, we need k=ceil(m/d) workers. Group j handles bugs[j*d..(j+1)*d-1]
    // (sorted hardest-first), so group j needs a worker with skill >= bugs[j*d].diff.
    // Greedily pick the cheapest eligible worker per group (hardest group first).
    auto check = [&](int d) -> vi {
        int k = (m + d - 1) / d;
        if (k > n) return {};

        pq<pii, vii, greater<pii>> avail;
        int si = 0;
        vi chosen(k);

        range(j, 0, k) {
            int threshold = bugs[j * d].first;
            while (si < n && sk[ord[si]] >= threshold)
                avail.push({cost[ord[si]], si++});
            if (avail.empty()) return {};
            auto [c, sidx] = avail.top(); avail.pop();
            chosen[j] = sidx;
        }

        int total = 0;
        range(j, 0, k) total += cost[ord[chosen[j]]];
        if (total > s) return {};

        vi assign(m);
        range(j, 0, k) {
            int start = j * d, end = min((j + 1) * d, m);
            range(i, start, end) assign[bugs[i].second] = ord[chosen[j]] + 1;
        }
        return assign;
    };

    // binary search on d (max bugs per worker = days)
    int lo = 1, hi = m;
    vi best;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        vi res = check(mid);
        if (!res.empty()) { best = res; hi = mid - 1; }
        else lo = mid + 1;
    }

    if (best.empty()) {
        cout << "NO" newline;
    } else {
        cout << "YES" newline;
        range(i, 0, m) cout << best[i] << " \n"[i == m - 1];
    }
    return 0;
}