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
#include <bit>

int n;
i32 main () {
    int m;
    cin >> n >> m;

    vector<int> s(m + 1), e(m + 1), c(m + 1);
    vector<int> index(m);
    vector<int> ans(n + 1);

    for (int i = 1; i <= m; i++) {
        cin >> s[i] >> e[i] >> c[i];
        index[i - 1] = i;
        ans[e[i]] = m + 1;
    }
    
    // sort by end date ascending - earliest first
    sort(all(index), [&](int a, int b) {
        return e[a] < e[b];
    });
    
    // study for this exam
    for (auto id : index) {
        int need = c[id];
        for (int day = s[id]; day < e[id] && need > 0; day++) {
            if (ans[day] == 0) {
                ans[day] = id;
                need--; 
            }
        }
        if (need > 0) {
            cout << -1 newline;
            return 0;
        }
    }
    for (int i = 1; i <= n; i++) {
        cout << ans[i] space;
    }
    cout newline;

    return 0;


}
