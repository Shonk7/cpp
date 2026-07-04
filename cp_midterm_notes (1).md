# Programming Challenges — Midterm Reference Sheet

**Print double-sided. Sections: Setup → STL → Core Techniques → DP → Graphs → Data Structures → Checklist.**

---

## 1. Master Template

```cpp
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define all(x) x.begin(), x.end()

const ll INF = 4e18;          // for ll; use 1e9 for int
const ll MOD = 1'000'000'007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

}
```

**Gotchas:**
- `#define int long long` requires `signed main()`. Safer: use `ll` explicitly where sums/products can overflow.
- Overflow check: `n ≤ 1e5` values `≤ 1e9` → sum up to `1e14` → **needs ll**. Product of two ints → **needs ll cast**: `(ll)a * b`.
- Global arrays are zero-initialised; locals are not. Size arrays as `const int N = 100100;` (slightly over the bound).
- `'\n'` not `endl` (endl flushes).
- Read but unused input still must be read.

---

## 2. STL Cheat Sheet

| Container | Key ops | Complexity |
|---|---|---|
| `vector<int> v` | `push_back, pop_back, v[i], back()` | O(1) amortised |
| | `insert/erase(v.begin()+i)` | O(n) |
| `deque<int> dq` | `push/pop_front, push/pop_back` | O(1) |
| `stack<int> st` | `push, top, pop` | O(1) |
| `queue<int> q` | `push, front, pop` | O(1) |
| `priority_queue<int>` | `push, top, pop` (max-heap) | O(log n) |
| `priority_queue<int, vector<int>, greater<int>>` | min-heap | O(log n) |
| `set<int> s` / `map<int,int>` | `insert, erase, count, find, lower_bound` | O(log n) |
| `multiset<int> ms` | `ms.erase(ms.find(x))` erases **one** copy; `ms.erase(x)` erases **all** | O(log n) |
| `unordered_map<int,int>` | avg O(1); can be hacked to O(n) | O(1) avg |

**Algorithms:** `sort(all(v))` · `sort(v.rbegin(), v.rend())` · `stable_sort` · `reverse` · `unique` (after sort) · `min_element / max_element` (return iterators) · `accumulate(all(v), 0LL)` (note `0LL`!) · `count(a, a+n, x)` · `next_permutation` · `binary_search / lower_bound / upper_bound` (sorted only).

- `lower_bound(all(v), x)` → first element **≥ x**
- `upper_bound(all(v), x)` → first element **> x**
- Index: `lower_bound(...) - v.begin()`
- On `set`/`multiset` use **member** versions: `s.lower_bound(x)` (the free function is O(n) on sets).

**Structured bindings:** `for (auto [k, val] : mp)` · `auto [x, y] = pr;`

### Sorting & Comparators

```cpp
sort(all(v));                          // ascending
sort(all(v), greater<>());             // descending
sort(v.rbegin(), v.rend());            // descending (alt)
stable_sort(all(v));                   // preserves order of equal elements
```

**Pairs sort lexicographically by default** — `.first` then `.second`. Exploit this by choosing what goes in `.first`:

```cpp
vector<pair<int,int>> v;
sort(all(v));                          // by first asc, ties by second asc
// Stabbing trick (lectures): store as (end, start) so plain sort() sorts by end

// by second ascending:
sort(all(v), [](auto &a, auto &b){ return a.second < b.second; });

// by first asc, ties by second DESC (Russian Doll Envelopes pattern):
sort(all(v), [](auto &a, auto &b){
    if (a.first != b.first) return a.first < b.first;
    return a.second > b.second;
});
```

**Tuples also sort lexicographically:** `vector<tuple<int,int,int>> v; sort(all(v));` — access with `get<0>(t)` or `auto [a, b, c] = t;`

**Comparator for structs:**

```cpp
struct Job { int deadline, profit; };
sort(all(jobs), [](const Job &a, const Job &b){ return a.deadline < b.deadline; });
```

**Multi-key comparator via tie (cleanest):**

