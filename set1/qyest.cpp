#pragma region

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

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

#pragma endregion

// ========================================================================== //

bool canK(const vi& arr, int n, int c, int d, int k) {
    int cycleLen = k + 1;
    int take = min(n, cycleLen);

    int cycleSum = 0;
    range(i, 0, take) cycleSum += arr[i];

    int fullCycles = d / cycleLen;
    int rem = d % cycleLen;

    int total = fullCycles * cycleSum;
    range(i, 0, min(take, rem)) total += arr[i];

    return total >= c;
}

i32 main() {
    fastio;

    get(t);
    while (t--) {
        get(n);
        get(c);
        get(d);

        vi arr;
        arr.reserve(n);
        range(i, 0, n) {
            get(x);
            arr.push_back(x);
        }

        sort(all(arr), greater<int>());

        int maxSum = arr[0] * d;
        if (maxSum < c) {
            cout << "Impossible" newline;
            continue;
        }

        maxSum = 0;
        range(i, 0, min(n, d)) maxSum += arr[i];
        if (maxSum >= c) {
            cout << "Infinity" newline;
            continue;
        }

        int lo = 0;
        int hi = d;
        int ans = 0;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (canK(arr, n, c, d, mid)) {
                ans = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        cout << ans newline;
    }

    return 0;
}
