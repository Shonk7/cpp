# Competitive Programming Contest Kit

Complete starter pack with lecture materials, reference code, and templates.

## Structure

### `lectures/` — COMP4128 Course Materials
- `3-paradigms.pdf` — DP, greedy, divide-and-conquer
- `4-datastructures.pdf` — Segment trees, Fenwick, order statistic trees
- `5-dp.pdf` — Advanced DP techniques
- `7-extension.pdf` — Miscellaneous advanced topics
- `8-shortestpaths.pdf` — Dijkstra, Bellman-Ford, Floyd-Warshall
- `9-network-flow.pdf` — Max flow, min cut, applications

### `code/` — Reference & Solutions
- `cp_cheatsheet.md` — Complete cheatsheet with all algorithms, patterns, and code snippets
- `wifi.cpp` — CF1213F solution (segment tree DP + min-cut pattern example)

### `templates/` — Quick Copy-Paste Templates
- `segtree.cpp` — Generic segment tree (range min query)
- `fenwick.cpp` — Fenwick tree (BIT) for prefix sums
- `dijkstra.cpp` — Single-source shortest path
- `dsu.cpp` — Disjoint set union with union-by-size
- `maxflow.cpp` — Dinic's algorithm for max flow

## Quick Start

1. Read `lectures/` for conceptual understanding
2. Check `code/cp_cheatsheet.md` for algorithm details and code snippets
3. Copy templates from `templates/` as base for contest solutions
4. Reference `wifi.cpp` for DP + data structure pattern

## Key Takeaways

- **Segment Tree**: O(log n) range queries, O(n log n) preprocessing
- **Fenwick**: Simpler than segtree for prefix sums, O(log n) both
- **Dijkstra**: O(E log V) with priority queue, no negative edges
- **Max Flow**: Dinic's O(V²E) for general graphs, O(V² √E) for unit capacity
- **DSU**: O(α(n)) amortized with path compression + union-by-size
- **DP**: State = position + constraint; recurrence = decisions at that state
- **Min-Cut Reduction**: Project selection, bipartite matching gadgets

## Transfer to CSE Machine

```bash
# Copy entire kit
scp -r contest-kit/ <cseid>@remote.server:~/
# Then on remote:
cd ~/contest-kit
cat code/cp_cheatsheet.md  # View reference
g++ -o solution templates/segtree.cpp  # Build from template
```

All code is C++17 compatible. Adjust `-std=c++XX` as needed for your judge.