```cpp
sort(all(v), [](const Job &a, const Job &b){
    return tie(a.deadline, a.profit) < tie(b.deadline, b.profit);
});
// mix directions: negate the field, or swap operands for that key only
```

**Comparator rules (or you get RTE/wrong answer):**
- Must be **strict weak ordering**: return `false` for equal elements. Using `<=` in a comparator = undefined behaviour, can crash.
- Same rule applies to `priority_queue` and `set` custom comparators.

**Custom comparator on set / priority_queue:**

```cpp
auto cmp = [](const pair<int,int> &a, const pair<int,int> &b){ return a.second < b.second; };
set<pair<int,int>, decltype(cmp)> s(cmp);
priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);
// pq gotcha: comparator is REVERSED vs sort — a less-than comparator gives a MAX-heap
```

**Sort indices instead of values** (keep original array intact / need original positions):

```cpp
vector<int> idx(n);
iota(all(idx), 0);
sort(all(idx), [&](int i, int j){ return a[i] < a[j]; });
```

Or the lecture pattern: store `(value, original_index)` pairs, sort, read the indices back out (Sodor).

**Partial ops:** `nth_element(v.begin(), v.begin()+k, v.end());` — O(n) average, puts the k-th smallest at index k with smaller elements before it (median without full sort). `partial_sort` fully sorts just the first k.

### lower_bound / upper_bound on pairs

```cpp
// first pair with .first >= x (use sentinel second field)
auto it = lower_bound(all(v), make_pair(x, INT_MIN));
// all pairs with .first == x lie in [lower_bound({x, MIN}), upper_bound({x, MAX}))
```

### String ops

```cpp
string s;  getline(cin, s);            // whole line (careful mixing with cin >>: stray '\n')
s.substr(pos, len);   s.find("ab");    // find returns string::npos if absent
to_string(42);        stoi(s); stoll(s);
sort(all(s));                          // sort chars — anagram check
tolower(c); toupper(c); isdigit(c); isalpha(c);   // <cctype>
s += c;  // O(1) amortised append; NEVER s = s + c in a loop (O(n) each)
```

---

## 3. Complete Search / Backtracking

**Recognise:** tiny bounds (n ≤ ~12 for permutations, n ≤ ~20–25 for subsets), "count all / print all configurations", constraint satisfaction.

```cpp
// N-Queens style: place row by row, check vs earlier rows
int n, a[12];
bool ok(int i, int j) {
    for (int k = 0; k < i; k++)
        if (a[k] == j || i-k == a[k]-j || i-k == j-a[k]) return false;
    return true;
}
void go(int i) {
    if (i == n) { /* record/print solution */ return; }
    for (int j = 0; j < n; j++)
        if (ok(i, j)) { a[i] = j; go(i+1); }
}
```

**Subset enumeration (n ≤ 25ish):** loop `for (int mask = 0; mask < (1<<n); mask++)` and test bits.

**Problems:** N-Queens (lectures) · generate all subsets summing to K · brute-force baseline to stress-test a cleverer solution.

---

## 4. Prefix Sums / Difference Array

**Recognise prefix sums:** many range-sum queries on a **static** array; "sum of subarray [l, r]"; 2D grid sums.

```cpp
pre[0] = 0;
for (int i = 1; i <= n; i++) pre[i] = pre[i-1] + a[i];
// sum of a[l..r] (1-based, inclusive):
ll s = pre[r] - pre[l-1];
```

**Recognise difference array:** many range **updates** ("add x to [l, r]"), one final read-out.

```cpp
diff[l] += x; diff[r+1] -= x;
for (int i = 1; i <= n; i++) diff[i] += diff[i-1]; // recover array
```

**Problems:** max sum of k consecutive elements (Sum 2, lectures — see sliding window) · Robot Moves precomp (prefix + suffix move sums, lectures) · CSES Static Range Sum Queries · "n intervals each add 1, find max coverage".

---

## 5. Sliding Window / Two Pointers

**Recognise:** contiguous subarray/substring, "longest/shortest window satisfying X", fixed-size window aggregate, all values non-negative (window feasibility is monotone).

