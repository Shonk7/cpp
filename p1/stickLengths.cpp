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
vector<int> arr(N, LLONG_MAX);
i32 main() {
    fastio;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    sort(all(arr));
    int median = arr[n / 2];
    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans += abs(arr[i] - median);
    }

    cout << ans newline;
    return 0;
}
