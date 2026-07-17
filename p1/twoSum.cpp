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
unordered_map<int, int> hashMap;
int arr[N];
i32 main() {
    fastio;
    int target;
    cin >> n >> target;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        hashMap.reserve(2 * n);
        if (hashMap.find(arr[i]) != hashMap.end()) {
            cout << hashMap[arr[i]] space << i + 1 newline;
            return 0;
        }
        hashMap.insert({target - arr[i], i + 1});
    }


    cout << "IMPOSSIBLE" newline;
    return 0;
}