```cpp
// Fixed window of size k (Sum 2, lectures)
ll ret = -INF, sum = 0;
for (int i = 0; i < n; i++) {
    if (i >= k) sum -= a[i-k];
    sum += a[i];
    if (i >= k-1) ret = max(ret, sum);
}

// Variable window: longest window with property
int l = 0;
for (int r = 0; r < n; r++) {
    add(a[r]);
    while (bad()) remove(a[l++]);
    ans = max(ans, r - l + 1);
}
```

**Sliding window + multiset trick** (window min/max/median): keep a `multiset` of window contents, erase with `ms.erase(ms.find(x))`. You used this for sliding window median (two multisets, rebalance sizes).

**Problems:** Sum 2 (lectures) · Longest Substring Without Repeating Characters (LC 3) · Sliding Window Maximum (LC 239, monotonic deque) · Sliding Window Median (LC 480, two multisets).

---

## 6. Greedy

**Recognise:** "minimise number of X", "maximise count of non-overlapping X", exchange-argument feel — a local choice never blocks a better future. **Always sort first, then ask: by what key?**

```cpp
// Interval stabbing / activity selection: sort by END point
sort(v, v+n);                      // pairs stored (end, start)
int last = -2e9, res = 0;
for (int i = 0; i < n; i++) {
    if (v[i].second <= last) continue;  // already stabbed
    res++; last = v[i].first;           // stab at endpoint
}
```

**Common greedy keys:**
- Interval scheduling / stabbing → sort by **end**.
- Deadline scheduling (max jobs / max profit) → sort by deadline, min-heap of taken jobs; or union-find "latest free slot" (you drilled both).
- Pair small with large → sort, two pointers from both ends.
- "Take biggest available each round" → `sort greater<>()` or max-heap (Restaurants, lectures).

**Problems:** Stabbing (lectures) · Restaurants (lectures) · Psychological (lectures — greedy over which opponent value to "target", O(n²) enumeration) · CSES Movie Festival · job sequencing with deadlines (union-find variant) · Array Division-style feasibility checks.

**Warning:** if you can't state the exchange argument in one sentence, suspect DP instead.

---

## 7. Binary Search

**Recognise "on answer":** "maximise the minimum / minimise the maximum", "smallest k such that feasible(k)", feasibility is **monotone** in the answer. Write `canDo(x)` checker (usually greedy/linear), then binary search x.

```cpp
// Smallest x with f(x) true   (f: F F F T T T)
ll lo = L, hi = R, ans = -1;
while (lo <= hi) {
    ll mid = lo + (hi - lo) / 2;
    if (f(mid)) { ans = mid; hi = mid - 1; }
    else        { lo = mid + 1; }
}

// Largest x with f(x) true    (f: T T T F F F)
while (lo <= hi) {
    ll mid = lo + (hi - lo) / 2;
    if (f(mid)) { ans = mid; lo = mid + 1; }
    else        { hi = mid - 1; }
}
```

**Library:** `binary_search(a, a+n, x)` (bool) · `lower_bound` / `upper_bound` (see §2). Array must be sorted.

**Problems:** Chocolate Bar (lectures — maximise the min piece happiness; greedy cut checker) · Robot Moves (lectures — minimise replaced segment length; checker uses prefix/suffix sums + parity/range reachability) · CSES Factory Machines · Aggressive Cows (SPOJ) — maximise min gap.

**Checklist for the checker:** is `f` really monotone? Are `lo`/`hi` valid bounds? Does `canDo` run in O(n)-ish?

---

## 8. Coordinate Compression

**Recognise:** values up to 1e9/1e18 but only n ≤ 1e5 **distinct positions matter** — needed before indexing into arrays / Fenwick / segment trees.

```cpp
vector<ll> vals = a;                       // copy
sort(all(vals));
vals.erase(unique(all(vals)), vals.end()); // dedupe
// compressed index of x:
int cx = lower_bound(all(vals), x) - vals.begin();
```

Lecture alternative: `map<int,int>` version, or the pair-sort trick (`(value, original index)`, `stable_sort`, then invert) — note that version gives **unequal** compressed coords to equal values, which may or may not be what you want.

