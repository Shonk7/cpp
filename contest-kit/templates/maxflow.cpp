#include <bits/stdc++.h>
using namespace std;
#define int long long

const int INF = 1e18;

struct FlowNetwork {
    int n;
    vector<vector<int>> cap, adj;

    FlowNetwork(int _n) : n(_n), cap(_n, vector<int>(_n, 0)), adj(_n) {}

    void add_edge(int u, int v, int c) {
        cap[u][v] += c;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> level;
    int bfs(int s, int t) {
        level.assign(n, -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (level[v] < 0 && cap[u][v] > 0) {
                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }
        return level[t] >= 0;
    }

    int dfs(int u, int t, int f, vector<int>& iter) {
        if (u == t) return f;
        for (int& i = iter[u]; i < adj[u].size(); i++) {
            int v = adj[u][i];
            if (level[u] < level[v] && cap[u][v] > 0) {
                int d = dfs(v, t, min(f, cap[u][v]), iter);
                if (d > 0) {
                    cap[u][v] -= d;
                    cap[v][u] += d;
                    return d;
                }
            }
        }
        return 0;
    }

    int dinic(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            vector<int> iter(n, 0);
            int f;
            while ((f = dfs(s, t, INF, iter)) > 0)
                flow += f;
        }
        return flow;
    }
};

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    FlowNetwork flow(n);

    for (int i = 0; i < m; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        flow.add_edge(u, v, c);
    }

    cout << flow.dinic(0, n - 1) << "\n";

    return 0;
}
