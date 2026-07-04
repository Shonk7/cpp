/*
 *
 * code for graphs
 *
 */

/*
 * adjacency list
 */

#include <iostream>
#include <vector>

int N = 1001001; // number of vertices in graph
vector<int> edges[N]; // each vertex has a list of connected vertices

void add(int u, int v) {
  edges[u].push_back(v);
  // Warning: If the graph has self-loops, you need to check this.
  if (v != u) { edges[v].push_back(u); }
}

...

// iterate over edges from u (since C++11)
for (int v : edges[u]) { cout << v << '\n'; }

// iterate over edges from u (before C++11)
vector<int>::iterator it = edges[u].begin();
for (; it != edges[u].end(); ++it) {
  int v = *it;
  cout << v << '\n';
}

// or just a regular for loop will work too
for (unsigned int i = 0; i < edges[u].size(); i++) {
    cout << edges[u][i] << '\n';
}

/*
 * bfs
 */

vector<int> edges[N];
// dist from start. -1 if unreachable.
int dist[N];
// previous node on a shortest path to the start
// Useful for reconstructing shortest paths
int pred[N];

void bfs(int start) {
    fill(dist, dist+N, -1);
    dist[start] = 0;
    pred[start] = -1;

    queue<int> q;
    q.push(start);
    while (!q.empty()) {
        int c = q.front();
        q.pop();
        for (int nxt : edges[c]) {
            // Push if we have not seen it already.
            if (dist[nxt] == -1) {
                dist[nxt] = dist[c] + 1;
                pred[nxt] = c;
                q.push(nxt);
            }
        }
    }
}

/*
 * dfs
 */

// global arrays are initialised to zero for you
bool seen[N];

void dfs(int u) {
  if (seen[u]) { return; }
  seen[u] = true;
  for (int v : edges[u]) { dfs(v); }
}

/*
 * bridges
 */

int preorder[N]; // order of pushing to the DFS stack
                 // initialise to -1
int T = 0; // counter for preorder sequence
int reach[N]; // reach[u] is the smallest preorder index
            // of any vertex reachable from u
vector<pair<int,int>> bridges;

void dfs(int u, int from = -1) {
  preorder[u] = T++;
  reach[u] = preorder[u];

  for (int v : edges[u]) if (v != from) {
    if (preorder[v] == -1) { // u--v is a tree edge
      // v hasn't been seen before, so recurse
      dfs(v, u);
      // if v can't reach anything earlier than itself
      // then u--v is a bridge
      if (reach[v] == preorder[v]) { bridges.emplace_back(u,v); }
    }
    // anything reachable from v is reachable from u
    reach[u] = min(reach[u], reach[v]);
  }
}

/*
 * directed cycles
 */

const int UNSEEN = 0, ACTIVE = 1, COMPLETE = 2;
int status[N]; // initially UNSEEN

bool has_cycle(int u) {
  if (status[u] != UNSEEN) { return false; }
  status[u] = ACTIVE; // preorder (stack push)
  for (int v : edges[u]) {
    // if v is ACTIVE then it is lower down in the stack
    // so the cycle is from v to the top of the stack
    if (status[v] == ACTIVE || has_cycle(v)) { return true; }
  }
  status[u] = COMPLETE; // postorder (stack pop)
  return false;
}

/*
 * tree representation
 */

const int N = 1001001;

// We need the list of edges to construct our representation
// But we don't use it afterwards.
vector<int> edges[N];

int par[N]; // Parent. -1 for the root.
vector<int> children[N]; // Your children in the tree.
int size[N]; // As an example: size of each subtree.

void constructTree(int c, int cPar = -1) {
    par[c] = cPar;
    size[c] = 1;
    for (int nxt : edges[c]) if (nxt != par[c]) {
        constructTree(nxt, c);
        children[c].push_back(nxt);
        size[c] += size[nxt];
    }
}

/*
 * lca 1
 */