**Problems:** Fireworks with range tree (lectures — compress x-positions before RMQ) · counting inversions with Fenwick · any sweep over huge coordinates.

---

## 9. Bitmask Toolkit

**Recognise:** n ≤ ~20, "subsets", "each item used/unused", state = which elements chosen.

| Trick | Code |
|---|---|
| Set bit i | `mask \| (1 << i)` |
| Test bit i | `mask & (1 << i)` |
| Clear bit i | `mask & ~(1 << i)` |
| Toggle bit i | `mask ^ (1 << i)` |
| Lowest set bit | `mask & -mask` |
| Remove lowest set bit | `mask & (mask - 1)` |
| Popcount | `__builtin_popcount(mask)` (`ll` → `popcountll`) |
| Index of lowest bit | `__builtin_ctz(mask)` |
| Full set | `(1 << n) - 1` |

```cpp
// Enumerate all subsets of all sets: total O(3^n)
for (int set = 0; set < (1<<n); set++)
    for (int sub = set; sub; sub = (sub-1) & set)
        { /* use sub */ }
```

**Use `1LL << i` when i ≥ 31.**

---

## 10. DP Patterns

**Recognise DP:** "count the ways", "max/min value over sequences of decisions", overlapping subproblems, greedy has no clean exchange argument. **Define the state in words first**, then the transition, then base cases, then order of evaluation.

### 10.1 Top-down vs bottom-up skeleton

```cpp
// Top-down (memoisation)
int cache[N];                 // init to sentinel (e.g. -1) if 0 is a valid answer
int f(int n) {
    if (n <= 1) return 1;                 // base case
    if (cache[n] != -1) return cache[n];  // cache hit
    return cache[n] = f(n-1) + f(n-2);    // compute + store
}

// Bottom-up
f[0] = f[1] = 1;
for (int i = 2; i <= n; i++) f[i] = f[i-1] + f[i-2];
// Memory-optimised (only last k states needed): index with i % k
```

### 10.2 Knapsack — dp[i][j] = best value using first i items, capacity j

**Recognise:** items with weight+value, capacity budget, take/skip each item.

```cpp
// Bottom-up. dp zero-initialised (correct for i=0 or j=0).
for (int i = 1; i <= n; i++)
    for (int j = 0; j <= S; j++) {
        dp[i][j] = dp[i-1][j];                                  // skip item i
        if (j >= s[i]) dp[i][j] = max(dp[i][j], dp[i-1][j-s[i]] + v[i]); // take
    }
// 1D space-optimised: loop j DOWNWARDS (0/1 knapsack). Upwards = unbounded knapsack.
for (int i = 1; i <= n; i++)
    for (int j = S; j >= s[i]; j--)
        dp[j] = max(dp[j], dp[j-s[i]] + v[i]);
```

Top-down version (lectures): return `-INF` for `j < 0`, cache init to `-1`.

**Problems:** Knapsack (lectures) · subset sum ("can we hit exactly S": dp bool) · coin change count/min · partition into equal halves.

### 10.3 LIS — O(n log n)

**Recognise:** longest increasing subsequence, or "min chains to cover" (Dilworth).

```cpp
vector<int> lis;
for (int x : a) {
    auto it = lower_bound(all(lis), x);   // strictly increasing
    // use upper_bound for non-decreasing
    if (it == lis.end()) lis.push_back(x);
    else *it = x;
}
// answer = lis.size()   (lis itself is NOT the actual subsequence)
```

**Problems:** LIS (LC 300) · Russian Doll Envelopes (sort by w asc, h **desc**, LIS on h) · box stacking.

### 10.4 DP over positions with range lookup — Fireworks (lectures)

**Recognise:** dp[i] depends on max/min of dp[j] over a **range** of earlier positions. Naive O(n²); RMQ tree → O(n log n).

```cpp
// dp[i] = best score of a sequence ending at firework i
// Naive: for j < i with x[j] in [l[i], r[i]]: dp[i] = max(dp[i], dp[j] + s[i])
// Fast: max-query range tree over positions
ll res = 0;
for (int i = 1; i <= n; i++) {
    dp[i] = query(l[i], r[i] + 1) + s[i];  // best reachable predecessor
    update(x[i], dp[i]);                   // make i available as predecessor
    res = max(res, dp[i]);
}
```

