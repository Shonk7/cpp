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
const int N = 20100;
int arr[N];
i32 main() {
    fastio;

    cin >> n;
    int ans = 0;
    for (int i = 1; i <= n; i++) ans ^= i;
    for (int i = 1; i < n; i++) {
        int x;
        cin >> x;
        ans ^= x;
    }

    cout << ans newline;
    return 0;
}
