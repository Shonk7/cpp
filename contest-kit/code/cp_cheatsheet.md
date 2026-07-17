# Competitive Programming Cheat Sheet

Print this. Template matches `set4/wifi.cpp` style — minimal macros, plain `std::` types everywhere else.

---

## 0. Starter Template

```cpp
#include <bits/stdc++.h>
using namespace std;

#define i32 int32_t
#define int long long
#define uint unsigned long long

#define fastio        \
    cin.tie(nullptr); \
    cin.sync_with_stdio(false);

#define all(c) c.begin(), c.end()
#define space << " "
#define newline << "\n"

const int INF = 1e9;
const int MOD = 1e9 + 7;
const int N = 100100;

i32 main() {
    fastio;
    int t = 1;
    // cin >> t;
    while (t--) {
        // ...
    }
    return 0;
}
```

Compile with warnings/sanitizers while testing:
```
g++ -std=c++20 -O2 -Wall -Wextra -Wshadow -fsanitize=address,undefined -o sol sol.cpp
```
Compile fast for actual submission timing:
```
g++ -std=c++20 -O2 -o sol sol.cpp
```

---

## 1. Graphs — Basics (BFS/DFS/Components/Bipartite/Topo)

```cpp
vector<vector<int>> adj; // adjacency list, 0-indexed

vector<int> bfs(int src, int n) {
    vector<int> dist(n, -1);
    queue<int> q;
    dist[src] = 0; q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) if (dist[v] == -1) {
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
    return dist;
}

vector<bool> visited;
void dfs(int u) {
    visited[u] = true;
    for (int v : adj[u]) if (!visited[v]) dfs(v);
}

// connected components
int countComponents(int n) {
    visited.assign(n, false);
    int c = 0;
    for (int i = 0; i < n; i++) if (!visited[i]) { dfs(i); c++; }
    return c;
}

// bipartite check (2-coloring)
bool isBipartite(int n) {
    vector<int> color(n, -1);
    for (int s = 0; s < n; s++) {
        if (color[s] != -1) continue;
        color[s] = 0;
        queue<int> q; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) { color[v] = color[u] ^ 1; q.push(v); }
                else if (color[v] == color[u]) return false;
            }
        }
    }
    return true;
}

// topological sort (Kahn's), DAG only
vector<int> topoSort(int n) {
    vector<int> indeg(n, 0);
    for (int u = 0; u < n; u++) for (int v : adj[u]) indeg[v]++;
    vector<int> order;
    queue<int> q;
    for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) if (--indeg[v] == 0) q.push(v);
    }
    return order; // size < n => cycle exists
}
```

### Strongly Connected Components (Tarjan's)
```cpp
vector<vector<int>> adj;
vector<int> disc, low, sccId;
vector<bool> onStack;
stack<int> stk;
int timer_ = 0, sccCount = 0;

void tarjan(int u) {
    disc[u] = low[u] = timer_++;
    stk.push(u); onStack[u] = true;
    for (int v : adj[u]) {
        if (disc[v] == -1) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (onStack[v]) {
            low[u] = min(low[u], disc[v]);
        }
    }
    if (low[u] == disc[u]) {
        while (true) {
            int v = stk.top(); stk.pop(); onStack[v] = false;
            sccId[v] = sccCount;
            if (v == u) break;
        }
        sccCount++;
    }
}

void runTarjan(int n) {
    disc.assign(n, -1); low.assign(n, -1); onStack.assign(n, false);
    sccId.assign(n, -1); timer_ = 0; sccCount = 0;
    for (int i = 0; i < n; i++) if (disc[i] == -1) tarjan(i);
}
```

### Bridges & Articulation Points
```cpp
vector<vector<int>> adj;
vector<int> disc, low, parent;
vector<pair<int,int>> bridges;
set<int> articulation;
int timer2 = 0;

void dfsBridge(int u) {
    disc[u] = low[u] = timer2++;
    int children = 0;
    for (int v : adj[u]) {
        if (v == parent[u]) continue; // skip parent edge once (handle multi-edges w/ id if needed)
        if (disc[v] == -1) {
            children++;
            parent[v] = u;
            dfsBridge(v);
            low[u] = min(low[u], low[v]);
            if (low[v] > disc[u]) bridges.push_back({u, v});
            if (parent[u] != -1 && low[v] >= disc[u]) articulation.insert(u);
        } else {
            low[u] = min(low[u], disc[v]);
        }
    }
    if (parent[u] == -1 && children > 1) articulation.insert(u);
}
```

