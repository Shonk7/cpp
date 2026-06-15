
// Codeforces Substring Removal Game
/*
For each test case, amekl a vector of all the lengths of 1s and sort
011011101111
[2,3,4]

*/

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <set>
using namespace std;

vector<int> p;
vector<int> s;

int root(int a) {
    return (p[a] == a) ? a : p[a] = root(p[a]) ;
}
void join(int a, int b) {
    a = root(a); b = root(b);
    if (a == b) return;
    
    if (s[a] > s[b]) {
        p[b] = a;
        s[a] += s[b];
    } else {
        p[a] = b;
        s[b] += s[a];
    }

}
void init(int n) {
    p.resize(n + 1);
    s.resize(n + 1);
    for (int i = 1; i < n + 1; i++) {
        p[i] = i;
        s[i] = 1;
    }
}

int main () {
    // union job
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int n, q;
    cin >> n;
    cin >> q;
    init(n);
    set<pair<int, int>> ranges;
    for (int i = 1; i <= n; i++) {
        ranges.insert({i, i});
    }
    int qu, x, y;

    while (q--) {
        cin >> qu;
        cin >> x;
        cin >> y;

        if (qu == 1) {
            join(x, y);
        } else if (qu == 2) {
            //find the first range contains x
            // first range after that starts after x 
            // get range before it
            auto it = ranges.upper_bound({x, 1e9});
            // this range contains our x 
            it--;
            
            // need to merge ranges that are within x-y
            vector<set<pair<int, int>>::iterator> toMerge;
            toMerge.push_back(it);
            // stop once passed y
            while (it->second < y) {
                it++;
                toMerge.push_back(it);
            }
            
            // get newrange and delte the old ones
            int newStart = toMerge[0]->first;
            int newEnd = toMerge.back()->second;
            
            for (auto it : toMerge) {
                // need to union fidn the range when delting
                join(it->first, newStart);
                ranges.erase(it);
            }
            // insert after delting just in case we end up wiuth the same range
            ranges.insert({newStart, newEnd});

        } else {
            bool inUnion = (root(x) == root(y));
            if (inUnion) cout << "YES" << '\n';
            else cout << "NO" << '\n';
        }
    }
 
    return 0;
    
}