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
    int sum, max;
    int lval, rval;   // leftmost and rightmost values in range
    bool inc, dec;    // is range non-decreasing / non-increasing?
};
const int N = 100100;
Node rt[1 << 18];
int n;

Node merge(const Node& L, const Node& R) {
    Node ret;
    ret.sum  = L.sum + R.sum;
    ret.max  = max(L.max, R.max);
    ret.lval = L.lval;
    ret.rval = R.rval;
    ret.inc  = L.inc && R.inc && (L.rval <= R.lval);
    ret.dec  = L.dec && R.dec && (L.rval >= R.lval);
    return ret;
}

void update(int p, int v, int i = 1, int cL = 0, int cR = n) {
    // if node is a leaf
    if (cR - cL == 1) {
        rt[i] = {v, v, v, v, true, true};
        return;
    }
    int mid = (cL + cR) / 2;
    // need to recursively update both sides
    if (p < mid) {
        update(p, v, 2*i, cL, mid);
    } else {
        update(p, v, 2*i + 1, mid, cR);
    }
    // after updating children, merge them
    rt[i] = merge(rt[2*i], rt[2*i + 1]);
}

Node query(int qL, int qR, int i = 1, int cL = 0, int cR = n) {
    // if query matches current range
    if (qL == cL && qR == cR) return rt[i];

    int mid = (cL + cR) / 2;
    // query is entirely in left child
    if (qR <= mid) return query(qL, qR, 2*i, cL, mid);
    // query is entirely in right child
    if (qL >= mid) return query(qL, qR, 2*i + 1, mid, cR);

    return merge(query(qL, min(qR, mid), 2*i, cL, mid), query(max(qL, mid), qR, 2*i + 1, mid, cR));
    
}

i32 main() {
    fastio;

    int m;
    cin >> n >> m;

    range(i, 0, n) {
        get(int, x);
        update(i, x);
    }

    range(_, 0, m) {
        char op;
        int x, y;
        cin >> op >> x >> y;
        if (op == 'U') {
            update(x-1, y);
        } else {
            Node res = query(x-1, y);  // 1-based [x,y] -> 0-based [x-1, y)
            if      (op == 'M') cout << res.max newline;
            else if (op == 'S') cout << res.sum newline;
            else if (op == 'I') cout << res.inc newline;
            else if (op == 'D') cout << res.dec newline;
        }
    }

    return 0;
}