### 10.5 Segment Cover — DP + min range tree (lectures)

**Recognise:** cover [0, n] with weighted segments at min cost; dp indexed by endpoint; transition = min over dp of positions a segment could extend from.

```cpp
vector<pair<int,ll>> segs[N];  // segs[e] = list of (start, cost), grouped by END
// dp[i] = min cost to cover up to i; range-min tree over dp values
for (int i = 0; i <= n; i++) {
    dp[i] = INF;
    for (auto [s, c] : segs[i]) {
        ll prev = (s == 0) ? 0 : query(s-1, i);  // any covered point in [s-1, i)
        dp[i] = min(dp[i], prev + c);
    }
    update(i, dp[i]);
}
cout << dp[n] << '\n';
```

### 10.6 Bitmask DP / TSP — dp[mask][last]

**Recognise:** n ≤ ~20, order matters, "visit every X once", state = set visited + current position. O(2ⁿ·n²).

```cpp
const int N = 20; int n, adj[N][N];
int dp[1<<N][N];   // shortest path from 0 ending at `last`, visiting bits of mask

int tsp() {
    for (int m = 0; m < (1<<n); m++) fill(dp[m], dp[m]+n, INF);
    dp[1][0] = 0;                          // start at city 0
    int ans = INF;
    for (int mask = 1; mask < (1<<n); mask++)
        for (int cur = 0; cur < n; cur++) if (mask & (1<<cur)) {
            int cdp = dp[mask][cur];
            if (cdp == INF) continue;
            if (mask == (1<<n)-1) ans = min(ans, cdp);   // (+ adj[cur][0] if must return)
            for (int nxt = 0; nxt < n; nxt++) if (!(mask & (1<<nxt)))
                dp[mask|(1<<nxt)][nxt] = min(dp[mask|(1<<nxt)][nxt], cdp + adj[cur][nxt]);
        }
    return ans;
}
```

**Problems:** TSP (lectures) · assignment problem dp[mask] · "count Hamiltonian paths" · Fish (AtCoder-style pairing).

### 10.7 Broken-profile / row-mask DP — Roof Tiling (lectures)

**Recognise:** grid n × m with **m small** (≤ ~15), constraints between adjacent rows; state = mask of current row's pattern.

```cpp
for (int set = 0; set < (1<<m); set++)
    dp[m][set] = (__builtin_popcount(set) >= k);      // base row
for (int i = m+1; i <= n; i++) {
    fill(dp[i], dp[i] + (1<<m), 0);
    for (int set = 0; set < (1<<m); set++)
        if (__builtin_popcount(set) >= k)
            dp[i][set] = dp[i-1][set>>1] + dp[i-1][(set>>1) | (1<<(m-1))];
}
ll ans = 0;
for (int set = 0; set < (1<<m); set++) ans += dp[n][set];
```

### 10.8 Column DP with running max — Building (lectures)

**Recognise:** 2D grid, per-column decision (height h + direction bit b), transition needs `max` over previous column's states — maintain a running max `M` and running column sum `S` instead of an inner loop, dropping a factor of H.

### 10.9 Counting DP with combinatorics — Rooks (lectures)

**Recognise:** count placements/configurations mod 1e9+7; sort a structural parameter first, then dp[i][j] = ways using first i rows and j placed items; multiply by choices available.

```cpp
sort(l, l+n);          // row lengths ascending — key insight
dp[0][0] = 1;
for (int i = 1; i <= n; i++)
    for (int j = 0; j <= k; j++) {
        dp[i][j] = dp[i-1][j];                              // no rook this row
        if (j > 0)
            dp[i][j] = (dp[i][j] + dp[i-1][j-1] * (l[i-1] - (j-1))) % MOD;
    }
cout << dp[n][k] << '\n';
```

**Mod arithmetic:** take `% MOD` after every add/multiply; `(a - b + MOD) % MOD` for subtraction.

