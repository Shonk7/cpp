// Set1-C

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <queue>
using namespace std;
typedef long long ll;

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int n;
    ll t;
    ll x = 0;
    ll sum = 0;
    cin >> n >> t;
    deque<ll> window;
    int maxLen = 0;
    while (n--) {
        cin >> x;
        window.push_back(x);
        sum += x;
        while (sum > t) {
            sum -= window.front();
            window.pop_front();
        }
        maxLen = max(maxLen, int(window.size()));
    }

    cout << maxLen << '\n';

    return 0;
}