### Lowest Common Ancestor (Binary Lifting)
```cpp
const int LOG = 20;
vector<vector<int>> adj, up; // up[k][v]
vector<int> depth;

void dfsLCA(int u, int p) {
    up[0][u] = p;
    for (int k = 1; k < LOG; k++) up[k][u] = (up[k-1][u] == -1) ? -1 : up[k-1][up[k-1][u]];
    for (int v : adj[u]) if (v != p) { depth[v] = depth[u] + 1; dfsLCA(v, u); }
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    int diff = depth[u] - depth[v];
    for (int k = 0; k < LOG; k++) if ((diff >> k) & 1) u = up[k][u];
    if (u == v) return u;
    for (int k = LOG - 1; k >= 0; k--)
        if (up[k][u] != up[k][v]) { u = up[k][u]; v = up[k][v]; }
    return up[0][u];
}
// init: up.assign(LOG, vector<int>(n, -1)); depth.assign(n, 0); dfsLCA(root, -1);
```

---

## 2. Shortest Paths

### Dijkstra (non-negative weights)
```cpp
vector<vector<pair<int,int>>> adj; // {neighbor, weight}

vector<int> dijkstra(int src, int n) {
    vector<int> dist(n, LLONG_MAX);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq; // {dist, node}
    dist[src] = 0; pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

### Bellman-Ford (negative weights, cycle detection)
```cpp
struct Edge { int u, v, w; };

// returns {dist, hasNegativeCycle}
pair<vector<int>, bool> bellmanFord(int src, int n, vector<Edge>& edges) {
    vector<int> dist(n, LLONG_MAX);
    dist[src] = 0;
    for (int i = 0; i < n - 1; i++) {
        for (auto& e : edges) {
            if (dist[e.u] != LLONG_MAX && dist[e.u] + e.w < dist[e.v])
                dist[e.v] = dist[e.u] + e.w;
        }
    }
    bool negCycle = false;
    for (auto& e : edges)
        if (dist[e.u] != LLONG_MAX && dist[e.u] + e.w < dist[e.v]) negCycle = true;
    return {dist, negCycle};
}
```

### Floyd-Warshall (all pairs)
```cpp
void floydWarshall(vector<vector<int>>& dist, int n) { // dist[i][j] pre-filled, INF for no edge, 0 diag
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][k] < LLONG_MAX && dist[k][j] < LLONG_MAX)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
}
```

### 0-1 BFS (weights are 0 or 1)
```cpp
vector<int> zeroOneBFS(int src, int n, vector<vector<pair<int,int>>>& adj) {
    vector<int> dist(n, INT_MAX);
    deque<int> dq;
    dist[src] = 0; dq.push_back(src);
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v); else dq.push_back(v);
            }
        }
    }
    return dist;
}
```

---

## 3. Minimum Spanning Tree

### Kruskal + DSU
```cpp
struct DSU {
    vector<int> parent, rnk;
    DSU(int n) : parent(n), rnk(n, 0) { iota(all(parent), 0); }
    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rnk[a] < rnk[b]) swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b]) rnk[a]++;
        return true;
    }
};

int kruskal(int n, vector<array<int,3>>& edges) { // {w, u, v}
    sort(all(edges));
    DSU dsu(n);
    int total = 0;
    for (auto& e : edges) if (dsu.unite(e[1], e[2])) total += e[0];
    return total;
}
```

### Prim's (dense-friendly)
```cpp
int prim(int n, vector<vector<pair<int,int>>>& adj, int src = 0) {
    vector<int> dist(n, INT_MAX);
    vector<bool> inMST(n, false);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    dist[src] = 0; pq.push({0, src});
    int total = 0;
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true; total += d;
        for (auto [v, w] : adj[u]) if (!inMST[v] && w < dist[v]) {
            dist[v] = w; pq.push({w, v});
        }
    }
    return total;
}
```

---

## 4. Network Flow

Matches COMP4128-style `FlowNetwork`: adjacency matrix holds remaining capacity (`adjMat[u][v]`), adjacency list holds neighbours to iterate (`adjList[u]`). `INF` should exceed the sum of all finite capacities in the graph.

### FlowNetwork base struct
```cpp
struct FlowNetwork {
    int n;
    vector<vector<int>> adjMat;
    vector<vector<int>> adjList;