---

## 11. Graphs

**Recognise:** explicit graph, grid movement, dependencies/prerequisites, "connected components", states + transitions (implicit graph).

### DFS

```cpp
vector<int> g[N];
bool vis[N];
void dfs(int u) {
    vis[u] = true;
    for (int v : g[u]) if (!vis[v]) dfs(v);
}
// On trees, use parent instead of vis:
void dfs(int u, int p) { for (int v : g[u]) if (v != p) dfs(v, u); }
```

**Use for:** connected components, cycle detection, tree DP, flood fill.

### BFS — shortest path on **unweighted** graphs

```cpp
vector<int> dist(n, -1);
queue<int> q;
dist[s] = 0; q.push(s);
while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int v : g[u]) if (dist[v] == -1) {
        dist[v] = dist[u] + 1;
        q.push(v);
    }
}
```

**Grid BFS:** `int dx[] = {1,-1,0,0}, dy[] = {0,0,1,-1};` encode cell as `x*m + y` or use pair queue.

### Toposort (Kahn's)

```cpp
queue<int> q;
for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
while (!q.empty()) {
    int u = q.front(); q.pop(); order.push_back(u);
    for (int v : g[u]) if (--indeg[v] == 0) q.push(v);
}
// order.size() < n  =>  cycle exists
```

**Use for:** dependency ordering, DAG DP (longest path), course schedule.

### Dijkstra — shortest path, non-negative weights

```cpp
vector<ll> dist(n, INF);
priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<>> pq;
dist[s] = 0; pq.push({0, s});
while (!pq.empty()) {
    auto [d, u] = pq.top(); pq.pop();
    if (d > dist[u]) continue;              // stale entry — crucial
    for (auto [v, w] : g[u])                // g[u] = vector of (to, weight)
        if (dist[u] + w < dist[v]) {
            dist[v] = dist[u] + w;
            pq.push({dist[v], v});
        }
}
```

**Problems:** CSES Shortest Routes I · grid with weighted cells · "min cost with at most k discounts" (layered graph: node = (vertex, coupons used)).

### Union-Find (DSU)

**Recognise:** dynamic connectivity, "are u, v connected?", Kruskal MST, grouping merges, greedy latest-free-slot (deadline scheduling).

```cpp
int par[N], sz[N];
void init(int n) { for (int i = 0; i < n; i++) par[i] = i, sz[i] = 1; }
int find(int x) { return par[x] == x ? x : par[x] = find(par[x]); }
bool unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return false;
    if (sz[a] < sz[b]) swap(a, b);
    par[b] = a; sz[a] += sz[b];
    return true;
}
```

**Problems:** count components · Kruskal · job scheduling with deadlines (find(d) = latest free day ≤ d, then unite(slot, slot-1)) · "earliest time everyone connected".

---

## 12. Range Query Structures

**Decision table:**

| Need | Structure |
|---|---|
| Static array, sum queries | Prefix sums |
| Static array, min/max/gcd queries | Sparse table (O(1) query) |
| Point update + range sum | Fenwick (shortest code) |
| Point update + range min/max | Segment tree |
| Range update + point query | Fenwick over difference array |
| Anything fancier | Segment tree (with lazy if range-update+range-query) |

### Fenwick / BIT — 1-based!

```cpp
ll bit[N]; int n;
void update(int i, ll x) { for (; i <= n; i += i & -i) bit[i] += x; }
ll query(int i)          { ll s = 0; for (; i; i -= i & -i) s += bit[i]; return s; }
ll query(int l, int r)   { return query(r) - query(l-1); }   // sum a[l..r]
```

**Problems:** counting inversions (compress values, sweep right-to-left, query how many smaller already seen) · CSES Dynamic Range Sum · order statistics.

### Segment Tree (recursive, min — swap op for max/sum)

