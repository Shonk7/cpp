#include <bits/stdc++.h>
using namespace std;

#define i32 int32_t
#define int long long
#define uint unsigned long long

#define fastio        \
    cin.tie(nullptr); \
    cin.sync_with_stdio(false);

#define all(c) c.begin(), c.end()
#define space << " " <<
#define newline << "\n"

using namespace std;
const int INF = 1e18; 

struct Node {
    int min;
};
const int N = 200200;


int n, m, k;
vector<int> graph[N];

vector<int> parentNode;
vector<int> depth;
vector<int> colour;
vector<bool> visited;

int cycleBottom = -1;
int cycleTop = -1;
int minimumDifference = INT_MAX;

void dfs(int u, int parent) {
    visited[u] = true;
    parentNode[u] = parent;

    for (int v : graph[u]) {
        if (v == parent) {
            continue;
        }

        if (!visited[v]) {
            depth[v] = depth[u] + 1;
            colour[v] = colour[u] ^ 1;
            dfs(v, u);
        } else if (depth[v] < depth[u]) {
            /*
             * v is an ancestor of u.
             *
             * Tree path:
             * v -> ... -> u
             *
             * Extra edge:
             * u -> v
             *
             * This creates a cycle.
             */
            int difference = depth[u] - depth[v];

            if (difference < minimumDifference) {
                minimumDifference = difference;
                cycleBottom = u;
                cycleTop = v;
            }
        }
    }
}

i32 main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> k;

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;

        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    parentNode.assign(n + 1, -1);
    depth.assign(n + 1, 0);
    colour.assign(n + 1, 0);
    visited.assign(n + 1, false);

    dfs(1, -1);

    int required = (k + 1) / 2;

    /*
     * No back edge means the graph is a tree.
     */
    if (cycleBottom == -1) {
        vector<int> groups[2];

        for (int vertex = 1; vertex <= n; ++vertex) {
            groups[colour[vertex]].push_back(vertex);
        }

        int chosenGroup =
            groups[0].size() >= groups[1].size() ? 0 : 1;

        cout << 1 << '\n';

        for (int i = 0; i < required; ++i) {
            cout << groups[chosenGroup][i] << ' ';
        }

        cout << '\n';
        return 0;
    }

    /*
     * Reconstruct the cycle:
     *
     * cycleTop -> ... -> cycleBottom -> cycleTop
     */
    vector<int> cycle;

    int current = cycleBottom;

    while (current != cycleTop) {
        cycle.push_back(current);
        current = parentNode[current];
    }

    cycle.push_back(cycleTop);

    /*
     * The back edge itself adds one edge, so:
     *
     * cycle length = depth[bottom] - depth[top] + 1
     *
     * This is also cycle.size().
     */
    if ((int)cycle.size() <= k) {
        cout << 2 << '\n';
        cout << cycle.size() << '\n';

        for (int vertex : cycle) {
            cout << vertex << ' ';
        }

        cout << '\n';
        return 0;
    }

    /*
     * The cycle is longer than k.
     *
     * Take alternating vertices among the first k cycle vertices:
     * cycle[0], cycle[2], cycle[4], ...
     *
     * This gives ceil(k / 2) vertices.
     */
    cout << 1 << '\n';

    for (int i = 0; i < k; i += 2) {
        cout << cycle[i] << ' ';
    }

    cout << '\n';
}