// parent[u][k] is the 2^k-th parent of u
void preprocess() {
  for (int i = 0; i < n; i++) {
    // assume parent[i][0] (the parent of i) is already filled in
    for (int j = 1; (1<<j) < n; j++) { parent[i][j] = -1; }
  }

  // fill in the parent for each power of two up to n
  for (int j = 1; (1<<j) < n; j++) {
    for (int i = 0; i < n; i++) {
      if (parent[i][j-1] != -1) {
        // the 2^j-th parent is the 2^(j-1)-th parent of the 2^(j-1)-th parent
        parent[i][j] = parent[parent[i][j-1]][j-1];
      }
    }
  }
}

/*
 * lca 2
 */

int lca (int u, int v) {
  // make sure u is deeper than v
  if (depth[u] < depth[v]) { swap(u,v); }  
  // log2s[i] holds the largest k such that 2^k <= i
  // precompute by DP: log2s[i] = log2s[i/2] + 1
  for (int i = log2s[depth[u]]; i >= 0; i--) {
    // repeatedly raise u by the largest possible power of two until it is the same depth as v
    if (depth[u] - (1<<i) >= depth[v]) { u = parent[u][i]; }
  }

  if (u == v) return u;

  for (i = log2s[depth[u]]; i >= 0; i--)
    if (parent[u][i] != -1 && parent[u][i] != parent[v][i]) {
      // raise u and v as much as possible without having them coincide
      // this is important because we're looking for the lowest common ancestor, not just any
      u = parent[u][i];
      v = parent[v][i];
    }

  // u and v are now distinct but have the same parent, and that parent is the LCA
  return parent[u][0];
}

/*
 * tree distance
 */

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100100, LOGN = 20;
struct edge { int nd; long long d; };
int parent[MAXN][LOGN];
long long distToRoot[MAXN];
vector<edge> children[MAXN];
// Code to set up LCA and tree representation
void construct_tree(int c, int cPar = -1);
int lca(int a, int b);

void calc_dists_to_root(int c) {
    for (auto edg : children[c]) {
        distToRoot[edg.nd] = distToRoot[c] + edg.d;
        calc_dists_to_root(edg.nd);
    }
}

long long find_tree_dist(int a, int b) {
    int cLca = lca(a, b);
    return distToRoot[a] + distToRoot[b] - 2 * distToRoot[cLca];
}

/*
 * topological sort
 */

// if the edges are in ASCENDING order of node number,
// this produces the lexicographically GREATEST ordering

void dfs(int u, vector<int>& postorder) {
  if (seen[u]) return;
  seen[u] = true;
  for (int v : edges[u]) { dfs(v, postorder); }
  postorder.push_back(u);
} 

vector<int> topsort() {
  vector<int> res;
  for (int i = 0; i < n; i++) { dfs(i, res); }
  reverse(res.begin(), res.end()); // #include <algorithm>
  return res;
}

/*
 * tarjan 1
 */

// we will number the vertices in the order we see them in the DFS
int dfs_index[MAX_VERTICES];
// for each vertex, store the smallest number of any vertex we see
// in its DFS subtree
int lowlink[MAX_VERTICES];

// explicit stack
stack<int> s; // #include <stack>
bool in_stack[MAX_VERTICES];

// arbitrarily number the SCCs and remember which one things are in
int scc_counter;
int which_scc[MAX_VERTICES];

void connect(int v) {
  // a static variable doesn't get reset between function calls
  static int i = 1;
  // set the number for this vertex
  // the smallest numbered thing it can see so far is itself
  lowlink[v] = dfs_index[v] = i++;
  s.push(v);
  in_stack[v] = true;

  // continued

/*
 * tarjan 2
 */

  for (auto w : edges[v]) { // for each edge v -> w
    if (!dfs_index[w]) {  // w hasn't been visited yet
      connect(w);
      // if w can see something, v can too
      lowlink[v] = min(lowlink[v], lowlink[w]);
    }
    else if (in_stack[w]) {
      // w is already in the stack, but we can see it
      // this means v and w are in the same SCC
      lowlink[v] = min(lowlink[v], dfs_index[w]);
    }
  }
  // v is the root of an SCC
  if (lowlink[v] == dfs_index[v]) {
    ++scc_counter;
    int w;
    do {
      w = s.top(); s.pop();
      in_stack[w] = false;
      which_scc[w] = scc_counter;
    } while (w != v);
  }
}

