#include <bits/stdc++.h>
using namespace std;
#define int long long

const int INF = 1e18;
const int N = 100100;

vector<pair<int, int>> adj[N];
int dist[N];
int n, m;

void dijkstra(int s) {
    fill(dist, dist + n, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[s] = 0;
    pq.push({0, s});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (auto [w, v] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({w, v});
        adj[v].push_back({w, u});
    }

    dijkstra(0);

    for (int i = 0; i < n; i++) {
        cout << dist[i] << "\n";
    }

    return 0;
}
