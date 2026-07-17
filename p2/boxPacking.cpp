#include <bits/stdc++.h>
using namespace std;

#define i32 int32_t
#define int long long

#define fastio        \
    cin.tie(nullptr); \
    cin.sync_with_stdio(false);

#define all(c) c.begin(), c.end()
#define space << " "
#define newline << "\n"

int n;
const int N = 200100;
vector<int> arr(N);
i32 main() {
    fastio;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    sort(all(arr), greater<int>());
    int count = 1;
    int ans = 1;
    int cur = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] == cur) count++;
        else {
            count = 1;
            cur = arr[i];
        }
        ans = max(ans, count);
    }

    cout << ans newline;
    return 0;
}