    FlowNetwork(int _n) : n(_n) {
        adjMat.assign(n, vector<int>(n, 0));
        adjList.resize(n);
    }

    void add_edge(int u, int v, int c) {
        adjMat[u][v] += c; // sums onto existing edge, doesn't overwrite
        adjList[u].push_back(v);
        adjList[v].push_back(u); // so residual edge is reachable
    }

    void flow_edge(int u, int v, int c) {
        adjMat[u][v] -= c;
        adjMat[v][u] += c;
    }
```

### Dinic's algorithm (append inside FlowNetwork — the one to use by default)
```cpp
    vector<int> level, uptochild;

    bool bfs(int s, int t) {
        level.assign(n, -1);
        queue<int> q;
        q.push(s);
        level[s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            uptochild[u] = 0;
            for (int v : adjList[u]) {
                if (level[v] == -1 && adjMat[u][v] > 0) {
                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }
        return level[t] != -1;
    }

    int augment(int u, int t, int f) {
        if (u == t) return f;
        for (int& i = uptochild[u]; i < (int)adjList[u].size(); i++) {
            int v = adjList[u][i];
            if (level[v] == level[u] + 1 && adjMat[u][v] > 0) {
                int rf = augment(v, t, min(f, adjMat[u][v]));
                if (rf > 0) { flow_edge(u, v, rf); return rf; }
            }
        }
        level[u] = -1; // no augmenting path through u this phase
        return 0;
    }

    int dinic(int s, int t) {
        level.assign(n, -1); uptochild.assign(n, 0);
        int res = 0;
        while (bfs(s, t)) {
            while (int x = augment(s, t, INF)) res += x;
        }
        return res;
    }
```
Complexity `O(V^2 E)` worst case, `O(E sqrt(V))` on unit graphs (e.g. bipartite matching flow graphs) — almost always far faster than worst case in practice.

### Extract the min cut (after running dinic)
```cpp
    void check_reach(int u, vector<bool>& seen) {
        if (seen[u]) return;
        seen[u] = true;
        for (int v : adjList[u]) if (adjMat[u][v] > 0) check_reach(v, seen);
    }

    // returns edges crossing the cut; run dinic(s,t) first
    vector<pair<int,int>> min_cut(int s, int t) {
        vector<bool> seen(n, false);
        check_reach(s, seen);
        vector<pair<int,int>> ans;
        for (int u = 0; u < n; u++) {
            if (!seen[u]) continue;
            for (int v : adjList[u]) {
                if (!seen[v]) ans.push_back({u, v}); // skip if v is a "virtual" node you added
            }
        }
        return ans;
    }
}; // end FlowNetwork
```

### Ford-Fulkerson (any augmenting path, O(E·f)) and Edmonds-Karp (shortest augmenting path via BFS, O(V E^2)) exist as simpler/slower alternatives — default to Dinic's above unless asked to implement them specifically.

### Key gadgets
- **Vertex capacities**: split vertex `u` into `u_in` / `u_out`, add edge `u_in -> u_out` with capacity `c_u`; incoming edges go to `u_in`, outgoing edges leave from `u_out`.
- **Multiple sources/sinks**: add a supersource connected to each real source with an `INF` edge (and symmetrically a supersink).
- **Undirected edges**: add both directions as separate edges (not as a residual pair).
- **Max edge-disjoint paths**: unit-capacity edges, answer = maxflow.
- **Min vertex cover (bipartite)**: min cut on the standard bipartite-matching flow graph; a cut edge touching `s` or `t` means that endpoint is in the cover, an internal cut edge means pick either endpoint.

### Min-Cost Max-Flow (SPFA-based, Bellman-Ford augmenting)
```cpp
struct MCMF {
    struct Edge { int to, cap, cost, flow; };
    vector<Edge> edges;
    vector<vector<int>> g;
    int n;
    vector<int> dist, parentEdge;
    vector<bool> inQueue;

    MCMF(int n) : n(n), g(n) {}

    void addEdge(int u, int v, int cap, int cost) {
        g[u].push_back(edges.size()); edges.push_back({v, cap, cost, 0});
        g[v].push_back(edges.size()); edges.push_back({u, 0, -cost, 0});
    }

    pair<int,int> run(int s, int t) { // returns {maxflow, mincost}
        int flow = 0, cost = 0;
        while (true) {
            dist.assign(n, INT_MAX); parentEdge.assign(n, -1); inQueue.assign(n, false);
            dist[s] = 0;
            queue<int> q; q.push(s); inQueue[s] = true;
            while (!q.empty()) {
                int u = q.front(); q.pop(); inQueue[u] = false;
                for (int id : g[u]) {
                    auto& e = edges[id];
                    if (e.cap - e.flow > 0 && dist[u] + e.cost < dist[e.to]) {
                        dist[e.to] = dist[u] + e.cost;
                        parentEdge[e.to] = id;
                        if (!inQueue[e.to]) { q.push(e.to); inQueue[e.to] = true; }
                    }
                }
            }
            if (dist[t] == INT_MAX) break;
            int aug = INT_MAX;
            for (int v = t; v != s; ) {
                int id = parentEdge[v];
                aug = min(aug, edges[id].cap - edges[id].flow);
                v = edges[id ^ 1].to;
            }
            for (int v = t; v != s; ) {
                int id = parentEdge[v];
                edges[id].flow += aug;
                edges[id ^ 1].flow -= aug;
                v = edges[id ^ 1].to;
            }
            flow += aug; cost += aug * dist[t];
        }
        return {flow, cost};
    }
};
```

### Bipartite Matching (Kuhn's)
```cpp
vector<vector<int>> adj; // adj[left] -> list of right nodes
vector<int> matchL, matchR;
vector<bool> used;

bool tryKuhn(int u) {
    for (int v : adj[u]) {
        if (used[v]) continue;
        used[v] = true;
        if (matchR[v] == -1 || tryKuhn(matchR[v])) {
            matchR[v] = u; matchL[u] = v;
            return true;
        }
    }
    return false;
}

int maxBipartiteMatching(int nLeft, int nRight) {
    matchL.assign(nLeft, -1); matchR.assign(nRight, -1);
    int result = 0;
    for (int u = 0; u < nLeft; u++) {
        used.assign(nRight, false);
        if (tryKuhn(u)) result++;
    }
    return result;
}
```

### Worked min-cut pattern: Project Selection
Maximize `sum(revenue of chosen projects) - sum(cost of machines needed)`, where each project depends on a set of machines. Standard trick: rewrite as minimizing `sum(revenue of REJECTED projects) + sum(cost of USED machines)`, build a bipartite graph — source to each project with capacity = its revenue, each machine to sink with capacity = its cost, and an `INF` edge project -> machine for every dependency. Answer = `totalRevenue - mincut`.
```cpp
int n, m, d;
cin >> n >> m;
FlowNetwork net(n + m + 2);
int s = n + m, t = n + m + 1;
int totalRevenue = 0;
for (int i = 0; i < n; i++) {
    int r; cin >> r;
    net.add_edge(s, i, r);
    totalRevenue += r;
}
for (int j = 0; j < m; j++) {
    int c; cin >> c;
    net.add_edge(n + j, t, c);
}
cin >> d;
for (int k = 0; k < d; k++) {
    int a, b; cin >> a >> b; // project a depends on machine b, 1-indexed
    net.add_edge(a - 1, n + b - 1, INF);
}
cout << totalRevenue - net.dinic(s, t) << "\n";
```
This pattern (source→items with a value, sink←resources with a cost, `INF` edges for dependencies, answer = total value − mincut) generalizes to most "pick items to maximize profit subject to dependency/exclusivity constraints" problems. When constraints are "assign each element a binary label, pay a penalty if two connected elements disagree" (image segmentation / labeling), route through a single vertex per element (not two) so `s`-side membership vs `t`-side membership directly encodes the label — no extra `INF` gadget needed, unlike the naive two-node-per-element construction.

---

## 5. Trees / Range Structures

### STL container cheat sheet
- `vector` — O(1) amortized `push_back`, O(1) random access. Default choice everywhere.
- `stack` / `queue` — O(1) push/pop, LIFO/FIFO. Use for DFS/BFS or when only one end matters.
- `priority_queue` — max-heap by default; for min-heap: `priority_queue<T, vector<T>, greater<T>> pq;`. O(log n) push/pop, O(1) top. Use whenever you need "the current largest/smallest" (Dijkstra, greedy-by-value).
- `set`/`map` (red-black tree, O(log n)) — keep data **ordered**; use when you need `lower_bound`/`upper_bound`. `unordered_set`/`unordered_map` are O(1) average but unordered and slower in practice for small n — prefer `set`/`map` unless you specifically don't need order.
- **Never** call `binary_search`/`lower_bound` free functions on a `set`/`map`'s iterators — that's O(n) linear scan. Always call the container's own `.lower_bound()`/`.upper_bound()`/`.find()` method, which is O(log n).
- `s.find(x) != s.end()` — correct membership check; don't use `count` unless on a multiset where you want the actual count.

### Order Statistic Tree (GNU policy-based, k-th element / rank queries in O(log n))
```cpp
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
// ordered_set behaves like set<int> plus:
//   find_by_order(k) -> iterator to k-th smallest element (0-indexed)
//   order_of_key(x)  -> count of elements strictly less than x
// for a multiset-like version, use less_equal<int> and store (value, insertion_index) pairs
// for an ordered_map: tree<K, V, less<K>, rb_tree_tag, tree_order_statistics_node_update>
```

### Fenwick (Binary Indexed) Tree — point update, prefix sum
```cpp
struct Fenwick {
    vector<int> bit; int n;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void update(int i, int delta) { for (++i; i <= n; i += i & (-i)) bit[i] += delta; }
    int query(int i) { int s = 0; for (++i; i > 0; i -= i & (-i)) s += bit[i]; return s; } // prefix sum [0, i]
    int query(int l, int r) { return query(r) - (l ? query(l - 1) : 0); }
};
```

### Segment Tree — generic (sum here, swap merge for min/max)
```cpp
struct SegTree {
    int n;
    vector<int> tree;
    SegTree(int n) : n(n), tree(4 * n, 0) {}

    void build(vector<int>& a, int node, int l, int r) {
        if (l == r) { tree[node] = a[l]; return; }
        int mid = (l + r) / 2;
        build(a, 2*node, l, mid);
        build(a, 2*node+1, mid+1, r);
        tree[node] = tree[2*node] + tree[2*node+1];
    }

    void update(int node, int l, int r, int idx, int val) {
        if (l == r) { tree[node] = val; return; }
        int mid = (l + r) / 2;
        if (idx <= mid) update(2*node, l, mid, idx, val);
        else update(2*node+1, mid+1, r, idx, val);
        tree[node] = tree[2*node] + tree[2*node+1];
    }

    int query(int node, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        return query(2*node, l, mid, ql, qr) + query(2*node+1, mid+1, r, ql, qr);
    }
};
```

### DSU with size (union by size + path compression)
```cpp
struct DSUsz {
    vector<int> parent, sz;
    DSUsz(int n) : parent(n), sz(n, 1) { iota(all(parent), 0); }
    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a; sz[a] += sz[b];
        return true;
    }
};
```
Union by size (or rank) alone gives `O(log n)` per op; adding path compression (as above) gives amortized `O(α(n))` — effectively constant. Main limitation: **no delete/split** — can't un-union two sets. Use for incremental connectivity, Kruskal's MST, "group elements that must be treated as one" problems. If you need deletable connectivity, that's a much harder "dynamic connectivity" problem (link-cut trees) — not something to reach for casually.

### Range tree vs Fenwick tree — when to use which
- **Fenwick/BIT** (above): simplest, fastest constant factor, `O(log n)` point-update / prefix-query. Only works for **invertible** operations (sum, xor, product) — can't do min/max/gcd because you can't "subtract" them back out.
- **Segment tree** (above): works for **any associative** operation (min, max, gcd — not just invertible ones), and supports arbitrary custom merge logic (see below). Slightly higher constant factor than a Fenwick tree.
- Segment trees generalize to **range updates** via lazy propagation (cache an update at a node instead of pushing it all the way down; push it further only when a later query/update actually needs to descend past that node) — needed whenever an update touches a whole range, not just one point.
- Segment trees generalize to **trees** (subtree queries) by flattening with a DFS in/out time (Euler tour): a node's subtree becomes exactly the contiguous range `[in[v], out[v])` in the flattened array, so subtree update/query becomes an ordinary range update/query.
- **Custom merge trick**: when a query needs more than just "the answer for this range," store extra fields per node (e.g. `sum`, `bestPrefix`, `bestSuffix`, `bestSubarray` for "max subarray sum in a range" — see below) and write a `merge(left, right)` that combines all fields from two children. This pattern solves most "range query with a twist" problems: figure out what a merge needs to know, add fields until merging is possible with only local info.

```cpp
// max-subarray-sum-in-range node, merged from two children
struct MSState { int sum, bestPrefix, bestSuffix, best; };
MSState merge(const MSState& L, const MSState& R) {
    return {
        L.sum + R.sum,
        max(L.bestPrefix, L.sum + R.bestPrefix),
        max(R.bestSuffix, R.sum + L.bestSuffix),
        max({L.best, R.best, L.bestSuffix + R.bestPrefix})
    };
}
// leaf: {v, max(v,0), max(v,0), max(v,0)}
```

---

## 6. Math

### Fast exponentiation / modular inverse
```cpp
int power(int base, int exp, int mod = MOD) {
    base %= mod; int result = 1;
    while (exp > 0) {
        if (exp & 1) result = (__int128)result * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}
int modInverse(int a, int mod = MOD) { return power(a, mod - 2, mod); } // mod must be prime
```

### Sieve of Eratosthenes + smallest prime factor
```cpp
vector<int> spf; // smallest prime factor
void sieve(int n) {
    spf.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) {
        if (spf[i]) continue;
        for (int j = i; j <= n; j += i) if (!spf[j]) spf[j] = i;
    }
}
vector<int> factorize(int x) {
    vector<int> f;
    while (x > 1) { f.push_back(spf[x]); x /= spf[x]; }
    return f;
}
```

### GCD / LCM / Extended Euclid
```cpp
int gcdExt(int a, int b, int& x, int& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    int x1, y1;
    int g = gcdExt(b, a % b, x1, y1);
    x = y1; y = x1 - (a / b) * y1;
    return g;
}
int lcm(int a, int b) { return a / __gcd(a, b) * b; }
```

### Combinatorics (factorials + nCr mod p)
```cpp
const int MAXN = 200005;
int fact[MAXN], invFact[MAXN];
void precomputeFactorials() {
    fact[0] = 1;
    for (int i = 1; i < MAXN; i++) fact[i] = fact[i-1] * i % MOD;
    invFact[MAXN-1] = modInverse(fact[MAXN-1]);
    for (int i = MAXN-2; i >= 0; i--) invFact[i] = invFact[i+1] * (i+1) % MOD;
}
int nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n-r] % MOD;
}
```

### Matrix exponentiation (linear recurrences, e.g. Fibonacci)
```cpp
using Matrix = vector<vector<int>>;
Matrix matMul(const Matrix& A, const Matrix& B, int mod = MOD) {
    int n = A.size(), m = B[0].size(), k = B.size();
    Matrix C(n, vector<int>(m, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            for (int l = 0; l < k; l++)
                C[i][j] = (C[i][j] + A[i][l] * B[l][j]) % mod;
    return C;
}
Matrix matPow(Matrix A, int p, int mod = MOD) {
    int n = A.size();
    Matrix result(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) result[i][i] = 1;
    while (p > 0) {
        if (p & 1) result = matMul(result, A, mod);
        A = matMul(A, A, mod);
        p >>= 1;
    }
    return result;
}
```

### Primality test (Miller-Rabin, deterministic for u64 range with these bases)
```cpp
bool millerRabin(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t p : {2,3,5,7,11,13,17,19,23,29,31,37}) {
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1; int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    for (uint64_t a : {2,3,5,7,11,13,17,19,23,29,31,37}) {
        if (a >= n) continue;
        uint64_t x = 1, base = a, e = d, m = n;
        while (e) { if (e & 1) x = (__int128)x * base % m; base = (__int128)base * base % m; e >>= 1; }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int i = 0; i < r - 1; i++) {
            x = (__int128)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}
```

---

## 7. Common Patterns / Techniques

### Greedy — general approach
Sort/order the input by some natural key, make the locally-best choice at each step, and never revisit it. Only valid if you can **prove** a greedy choice is never worse than any alternative (usually via an exchange argument: show any optimal solution can be transformed into the greedy one without getting worse). If a greedy idea doesn't have an obvious proof, try small hand-worked examples first — if a counterexample doesn't appear after real effort, look for the exchange-argument proof; if the greedy breaks on an example, that's a strong signal to move to DP instead.

### Binary Search on Answer (discrete binary search)
The core reduction: an **optimization** problem ("what's the max/min x such that...") is usually far harder than the corresponding **decision** problem ("is x feasible?"). If feasibility is monotonic in x (feasible for x implies feasible for x±1 in the direction you'd expect), binary search the smallest/largest feasible x, and implement only the decision problem — which is often easy to make greedy once x is fixed (fixing x removes one degree of freedom from the choices).
```cpp
// find the smallest X such that f(X) is true, given f is monotone (false...false,true...true)
int binarySearchAnswer(int lo, int hi, function<bool(int)> feasible) {
    int best = -1; // no valid answer found
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (feasible(mid)) { best = mid; hi = mid - 1; }
        else lo = mid + 1;
    }
    return best;
}
```
Don't hand-roll binary search over an explicit sorted array/container — use `<algorithm>`'s `binary_search(first,last,x)`, `lower_bound`, `upper_bound`, `equal_range`, and for `set`/`map` always call the container's own `.lower_bound()`/`.upper_bound()` method (the free-function versions on tree iterators silently degrade to O(n)).

### Coordinate Compression
When only the **relative order** of values matters (not their magnitude) — e.g. indexing a Fenwick/segment tree by value — replace each value with its rank among all values to shrink the domain to `O(n)`.
```cpp
vector<int> compress(vector<int> vals) {
    vector<int> sorted = vals;
    sort(all(sorted)); sorted.erase(unique(all(sorted)), sorted.end());
    for (auto& v : vals) v = lower_bound(all(sorted), v) - sorted.begin();
    return vals;
}
```

### Two Pointers (variable window)
```cpp
int longestSubarrayAtMostK(vector<int>& a, int k) {
    int l = 0, sum = 0, best = 0;
    for (int r = 0; r < (int)a.size(); r++) {
        sum += a[r];
        while (sum > k) sum -= a[l++];
        best = max(best, r - l + 1);
    }
    return best;
}
```

### Prefix Sums (1D and 2D)
```cpp
vector<int> prefix1D(vector<int>& a) {
    vector<int> p(a.size() + 1, 0);
    for (int i = 0; i < (int)a.size(); i++) p[i+1] = p[i] + a[i];
    return p; // sum(l, r] inclusive-exclusive: p[r+1]-p[l]
}
vector<vector<int>> prefix2D(vector<vector<int>>& a) {
    int n = a.size(), m = a[0].size();
    vector<vector<int>> p(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            p[i+1][j+1] = a[i][j] + p[i][j+1] + p[i+1][j] - p[i][j];
    return p;
}
```

### Knapsack DP (0/1)
```cpp
int knapsack01(vector<int>& wt, vector<int>& val, int cap) {
    int n = wt.size();
    vector<int> dp(cap + 1, 0);
    for (int i = 0; i < n; i++)
        for (int c = cap; c >= wt[i]; c--)
            dp[c] = max(dp[c], dp[c - wt[i]] + val[i]);
    return dp[cap];
}
```

### Longest Increasing Subsequence — O(n log n)
```cpp
int lis(vector<int>& a) {
    vector<int> tail;
    for (int x : a) {
        auto it = lower_bound(all(tail), x);
        if (it == tail.end()) tail.push_back(x); else *it = x;
    }
    return tail.size();
}
```

### Digit DP skeleton
```cpp
// memoize on [pos][tight][state...]
int digitDP(string& num, int pos, bool tight /*, ...state */) {
    if (pos == (int)num.size()) return 1; // base case: count this number
    int limit = tight ? num[pos] - '0' : 9;
    int result = 0;
    for (int d = 0; d <= limit; d++) {
        result += digitDP(num, pos + 1, tight && (d == limit) /*, updated state */);
    }
    return result;
}
```

### Bitmask DP (TSP-style, `n ≤ ~20`)
When a subproblem needs "which of these n items have I used," encode the subset as a bitmask instead of an actual set — turns `O(n!)` into `O(2^n · poly(n))`, which is still exponential but usually feasible for `n ≤ 20`. `dp[mask][last]` = best answer having visited exactly the items in `mask`, currently at `last`.
```cpp
int n; int adjMat[20][20]; // assume filled, adjMat[u][v] = edge weight
int dp[1<<20][20]; // dp[mask][last] = shortest path visiting exactly `mask`, ending at `last`
int tsp() {
    for (auto& row : dp) fill(all(row), INT_MAX / 2);
    dp[1][0] = 0; // start at node 0, mask = {0}
    int ans = INT_MAX;
    for (int mask = 1; mask < (1 << n); mask++)
        for (int cur = 0; cur < n; cur++) {
            if (!(mask & (1 << cur)) || dp[mask][cur] == INT_MAX / 2) continue;
            if (mask == (1 << n) - 1) ans = min(ans, dp[mask][cur]);
            for (int nxt = 0; nxt < n; nxt++) {
                if (mask & (1 << nxt)) continue;
                dp[mask | (1 << nxt)][nxt] = min(dp[mask | (1 << nxt)][nxt], dp[mask][cur] + adjMat[cur][nxt]);
            }
        }
    return ans;
}
```
Useful bit tricks: `1<<i` singleton, `~x` complement, `x&y`/`x|y`/`x^y` set ops, `x&(1<<i)` membership, `popcount(x)` (C++20 `<bit>`) or `__builtin_popcount(x)` for size, `x&(-x)` isolates the lowest set bit. Iterate all subsets of a mask in `O(3^n)` total across all masks: `for (int sub = mask; sub; sub = (sub-1)&mask)`.

### DP paradigm reminders
- **State selection is trial and error, not magic**: pick a natural processing order first (it usually implies the state), write the recurrence, and if some transition can't be validated from the state alone, add exactly the missing piece of information and retry. Don't preemptively cram extra fields into the state "just in case."
- If the order given in the problem doesn't lead anywhere, **try a different order** — nothing says you must process the input in the order it's given. A classic trick: sort/reorder input to make dependencies acyclic and local (see the exercise of sorting rows by length in a "count rook placements on a jagged board" problem — sorting turns an intractable subset-state into a simple counter).
- If your recurrence range-queries or range-updates a table indexed by state, that's the signal to swap the naive loop for a Fenwick/segment tree — same idea as speeding up LIS's `best[i] = 1 + max(bestWithEnd[0, a[i]))` from `O(n²)` to `O(n log n)`.
- Top-down (memoized recursion) only touches states actually needed and mirrors the recurrence directly, but pays recursion overhead. Bottom-up (iterative, explicit order) is faster and enables rolling-array space optimization (`dp[i%2][...]`) but requires you to schedule states in an order consistent with the dependency DAG yourself.
- Guideline: ~200M simple operations/sec on a typical judge; floating point is much slower — avoid it in tight loops.

---

## 8. Complexity / Limits Cheat Sheet

| n            | Feasible complexity      |
|--------------|---------------------------|
| ≤ 10         | O(n!) / O(2^n * n)        |
| ≤ 20         | O(2^n)                    |
| ≤ 500        | O(n^3)                    |
| ≤ 5,000      | O(n^2)                    |
| ≤ 10^5-10^6  | O(n log n)                |
| ≤ 10^7-10^8  | O(n)                      |
| ≤ 10^18      | O(log n) — binary search, matrix exp, fast pow |

Time limit is usually ~1-2s → aim for ~10^8-10^9 simple ops.

---

## 9. Checklist Before Submitting

- [ ] Read constraints — pick right complexity
- [ ] Check overflow — `int` is redefined as `long long` in the template
- [ ] Multiple test cases? Reset global state each test
- [ ] Edge cases: n=0, n=1, all same values, negative numbers, empty input
- [ ] Off-by-one on ranges (inclusive vs exclusive)
- [ ] Fast IO enabled (`fastio;`), avoid `endl`
- [ ] Array sizes large enough (`N`/`MAXN` + buffer)
