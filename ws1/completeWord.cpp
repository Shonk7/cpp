#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	string s;
	cin >> s;

	int n = (int)s.size();
	if (n < 26) {
		cout << -1 << '\n';
		return 0;
	}

	for (int l = 0; l + 26 <= n; l++) {
		vector<int> cnt(26, 0);
		int q = 0;

		for (int i = l; i < l + 26; i++) {
			if (s[i] == '?') {
				q++;
			} else {
				cnt[s[i] - 'A']++;
			}
		}

		bool ok = true;
		int distinct = 0;
		for (int c : cnt) {
			if (c > 1) {
				ok = false;
				break;
			}
			if (c == 1) distinct++;
		}

		if (!ok) continue;
		if (distinct + q != 26) continue;

		vector<char> missing;
		for (int k = 0; k < 26; k++) {
			if (cnt[k] == 0) missing.push_back((char)('A' + k));
		}

		int ptr = 0;
		for (int i = l; i < l + 26; i++) {
			if (s[i] == '?') {
				s[i] = missing[ptr++];
			}
		}

		for (char &ch : s) {
			if (ch == '?') ch = 'A';
		}

		cout << s << '\n';
		return 0;
	}

	cout << -1 << '\n';
	return 0;
}
