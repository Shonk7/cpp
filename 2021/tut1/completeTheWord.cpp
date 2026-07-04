
// Codeforces 716B - Complete the Word

/* consider eahc consecutitive characters independently
Count the frequency of each letter in the 26 chracters
If each letter appears at most once, fill the in the ? with unused chracters, fill in other chracters with anything (A's)
if a letter appears more than once move onto the next 26 characters
*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define MAXN 50010

vector<int> freq(200, 0);

int main() {

    string s;
    cin >> s;

    int n = s.size();

    if (n < 26) {
        cout << -1 << '\n';
        return 0; 
    }

    for (int i = 0 ; i <= n - 26; ++i) {
        
        for (char c = 'A';  c <= 'Z'; ++c) {
           freq[c] = 0;
        }

        for (int j = i; j < i + 26; j++) {
            freq[s[j]]++;
        }

        bool isValid = true;
        for (char c = 'A';  c <= 'Z'; ++c) {
            if (freq[c] > 1) isValid = false;
        }

        if (!isValid) continue;

        for (int j = i; j < i + 26; j++) {
            if (s[j] == '?') {
                for (char c = 'A';  c <= 'Z'; ++c) {
                    if (!freq[c]) {
                        s[j] = c;
                        freq[c] = 1;
                        break;
                    }
                }
            }
        }

        for (int j = 0; j < n; ++j) {
            if (s[j] == '?') {
                s[j] = 'A';
            }
        }

        cout << s << endl;
        return 0;
    }


	cout << -1 << '\n';
	return 0;
}