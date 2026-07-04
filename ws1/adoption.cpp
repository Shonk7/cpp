#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;

	vector<long long> gain;
	gain.reserve(2 * n);
	long long ans = 0;

	for (int i = 0; i < 2 * n; i++) {
		long long c, d;
		cin >> c >> d;
		ans += d;
		gain.push_back(c - d);
	}

	sort(gain.begin(), gain.end(), greater<long long>());
	for (int i = 0; i < n; i++) {
		ans += gain[i];
	}

	cout << ans << '\n';
	return 0;
}
