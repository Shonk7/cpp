
// Codeforces Substring Removal Game
/*
For each test case, amekl a vector of all the lengths of 1s and sort
011011101111
[2,3,4]

*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main () {
    
    string s;
    int k;
    cin >> k;
    while (k--) {
        cin >> s;
        vector<int> v;
        int oneCount = 0;
        for (char c : s) {
            if (c == '1') {
                oneCount++;
            } else {
                if (oneCount > 0) {
                    v.push_back(oneCount);
                    oneCount = 0;
                }
            }
        }
        v.push_back(oneCount);
        sort(v.begin(), v.end(), greater<int>());
        
        int ans = 0;
        for (int i = 0; i < v.size(); ++i) {
            if (i % 2 == 0) ans += v[i];
        }
        cout << ans << endl;
    }
    return 0;
    
}