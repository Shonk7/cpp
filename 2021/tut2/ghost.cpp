
// Codeforces Substring Removal Game
/*
For each test case, amekl a vector of all the lengths of 1s and sort
011011101111
[2,3,4]

*/

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;


int main () {
    // subtask 1 where all ghosts appear at the same time
    // simply jsut ahve an array with all x coordinates
    // and everytime one appears incrment it
    // then return the value with the highest number

    // full solution
    // for each ghost work out which time you need to start at the reach it
    // have an array counting how many ghosts you can reach if you start at suchba time

    // can be negative offset time by 30,000
    cin.tie(nullptr);
    cin.sync_with_stdio(false);

    int n, k;
    cin >> n;
    cin >> k;
    int x, t;

    unordered_map<int, int> m;
    while (n--) {
        cin >> x;
        cin >> t;
        
        int time = t - x * k;
        m[time]++;
    }
    int mG = 0;
    for (auto p : m) {
        mG = max(mG, p.second);
    }

    cout << mG << '\n';
    return 0;
    
}