```cpp
ll tree[1<<18];  // ~4n; init to identity (INF for min, 0 for sum)
int n;

void update(int p, ll v, int i = 1, int cL = 0, int cR = -1) {
    if (cR == -1) cR = n;                    // root covers [0, n)
    if (cR - cL == 1) { tree[i] = v; return; }
    int mid = (cL + cR) / 2;
    if (p < mid) update(p, v, 2*i,   cL,  mid);
    else         update(p, v, 2*i+1, mid, cR);
    tree[i] = min(tree[2*i], tree[2*i+1]);
}

ll query(int qL, int qR, int i = 1, int cL = 0, int cR = -1) {  // [qL, qR)
    if (cR == -1) cR = n;
    if (qL <= cL && cR <= qR) return tree[i];        // fully inside
    if (qR <= cL || cR <= qL) return INF;            // fully outside (identity)
    int mid = (cL + cR) / 2;
    return min(query(qL, qR, 2*i, cL, mid), query(qL, qR, 2*i+1, mid, cR));
}
```

Children: `2i` and `2i+1`. Build/update O(log n) per op. Half-open ranges throughout.

**Problems:** Fireworks (max tree, §10.4) · Segment Cover (min tree, §10.5) · CSES Dynamic Range Minimum.

### Sparse Table — static min/max/gcd, O(1) query

```cpp
int LOG = 17;                 // > log2(n)
ll sp[17][N];
// build: O(n log n)
for (int i = 0; i < n; i++) sp[0][i] = a[i];
for (int j = 1; j < LOG; j++)
    for (int i = 0; i + (1<<j) <= n; i++)
        sp[j][i] = min(sp[j-1][i], sp[j-1][i + (1<<(j-1))]);
// query [l, r] inclusive: overlapping halves — idempotent ops ONLY (min/max/gcd, NOT sum)
ll query(int l, int r) {
    int j = 31 - __builtin_clz(r - l + 1);   // floor(log2(len))
    return min(sp[j][l], sp[j][r - (1<<j) + 1]);
}
```

**Problems:** RMQ with no updates (you built this already) · LCA via Euler tour · max in every window (also doable with deque).

---

## 13. Misc That Wins Marks

**`assert` for debugging:** `assert(cond);` — crashes with line number. Remove/keep for exam judge as appropriate. (Lecture gotcha: a global array named `count` conflicts with `std::count` under `using namespace std` — rename variables like `cnt`, or drop `using namespace std`.)

**Read until EOF:** `while (cin >> x) { ... }`

**Fast IO already in template.** Never mix `scanf` with unsynced `cin`.

**Estimate before coding:** ~1e8 simple ops/sec.

| n | Intended complexity |
|---|---|
| ≤ 12 | O(n!) |
| ≤ 20–25 | O(2ⁿ·n) |
| ≤ 500 | O(n³) |
| ≤ 5,000 | O(n²) |
| ≤ 1e5–1e6 | O(n log n) |
| ≥ 1e7 | O(n) / O(1)-per-query |

---

## 14. Pre-Coding Checklist

Before touching the keyboard, run through:

1. **Constraints** → target complexity (table above). n ≤ 20 screams bitmask; n ≤ 12 screams brute force.
2. **Sorted input helps?** → greedy or two pointers or binary search.
3. **"Max the min" / "min the max" / monotone feasibility?** → binary search on answer + greedy checker.
4. **Range sums, static?** → prefix sums. Range updates? → difference array.
5. **Contiguous subarray/window?** → sliding window / two pointers / prefix sums.
6. **Count ways / optimal sequence of choices, overlapping subproblems?** → DP. Write state in English first.
7. **Set of used items in the state?** → bitmask DP.
8. **dp transition = max/min over a range?** → add a range tree (Fireworks/Segment Cover pattern).
9. **Explicit or implicit graph? Unweighted?** → BFS. Weighted non-negative? → Dijkstra. Dependencies? → toposort.
10. **Dynamic connectivity / merging groups?** → DSU.
11. **Huge coordinate values, few of them?** → coordinate compression.
12. **Overflow?** → `ll` everywhere sums/products appear. **Mod?** → after every op.
13. Edge cases: n = 0/1, all equal, empty answer, answer = 0 vs "impossible" sentinel.

---

*Good luck. Read every problem statement twice; the constraint line is the biggest hint on the page.*
