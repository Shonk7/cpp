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

struct Node {
    int res;
};

int n_exp, sz;
Node rt[1 << 18]; // 2*sz max nodes, sz <= 2^17

Node merge(const Node& L, const Node& R, int depth) {
    bool use_or = (n_exp - 1 - depth) % 2 == 0;
    return {use_or ? (L.res | R.res) : (L.res ^ R.res)};
}

void update(int p, int v, int i, int cL, int cR, int depth) {
    if (cR - cL == 1) {
        rt[i].res = v;
        return;
    }
    int mid = (cL + cR) / 2;
    if (p < mid) update(p, v, 2*i, cL, mid, depth + 1);
    else         update(p, v, 2*i+1, mid, cR, depth + 1);
    rt[i] = merge(rt[2*i], rt[2*i+1], depth);
}

i32 main() {
    fastio;

    int m;
    cin >> n_exp >> m;
    sz = 1 << n_exp;

    range(i, 0, sz) {
        get(x);
        update(i, x, 1, 0, sz, 0);
    }

    range(_, 0, m) {
        int i, x;
        cin >> i >> x;
        update(i-1, x, 1, 0, sz, 0);
        cout << rt[1].res newline;
    }

    return 0;
}