// call connect for each vertex once
for (int v = 0; v < n; ++v) if (!dfs_index[v]) connect(v);

/*
 * kosaraju
 */

void dfs(int u) {
	if (seen[u]) { return; }
	seen[u] = true;
	for (int v : edges[u]) { dfs(v); }
	postorder[p++] = u;
}

void dfs_r(int u, int mark) {
	if (seen_r[u]) { return; }
	seen_r[u] = true;
	scc[u] = mark;
	for (int v : edges_r[u]) { dfs_r(v, mark); }
} 

int compute_sccs() {
	int sccs = 0;
	for (int i = 0; i < n; i++) if (!seen[i]) { dfs(i); }
	for (int i = p - 1; i >= 0; i--) {
		int u = postorder[i];
		// ignore visited vertices
		if (!seen_r[u]) { dfs_r(u, sccs++); }
	}
	return sccs;
}

/*
 * 2sat
 */

void add_edge (int i, int j); // append to edges[i] and edges_r[j]
int compute_sccs (void); // unused return value (#sccs)

// vertices 0..n-1 are the variables, and n..2n-1 their negatives
inline int neg (int i) { return (i < n) ? (i + n): (i - n); }

void add_or (int i, int j) { // clause (i OR j)
  add_edge(neg(i), j); // NOT i implies j
  add_edge(neg(j), i); // NOT j implies i
}

bool is_2sat_solvable (void) {
  compute_sccs();
  for (int i = 0; i < n; i++) { // impossible iff T and F together
    if (scc[i] == scc[neg(i)]) { return false; }
  }
  return true;
}

vector<bool> find_2sat_solution (void) {
  vector<bool> solution(n);
  assert(is_2sat_solvable());
  // Kosaraju finds the SCCs in topological order
  // if F before T, then T // if T before F, then F
  for (int i = 0; i < n; i++) { solution[i] = (scc[i] > scc[neg(i)]); }
  return solution;
}

/*
 * kruskal
 */

struct edge { int u, v, w; }; // endpoint, endpoint, weight
bool operator< (const edge& a, const edge& b) { return a.w < b.w; }

edge edges[N];

int root (int u); // union-find root with path compression
void join (int u, int v); // union-find join with size heuristic

long long kruskal() {
  sort(edges, edges+m); // sort by increasing weight
  long long total_weight = 0;
  for (int i = 0; i < m; i++) {
    edge& e = edges[i];
    // if the endpoints are in different trees, join them
    if (root(e.u) != root(e.v)) {
      total_weight += e.w;
      join(e.u, e.v);
    }
  }
  return total_weight;
}

/*
 * prim
 */

typedef pair<int,int> ii;

vector<ii> edges[N]; // (weight, destination)
bool in_tree[N];
// use 'greater' comparator for min-heap
priority_queue<ii, vector<ii>, greater<ii>> pq;

long long prim() {
  long long total_weight = 0;
  pq.emplace(0,0); // start at vertex 0 with total 0
  while (!pq.empty()) {
    auto cur = pq.top();
    pq.pop();
    int weight = cur.first, v = cur.second;
    // if this vertex was already seen before
    // it must have been seen at a better distance
    if (in_tree[v]) { continue; }
    in_tree[v] = true;
    total_weight += weight;
    // add all edges from this vertex
    for (auto nxt : edges[v]) { pq.push(nxt); }
  }
  return total_weight;
}
